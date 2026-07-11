#ifndef BIOS_H
#define BIOS_H

#include "types.h"

namespace RedPandaDS {
namespace Core {

class ARM;

/**
 * @brief Virtual BIOS class.
 */
class BIOS {
public:
    BIOS();
    ~BIOS();

    /**
     * @brief Sets cpu `bus` property to the supplied argument. This function must be called
     * before any execution takes place.
     *
     * @param target The ARM cpu to "connect" to.
     */
    void bindCPU(ARM* target) { cpu = target; }

    /**
     * @brief Handle a software interrupt call.
     *
     * @param vector The immediate argument of the SWI (SVC) call.
     * @return cycles
     */
    virtual cycles handleSWI(uint32_t vector) = 0;

protected:
    ARM* cpu = nullptr;
};

/**
 * @brief Virtual NDS BIOS class.
 */
class NDS_BIOS : public BIOS {
public:
    NDS_BIOS();
    ~NDS_BIOS();

    /**
     * @brief Helper function for cleaning up the instruction pipeline and rebuilding registers.
     */
    void cleanup();

    /**
     * @brief Invalid SWI handler.
     *
     * @param vector The immediate argument of the SWI (SVC) call.
     *
     * @return cycles
     */
    cycles unknownSWI(uint32_t vector);

    // Emulated SWI handlers.
    cycles WaitByLoop();
    cycles IntrWait();
};
/**
 * @brief NDS's ARM9 BIOS emulation model.
 */
class NDS_ARM9_BIOS : public NDS_BIOS {
public:
    NDS_ARM9_BIOS();
    ~NDS_ARM9_BIOS();

    // Overrides.
    cycles handleSWI(uint32_t vector) override;
};

/**
 * @brief NDS's ARM7 BIOS emulation model.
 */
class NDS_ARM7_BIOS : public NDS_BIOS {
public:
    NDS_ARM7_BIOS();
    ~NDS_ARM7_BIOS();

    // Overrides.
    cycles handleSWI(uint32_t vector) override;
};

}  // namespace Core
}  // namespace RedPandaDS

#endif
