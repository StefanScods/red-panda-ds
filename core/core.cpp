#include "core.h"

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
    if (arm7 != nullptr) delete arm7;
    arm7 = nullptr;
    if (arm9 != nullptr) delete arm9;
    arm9 = nullptr;
    if (bus != nullptr) delete bus;
    bus = nullptr;
}
// ==================================================================================================
void DSEmuCore::init() {
    reset();
}
// ==================================================================================================
void DSEmuCore::reset() {
    arm7->reset();
    arm9->reset();
}
// ==================================================================================================

}  // namespace Core
}  // namespace RedPandaDS
