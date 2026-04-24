#ifndef DEBUGGER_H
#define DEBUGGER_H

#include "cpu.h"

namespace RedPandaDS {
namespace Core {

// Forward declare;
class DSEmuCore;

/**
 * @brief Provides a debugging interface for the emulator core.
 */
class DebuggerInterface {
public:
    DebuggerInterface();
    /**
     * @brief Add the core pointer to the interface
     *
     * @param d_core The DS emu core.
     */
    void bindCore(DSEmuCore* d_core) { core = d_core; }
    /**
     * @brief Resets the debugging interface.
     */
    void reset();
    /**
     * @brief Changes the target CPU being debugged.
     *
     * @param targetARM9 true if targeting the arm9.
     */
    void changeDebugCPU(bool targetARM9);
    /**
     * @brief Gets the CPU currently being debugged.
     *
     * @return ARM*
     */
    ARM* getDebugCPU() { return cpuToDebug; }
    /**
     * @brief Toggle the breakpoint. Enables if disabled or unset, removes is enabled.
     *
     * @param addr Address of the breakpoint.
     */
    void toggleBreakpoint(uint32_t addr);
    /**
     * @brief Adds a breakpoint to the currently targetted CPU.
     *
     * @param addr Address of the breakpoint.
     */
    void addBreakpoint(uint32_t addr);
    /**
     * @brief Removes a breakpoint from the currently targetted CPU.
     *
     * @param addr Address of the breakpoint.
     */
    void removeBreakpoint(uint32_t addr);
    /**
     * @brief Adds a disabled breakpoint to the currently targetted CPU.
     *
     * @param addr Address of the breakpoint.
     */
    void disableBreakpoint(uint32_t addr);
    /**
     * @brief Disables all current breakpoints.
     */
    void disableAllBreakpoints();
    /**
     * @brief Checks if a breakpoint is enabled.
     *
     * @param addr Address of the breakpoint.
     *
     * @return `bool`
     */
    bool isEnabledBreakpoint(uint32_t addr) const { return enabledBreakpoints.contains(addr); }
    /**
     * @brief Checks if a breakpoint is disabled.
     *
     * @param addr Address of the breakpoint.
     *
     * @return `bool`
     */
    bool isDisabledBreakpoint(uint32_t addr) const { return disabledBreakpoints.contains(addr); }

private:
    DSEmuCore* core = nullptr;
    ARM* cpuToDebug = nullptr;
    std::unordered_set<uint32_t> enabledBreakpoints;
    std::unordered_set<uint32_t> disabledBreakpoints;
};

}  // namespace Core
}  // namespace RedPandaDS

#endif
