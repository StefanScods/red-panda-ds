#include "interconnect.h"

#include "cpu.h"

// Control print statements.
#define LOG_LEVEL 2
#include <common.h>

#include "logger.h"

namespace RedPandaDS {
namespace Core {

// ==================================================================================================
Interconnect::Interconnect() {
}
// ==================================================================================================
Interconnect::~Interconnect() {
    arm7 = nullptr;
    arm9 = nullptr;

    DELETE_DYNAMIC_ARRAY_POINTER(mainRAM);
    DELETE_DYNAMIC_ARRAY_POINTER(sharedWRAM);
    DELETE_DYNAMIC_ARRAY_POINTER(arm7WRAM);
}
// ==================================================================================================
void Interconnect::init() {
    // Malloc Data.

    // RAM.
    mainRAM = new uint8_t[MAIN_RAM_SIZE];
    if (mainRAM == nullptr) {
        LogError("Could not malloc main RAM!");
        exit(1);
    }
    arm7WRAM = new uint8_t[ARM7_WRAM_SIZE];
    if (arm7WRAM == nullptr) {
        LogError("Could not malloc ARM7 Work RAM!");
        exit(1);
    }
}
// ==================================================================================================
void Interconnect::bindARM7(ARM* cpu) {
    arm7 = cpu;
    arm7->connectToInterconnect(this);
}
// ==================================================================================================
void Interconnect::bindARM9(ARM* cpu) {
    arm9 = cpu;
    arm9->connectToInterconnect(this);
}
// ==================================================================================================
uint32_t Interconnect::read32ARM7(uint32_t addr) {
    // Word align mem accesses.
    addr &= ~0x3;
    // Memory map.
    uint8_t memRegion = addr >> 24;
    switch (memRegion) {
        case (ARM7MemoryRegionNum::MAIN_RAM): {
            uint32_t val = *reinterpret_cast<uint32_t*>(mainRAM + (addr & MAIN_RAM_MASK));
            LogDebug("Main RAM - 32 bit read at " << hexString(addr) << ": " << hexString(val));
            return val;
        }
        case (ARM7MemoryRegionNum::WRAM): {
            if (addr >= ARM7_WRAM_START && addr < ARM7_WRAM_START + ARM7_WRAM_SIZE) {
                uint32_t val = *reinterpret_cast<uint32_t*>(arm7WRAM + (addr & ARM7_WRAM_MASK));
                LogDebug("ARM7 WRAM - 32 bit read at " << hexString(addr) << ": "
                                                       << hexString(val));
                return val;
            }
        }
        case (ARM7MemoryRegionNum::IO): {
            return read32IOARM7(addr);
        }

        default:
            LogError("Unsupported memory access at " << hexString(addr));
            return INVALID_MEM_32BIT;
    }
    // Unreachable.
    return INVALID_MEM_32BIT;
}
// ==================================================================================================
uint16_t Interconnect::read16ARM7(uint32_t addr) {
    // Half word align mem accesses.
    addr &= ~0x1;
    // Memory map.
    uint8_t memRegion = addr >> 24;
    switch (memRegion) {
        case (ARM7MemoryRegionNum::MAIN_RAM): {
            uint16_t val = *reinterpret_cast<uint16_t*>(mainRAM + (addr & MAIN_RAM_MASK));
            LogDebug("Main RAM - 16 bit read at " << hexString(addr) << ": " << hexString(val));
            return val;
        }
        case (ARM7MemoryRegionNum::WRAM): {
            if (addr >= ARM7_WRAM_START && addr < ARM7_WRAM_START + ARM7_WRAM_SIZE) {
                uint16_t val = *reinterpret_cast<uint16_t*>(arm7WRAM + (addr & ARM7_WRAM_MASK));
                LogDebug("ARM7 WRAM - 16 bit read at " << hexString(addr) << ": "
                                                       << hexString(val));
                return val;
            }
        }
        case (ARM7MemoryRegionNum::IO): {
            return read16IOARM7(addr);
        }
        default:
            LogError("Unsupported memory access at " << hexString(addr));
            return INVALID_MEM_16BIT;
    }
    // Unreachable.
    return INVALID_MEM_16BIT;
}
// ==================================================================================================
uint8_t Interconnect::read8ARM7(uint32_t addr) {
    // Memory map.
    uint8_t memRegion = addr >> 24;
    switch (memRegion) {
        case (ARM7MemoryRegionNum::MAIN_RAM): {
            uint8_t val = *(mainRAM + (addr & MAIN_RAM_MASK));
            LogDebug("Main RAM - 8 bit read at " << hexString(addr) << ": " << hexString(val));
            return val;
        }
        case (ARM7MemoryRegionNum::WRAM): {
            if (addr >= ARM7_WRAM_START && addr < ARM7_WRAM_START + ARM7_WRAM_SIZE) {
                uint8_t val = *(arm7WRAM + (addr & ARM7_WRAM_MASK));
                LogDebug("ARM7 WRAM - 8 bit read at " << hexString(addr) << ": " << hexString(val));
                return val;
            }
        }
        case (ARM7MemoryRegionNum::IO): {
            return read8IOARM7(addr);
        }
        default:
            LogError("Unsupported memory access at " << hexString(addr));
            return INVALID_MEM_8BIT;
    }
    // Unreachable.
    return INVALID_MEM_8BIT;
}
// ==================================================================================================
void Interconnect::write32ARM7(uint32_t addr, uint32_t data) {
    // Byte align mem accesses.
    addr &= ~0x3;
    // Memory map.
    uint8_t memRegion = addr >> 24;
    switch (memRegion) {
        case (ARM7MemoryRegionNum::MAIN_RAM): {
            *reinterpret_cast<uint32_t*>(mainRAM + (addr & MAIN_RAM_MASK)) = data;
            LogDebug("Main RAM - 32 bit write at " << hexString(addr) << ": " << hexString(data));
            break;
        }
        case (ARM7MemoryRegionNum::WRAM): {
            if (addr >= ARM7_WRAM_START && addr < ARM7_WRAM_START + ARM7_WRAM_SIZE) {
                *reinterpret_cast<uint32_t*>(arm7WRAM + (addr & ARM7_WRAM_MASK)) = data;
                LogDebug("ARM7 WRAM - 32 bit write at " << hexString(addr) << ": "
                                                        << hexString(data));
                break;
            }
        }
        case (ARM7MemoryRegionNum::IO): {
            write32IOARM7(addr, data);
            break;
        }

        default:
            LogError("Unsupported memory access at " << hexString(addr));
            break;
    }
}
// ==================================================================================================
void Interconnect::write16ARM7(uint32_t addr, uint16_t data) {
    // Byte align mem accesses.
    addr &= ~0x1;
    // Memory map.
    uint8_t memRegion = addr >> 24;
    switch (memRegion) {
        case (ARM7MemoryRegionNum::MAIN_RAM): {
            *reinterpret_cast<uint16_t*>(mainRAM + (addr & MAIN_RAM_MASK)) = data;
            LogDebug("Main RAM - 16 bit write at " << hexString(addr) << ": " << hexString(data));
            break;
        }
        case (ARM7MemoryRegionNum::WRAM): {
            if (addr >= ARM7_WRAM_START && addr < ARM7_WRAM_START + ARM7_WRAM_SIZE) {
                *reinterpret_cast<uint16_t*>(arm7WRAM + (addr & ARM7_WRAM_MASK)) = data;
                LogDebug("ARM7 WRAM - 16 bit write at " << hexString(addr) << ": "
                                                        << hexString(data));
                break;
            }
        }
        case (ARM7MemoryRegionNum::IO): {
            write16IOARM7(addr, data);
            break;
        }

        default:
            LogError("Unsupported memory access at " << hexString(addr));
            break;
    }
}
// ==================================================================================================
void Interconnect::write8ARM7(uint32_t addr, uint8_t data) {
    // Memory map.
    uint8_t memRegion = addr >> 24;
    switch (memRegion) {
        case (ARM7MemoryRegionNum::MAIN_RAM): {
            *(mainRAM + (addr & MAIN_RAM_MASK)) = data;
            LogDebug("Main RAM - 8 bit write at " << hexString(addr) << ": " << hexString(data));
            break;
        }
        case (ARM7MemoryRegionNum::WRAM): {
            if (addr >= ARM7_WRAM_START && addr < ARM7_WRAM_START + ARM7_WRAM_SIZE) {
                *(arm7WRAM + (addr & ARM7_WRAM_MASK)) = data;
                LogDebug("ARM7 WRAM - 8 bit write at " << hexString(addr) << ": "
                                                       << hexString(data));
                break;
            }
        }
        case (ARM7MemoryRegionNum::IO): {
            write8IOARM7(addr, data);
            break;
        }

        default:
            LogError("Unsupported memory access at " << hexString(addr));
            break;
    }
}
// ==================================================================================================
uint32_t Interconnect::read32ARM9(uint32_t addr) {
    // Word align mem accesses.
    addr &= ~0x3;
    // Memory map.
    uint8_t memRegion = addr >> 24;
    switch (memRegion) {
        case (ARM9MemoryRegionNum::MAIN_RAM): {
            uint32_t val = *reinterpret_cast<uint32_t*>(mainRAM + (addr & MAIN_RAM_MASK));
            LogDebug("Main RAM - 32 bit read at " << hexString(addr) << ": " << hexString(val));
            return val;
        }
        case (ARM9MemoryRegionNum::IO): {
            return read32IOARM9(addr);
        }

        default:
            LogError("Unsupported memory access at " << hexString(addr));
            return INVALID_MEM_32BIT;
    }
    // Unreachable.
    return INVALID_MEM_32BIT;
}
// ==================================================================================================
uint16_t Interconnect::read16ARM9(uint32_t addr) {
    // Half word align mem accesses.
    addr &= ~0x1;
    // Memory map.
    uint8_t memRegion = addr >> 24;
    switch (memRegion) {
        case (ARM9MemoryRegionNum::MAIN_RAM): {
            uint16_t val = *reinterpret_cast<uint16_t*>(mainRAM + (addr & MAIN_RAM_MASK));
            LogDebug("Main RAM - 16 bit read at " << hexString(addr) << ": " << hexString(val));
            return val;
        }
        case (ARM9MemoryRegionNum::IO): {
            return read16IOARM9(addr);
        }
        default:
            LogError("Unsupported memory access at " << hexString(addr));
            return INVALID_MEM_16BIT;
    }
    // Unreachable.
    return INVALID_MEM_16BIT;
}
// ==================================================================================================
uint8_t Interconnect::read8ARM9(uint32_t addr) {
    // Memory map.
    uint8_t memRegion = addr >> 24;
    switch (memRegion) {
        case (ARM9MemoryRegionNum::MAIN_RAM): {
            uint8_t val = *(mainRAM + (addr & MAIN_RAM_MASK));
            LogDebug("Main RAM - 8 bit read at " << hexString(addr) << ": " << hexString(val));
            return val;
        }
        case (ARM9MemoryRegionNum::IO): {
            return read8IOARM9(addr);
        }
        default:
            LogError("Unsupported memory access at " << hexString(addr));
            return INVALID_MEM_8BIT;
    }
    // Unreachable.
    return INVALID_MEM_8BIT;
}
// ==================================================================================================
void Interconnect::write32ARM9(uint32_t addr, uint32_t data) {
    // Byte align mem accesses.
    addr &= ~0x3;
    // Memory map.
    uint8_t memRegion = addr >> 24;
    switch (memRegion) {
        case (ARM9MemoryRegionNum::MAIN_RAM): {
            *reinterpret_cast<uint32_t*>(mainRAM + (addr & MAIN_RAM_MASK)) = data;
            LogDebug("Main RAM - 32 bit write at " << hexString(addr) << ": " << hexString(data));
            break;
        }
        case (ARM9MemoryRegionNum::IO): {
            write32IOARM9(addr, data);
            break;
        }

        default:
            LogError("Unsupported memory access at " << hexString(addr));
            break;
    }
}
// ==================================================================================================
void Interconnect::write16ARM9(uint32_t addr, uint16_t data) {
    // Byte align mem accesses.
    addr &= ~0x1;
    // Memory map.
    uint8_t memRegion = addr >> 24;
    switch (memRegion) {
        case (ARM9MemoryRegionNum::MAIN_RAM): {
            *reinterpret_cast<uint16_t*>(mainRAM + (addr & MAIN_RAM_MASK)) = data;
            LogDebug("Main RAM - 16 bit write at " << hexString(addr) << ": " << hexString(data));
            break;
        }
        case (ARM9MemoryRegionNum::IO): {
            write16IOARM9(addr, data);
            break;
        }

        default:
            LogError("Unsupported memory access at " << hexString(addr));
            break;
    }
}
// ==================================================================================================
void Interconnect::write8ARM9(uint32_t addr, uint8_t data) {
    // Memory map.
    uint8_t memRegion = addr >> 24;
    switch (memRegion) {
        case (ARM9MemoryRegionNum::MAIN_RAM): {
            *(mainRAM + (addr & MAIN_RAM_MASK)) = data;
            LogDebug("Main RAM - 8 bit write at " << hexString(addr) << ": " << hexString(data));
            break;
        }
        case (ARM9MemoryRegionNum::IO): {
            write8IOARM9(addr, data);
            break;
        }

        default:
            LogError("Unsupported memory access at " << hexString(addr));
            break;
    }
}
// ==================================================================================================
bool Interconnect::isAddressValidARM7(uint32_t addr) {
    uint8_t memRegion = addr >> 24;
    switch (memRegion) {
        case (ARM7MemoryRegionNum::MAIN_RAM): {
            return true;
        }
        case (ARM7MemoryRegionNum::WRAM): {
            if (addr >= ARM7_WRAM_START && addr < ARM7_WRAM_START + ARM7_WRAM_SIZE) {
                return true;
            }
            return false;
        }
        case (ARM7MemoryRegionNum::IO): {
            return isIOAddressValidARM7(addr);
        }
        default:
            return false;
    }
    return false;
}
// ==================================================================================================
bool Interconnect::isAddressValidARM9(uint32_t addr) {
    uint8_t memRegion = addr >> 24;
    switch (memRegion) {
        case (ARM9MemoryRegionNum::MAIN_RAM): {
            return true;
        }
        case (ARM9MemoryRegionNum::IO): {
            return isIOAddressValidARM9(addr);
        }
        default:
            return false;
    }
    return false;
}
// ==================================================================================================
void Interconnect::loadProgramFromROM(uint32_t destAddr, uint32_t size, uint32_t srcAddr,
                                      NDS_Cartridge* cart) {
    uint8_t memRegion = destAddr >> 24;
    if (memRegion != ARM7MemoryRegionNum::MAIN_RAM) {
        LogError("Trying to copy program into memory region other than main RAM! Target address: "
                 << PrintHex(destAddr));
        return;
    }
    LogDebug("Reading " << size << "byte(s) from ROM address " << PrintHex(srcAddr)
                        << " into RAM address " << PrintHex(destAddr));
    cart->readFromROM(srcAddr, size, (mainRAM + (destAddr & MAIN_RAM_MASK)));
}
// ==================================================================================================

}  // namespace Core
}  // namespace RedPandaDS
