#include "cpu.h"
#include "interconnect.h"

// Control print statements.
#define LOG_LEVEL 2
#include "logger.h"

namespace RedPandaDS {
namespace Core {

namespace ARM9IOMemoryAddress {
enum ARM9IOMemoryAddress : uint32_t {
    DMA0SAD = 0x40000B0,
    DMA0DAD = 0x40000B4,
    DMA0CNT_L = 0x40000B8,
    DMA0CNT_H = 0x40000BA,
    DMA1SAD = 0x40000BC,
    DMA1DAD = 0x40000C0,
    DMA1CNT_L = 0x40000C4,
    DMA1CNT_H = 0x40000C6,
    DMA2SAD = 0x40000C8,
    DMA2DAD = 0x40000CC,
    DMA2CNT_L = 0x40000D0,
    DMA2CNT_H = 0x40000D2,
    DMA3SAD = 0x40000D4,
    DMA3DAD = 0x40000D8,
    DMA3CNT_L = 0x40000DC,
    DMA3CNT_H = 0x40000DE,
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
        case ARM9IOMemoryAddress::DMA0SAD:
        case ARM9IOMemoryAddress::DMA0DAD:
        case ARM9IOMemoryAddress::DMA0CNT_L:
        case ARM9IOMemoryAddress::DMA0CNT_H:
        case ARM9IOMemoryAddress::DMA1SAD:
        case ARM9IOMemoryAddress::DMA1DAD:
        case ARM9IOMemoryAddress::DMA1CNT_L:
        case ARM9IOMemoryAddress::DMA1CNT_H:
        case ARM9IOMemoryAddress::DMA2SAD:
        case ARM9IOMemoryAddress::DMA2DAD:
        case ARM9IOMemoryAddress::DMA2CNT_L:
        case ARM9IOMemoryAddress::DMA2CNT_H:
        case ARM9IOMemoryAddress::DMA3SAD:
        case ARM9IOMemoryAddress::DMA3DAD:
        case ARM9IOMemoryAddress::DMA3CNT_L:
        case ARM9IOMemoryAddress::DMA3CNT_H:
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
        case ARM9IOMemoryAddress::DMA0SAD: {
            return arm9ptr->readDMASourceAddress(0);
        }
        case ARM9IOMemoryAddress::DMA0DAD: {
            return arm9ptr->readDMADestAddress(0);
        }
        case ARM9IOMemoryAddress::DMA0CNT_L: {
            uint32_t result = arm9ptr->readDMAControl(0, false);
            result = (result << 16) | arm9ptr->readDMAControl(0, true);
            return result;
        }
        case ARM9IOMemoryAddress::DMA1SAD: {
            return arm9ptr->readDMASourceAddress(1);
        }
        case ARM9IOMemoryAddress::DMA1DAD: {
            return arm9ptr->readDMADestAddress(1);
        }
        case ARM9IOMemoryAddress::DMA1CNT_L: {
            uint32_t result = arm9ptr->readDMAControl(1, false);
            result = (result << 16) | arm9ptr->readDMAControl(1, true);
            return result;
        }
        case ARM9IOMemoryAddress::DMA2SAD: {
            return arm9ptr->readDMASourceAddress(2);
        }
        case ARM9IOMemoryAddress::DMA2DAD: {
            return arm9ptr->readDMADestAddress(2);
        }
        case ARM9IOMemoryAddress::DMA2CNT_L: {
            uint32_t result = arm9ptr->readDMAControl(2, false);
            result = (result << 16) | arm9ptr->readDMAControl(2, true);
            return result;
        }
        case ARM9IOMemoryAddress::DMA3SAD: {
            return arm9ptr->readDMASourceAddress(3);
        }
        case ARM9IOMemoryAddress::DMA3DAD: {
            return arm9ptr->readDMADestAddress(3);
        }
        case ARM9IOMemoryAddress::DMA3CNT_L: {
            uint32_t result = arm9ptr->readDMAControl(3, false);
            result = (result << 16) | arm9ptr->readDMAControl(3, true);
            return result;
        }
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
            arm9ptr->DEBUG_pauseExecution();
            LogError("Unsupported ARM9 IO 32-bit read at " << hexString(addr));
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
        case ARM9IOMemoryAddress::DMA0SAD: {
            return readBits(arm9ptr->readDMASourceAddress(0), 0, 15);
        }
        case ARM9IOMemoryAddress::DMA0SAD + 2: {
            return readBits(arm9ptr->readDMASourceAddress(0), 16, 31);
        }
        case ARM9IOMemoryAddress::DMA0DAD: {
            return readBits(arm9ptr->readDMADestAddress(0), 0, 15);
        }
        case ARM9IOMemoryAddress::DMA0DAD + 2: {
            return readBits(arm9ptr->readDMADestAddress(0), 16, 31);
        }
        case ARM9IOMemoryAddress::DMA0CNT_L: {
            return arm9ptr->readDMAControl(0, true);
        }
        case ARM9IOMemoryAddress::DMA0CNT_H: {
            return arm9ptr->readDMAControl(0, false);
        }
        case ARM9IOMemoryAddress::DMA1SAD: {
            return readBits(arm9ptr->readDMASourceAddress(1), 0, 15);
        }
        case ARM9IOMemoryAddress::DMA1SAD + 2: {
            return readBits(arm9ptr->readDMASourceAddress(1), 16, 31);
        }
        case ARM9IOMemoryAddress::DMA1DAD: {
            return readBits(arm9ptr->readDMADestAddress(1), 0, 15);
        }
        case ARM9IOMemoryAddress::DMA1DAD + 2: {
            return readBits(arm9ptr->readDMADestAddress(1), 16, 31);
        }
        case ARM9IOMemoryAddress::DMA1CNT_L: {
            return arm9ptr->readDMAControl(1, true);
        }
        case ARM9IOMemoryAddress::DMA1CNT_H: {
            return arm9ptr->readDMAControl(1, false);
        }
        case ARM9IOMemoryAddress::DMA2SAD: {
            return readBits(arm9ptr->readDMASourceAddress(2), 0, 15);
        }
        case ARM9IOMemoryAddress::DMA2SAD + 2: {
            return readBits(arm9ptr->readDMASourceAddress(2), 16, 31);
        }
        case ARM9IOMemoryAddress::DMA2DAD: {
            return readBits(arm9ptr->readDMADestAddress(2), 0, 15);
        }
        case ARM9IOMemoryAddress::DMA2DAD + 2: {
            return readBits(arm9ptr->readDMADestAddress(2), 16, 31);
        }
        case ARM9IOMemoryAddress::DMA2CNT_L: {
            return arm9ptr->readDMAControl(2, true);
        }
        case ARM9IOMemoryAddress::DMA2CNT_H: {
            return arm9ptr->readDMAControl(2, false);
        }
        case ARM9IOMemoryAddress::DMA3SAD: {
            return readBits(arm9ptr->readDMASourceAddress(3), 0, 15);
        }
        case ARM9IOMemoryAddress::DMA3SAD + 2: {
            return readBits(arm9ptr->readDMASourceAddress(3), 16, 31);
        }
        case ARM9IOMemoryAddress::DMA3DAD: {
            return readBits(arm9ptr->readDMADestAddress(3), 0, 15);
        }
        case ARM9IOMemoryAddress::DMA3DAD + 2: {
            return readBits(arm9ptr->readDMADestAddress(3), 16, 31);
        }
        case ARM9IOMemoryAddress::DMA3CNT_L: {
            return arm9ptr->readDMAControl(3, true);
        }
        case ARM9IOMemoryAddress::DMA3CNT_H: {
            return arm9ptr->readDMAControl(3, false);
        }
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
            arm9ptr->DEBUG_pauseExecution();
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
        case ARM9IOMemoryAddress::DMA0SAD: {
            return readBits(arm9ptr->readDMASourceAddress(0), 0, 7);
        }
        case ARM9IOMemoryAddress::DMA0SAD + 1: {
            return readBits(arm9ptr->readDMASourceAddress(0), 8, 15);
        }
        case ARM9IOMemoryAddress::DMA0SAD + 2: {
            return readBits(arm9ptr->readDMASourceAddress(0), 16, 23);
        }
        case ARM9IOMemoryAddress::DMA0SAD + 3: {
            return readBits(arm9ptr->readDMASourceAddress(0), 24, 31);
        }
        case ARM9IOMemoryAddress::DMA0DAD: {
            return readBits(arm9ptr->readDMADestAddress(0), 0, 7);
        }
        case ARM9IOMemoryAddress::DMA0DAD + 1: {
            return readBits(arm9ptr->readDMADestAddress(0), 8, 15);
        }
        case ARM9IOMemoryAddress::DMA0DAD + 2: {
            return readBits(arm9ptr->readDMADestAddress(0), 16, 23);
        }
        case ARM9IOMemoryAddress::DMA0DAD + 3: {
            return readBits(arm9ptr->readDMADestAddress(0), 24, 31);
        }
        case ARM9IOMemoryAddress::DMA0CNT_L: {
            return readBits(arm9ptr->readDMAControl(0, true), 0, 7);
        }
        case ARM9IOMemoryAddress::DMA0CNT_L + 1: {
            return readBits(arm9ptr->readDMAControl(0, true), 8, 15);
        }
        case ARM9IOMemoryAddress::DMA0CNT_H: {
            return readBits(arm9ptr->readDMAControl(0, false), 0, 7);
        }
        case ARM9IOMemoryAddress::DMA0CNT_H + 1: {
            return readBits(arm9ptr->readDMAControl(0, false), 8, 15);
        }
        case ARM9IOMemoryAddress::DMA1SAD: {
            return readBits(arm9ptr->readDMASourceAddress(1), 0, 7);
        }
        case ARM9IOMemoryAddress::DMA1SAD + 1: {
            return readBits(arm9ptr->readDMASourceAddress(1), 8, 15);
        }
        case ARM9IOMemoryAddress::DMA1SAD + 2: {
            return readBits(arm9ptr->readDMASourceAddress(1), 16, 23);
        }
        case ARM9IOMemoryAddress::DMA1SAD + 3: {
            return readBits(arm9ptr->readDMASourceAddress(1), 24, 31);
        }
        case ARM9IOMemoryAddress::DMA1DAD: {
            return readBits(arm9ptr->readDMADestAddress(1), 0, 7);
        }
        case ARM9IOMemoryAddress::DMA1DAD + 1: {
            return readBits(arm9ptr->readDMADestAddress(1), 8, 15);
        }
        case ARM9IOMemoryAddress::DMA1DAD + 2: {
            return readBits(arm9ptr->readDMADestAddress(1), 16, 23);
        }
        case ARM9IOMemoryAddress::DMA1DAD + 3: {
            return readBits(arm9ptr->readDMADestAddress(1), 24, 31);
        }
        case ARM9IOMemoryAddress::DMA1CNT_L: {
            return readBits(arm9ptr->readDMAControl(1, true), 0, 7);
        }
        case ARM9IOMemoryAddress::DMA1CNT_L + 1: {
            return readBits(arm9ptr->readDMAControl(1, true), 8, 15);
        }
        case ARM9IOMemoryAddress::DMA1CNT_H: {
            return readBits(arm9ptr->readDMAControl(1, false), 0, 7);
        }
        case ARM9IOMemoryAddress::DMA1CNT_H + 1: {
            return readBits(arm9ptr->readDMAControl(1, false), 8, 15);
        }
        case ARM9IOMemoryAddress::DMA2SAD: {
            return readBits(arm9ptr->readDMASourceAddress(2), 0, 7);
        }
        case ARM9IOMemoryAddress::DMA2SAD + 1: {
            return readBits(arm9ptr->readDMASourceAddress(2), 8, 15);
        }
        case ARM9IOMemoryAddress::DMA2SAD + 2: {
            return readBits(arm9ptr->readDMASourceAddress(2), 16, 23);
        }
        case ARM9IOMemoryAddress::DMA2SAD + 3: {
            return readBits(arm9ptr->readDMASourceAddress(2), 24, 31);
        }
        case ARM9IOMemoryAddress::DMA2DAD: {
            return readBits(arm9ptr->readDMADestAddress(2), 0, 7);
        }
        case ARM9IOMemoryAddress::DMA2DAD + 1: {
            return readBits(arm9ptr->readDMADestAddress(2), 8, 15);
        }
        case ARM9IOMemoryAddress::DMA2DAD + 2: {
            return readBits(arm9ptr->readDMADestAddress(2), 16, 23);
        }
        case ARM9IOMemoryAddress::DMA2DAD + 3: {
            return readBits(arm9ptr->readDMADestAddress(2), 24, 31);
        }
        case ARM9IOMemoryAddress::DMA2CNT_L: {
            return readBits(arm9ptr->readDMAControl(2, true), 0, 7);
        }
        case ARM9IOMemoryAddress::DMA2CNT_L + 1: {
            return readBits(arm9ptr->readDMAControl(2, true), 8, 15);
        }
        case ARM9IOMemoryAddress::DMA2CNT_H: {
            return readBits(arm9ptr->readDMAControl(2, false), 0, 7);
        }
        case ARM9IOMemoryAddress::DMA2CNT_H + 1: {
            return readBits(arm9ptr->readDMAControl(2, false), 8, 15);
        }
        case ARM9IOMemoryAddress::DMA3SAD: {
            return readBits(arm9ptr->readDMASourceAddress(3), 0, 7);
        }
        case ARM9IOMemoryAddress::DMA3SAD + 1: {
            return readBits(arm9ptr->readDMASourceAddress(3), 8, 15);
        }
        case ARM9IOMemoryAddress::DMA3SAD + 2: {
            return readBits(arm9ptr->readDMASourceAddress(3), 16, 23);
        }
        case ARM9IOMemoryAddress::DMA3SAD + 3: {
            return readBits(arm9ptr->readDMASourceAddress(3), 24, 31);
        }
        case ARM9IOMemoryAddress::DMA3DAD: {
            return readBits(arm9ptr->readDMADestAddress(3), 0, 7);
        }
        case ARM9IOMemoryAddress::DMA3DAD + 1: {
            return readBits(arm9ptr->readDMADestAddress(3), 8, 15);
        }
        case ARM9IOMemoryAddress::DMA3DAD + 2: {
            return readBits(arm9ptr->readDMADestAddress(3), 16, 23);
        }
        case ARM9IOMemoryAddress::DMA3DAD + 3: {
            return readBits(arm9ptr->readDMADestAddress(3), 24, 31);
        }
        case ARM9IOMemoryAddress::DMA3CNT_L: {
            return readBits(arm9ptr->readDMAControl(3, true), 0, 7);
        }
        case ARM9IOMemoryAddress::DMA3CNT_L + 1: {
            return readBits(arm9ptr->readDMAControl(3, true), 8, 15);
        }
        case ARM9IOMemoryAddress::DMA3CNT_H: {
            return readBits(arm9ptr->readDMAControl(3, false), 0, 7);
        }
        case ARM9IOMemoryAddress::DMA3CNT_H + 1: {
            return readBits(arm9ptr->readDMAControl(3, false), 8, 15);
        }
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
            arm9ptr->DEBUG_pauseExecution();
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
        case ARM9IOMemoryAddress::DMA0SAD: {
            return arm9ptr->writeDMASourceAddress(0, data);
        }
        case ARM9IOMemoryAddress::DMA0DAD: {
            return arm9ptr->writeDMADestAddress(0, data);
        }
        case ARM9IOMemoryAddress::DMA1SAD: {
            return arm9ptr->writeDMASourceAddress(1, data);
        }
        case ARM9IOMemoryAddress::DMA1DAD: {
            return arm9ptr->writeDMADestAddress(1, data);
        }
        case ARM9IOMemoryAddress::DMA2SAD: {
            return arm9ptr->writeDMASourceAddress(2, data);
        }
        case ARM9IOMemoryAddress::DMA2DAD: {
            return arm9ptr->writeDMADestAddress(2, data);
        }
        case ARM9IOMemoryAddress::DMA3SAD: {
            return arm9ptr->writeDMASourceAddress(3, data);
        }
        case ARM9IOMemoryAddress::DMA3DAD: {
            return arm9ptr->writeDMADestAddress(3, data);
        }
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
            arm9ptr->DEBUG_pauseExecution();
            LogError("Unsupported ARM9 IO 32-bit write at " << hexString(addr));
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
        case ARM9IOMemoryAddress::DMA0CNT_L: {
            return arm9ptr->writeDMAControl(0, data, true);
        }
        case ARM9IOMemoryAddress::DMA0CNT_H: {
            return arm9ptr->writeDMAControl(0, data, false);
        }
        case ARM9IOMemoryAddress::DMA1CNT_L: {
            return arm9ptr->writeDMAControl(1, data, true);
        }
        case ARM9IOMemoryAddress::DMA1CNT_H: {
            return arm9ptr->writeDMAControl(1, data, false);
        }
        case ARM9IOMemoryAddress::DMA2CNT_L: {
            return arm9ptr->writeDMAControl(2, data, true);
        }
        case ARM9IOMemoryAddress::DMA2CNT_H: {
            return arm9ptr->writeDMAControl(2, data, false);
        }
        case ARM9IOMemoryAddress::DMA3CNT_L: {
            return arm9ptr->writeDMAControl(3, data, true);
        }
        case ARM9IOMemoryAddress::DMA3CNT_H: {
            return arm9ptr->writeDMAControl(3, data, false);
        }
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
            arm9ptr->DEBUG_pauseExecution();
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
            arm9ptr->DEBUG_pauseExecution();
            LogError("Unsupported ARM9 IO 8-bit write at " << hexString(addr));
            break;
    }
}
// ==================================================================================================
}  // namespace Core
}  // namespace RedPandaDS
