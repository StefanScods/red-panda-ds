#ifndef CORE_H
#define CORE_H

#include <queue>

#include "cpu.h"
#include "events.h"
#include "interconnect.h"
#include "lcd.h"

namespace RedPandaDS {
namespace Core {

constexpr int ARM9_CYCLE_RATIO = 1;
constexpr int ARM7_CYCLE_RATIO = 2;

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

    /**
     * @brief Processes the next event.
     *
     * @return `cycles` The number of cycles executed.
     */
    cycles processNextEvent();

private:
    // Components.
    Interconnect* bus = nullptr;
    ARM7TDMI* arm7 = nullptr;
    ARM946ES* arm9 = nullptr;
    NDS_LCD* ndsLCD = nullptr;

    // Event queue and cycle counter.
    std::priority_queue<CoreEvent*, std::vector<CoreEvent*>, CompareCoreEvents> eventQueue;
    cycles currentCycle = 0;
};

}  // namespace Core
}  // namespace RedPandaDS

#endif
