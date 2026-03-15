#ifndef CORE_H
#define CORE_H

#include "cpu.h"
#include "interconnect.h"

namespace RedPandaDS {
namespace Core {

class DSEmuCore {
public:
    DSEmuCore();
    ~DSEmuCore();

    // Component Accessors.
    Interconnect* getInterconnect() const { return bus; }
    ARM7TDMI* getARM7Core() const { return arm7; }
    ARM946ES* getARM9Core() const { return arm9; }

    /**
     * @brief Initializes the core.
     */
    void init();

    /**
     * @brief Resets the core.
     */
    void reset();

private:
    Interconnect* bus = nullptr;
    ARM7TDMI* arm7 = nullptr;
    ARM946ES* arm9 = nullptr;
};

}  // namespace Core
}  // namespace RedPandaDS

#endif
