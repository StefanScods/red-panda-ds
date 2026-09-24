#include "cpu.h"
#include "interconnect.h"

// Control print statements.
#define LOG_LEVEL 2
#include "logger.h"

namespace RedPandaDS {
namespace Core {

namespace ARM9IOMemoryAddress {
enum ARM9IOMemoryAddress : uint32_t {
    IPCSYNC = 0x4000180,
    IPCFIFOCNT = 0x4000184,
    IPCFIFOSEND = 0x4000188,
    IME = 0x04000208,
    IE = 0x04000210,
    IF = 0x04000214,
    EXMEMCNT = 0x04000204,
    WRAMCNT = 0x04000247,
    IPCFIFORECV = 0x4100000,
};
}

// ==================================================================================================
bool Interconnect::isIOAddressValidARM9(uint32_t addr) {
    // Word align mem accesses.
    addr &= ~0x3;
    // IO memory map.
    switch (addr) {
        case ARM9IOMemoryAddress::IPCSYNC:
        case ARM9IOMemoryAddress::IPCFIFOCNT:
        case ARM9IOMemoryAddress::IPCFIFOSEND:
        case ARM9IOMemoryAddress::IME:
        case ARM9IOMemoryAddress::IE:
        case ARM9IOMemoryAddress::IF:
        case ARM9IOMemoryAddress::EXMEMCNT:
        case ARM9IOMemoryAddress::IPCFIFORECV:
            return true;
        default:
            return false;
    }
    return false;
}
// ==================================================================================================
uint32_t Interconnect::read32IOARM9(uint32_t addr) {
    ARM946ES* arm9ptr = (ARM946ES*)arm9;
    // Word align mem accesses.
    addr &= ~0x3;
    // IO memory map.
    switch (addr) {
        case ARM9IOMemoryAddress::IPCSYNC: {
            return arm9ptr->readIPCSYNC();
        }
        case ARM9IOMemoryAddress::IPCFIFOCNT: {
            return arm9ptr->readIPCFIFOCNT();
        }
        case ARM9IOMemoryAddress::IME: {
            return arm9ptr->readIME();
        }
        case ARM9IOMemoryAddress::IE: {
            return arm9ptr->readIE();
        }
        case ARM9IOMemoryAddress::IF: {
            return arm9ptr->readIF();
        }
        case ARM9IOMemoryAddress::IPCFIFORECV: {
            return arm9ptr->readIPCFIFORECV();
        }
        default:
            LogError("Unsupported ARM9 IO 32-bit read at " << hexString(addr));
            exit(1);
            return INVALID_MEM_32BIT;
    }
    return INVALID_MEM_32BIT;
}
// ==================================================================================================
uint16_t Interconnect::read16IOARM9(uint32_t addr) {
    ARM946ES* arm9ptr = (ARM946ES*)arm9;
    // Half word align mem accesses.
    addr &= ~0x1;
    // IO memory map.
    switch (addr) {
        case ARM9IOMemoryAddress::IPCSYNC: {
            return arm9ptr->readIPCSYNC();
        }
        case ARM9IOMemoryAddress::IPCFIFOCNT: {
            return arm9ptr->readIPCFIFOCNT();
        }
        case ARM9IOMemoryAddress::IME: {
            return readBits(arm9ptr->readIME(), 0, 15);
        }
        case ARM9IOMemoryAddress::IME + 2: {
            return readBits(arm9ptr->readIME(), 16, 31);
        }
        case ARM9IOMemoryAddress::IE: {
            return readBits(arm9ptr->readIE(), 0, 15);
        }
        case ARM9IOMemoryAddress::IE + 2: {
            return readBits(arm9ptr->readIE(), 16, 31);
        }
        case ARM9IOMemoryAddress::IF: {
            return readBits(arm9ptr->readIF(), 0, 15);
        }
        case ARM9IOMemoryAddress::IF + 2: {
            return readBits(arm9ptr->readIF(), 16, 31);
        }
        case ARM9IOMemoryAddress::EXMEMCNT: {
            return arm9ptr->readEXMEMCNT();
        }
        default:
            LogError("Unsupported ARM9 IO 16-bit read at " << hexString(addr));
            return INVALID_MEM_16BIT;
    }
    return INVALID_MEM_16BIT;
}
// ==================================================================================================
uint8_t Interconnect::read8IOARM9(uint32_t addr) {
    ARM946ES* arm9ptr = (ARM946ES*)arm9;
    // IO memory map.
    switch (addr) {
        case ARM9IOMemoryAddress::IME: {
            return readBits(arm9ptr->readIME(), 0, 7);
        }
        case ARM9IOMemoryAddress::IME + 1: {
            return readBits(arm9ptr->readIME(), 8, 15);
        }
        case ARM9IOMemoryAddress::IME + 2: {
            return readBits(arm9ptr->readIME(), 16, 23);
        }
        case ARM9IOMemoryAddress::IME + 3: {
            return readBits(arm9ptr->readIME(), 24, 31);
        }
        case ARM9IOMemoryAddress::IE: {
            return readBits(arm9ptr->readIE(), 0, 7);
        }
        case ARM9IOMemoryAddress::IE + 1: {
            return readBits(arm9ptr->readIE(), 8, 15);
        }
        case ARM9IOMemoryAddress::IE + 2: {
            return readBits(arm9ptr->readIE(), 16, 23);
        }
        case ARM9IOMemoryAddress::IE + 3: {
            return readBits(arm9ptr->readIE(), 24, 31);
        }
        case ARM9IOMemoryAddress::IF: {
            return readBits(arm9ptr->readIF(), 0, 7);
        }
        case ARM9IOMemoryAddress::IF + 1: {
            return readBits(arm9ptr->readIF(), 8, 15);
        }
        case ARM9IOMemoryAddress::IF + 2: {
            return readBits(arm9ptr->readIF(), 16, 23);
        }
        case ARM9IOMemoryAddress::IF + 3: {
            return readBits(arm9ptr->readIF(), 24, 31);
        }
        case ARM9IOMemoryAddress::WRAMCNT: {
            return arm9ptr->readWRAMCNT();
        }
        default:
            LogError("Unsupported ARM9 IO 8-bit read at " << hexString(addr));
            return INVALID_MEM_8BIT;
    }
    return INVALID_MEM_8BIT;
}
// ==================================================================================================
void Interconnect::write32IOARM9(uint32_t addr, uint32_t data) {
    ARM946ES* arm9ptr = (ARM946ES*)arm9;
    // Word align mem accesses.
    addr &= ~0x3;
    // IO memory map.
    switch (addr) {
        case ARM9IOMemoryAddress::IPCSYNC: {
            return arm9ptr->writeIPCSYNC(data);
        }
        case ARM9IOMemoryAddress::IPCFIFOCNT: {
            return arm9ptr->writeIPCFIFOCNT(data);
        }
        case ARM9IOMemoryAddress::IPCFIFOSEND: {
            return arm9ptr->writeIPCFIFOSEND(data);
        }
        case ARM9IOMemoryAddress::IME: {
            return arm9ptr->writeIME(data);
        }
        case ARM9IOMemoryAddress::IE: {
            return arm9ptr->writeIE(data);
        }
        case ARM9IOMemoryAddress::IF: {
            return arm9ptr->writeIF(data);
        }
        default:
            LogError("Unsupported ARM9 IO 32-bit write at " << hexString(addr));
            exit(1);
            break;
    }
}
// ==================================================================================================
void Interconnect::write16IOARM9(uint32_t addr, uint16_t data) {
    ARM946ES* arm9ptr = (ARM946ES*)arm9;
    // Half word align mem accesses.
    addr &= ~0x1;
    // IO memory map.
    switch (addr) {
        case ARM9IOMemoryAddress::IPCSYNC: {
            return arm9ptr->writeIPCSYNC(data);
        }
        case ARM9IOMemoryAddress::IPCFIFOCNT: {
            return arm9ptr->writeIPCFIFOCNT(data);
        }
        case ARM9IOMemoryAddress::IME: {
            uint32_t val = arm9ptr->readIME();
            writeBits(val, (uint32_t)data, 0, 15);
            arm9ptr->writeIME(val);
            break;
        }
        case ARM9IOMemoryAddress::IME + 2: {
            uint32_t val = arm9ptr->readIME();
            writeBits(val, (uint32_t)data, 16, 31);
            arm9ptr->writeIME(val);
            break;
        }
        case ARM9IOMemoryAddress::IE: {
            uint32_t val = arm9ptr->readIE();
            writeBits(val, (uint32_t)data, 0, 15);
            arm9ptr->writeIE(val);
            break;
        }
        case ARM9IOMemoryAddress::IE + 2: {
            uint32_t val = arm9ptr->readIE();
            writeBits(val, (uint32_t)data, 16, 31);
            arm9ptr->writeIE(val);
            break;
        }
        case ARM9IOMemoryAddress::IF: {
            uint32_t val = arm9ptr->readIF();
            writeBits(val, (uint32_t)data, 0, 15);
            arm9ptr->writeIF(val);
            break;
        }
        case ARM9IOMemoryAddress::IF + 2: {
            uint32_t val = arm9ptr->readIF();
            writeBits(val, (uint32_t)data, 16, 31);
            arm9ptr->writeIF(val);
            break;
        }
        case ARM9IOMemoryAddress::EXMEMCNT: {
            return arm9ptr->writeEXMEMCNT(data);
        }
        default:
            LogError("Unsupported ARM9 IO 16-bit write at " << hexString(addr));
            break;
    }
}
// ==================================================================================================
void Interconnect::write8IOARM9(uint32_t addr, uint8_t data) {
    ARM946ES* arm9ptr = (ARM946ES*)arm9;
    // IO memory map.
    switch (addr) {
        case ARM9IOMemoryAddress::IME: {
            uint32_t val = arm9ptr->readIME();
            writeBits(val, (uint32_t)data, 0, 7);
            arm9ptr->writeIME(val);
            break;
        }
        case ARM9IOMemoryAddress::IME + 1: {
            uint32_t val = arm9ptr->readIME();
            writeBits(val, (uint32_t)data, 8, 15);
            arm9ptr->writeIME(val);
            break;
        }
        case ARM9IOMemoryAddress::IME + 2: {
            uint32_t val = arm9ptr->readIME();
            writeBits(val, (uint32_t)data, 16, 23);
            arm9ptr->writeIME(val);
            break;
        }
        case ARM9IOMemoryAddress::IME + 3: {
            uint32_t val = arm9ptr->readIME();
            writeBits(val, (uint32_t)data, 24, 31);
            arm9ptr->writeIME(val);
            break;
        }
        case ARM9IOMemoryAddress::IE: {
            uint32_t val = arm9ptr->readIE();
            writeBits(val, (uint32_t)data, 0, 7);
            arm9ptr->writeIE(val);
            break;
        }
        case ARM9IOMemoryAddress::IE + 1: {
            uint32_t val = arm9ptr->readIE();
            writeBits(val, (uint32_t)data, 8, 15);
            arm9ptr->writeIE(val);
            break;
        }
        case ARM9IOMemoryAddress::IE + 2: {
            uint32_t val = arm9ptr->readIE();
            writeBits(val, (uint32_t)data, 16, 23);
            arm9ptr->writeIE(val);
            break;
        }
        case ARM9IOMemoryAddress::IE + 3: {
            uint32_t val = arm9ptr->readIE();
            writeBits(val, (uint32_t)data, 24, 31);
            arm9ptr->writeIE(val);
            break;
        }
        case ARM9IOMemoryAddress::IF: {
            uint32_t val = arm9ptr->readIF();
            writeBits(val, (uint32_t)data, 0, 7);
            arm9ptr->writeIF(val);
            break;
        }
        case ARM9IOMemoryAddress::IF + 1: {
            uint32_t val = arm9ptr->readIF();
            writeBits(val, (uint32_t)data, 8, 15);
            arm9ptr->writeIF(val);
            break;
        }
        case ARM9IOMemoryAddress::IF + 2: {
            uint32_t val = arm9ptr->readIF();
            writeBits(val, (uint32_t)data, 16, 23);
            arm9ptr->writeIF(val);
            break;
        }
        case ARM9IOMemoryAddress::IF + 3: {
            uint32_t val = arm9ptr->readIF();
            writeBits(val, (uint32_t)data, 24, 31);
            arm9ptr->writeIF(val);
            break;
        }
        case ARM9IOMemoryAddress::WRAMCNT: {
            arm9ptr->writeWRAMCNT(data);
            break;
        }
        default:
            LogError("Unsupported ARM9 IO 8-bit write at " << hexString(addr));
            break;
    }
}
// ==================================================================================================
}  // namespace Core
}  // namespace RedPandaDS
