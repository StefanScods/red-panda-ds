#ifndef INTERCONNECT_H
#define INTERCONNECT_H

#include <cstdint>

#include "cartridge.h"
#include "memoryDefines.h"

namespace RedPandaDS {
namespace Core {

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
     * @brief Read 32 bits from the specified IO address.
     *
     * @param addr Address to read from
     *
     * @return uint32_t
     */
    uint32_t read32IOARM7(uint32_t addr);
    /**
     * @brief Read 16 bits from the specified address.
     *
     * @param addr Address to read from
     *
     * @return uint16_t
     */
    uint16_t read16ARM7(uint32_t addr);
    /**
     * @brief Read 16 bits from the specified IO address.
     *
     * @param addr Address to read from
     *
     * @return uint16_t
     */
    uint16_t read16IOARM7(uint32_t addr);
    /**
     * @brief Read 8 bits from the specified address.
     *
     * @param addr Address to read from
     *
     * @return uint8_t
     */
    uint8_t read8ARM7(uint32_t addr);
    /**
     * @brief Read 8 bits from the specified IO address.
     *
     * @param addr Address to read from
     *
     * @return uint8_t
     */
    uint8_t read8IOARM7(uint32_t addr);
    /**
     * @brief Write 32 bits to the specified address.
     *
     * @param addr Address to write to
     * @param data Data to write
     */
    void write32ARM7(uint32_t addr, uint32_t data);
    /**
     * @brief Write 32 bits to the specified IO address.
     *
     * @param addr Address to write to
     * @param data Data to write
     */
    void write32IOARM7(uint32_t addr, uint32_t data);
    /**
     * @brief Write 16 bits to the specified address.
     *
     * @param addr Address to write to
     * @param data Data to write
     */
    void write16ARM7(uint32_t addr, uint16_t data);
    /**
     * @brief Write 16 bits to the specified IO address.
     *
     * @param addr Address to write to
     * @param data Data to write
     */
    void write16IOARM7(uint32_t addr, uint16_t data);
    /**
     * @brief Write 8 bits to the specified address.
     *
     * @param addr Address to write to
     * @param data Data to write
     */
    void write8ARM7(uint32_t addr, uint8_t data);
    /**
     * @brief Write 8 bits to the specified IO address.
     *
     * @param addr Address to write to
     * @param data Data to write
     */
    void write8IOARM7(uint32_t addr, uint8_t data);
    /**
     * @brief Read 32 bits from the specified address.
     *
     * @param addr Address to read from
     *
     * @return uint32_t
     */
    uint32_t read32ARM9(uint32_t addr);
    /**
     * @brief Read 32 bits from the specified IO address.
     *
     * @param addr Address to read from
     *
     * @return uint32_t
     */
    uint32_t read32IOARM9(uint32_t addr);
    /**
     * @brief Read 16 bits from the specified address.
     *
     * @param addr Address to read from
     *
     * @return uint16_t
     */
    uint16_t read16ARM9(uint32_t addr);
    /**
     * @brief Read 16 bits from the specified IO address.
     *
     * @param addr Address to read from
     *
     * @return uint16_t
     */
    uint16_t read16IOARM9(uint32_t addr);
    /**
     * @brief Read 8 bits from the specified address.
     *
     * @param addr Address to read from
     *
     * @return uint8_t
     */
    uint8_t read8ARM9(uint32_t addr);
    /**
     * @brief Read 8 bits from the specified IO address.
     *
     * @param addr Address to read from
     *
     * @return uint8_t
     */
    uint8_t read8IOARM9(uint32_t addr);
    /**
     * @brief Write 32 bits to the specified address.
     *
     * @param addr Address to write to
     * @param data Data to write
     */
    void write32ARM9(uint32_t addr, uint32_t data);
    /**
     * @brief Write 32 bits to the specified IO address.
     *
     * @param addr Address to write to
     * @param data Data to write
     */
    void write32IOARM9(uint32_t addr, uint32_t data);
    /**
     * @brief Write 16 bits to the specified address.
     *
     * @param addr Address to write to
     * @param data Data to write
     */
    void write16ARM9(uint32_t addr, uint16_t data);
    /**
     * @brief Write 16 bits to the specified IO address.
     *
     * @param addr Address to write to
     * @param data Data to write
     */
    void write16IOARM9(uint32_t addr, uint16_t data);
    /**
     * @brief Write 8 bits to the specified address.
     *
     * @param addr Address to write to
     * @param data Data to write
     */
    void write8ARM9(uint32_t addr, uint8_t data);
    /**
     * @brief Write 8 bits to the specified IO address.
     *
     * @param addr Address to write to
     * @param data Data to write
     */
    void write8IOARM9(uint32_t addr, uint8_t data);
    /**
     * @brief Check to see if an address is valid for the ARM7.
     *
     * @param addr Address to check
     */
    bool isAddressValidARM7(uint32_t addr);
    /**
     * @brief Check to see if an IO address is valid for the ARM7.
     *
     * @param addr Address to check
     */
    bool isIOAddressValidARM7(uint32_t addr);
    /**
     * @brief Check to see if an address is valid for the ARM9.
     *
     * @param addr Address to check
     */
    bool isAddressValidARM9(uint32_t addr);
    /**
     * @brief Check to see if an IO address is valid for the ARM9.
     *
     * @param addr Address to check
     */
    bool isIOAddressValidARM9(uint32_t addr);

    /**
     * @brief Loads a section of program into RAM from the ROM.
     *
     * @param destAddr The target address (within RAM) to place the program.
     * @param size The size of the program to copy.
     * @param srcAddr The address in ROM where the program segment starts.
     * @param cart The cartridge holding the ROM.
     */
    void loadProgramFromROM(uint32_t destAddr, uint32_t size, uint32_t srcAddr,
                            NDS_Cartridge* cart);
};

}  // namespace Core
}  // namespace RedPandaDS

#endif
