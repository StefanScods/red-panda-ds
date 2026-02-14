#ifndef INTERCONNECT_H
#define INTERCONNECT_H

#include <cstdint>

#include "memoryDefines.h"

// Forward declarations.
class ARM;

/**
 * @brief Class which implements the interface between compents within the emulator.
 */
class Interconnect {
private:
    ARM* arm7 = nullptr;
    ARM* arm9 = nullptr;

    uint8_t* mainRAM = nullptr;

public:
    Interconnect();
    ~Interconnect();

    /**
     * @brief Set up the component.
     */
    void init();

    /**
     * @brief Add the ARM7 cpu to the interconnect.
     *
     * @param cpu The ARM7 cpu to add
     */
    void bindARM7(ARM* cpu);
    /**
     * @brief Add the ARM9 cpu to the interconnect.
     *
     * @param cpu The ARM9 cpu to add
     */
    void bindARM9(ARM* cpu);

    /**
     * @brief Read 32 bits from the specified address.
     *
     * @param addr Address to read from
     *
     * @return uint32_t
     */
    uint32_t read32ARM7(uint32_t addr);
    /**
     * @brief Read 16 bits from the specified address.
     *
     * @param addr Address to read from
     *
     * @return uint16_t
     */
    uint16_t read16ARM7(uint32_t addr);
    /**
     * @brief Read 8 bits from the specified address.
     *
     * @param addr Address to read from
     *
     * @return uint8_t
     */
    uint8_t read8ARM7(uint32_t addr);
    /**
     * @brief Write 32 bits to the specified address.
     *
     * @param addr Address to write to
     * @param data Data to write
     */
    void write32ARM7(uint32_t addr, uint32_t data);
    /**
     * @brief Write 16 bits to the specified address.
     *
     * @param addr Address to write to
     * @param data Data to write
     */
    void write16ARM7(uint32_t addr, uint16_t data);
    /**
     * @brief Write 8 bits to the specified address.
     *
     * @param addr Address to write to
     * @param data Data to write
     */
    void write8ARM7(uint32_t addr, uint8_t data);
    /**
     * @brief Read 32 bits from the specified address.
     *
     * @param addr Address to read from
     *
     * @return uint32_t
     */
    uint32_t read32ARM9(uint32_t addr);
    /**
     * @brief Read 16 bits from the specified address.
     *
     * @param addr Address to read from
     *
     * @return uint16_t
     */
    uint16_t read16ARM9(uint32_t addr);
    /**
     * @brief Read 8 bits from the specified address.
     *
     * @param addr Address to read from
     *
     * @return uint8_t
     */
    uint8_t read8ARM9(uint32_t addr);
    /**
     * @brief Write 32 bits to the specified address.
     *
     * @param addr Address to write to
     * @param data Data to write
     */
    void write32ARM9(uint32_t addr, uint32_t data);
    /**
     * @brief Write 16 bits to the specified address.
     *
     * @param addr Address to write to
     * @param data Data to write
     */
    void write16ARM9(uint32_t addr, uint16_t data);
    /**
     * @brief Write 8 bits to the specified address.
     *
     * @param addr Address to write to
     * @param data Data to write
     */
    void write8ARM9(uint32_t addr, uint8_t data);
};
#endif