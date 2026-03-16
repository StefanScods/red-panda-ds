#include "core.h"

#include "common.h"

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
    bus->init();
    bus->bindARM7(arm7);
    bus->bindARM9(arm9);
}
// ==================================================================================================
DSEmuCore::~DSEmuCore() {
    DELETE_DYNAMIC_POINTER(arm7);
    DELETE_DYNAMIC_POINTER(arm9);
    DELETE_DYNAMIC_POINTER(bus);
}
// ==================================================================================================
void DSEmuCore::init() {
    reset();
}
// ==================================================================================================
void DSEmuCore::reset() {
    eventQueue = {};
    cycles currentCycle = 0;
    arm7->reset();
    arm9->reset();
}
// ==================================================================================================
cycles DSEmuCore::processNextEvent() {
    if (eventQueue.empty()) {
        LogErrorPrefixed("No pending events in the event queue.", "Core");
        return 0;
    }
    // Get the next event and determine the target cycle.
    coreEvent nextEvent = eventQueue.top();
    eventQueue.pop();
    cycles cyclesElapsed = nextEvent.timestamp - currentCycle;
    currentCycle = nextEvent.timestamp;
    // Cycle the ARM9 CPU.
    arm9->setTargetCycle(currentCycle / ARM9_CYCLE_RATIO);
    arm9->cycle();
    // Cycle the ARM7 CPU.
    arm7->setTargetCycle(currentCycle / ARM7_CYCLE_RATIO);
    arm7->cycle();

    return cyclesElapsed;
}
// ==================================================================================================

}  // namespace Core
}  // namespace RedPandaDS
