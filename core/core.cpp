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
    eventQueue = {};
    cycles currentCycle = 0;
    ndsLCD->reset();
    arm7->reset();
    arm9->reset();
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
            // For now just always drive the arm9.
            finishedARM9 = true;
            arm9->setTargetCycle(nextEvent->timestamp / ARM9_CYCLE_RATIO);
            cyclesElapsed = arm9->cycle();
            break;
        case EventTargetComponent::ARM7:
            finishedARM7 = true;
            arm7->setTargetCycle(nextEvent->timestamp / ARM7_CYCLE_RATIO);
            cyclesElapsed = arm7->cycle() * ARM7_CYCLE_RATIO;
            break;
        case EventTargetComponent::ARM9:
            finishedARM9 = true;
            arm9->setTargetCycle(nextEvent->timestamp / ARM9_CYCLE_RATIO);
            cyclesElapsed = arm9->cycle();
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

}  // namespace Core
}  // namespace RedPandaDS
