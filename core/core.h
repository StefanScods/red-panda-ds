#ifndef CORE_H
#define CORE_H

#include <queue>
#include <unordered_set>

#include "cpu.h"
#include "events.h"
#include "interconnect.h"
#include "lcd.h"

namespace RedPandaDS {
namespace Core {

constexpr int ARM9_CYCLE_RATIO = 1;
constexpr int ARM7_CYCLE_RATIO = 2;

namespace ApplicationState {
enum ApplicationState {
    stopped = 0,
    paused,
    running,
};
}

class DSEmuCore {
public:
    DSEmuCore();
    ~DSEmuCore();

    // Component Accessors.
    Interconnect* getInterconnect() const { return bus; }
    ARM7TDMI* getARM7Core() const { return arm7; }
    ARM946ES* getARM9Core() const { return arm9; }
    NDS_LCD* getNDS_LCD() const { return ndsLCD; }

    /**
     * @brief Initializes the core.
     */
    void init();

    /**
     * @brief Resets the core.
     */
    void reset();
    /**
     * @brief Adds en event to the Core event queue.
     *
     * @tparam T Type of event to add.
     *
     * @param args Args to pass to the event constructor. The core pointer is automatically passed
     * as the first argument.
     */
    template <typename T, typename... Args>
    void addEventToQueue(Args&&... args) {
        static_assert(std::is_base_of<CoreEvent, T>::value, "T must derive from CoreEvent");
        CoreEvent* newEvent = new T(this, std::forward<Args>(args)...);
        eventQueue.push(newEvent);
    }

    ApplicationState::ApplicationState getState() { return state; }
    void togglePausedState();

    void runApplicationFrame();

    void setStepCPUEvent();

    /**
     * @brief Processes the next event.
     *
     * @return `cycles` The number of cycles executed.
     */
    cycles processNextEvent();

    // Breakpoint helpers
    void changeDebugCPU(bool targetARM9) { cpuToDebug = targetARM9 ? (ARM*)arm9 : (ARM*)arm7; }
    ARM* getDebugCPU() { return cpuToDebug; }
    void toggleBreakpoint(uint32_t addr);
    void addBreakpoint(uint32_t addr);
    void removeBreakpoint(uint32_t addr);
    void disableBreakpoint(uint32_t addr);
    void disableAllBreakpoints();
    bool isEnabledBreakpoint(uint32_t addr) const { return enabledBreakpoints.contains(addr); }
    bool isDisabledBreakpoint(uint32_t addr) const { return disabledBreakpoints.contains(addr); }

private:
    // Components.
    Interconnect* bus = nullptr;
    ARM7TDMI* arm7 = nullptr;
    ARM946ES* arm9 = nullptr;
    NDS_LCD* ndsLCD = nullptr;
    ARM* cpuToDebug = nullptr;

    std::unordered_set<uint32_t> enabledBreakpoints;
    std::unordered_set<uint32_t> disabledBreakpoints;

    // Event queue and cycle counter.
    ApplicationState::ApplicationState state;
    std::priority_queue<CoreEvent*, std::vector<CoreEvent*>, CompareCoreEvents> eventQueue;
    cycles currentCycle = 0;
};

}  // namespace Core
}  // namespace RedPandaDS

#endif
