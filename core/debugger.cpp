#include "debugger.h"

#include "core.h"

// Control print statements.
#define LOG_LEVEL 1
#include "logger.h"

namespace RedPandaDS {
namespace Core {

// ==================================================================================================
DebuggerInterface::DebuggerInterface() {
}
// ==================================================================================================
void DebuggerInterface::reset() {
    changeDebugCPU(true);
    enabledBreakpoints.clear();
    disabledBreakpoints.clear();
}
// ==================================================================================================
void DebuggerInterface::changeDebugCPU(bool targetARM9) {
    ARM* newTarget = targetARM9 ? (ARM*)core->arm9 : (ARM*)core->arm7;
    if (newTarget == cpuToDebug) return;
    // Switched CPU to debug.
    disableAllBreakpoints();
    cpuToDebug = newTarget;
}
// ==================================================================================================
void DebuggerInterface::addBreakpoint(uint32_t addr) {
    enabledBreakpoints.insert(addr);
    disabledBreakpoints.erase(addr);
    if (cpuToDebug != nullptr) cpuToDebug->setBreakpoints(enabledBreakpoints);
}
// ==================================================================================================
void DebuggerInterface::removeBreakpoint(uint32_t addr) {
    enabledBreakpoints.erase(addr);
    disabledBreakpoints.erase(addr);
    if (cpuToDebug != nullptr) cpuToDebug->setBreakpoints(enabledBreakpoints);
}
// ==================================================================================================
void DebuggerInterface::disableBreakpoint(uint32_t addr) {
    enabledBreakpoints.erase(addr);
    disabledBreakpoints.insert(addr);
    if (cpuToDebug != nullptr) cpuToDebug->setBreakpoints(enabledBreakpoints);
}
// ==================================================================================================
void DebuggerInterface::disableAllBreakpoints() {
    disabledBreakpoints.insert(enabledBreakpoints.begin(), enabledBreakpoints.end());
    enabledBreakpoints.clear();
    if (cpuToDebug != nullptr) cpuToDebug->setBreakpoints(enabledBreakpoints);
}
// ==================================================================================================
void DebuggerInterface::toggleBreakpoint(uint32_t addr) {
    if (!enabledBreakpoints.contains(addr)) {
        addBreakpoint(addr);
    } else {
        removeBreakpoint(addr);
    }
}
// ==================================================================================================
}  // namespace Core
}  // namespace RedPandaDS
