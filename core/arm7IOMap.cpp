#include "cpu.h"
#include "interconnect.h"

// Control print statements.
#define LOG_LEVEL 2
#include "logger.h"

namespace RedPandaDS {
namespace Core {

namespace ARM7IOMemoryAddress {
enum ARM7IOMemoryAddress : uint32_t {
    IME = 0x04000208,
    IE = 0x04000210,
    IF = 0x04000214,
};
}

// ==================================================================================================
bool Interconnect::isIOAddressValidARM7(uint32_t addr) {
    // Word align mem accesses.
    addr &= ~0x3;
    // IO memory map.
    switch (addr) {
        case ARM7IOMemoryAddress::IME:
        case ARM7IOMemoryAddress::IE:
        case ARM7IOMemoryAddress::IF:
            return true;
        default:
            return false;
    }
    return false;
}
// ==================================================================================================
uint32_t Interconnect::read32IOARM7(uint32_t addr) {
    // Word align mem accesses.
    addr &= ~0x3;
    // IO memory map.
    switch (addr) {
        case ARM7IOMemoryAddress::IME: {
            return arm7->readIME();
        }
        case ARM7IOMemoryAddress::IE: {
            return arm7->readIE();
        }
        case ARM7IOMemoryAddress::IF: {
            return arm7->readIF();
        }
        default:
            LogError("Unsupported IO access at " << hexString(addr));
            return INVALID_MEM_32BIT;
    }
    return INVALID_MEM_32BIT;
}
// ==================================================================================================
uint16_t Interconnect::read16IOARM7(uint32_t addr) {
    // Half word align mem accesses.
    addr &= ~0x1;
    // IO memory map.
    switch (addr) {
        case ARM7IOMemoryAddress::IME: {
            return readBits(arm7->readIME(), 0, 15);
        }
        case ARM7IOMemoryAddress::IME + 2: {
            return readBits(arm7->readIME(), 16, 31);
        }
        case ARM7IOMemoryAddress::IE: {
            return readBits(arm7->readIE(), 0, 15);
        }
        case ARM7IOMemoryAddress::IE + 2: {
            return readBits(arm7->readIE(), 16, 31);
        }
        case ARM7IOMemoryAddress::IF: {
            return readBits(arm7->readIF(), 0, 15);
        }
        case ARM7IOMemoryAddress::IF + 2: {
            return readBits(arm7->readIF(), 16, 31);
        }
        default:
            LogError("Unsupported IO access at " << hexString(addr));
            return INVALID_MEM_16BIT;
    }
    return INVALID_MEM_16BIT;
}
// ==================================================================================================
uint8_t Interconnect::read8IOARM7(uint32_t addr) {
    // IO memory map.
    switch (addr) {
        case ARM7IOMemoryAddress::IME: {
            return readBits(arm7->readIME(), 0, 7);
        }
        case ARM7IOMemoryAddress::IME + 1: {
            return readBits(arm7->readIME(), 8, 15);
        }
        case ARM7IOMemoryAddress::IME + 2: {
            return readBits(arm7->readIME(), 16, 23);
        }
        case ARM7IOMemoryAddress::IME + 3: {
            return readBits(arm7->readIME(), 24, 31);
        }
        case ARM7IOMemoryAddress::IE: {
            return readBits(arm7->readIE(), 0, 7);
        }
        case ARM7IOMemoryAddress::IE + 1: {
            return readBits(arm7->readIE(), 8, 15);
        }
        case ARM7IOMemoryAddress::IE + 2: {
            return readBits(arm7->readIE(), 16, 23);
        }
        case ARM7IOMemoryAddress::IE + 3: {
            return readBits(arm7->readIE(), 24, 31);
        }
        case ARM7IOMemoryAddress::IF: {
            return readBits(arm7->readIF(), 0, 7);
        }
        case ARM7IOMemoryAddress::IF + 1: {
            return readBits(arm7->readIF(), 8, 15);
        }
        case ARM7IOMemoryAddress::IF + 2: {
            return readBits(arm7->readIF(), 16, 23);
        }
        case ARM7IOMemoryAddress::IF + 3: {
            return readBits(arm7->readIF(), 24, 31);
        }
        default:
            LogError("Unsupported IO access at " << hexString(addr));
            return INVALID_MEM_8BIT;
    }
    return INVALID_MEM_8BIT;
}
// ==================================================================================================
void Interconnect::write32IOARM7(uint32_t addr, uint32_t data) {
    // Word align mem accesses.
    addr &= ~0x3;
    // IO memory map.
    switch (addr) {
        case ARM7IOMemoryAddress::IME: {
            return arm7->writeIME(data);
        }
        case ARM7IOMemoryAddress::IE: {
            return arm7->writeIE(data);
        }
        case ARM7IOMemoryAddress::IF: {
            return arm7->writeIF(data);
        }
        default:
            LogError("Unsupported IO access at " << hexString(addr));
            break;
    }
}
// ==================================================================================================
void Interconnect::write16IOARM7(uint32_t addr, uint16_t data) {
    // Half word align mem accesses.
    addr &= ~0x1;
    // IO memory map.
    switch (addr) {
        case ARM7IOMemoryAddress::IME: {
            uint32_t val = arm7->readIME();
            writeBits(val, (uint32_t)data, 0, 15);
            arm7->writeIME(val);
            break;
        }
        case ARM7IOMemoryAddress::IME + 2: {
            uint32_t val = arm7->readIME();
            writeBits(val, (uint32_t)data, 16, 31);
            arm7->writeIME(val);
            break;
        }
        case ARM7IOMemoryAddress::IE: {
            uint32_t val = arm7->readIE();
            writeBits(val, (uint32_t)data, 0, 15);
            arm7->writeIE(val);
            break;
        }
        case ARM7IOMemoryAddress::IE + 2: {
            uint32_t val = arm7->readIE();
            writeBits(val, (uint32_t)data, 16, 31);
            arm7->writeIE(val);
            break;
        }
        case ARM7IOMemoryAddress::IF: {
            uint32_t val = arm7->readIF();
            writeBits(val, (uint32_t)data, 0, 15);
            arm7->writeIF(val);
            break;
        }
        case ARM7IOMemoryAddress::IF + 2: {
            uint32_t val = arm7->readIF();
            writeBits(val, (uint32_t)data, 16, 31);
            arm7->writeIF(val);
            break;
        }
        default:
            LogError("Unsupported IO access at " << hexString(addr));
            break;
    }
}
// ==================================================================================================
void Interconnect::write8IOARM7(uint32_t addr, uint8_t data) {
    // IO memory map.
    switch (addr) {
        case ARM7IOMemoryAddress::IME: {
            uint32_t val = arm7->readIME();
            writeBits(val, (uint32_t)data, 0, 7);
            arm7->writeIME(val);
            break;
        }
        case ARM7IOMemoryAddress::IME + 1: {
            uint32_t val = arm7->readIME();
            writeBits(val, (uint32_t)data, 8, 15);
            arm7->writeIME(val);
            break;
        }
        case ARM7IOMemoryAddress::IME + 2: {
            uint32_t val = arm7->readIME();
            writeBits(val, (uint32_t)data, 16, 23);
            arm7->writeIME(val);
            break;
        }
        case ARM7IOMemoryAddress::IME + 3: {
            uint32_t val = arm7->readIME();
            writeBits(val, (uint32_t)data, 24, 31);
            arm7->writeIME(val);
            break;
        }
        case ARM7IOMemoryAddress::IE: {
            uint32_t val = arm7->readIE();
            writeBits(val, (uint32_t)data, 0, 7);
            arm7->writeIE(val);
            break;
        }
        case ARM7IOMemoryAddress::IE + 1: {
            uint32_t val = arm7->readIE();
            writeBits(val, (uint32_t)data, 8, 15);
            arm7->writeIE(val);
            break;
        }
        case ARM7IOMemoryAddress::IE + 2: {
            uint32_t val = arm7->readIE();
            writeBits(val, (uint32_t)data, 16, 23);
            arm7->writeIE(val);
            break;
        }
        case ARM7IOMemoryAddress::IE + 3: {
            uint32_t val = arm7->readIE();
            writeBits(val, (uint32_t)data, 24, 31);
            arm7->writeIE(val);
            break;
        }
        case ARM7IOMemoryAddress::IF: {
            uint32_t val = arm7->readIF();
            writeBits(val, (uint32_t)data, 0, 7);
            arm7->writeIF(val);
            break;
        }
        case ARM7IOMemoryAddress::IF + 1: {
            uint32_t val = arm7->readIF();
            writeBits(val, (uint32_t)data, 8, 15);
            arm7->writeIF(val);
            break;
        }
        case ARM7IOMemoryAddress::IF + 2: {
            uint32_t val = arm7->readIF();
            writeBits(val, (uint32_t)data, 16, 23);
            arm7->writeIF(val);
            break;
        }
        case ARM7IOMemoryAddress::IF + 3: {
            uint32_t val = arm7->readIF();
            writeBits(val, (uint32_t)data, 24, 31);
            arm7->writeIF(val);
            break;
        }
        default:
            LogError("Unsupported IO access at " << hexString(addr));
            break;
    }
}
// ==================================================================================================
}  // namespace Core
}  // namespace RedPandaDS
