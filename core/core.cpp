#include "core.h"

#include "common.h"
#include "utils/armEncode.h"

// Control print statements.
#define LOG_LEVEL 4
#include "logger.h"

namespace RedPandaDS {
namespace Core {

// ==================================================================================================
DSEmuCore::DSEmuCore() {
    // Create the components.
    arm7 = new ARM7TDMI();
    arm9 = new ARM946ES();
    bus = new Interconnect();
    ndsLCD = new NDS_LCD();
    ndsCartridge = new NDS_Cartridge();
    bus->init();
    bus->bindARM7(arm7);
    bus->bindARM9(arm9);
    debugger.bindCore(this);
}
// ==================================================================================================
DSEmuCore::~DSEmuCore() {
    DELETE_DYNAMIC_POINTER(arm7);
    DELETE_DYNAMIC_POINTER(arm9);
    DELETE_DYNAMIC_POINTER(bus);
    DELETE_DYNAMIC_POINTER(ndsLCD);
    DELETE_DYNAMIC_POINTER(ndsCartridge);
}
// ==================================================================================================
void DSEmuCore::init() {
    reset();
}
// ==================================================================================================
void DSEmuCore::reset() {
    setState(ApplicationState::stopped);

    currentCycle = 0;

    ndsLCD->reset();
    ndsCartridge->reset();
    arm7->reset();
    arm9->reset();
    debugger.changeDebugCPU(true);

    eventQueue = {};
    addEventToQueue<Core::StandardFrameEvent>(0);

    endOfFrameTargetTime = std::chrono::high_resolution_clock::now();

    // Signal to the reset of the emulator to update if a function is provided.
    if (onFrameEndCallback != nullptr) {
        onFrameEndCallback();
    }
}
// ==================================================================================================
void DSEmuCore::startExecution() {
    if (getState() != ApplicationState::stopped) {
        LogError("Emulation core already started!");
        return;
    }
    setState(ApplicationState::paused);  // TODO!!! start execution by "pausing". Wait until ROM
                                         // load is fully supported to switch back to running.
}
// ==================================================================================================
void DSEmuCore::stopExecution() {
    if (getState() == ApplicationState::stopped) {
        LogError("Emulation core already stopped!");
        return;
    }
    setState(ApplicationState::stopped);
}
// ==================================================================================================
bool DSEmuCore::loadROM(const std::string& romFile) {
    LogDebug("Loading ROM \"" << romFile << "\"...");
    // Reset the debugger.
    debugger.reset();
    // Reset the rest of the core.
    reset();
    // Load the cartridge.
    if (!ndsCartridge->loadROMFromFile(romFile)) {
        return false;
    }
    // Now that the ROM is loaded, copy the boot vector to RAM.
    const CartridgeHeader& cartHeader = ndsCartridge->getHeader();
    bus->loadProgramFromROM(cartHeader.arm9RamAddress, cartHeader.arm9Size,
                            cartHeader.arm9RomOffset, ndsCartridge);
    bus->loadProgramFromROM(cartHeader.arm7RamAddress, cartHeader.arm7Size,
                            cartHeader.arm7RomOffset, ndsCartridge);
    // Now set the CPUs start vector address.
    arm9->setPC(cartHeader.arm9EntryAddress);
    arm7->setPC(cartHeader.arm7EntryAddress);

    return true;
}
// ==================================================================================================
bool DSEmuCore::reloadCurrentROM() {
    if (!ndsCartridge->isOpen()) return true;
    // Make a copy of the file path string.
    std::string filePathToLoad = ndsCartridge->getLoadedROMFilePath();
    // Reload the ROM.
    return loadROM(filePathToLoad);
}
// ==================================================================================================
void DSEmuCore::closeROM() {
    stopExecution();
    // Close the ROM.
    ndsCartridge->closeROM();
    // Clear the display.
    ndsLCD->reset();
    // Remove all breakpoints.
    debugger.reset();
    // Signal to the reset of the emulator to update if a function is provided.
    if (onFrameEndCallback != nullptr) {
        onFrameEndCallback();
    }
}
// ==================================================================================================
void DSEmuCore::runApplicationIteration() {
    switch (state) {
        case ApplicationState::stopped:
        case ApplicationState::paused:
            break;
        case ApplicationState::running:
            processNextEvent();
            break;
        default:
            break;
    }
}
// ==================================================================================================
cycles DSEmuCore::processNextEvent() {
    if (eventQueue.empty()) {
        LogErrorPrefixed("No pending events in the event queue.", "Core");
        return 0;
    }
    // Get the next event.
    CoreEvent* nextEvent = eventQueue.top();
    // Determine the component of interest and cycle that component.
    // Bools to keep track of whats driven.
    bool finishedARM9 = false;
    bool finishedARM7 = false;
    cycles cyclesElapsed = 0;
    switch (nextEvent->target) {
        case EventTargetComponent::AnyCPU:
            // Drive the CPU being debugged.
            if (debugger.getDebugCPU() == arm7) {
                finishedARM7 = true;
                arm7->setTargetCycle(applyCycleRatio(nextEvent->timestamp, ARM7_CYCLE_RATIO));
                cyclesElapsed = arm7->cycle() * ARM7_CYCLE_RATIO;
            } else {
                finishedARM9 = true;
                arm9->setTargetCycle(applyCycleRatio(nextEvent->timestamp, ARM9_CYCLE_RATIO));
                cyclesElapsed = arm9->cycle() * ARM9_CYCLE_RATIO;
            }
            break;
        case EventTargetComponent::ARM7:
            finishedARM7 = true;
            arm7->setTargetCycle(applyCycleRatio(nextEvent->timestamp, ARM7_CYCLE_RATIO));
            cyclesElapsed = arm7->cycle() * ARM7_CYCLE_RATIO;
            break;
        case EventTargetComponent::ARM9:
            finishedARM9 = true;
            arm9->setTargetCycle(applyCycleRatio(nextEvent->timestamp, ARM9_CYCLE_RATIO));
            cyclesElapsed = arm9->cycle() * ARM9_CYCLE_RATIO;
            break;

        default:
            LogErrorPrefixed("Unsupported event target " << nextEvent->target, "Core");
            return 0;
    }
    currentCycle = currentCycle + cyclesElapsed;
    // Drive all the remaining components.
    if (!finishedARM9) {
        arm9->setTargetCycle(currentCycle / ARM9_CYCLE_RATIO);
        arm9->cycle();
    }
    if (!finishedARM7) {
        arm7->setTargetCycle(currentCycle / ARM7_CYCLE_RATIO);
        arm7->cycle();
    }

    // Hit a breakpoint -> pause the core.
    if (arm9->hitBreakpoint() || arm7->hitBreakpoint()) {
        setState(ApplicationState::paused);
    }

    // Check if the event is finished.
    if (currentCycle >= nextEvent->timestamp) {
        // Trigger an on event finish function if defined.
        if (nextEvent->onEventFinishCallback != nullptr) {
            nextEvent->onEventFinishCallback();
        }
        // Remove the old event.
        DELETE_DYNAMIC_POINTER(nextEvent);
        eventQueue.pop();
    }
    return cyclesElapsed;
}
// ==================================================================================================
void DSEmuCore::endNDSFrame() {
    // Run any end of frame functions which need to run.
    if (onFrameEndCallback != nullptr) {
        onFrameEndCallback();
    }
    // Target 60 FPS.
    // Finished frame fast, sleep and set the next target.
    while (std::chrono::high_resolution_clock::now() < endOfFrameTargetTime) {
        std::this_thread::yield();
    }
    endOfFrameTargetTime = std::chrono::high_resolution_clock::now() + FPS_targetFrameTime;
}
// ==================================================================================================
void DSEmuCore::setState(ApplicationState::ApplicationState newState) {
    if (state == newState) return;
    state = newState;
    if (onStateChangeCallback != nullptr) {
        onStateChangeCallback();
    }
}
// ==================================================================================================
void DSEmuCore::togglePausedState() {
    if (state != ApplicationState::running) {
        setState(ApplicationState::running);
    } else {
        setState(ApplicationState::paused);
    }
}
// ==================================================================================================
void DSEmuCore::stepCPU() {
    debugger.getDebugCPU()->setExecutionLimit(1);
    setState(ApplicationState::running);
}
// ==================================================================================================
}  // namespace Core
}  // namespace RedPandaDS
