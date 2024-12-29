#ifndef CPU_H
#define CPU_H

#include <cstdint>

#include "types.h"
#include "utils.h"

// Forward declarations.
class Interconnect;

// TODO!!! Make sure this is an invalid instruction for both CPUs
#define NO_INSTRUCT 0b11110111111111111111111111111111

#define NUM_OF_STANDARD_REGISTERS 16
#define NUM_OF_FIQ_REGISTERS 8
#define NUM_OF_SVC_REGISTERS 3
#define NUM_OF_ABT_REGISTERS 3
#define NUM_OF_IRQ_REGISTERS 3
#define NUM_OF_UND_REGISTERS 3

#define INSTUCTION_PIPELINE_LENGTH 3

#define PC_REGISTER_NUM 15
/**
 * @brief The base CPU object to implement common logic shared between the NDS's
 * two CPUs.
 */
class ARM {
protected:
    Interconnect* bus = nullptr;

    uint32_t reg[NUM_OF_STANDARD_REGISTERS];
    uint32_t cpsr;
    // Banked registers.
    uint32_t regFIQ[NUM_OF_FIQ_REGISTERS];
    uint32_t regSVC[NUM_OF_SVC_REGISTERS];
    uint32_t regABT[NUM_OF_ABT_REGISTERS];
    uint32_t regIRQ[NUM_OF_IRQ_REGISTERS];
    uint32_t regUND[NUM_OF_UND_REGISTERS];

    // Program counter maps to the 15th standard register in all modes.
    uint32_t& pc = reg[PC_REGISTER_NUM];

    // Keep track of the component's cycle time.
    cycles fetchCooldown = 0;
    cycles executeCooldown = 0;
    cycles cyclesElapsed = 0;
    cycles currentCycle = 0;
    cycles targetCycle = 0;

    bool shouldFetchInst = false;

    uint32_t instuctionPipeLine[INSTUCTION_PIPELINE_LENGTH];

    /**
     * @brief adds the current value of `cyclesElapsed` to the `currentCycle` and reduces fetch
     * and execute cooldowns. Zeros `cyclesElapsed`.
     *
     */
    inline void addCyclesElapsed() {
        currentCycle += cyclesElapsed;
        fetchCooldown = underflowSafeSubtract(fetchCooldown, cyclesElapsed);
        executeCooldown = underflowSafeSubtract(executeCooldown, cyclesElapsed);
        cyclesElapsed = 0;
    }

public:
    ARM();
    ~ARM();

    /**
     * @brief Set all vars back to their initial value.
     */
    void reset();

    /**
     * @brief
     *
     * @return cycles
     */
    virtual cycles execute();
    virtual cycles fetch();
    virtual cycles cycle();

    /**
     * @brief Sets the `bus` property to the supplied argument. This function must be called
     * before any execution takes place.
     *
     * @param d_bus The interconnect to "connect" to.
     */
    void connectToInterconnect(Interconnect* d_bus) { bus = d_bus; }

    /**
     * @brief Sets the target number of cycles -> determines how long to execute for.
     *
     * @param target Target number of cycles to set this component too.
     */
    void setTargetCycle(cycles target) { targetCycle = target; }

    /**
     * @brief Reads 16 bits from the specified address.
     *
     * @param addr Address to read
     * @return uint16_t
     */
    virtual uint16_t read16Bits(uint32_t addr);
    /**
     * @brief Reads 32 bits from the specified address.
     *
     * @param addr Address to read
     * @return uint32_t
     */
    virtual uint32_t read32Bits(uint32_t addr);

    /**
     * @brief Writes 16 bits to the specified address.
     *
     * @param addr Address to write
     * @param data Data to write
     */
    virtual void write16Bits(uint32_t addr, uint16_t data);
    /**
     * @brief Writes 32 bits to the specified address.
     *
     * @param addr Address to write
     * @param data Data to write
     */
    virtual void write32Bits(uint32_t addr, uint32_t data);
};

/**
 * @brief CPU model for the ARM7TDMI. The ARM7TDMI is mainly responsible for handling NDS I/O and
 * is the primary processor of the GBA. CPU targets the ARMv4 instruction set.
 */
class ARM7TDMI : public ARM {
private:
public:
    ARM7TDMI();
    ~ARM7TDMI();

    // Function overrides.
    cycles execute() override;
    cycles fetch() override;
    cycles cycle() override;
    uint16_t read16Bits(uint32_t addr) override;
    uint32_t read32Bits(uint32_t addr) override;
    void write16Bits(uint32_t addr, uint16_t data) override;
    void write32Bits(uint32_t addr, uint32_t data) override;
};

/**
 * @brief CPU model for the ARM946E-S. The ARM946E-S is NDS primary processor. CPU targets the
 * ARMv5TE instuction set.
 */
class ARM946ES : public ARM {
private:
public:
    ARM946ES();
    ~ARM946ES();
};

#endif