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
}
// ==================================================================================================
void DSEmuCore::init() {
    reset();

    // temp program to test
    // For now set PC to main ram.
    arm7->setPC(MAIN_RAM_START);
    arm9->setPC(MAIN_RAM_START);
    writeProgramToMemory(
        "start:\n"
        "MOV R0, #0\n"
        "loop1:\n"
        "MOV R1, #0\n"
        "loop2:\n"
        "ADD R1, R1, #1\n"
        "CMP R1, #0xFF00\n"
        "Bne loop2\n"
        "ADD R0, R0, #1\n"
        "CMP R0, #0xFF00\n"
        "Bne loop1\n"
        "B start\n",
        MAIN_RAM_START, bus, arm9->isARM7());
}
// ==================================================================================================
void DSEmuCore::reset() {
    setState(ApplicationState::running);

    eventQueue = {};
    cycles currentCycle = 0;
    ndsLCD->reset();
    arm7->reset();
    arm9->reset();
    debugger.reset();

    addEventToQueue<Core::StandardFrameEvent>(0);
    endOfFrameTargetTime = std::chrono::high_resolution_clock::now();
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
    auto now = std::chrono::high_resolution_clock::now();
    if (now < endOfFrameTargetTime) {
        // Finished frame fast, sleep and set the next target.
        std::this_thread::sleep_until(endOfFrameTargetTime);
        endOfFrameTargetTime += FPS_targetFrameTime;
    } else {
        // Missed the frame target. Drop FPS but keep targeting 60 fps moving forward,
        endOfFrameTargetTime = now + FPS_targetFrameTime;
    }
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
