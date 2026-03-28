#include "interconnect.h"

#include "cpu.h"

// Control print statements.
#define LOG_LEVEL 2
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

    if (mainRAM != nullptr) {
        delete[] mainRAM;
        mainRAM = nullptr;
    }
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
        default:
            return false;
    }
    return false;
}
// ==================================================================================================
bool Interconnect::isAddressValidARM9(uint32_t addr) {
    uint8_t memRegion = addr >> 24;
    switch (memRegion) {
        case (ARM7MemoryRegionNum::MAIN_RAM): {
            return true;
        }
        default:
            return false;
    }
    return false;
}
// ==================================================================================================

}  // namespace Core
}  // namespace RedPandaDS
