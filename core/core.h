#ifndef CORE_H
#define CORE_H

#include <queue>

#include "cpu.h"
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

    cycles processNextEvent();

private:
    // Components.
    Interconnect* bus = nullptr;
    ARM7TDMI* arm7 = nullptr;
    ARM946ES* arm9 = nullptr;
    NDS_LCD* ndsLCD = nullptr;

    // Event queue and cycle counter.
    std::priority_queue<coreEvent> eventQueue;
    cycles currentCycle = 0;
};

}  // namespace Core
}  // namespace RedPandaDS

#endif
