#include "cpu.h"
#include "interconnect.h"

// Control print statements.
#define LOG_LEVEL 2
#include "logger.h"

namespace RedPandaDS {
namespace Core {

namespace ARM7IOMemoryAddress {
enum ARM7IOMemoryAddress : uint32_t {
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
    IPCFIFORECV = 0x4100000,
};
}

// ==================================================================================================
bool Interconnect::isIOAddressValidARM7(uint32_t addr) {
    // Word align mem accesses.
    addr &= ~0x3;
    // IO memory map.
    switch (addr) {
        case ARM7IOMemoryAddress::IPCSYNC:
        case ARM7IOMemoryAddress::IPCFIFOCNT:
        case ARM7IOMemoryAddress::IPCFIFOSEND:
        case ARM7IOMemoryAddress::DMA0SAD:
        case ARM7IOMemoryAddress::DMA0DAD:
        case ARM7IOMemoryAddress::DMA0CNT_L:
        case ARM7IOMemoryAddress::DMA0CNT_H:
        case ARM7IOMemoryAddress::DMA1SAD:
        case ARM7IOMemoryAddress::DMA1DAD:
        case ARM7IOMemoryAddress::DMA1CNT_L:
        case ARM7IOMemoryAddress::DMA1CNT_H:
        case ARM7IOMemoryAddress::DMA2SAD:
        case ARM7IOMemoryAddress::DMA2DAD:
        case ARM7IOMemoryAddress::DMA2CNT_L:
        case ARM7IOMemoryAddress::DMA2CNT_H:
        case ARM7IOMemoryAddress::DMA3SAD:
        case ARM7IOMemoryAddress::DMA3DAD:
        case ARM7IOMemoryAddress::DMA3CNT_L:
        case ARM7IOMemoryAddress::DMA3CNT_H:
        case ARM7IOMemoryAddress::IME:
        case ARM7IOMemoryAddress::IE:
        case ARM7IOMemoryAddress::IF:
        case ARM7IOMemoryAddress::IPCFIFORECV:
            return true;
        default:
            return false;
    }
    return false;
}
// ==================================================================================================
uint32_t Interconnect::read32IOARM7(uint32_t addr) {
    ARM7TDMI* arm7ptr = (ARM7TDMI*)arm7;
    // Word align mem accesses.
    addr &= ~0x3;
    // IO memory map.
    switch (addr) {
        case ARM7IOMemoryAddress::DMA0SAD: {
            return arm7ptr->readDMASourceAddress(0);
        }
        case ARM7IOMemoryAddress::DMA0DAD: {
            return arm7ptr->readDMADestAddress(0);
        }
        case ARM7IOMemoryAddress::DMA0CNT_L: {
            uint32_t result = arm7ptr->readDMAControl(0, false);
            result = (result << 16) | arm7ptr->readDMAControl(0, true);
            return result;
        }
        case ARM7IOMemoryAddress::DMA1SAD: {
            return arm7ptr->readDMASourceAddress(1);
        }
        case ARM7IOMemoryAddress::DMA1DAD: {
            return arm7ptr->readDMADestAddress(1);
        }
        case ARM7IOMemoryAddress::DMA1CNT_L: {
            uint32_t result = arm7ptr->readDMAControl(1, false);
            result = (result << 16) | arm7ptr->readDMAControl(1, true);
            return result;
        }
        case ARM7IOMemoryAddress::DMA2SAD: {
            return arm7ptr->readDMASourceAddress(2);
        }
        case ARM7IOMemoryAddress::DMA2DAD: {
            return arm7ptr->readDMADestAddress(2);
        }
        case ARM7IOMemoryAddress::DMA2CNT_L: {
            uint32_t result = arm7ptr->readDMAControl(2, false);
            result = (result << 16) | arm7ptr->readDMAControl(2, true);
            return result;
        }
        case ARM7IOMemoryAddress::DMA3SAD: {
            return arm7ptr->readDMASourceAddress(3);
        }
        case ARM7IOMemoryAddress::DMA3DAD: {
            return arm7ptr->readDMADestAddress(3);
        }
        case ARM7IOMemoryAddress::DMA3CNT_L: {
            uint32_t result = arm7ptr->readDMAControl(3, false);
            result = (result << 16) | arm7ptr->readDMAControl(3, true);
            return result;
        }
        case ARM7IOMemoryAddress::IPCSYNC: {
            return arm7ptr->readIPCSYNC();
        }
        case ARM7IOMemoryAddress::IPCFIFOCNT: {
            return arm7ptr->readIPCFIFOCNT();
        }
        case ARM7IOMemoryAddress::IME: {
            return arm7ptr->readIME();
        }
        case ARM7IOMemoryAddress::IE: {
            return arm7ptr->readIE();
        }
        case ARM7IOMemoryAddress::IF: {
            return arm7ptr->readIF();
        }
        case ARM7IOMemoryAddress::IPCFIFORECV: {
            return arm7ptr->readIPCFIFORECV();
        }
        default:
            arm7ptr->DEBUG_pauseExecution();
            LogError("Unsupported ARM7 IO 32-bit read at " << hexString(addr));
            return INVALID_MEM_32BIT;
    }
    return INVALID_MEM_32BIT;
}
// ==================================================================================================
uint16_t Interconnect::read16IOARM7(uint32_t addr) {
    ARM7TDMI* arm7ptr = (ARM7TDMI*)arm7;
    // Half word align mem accesses.
    addr &= ~0x1;
    // IO memory map.
    switch (addr) {
        case ARM7IOMemoryAddress::DMA0SAD: {
            return readBits(arm7ptr->readDMASourceAddress(0), 0, 15);
        }
        case ARM7IOMemoryAddress::DMA0SAD + 2: {
            return readBits(arm7ptr->readDMASourceAddress(0), 16, 31);
        }
        case ARM7IOMemoryAddress::DMA0DAD: {
            return readBits(arm7ptr->readDMADestAddress(0), 0, 15);
        }
        case ARM7IOMemoryAddress::DMA0DAD + 2: {
            return readBits(arm7ptr->readDMADestAddress(0), 16, 31);
        }
        case ARM7IOMemoryAddress::DMA0CNT_L: {
            return arm7ptr->readDMAControl(0, true);
        }
        case ARM7IOMemoryAddress::DMA0CNT_H: {
            return arm7ptr->readDMAControl(0, false);
        }
        case ARM7IOMemoryAddress::DMA1SAD: {
            return readBits(arm7ptr->readDMASourceAddress(1), 0, 15);
        }
        case ARM7IOMemoryAddress::DMA1SAD + 2: {
            return readBits(arm7ptr->readDMASourceAddress(1), 16, 31);
        }
        case ARM7IOMemoryAddress::DMA1DAD: {
            return readBits(arm7ptr->readDMADestAddress(1), 0, 15);
        }
        case ARM7IOMemoryAddress::DMA1DAD + 2: {
            return readBits(arm7ptr->readDMADestAddress(1), 16, 31);
        }
        case ARM7IOMemoryAddress::DMA1CNT_L: {
            return arm7ptr->readDMAControl(1, true);
        }
        case ARM7IOMemoryAddress::DMA1CNT_H: {
            return arm7ptr->readDMAControl(1, false);
        }
        case ARM7IOMemoryAddress::DMA2SAD: {
            return readBits(arm7ptr->readDMASourceAddress(2), 0, 15);
        }
        case ARM7IOMemoryAddress::DMA2SAD + 2: {
            return readBits(arm7ptr->readDMASourceAddress(2), 16, 31);
        }
        case ARM7IOMemoryAddress::DMA2DAD: {
            return readBits(arm7ptr->readDMADestAddress(2), 0, 15);
        }
        case ARM7IOMemoryAddress::DMA2DAD + 2: {
            return readBits(arm7ptr->readDMADestAddress(2), 16, 31);
        }
        case ARM7IOMemoryAddress::DMA2CNT_L: {
            return arm7ptr->readDMAControl(2, true);
        }
        case ARM7IOMemoryAddress::DMA2CNT_H: {
            return arm7ptr->readDMAControl(2, false);
        }
        case ARM7IOMemoryAddress::DMA3SAD: {
            return readBits(arm7ptr->readDMASourceAddress(3), 0, 15);
        }
        case ARM7IOMemoryAddress::DMA3SAD + 2: {
            return readBits(arm7ptr->readDMASourceAddress(3), 16, 31);
        }
        case ARM7IOMemoryAddress::DMA3DAD: {
            return readBits(arm7ptr->readDMADestAddress(3), 0, 15);
        }
        case ARM7IOMemoryAddress::DMA3DAD + 2: {
            return readBits(arm7ptr->readDMADestAddress(3), 16, 31);
        }
        case ARM7IOMemoryAddress::DMA3CNT_L: {
            return arm7ptr->readDMAControl(3, true);
        }
        case ARM7IOMemoryAddress::DMA3CNT_H: {
            return arm7ptr->readDMAControl(3, false);
        }
        case ARM7IOMemoryAddress::IPCSYNC: {
            return arm7ptr->readIPCSYNC();
        }
        case ARM7IOMemoryAddress::IPCFIFOCNT: {
            return arm7ptr->readIPCFIFOCNT();
        }
        case ARM7IOMemoryAddress::IME: {
            return readBits(arm7ptr->readIME(), 0, 15);
        }
        case ARM7IOMemoryAddress::IME + 2: {
            return readBits(arm7ptr->readIME(), 16, 31);
        }
        case ARM7IOMemoryAddress::IE: {
            return readBits(arm7ptr->readIE(), 0, 15);
        }
        case ARM7IOMemoryAddress::IE + 2: {
            return readBits(arm7ptr->readIE(), 16, 31);
        }
        case ARM7IOMemoryAddress::IF: {
            return readBits(arm7ptr->readIF(), 0, 15);
        }
        case ARM7IOMemoryAddress::IF + 2: {
            return readBits(arm7ptr->readIF(), 16, 31);
        }
        default:
            arm7ptr->DEBUG_pauseExecution();
            LogError("Unsupported ARM7 IO 16-bit read at " << hexString(addr));
            return INVALID_MEM_16BIT;
    }
    return INVALID_MEM_16BIT;
}
// ==================================================================================================
uint8_t Interconnect::read8IOARM7(uint32_t addr) {
    ARM7TDMI* arm7ptr = (ARM7TDMI*)arm7;
    // IO memory map.
    switch (addr) {
        case ARM7IOMemoryAddress::DMA0SAD: {
            return readBits(arm7ptr->readDMASourceAddress(0), 0, 7);
        }
        case ARM7IOMemoryAddress::DMA0SAD + 1: {
            return readBits(arm7ptr->readDMASourceAddress(0), 8, 15);
        }
        case ARM7IOMemoryAddress::DMA0SAD + 2: {
            return readBits(arm7ptr->readDMASourceAddress(0), 16, 23);
        }
        case ARM7IOMemoryAddress::DMA0SAD + 3: {
            return readBits(arm7ptr->readDMASourceAddress(0), 24, 31);
        }
        case ARM7IOMemoryAddress::DMA0DAD: {
            return readBits(arm7ptr->readDMADestAddress(0), 0, 7);
        }
        case ARM7IOMemoryAddress::DMA0DAD + 1: {
            return readBits(arm7ptr->readDMADestAddress(0), 8, 15);
        }
        case ARM7IOMemoryAddress::DMA0DAD + 2: {
            return readBits(arm7ptr->readDMADestAddress(0), 16, 23);
        }
        case ARM7IOMemoryAddress::DMA0DAD + 3: {
            return readBits(arm7ptr->readDMADestAddress(0), 24, 31);
        }
        case ARM7IOMemoryAddress::DMA0CNT_L: {
            return readBits(arm7ptr->readDMAControl(0, true), 0, 7);
        }
        case ARM7IOMemoryAddress::DMA0CNT_L + 1: {
            return readBits(arm7ptr->readDMAControl(0, true), 8, 15);
        }
        case ARM7IOMemoryAddress::DMA0CNT_H: {
            return readBits(arm7ptr->readDMAControl(0, false), 0, 7);
        }
        case ARM7IOMemoryAddress::DMA0CNT_H + 1: {
            return readBits(arm7ptr->readDMAControl(0, false), 8, 15);
        }
        case ARM7IOMemoryAddress::DMA1SAD: {
            return readBits(arm7ptr->readDMASourceAddress(1), 0, 7);
        }
        case ARM7IOMemoryAddress::DMA1SAD + 1: {
            return readBits(arm7ptr->readDMASourceAddress(1), 8, 15);
        }
        case ARM7IOMemoryAddress::DMA1SAD + 2: {
            return readBits(arm7ptr->readDMASourceAddress(1), 16, 23);
        }
        case ARM7IOMemoryAddress::DMA1SAD + 3: {
            return readBits(arm7ptr->readDMASourceAddress(1), 24, 31);
        }
        case ARM7IOMemoryAddress::DMA1DAD: {
            return readBits(arm7ptr->readDMADestAddress(1), 0, 7);
        }
        case ARM7IOMemoryAddress::DMA1DAD + 1: {
            return readBits(arm7ptr->readDMADestAddress(1), 8, 15);
        }
        case ARM7IOMemoryAddress::DMA1DAD + 2: {
            return readBits(arm7ptr->readDMADestAddress(1), 16, 23);
        }
        case ARM7IOMemoryAddress::DMA1DAD + 3: {
            return readBits(arm7ptr->readDMADestAddress(1), 24, 31);
        }
        case ARM7IOMemoryAddress::DMA1CNT_L: {
            return readBits(arm7ptr->readDMAControl(1, true), 0, 7);
        }
        case ARM7IOMemoryAddress::DMA1CNT_L + 1: {
            return readBits(arm7ptr->readDMAControl(1, true), 8, 15);
        }
        case ARM7IOMemoryAddress::DMA1CNT_H: {
            return readBits(arm7ptr->readDMAControl(1, false), 0, 7);
        }
        case ARM7IOMemoryAddress::DMA1CNT_H + 1: {
            return readBits(arm7ptr->readDMAControl(1, false), 8, 15);
        }
        case ARM7IOMemoryAddress::DMA2SAD: {
            return readBits(arm7ptr->readDMASourceAddress(2), 0, 7);
        }
        case ARM7IOMemoryAddress::DMA2SAD + 1: {
            return readBits(arm7ptr->readDMASourceAddress(2), 8, 15);
        }
        case ARM7IOMemoryAddress::DMA2SAD + 2: {
            return readBits(arm7ptr->readDMASourceAddress(2), 16, 23);
        }
        case ARM7IOMemoryAddress::DMA2SAD + 3: {
            return readBits(arm7ptr->readDMASourceAddress(2), 24, 31);
        }
        case ARM7IOMemoryAddress::DMA2DAD: {
            return readBits(arm7ptr->readDMADestAddress(2), 0, 7);
        }
        case ARM7IOMemoryAddress::DMA2DAD + 1: {
            return readBits(arm7ptr->readDMADestAddress(2), 8, 15);
        }
        case ARM7IOMemoryAddress::DMA2DAD + 2: {
            return readBits(arm7ptr->readDMADestAddress(2), 16, 23);
        }
        case ARM7IOMemoryAddress::DMA2DAD + 3: {
            return readBits(arm7ptr->readDMADestAddress(2), 24, 31);
        }
        case ARM7IOMemoryAddress::DMA2CNT_L: {
            return readBits(arm7ptr->readDMAControl(2, true), 0, 7);
        }
        case ARM7IOMemoryAddress::DMA2CNT_L + 1: {
            return readBits(arm7ptr->readDMAControl(2, true), 8, 15);
        }
        case ARM7IOMemoryAddress::DMA2CNT_H: {
            return readBits(arm7ptr->readDMAControl(2, false), 0, 7);
        }
        case ARM7IOMemoryAddress::DMA2CNT_H + 1: {
            return readBits(arm7ptr->readDMAControl(2, false), 8, 15);
        }
        case ARM7IOMemoryAddress::DMA3SAD: {
            return readBits(arm7ptr->readDMASourceAddress(3), 0, 7);
        }
        case ARM7IOMemoryAddress::DMA3SAD + 1: {
            return readBits(arm7ptr->readDMASourceAddress(3), 8, 15);
        }
        case ARM7IOMemoryAddress::DMA3SAD + 2: {
            return readBits(arm7ptr->readDMASourceAddress(3), 16, 23);
        }
        case ARM7IOMemoryAddress::DMA3SAD + 3: {
            return readBits(arm7ptr->readDMASourceAddress(3), 24, 31);
        }
        case ARM7IOMemoryAddress::DMA3DAD: {
            return readBits(arm7ptr->readDMADestAddress(3), 0, 7);
        }
        case ARM7IOMemoryAddress::DMA3DAD + 1: {
            return readBits(arm7ptr->readDMADestAddress(3), 8, 15);
        }
        case ARM7IOMemoryAddress::DMA3DAD + 2: {
            return readBits(arm7ptr->readDMADestAddress(3), 16, 23);
        }
        case ARM7IOMemoryAddress::DMA3DAD + 3: {
            return readBits(arm7ptr->readDMADestAddress(3), 24, 31);
        }
        case ARM7IOMemoryAddress::DMA3CNT_L: {
            return readBits(arm7ptr->readDMAControl(3, true), 0, 7);
        }
        case ARM7IOMemoryAddress::DMA3CNT_L + 1: {
            return readBits(arm7ptr->readDMAControl(3, true), 8, 15);
        }
        case ARM7IOMemoryAddress::DMA3CNT_H: {
            return readBits(arm7ptr->readDMAControl(3, false), 0, 7);
        }
        case ARM7IOMemoryAddress::DMA3CNT_H + 1: {
            return readBits(arm7ptr->readDMAControl(3, false), 8, 15);
        }
        case ARM7IOMemoryAddress::IME: {
            return readBits(arm7ptr->readIME(), 0, 7);
        }
        case ARM7IOMemoryAddress::IME + 1: {
            return readBits(arm7ptr->readIME(), 8, 15);
        }
        case ARM7IOMemoryAddress::IME + 2: {
            return readBits(arm7ptr->readIME(), 16, 23);
        }
        case ARM7IOMemoryAddress::IME + 3: {
            return readBits(arm7ptr->readIME(), 24, 31);
        }
        case ARM7IOMemoryAddress::IE: {
            return readBits(arm7ptr->readIE(), 0, 7);
        }
        case ARM7IOMemoryAddress::IE + 1: {
            return readBits(arm7ptr->readIE(), 8, 15);
        }
        case ARM7IOMemoryAddress::IE + 2: {
            return readBits(arm7ptr->readIE(), 16, 23);
        }
        case ARM7IOMemoryAddress::IE + 3: {
            return readBits(arm7ptr->readIE(), 24, 31);
        }
        case ARM7IOMemoryAddress::IF: {
            return readBits(arm7ptr->readIF(), 0, 7);
        }
        case ARM7IOMemoryAddress::IF + 1: {
            return readBits(arm7ptr->readIF(), 8, 15);
        }
        case ARM7IOMemoryAddress::IF + 2: {
            return readBits(arm7ptr->readIF(), 16, 23);
        }
        case ARM7IOMemoryAddress::IF + 3: {
            return readBits(arm7ptr->readIF(), 24, 31);
        }
        default:
            arm7ptr->DEBUG_pauseExecution();
            LogError("Unsupported ARM7 IO 8-bit read at " << hexString(addr));
            return INVALID_MEM_8BIT;
    }
    return INVALID_MEM_8BIT;
}
// ==================================================================================================
void Interconnect::write32IOARM7(uint32_t addr, uint32_t data) {
    ARM7TDMI* arm7ptr = (ARM7TDMI*)arm7;
    // Word align mem accesses.
    addr &= ~0x3;
    // IO memory map.
    switch (addr) {
        case ARM7IOMemoryAddress::DMA0SAD: {
            return arm7ptr->writeDMASourceAddress(0, data);
        }
        case ARM7IOMemoryAddress::DMA0DAD: {
            return arm7ptr->writeDMADestAddress(0, data);
        }
        case ARM7IOMemoryAddress::DMA0CNT_L: {
            arm7ptr->writeDMAControl(0, data, true);
            return arm7ptr->writeDMAControl(0, data >> 16, false);
        }
        case ARM7IOMemoryAddress::DMA1SAD: {
            return arm7ptr->writeDMASourceAddress(1, data);
        }
        case ARM7IOMemoryAddress::DMA1DAD: {
            return arm7ptr->writeDMADestAddress(1, data);
        }
        case ARM7IOMemoryAddress::DMA1CNT_L: {
            arm7ptr->writeDMAControl(1, data, true);
            return arm7ptr->writeDMAControl(1, data >> 16, false);
        }
        case ARM7IOMemoryAddress::DMA2SAD: {
            return arm7ptr->writeDMASourceAddress(2, data);
        }
        case ARM7IOMemoryAddress::DMA2DAD: {
            return arm7ptr->writeDMADestAddress(2, data);
        }
        case ARM7IOMemoryAddress::DMA2CNT_L: {
            arm7ptr->writeDMAControl(2, data, true);
            return arm7ptr->writeDMAControl(2, data >> 16, false);
        }
        case ARM7IOMemoryAddress::DMA3SAD: {
            return arm7ptr->writeDMASourceAddress(3, data);
        }
        case ARM7IOMemoryAddress::DMA3DAD: {
            return arm7ptr->writeDMADestAddress(3, data);
        }
        case ARM7IOMemoryAddress::DMA3CNT_L: {
            arm7ptr->writeDMAControl(3, data, true);
            return arm7ptr->writeDMAControl(3, data >> 16, false);
        }
        case ARM7IOMemoryAddress::IPCSYNC: {
            return arm7ptr->writeIPCSYNC(data);
        }
        case ARM7IOMemoryAddress::IPCFIFOCNT: {
            return arm7ptr->writeIPCFIFOCNT(data);
        }
        case ARM7IOMemoryAddress::IPCFIFOSEND: {
            return arm7ptr->writeIPCFIFOSEND(data);
        }
        case ARM7IOMemoryAddress::IME: {
            return arm7ptr->writeIME(data);
        }
        case ARM7IOMemoryAddress::IE: {
            return arm7ptr->writeIE(data);
        }
        case ARM7IOMemoryAddress::IF: {
            return arm7ptr->writeIF(data);
        }
        default:
            arm7ptr->DEBUG_pauseExecution();
            LogError("Unsupported ARM7 IO 32-bit write at " << hexString(addr));
            break;
    }
}
// ==================================================================================================
void Interconnect::write16IOARM7(uint32_t addr, uint16_t data) {
    ARM7TDMI* arm7ptr = (ARM7TDMI*)arm7;
    // Half word align mem accesses.
    addr &= ~0x1;
    // IO memory map.
    switch (addr) {
        case ARM7IOMemoryAddress::DMA0CNT_L: {
            return arm7ptr->writeDMAControl(0, data, true);
        }
        case ARM7IOMemoryAddress::DMA0CNT_H: {
            return arm7ptr->writeDMAControl(0, data, false);
        }
        case ARM7IOMemoryAddress::DMA1CNT_L: {
            return arm7ptr->writeDMAControl(1, data, true);
        }
        case ARM7IOMemoryAddress::DMA1CNT_H: {
            return arm7ptr->writeDMAControl(1, data, false);
        }
        case ARM7IOMemoryAddress::DMA2CNT_L: {
            return arm7ptr->writeDMAControl(2, data, true);
        }
        case ARM7IOMemoryAddress::DMA2CNT_H: {
            return arm7ptr->writeDMAControl(2, data, false);
        }
        case ARM7IOMemoryAddress::DMA3CNT_L: {
            return arm7ptr->writeDMAControl(3, data, true);
        }
        case ARM7IOMemoryAddress::DMA3CNT_H: {
            return arm7ptr->writeDMAControl(3, data, false);
        }
        case ARM7IOMemoryAddress::IPCSYNC: {
            return arm7ptr->writeIPCSYNC(data);
        }
        case ARM7IOMemoryAddress::IPCFIFOCNT: {
            return arm7ptr->writeIPCFIFOCNT(data);
        }
        case ARM7IOMemoryAddress::IME: {
            uint32_t val = arm7ptr->readIME();
            writeBits(val, (uint32_t)data, 0, 15);
            arm7ptr->writeIME(val);
            break;
        }
        case ARM7IOMemoryAddress::IME + 2: {
            uint32_t val = arm7ptr->readIME();
            writeBits(val, (uint32_t)data, 16, 31);
            arm7ptr->writeIME(val);
            break;
        }
        case ARM7IOMemoryAddress::IE: {
            uint32_t val = arm7ptr->readIE();
            writeBits(val, (uint32_t)data, 0, 15);
            arm7ptr->writeIE(val);
            break;
        }
        case ARM7IOMemoryAddress::IE + 2: {
            uint32_t val = arm7ptr->readIE();
            writeBits(val, (uint32_t)data, 16, 31);
            arm7ptr->writeIE(val);
            break;
        }
        case ARM7IOMemoryAddress::IF: {
            uint32_t val = arm7ptr->readIF();
            writeBits(val, (uint32_t)data, 0, 15);
            arm7ptr->writeIF(val);
            break;
        }
        case ARM7IOMemoryAddress::IF + 2: {
            uint32_t val = arm7ptr->readIF();
            writeBits(val, (uint32_t)data, 16, 31);
            arm7ptr->writeIF(val);
            break;
        }
        default:
            arm7ptr->DEBUG_pauseExecution();
            LogError("Unsupported ARM7 IO 16-bit write at " << hexString(addr));
            break;
    }
}
// ==================================================================================================
void Interconnect::write8IOARM7(uint32_t addr, uint8_t data) {
    ARM7TDMI* arm7ptr = (ARM7TDMI*)arm7;
    // Half word align mem accesses.
    addr &= ~0x1;
    // IO memory map.
    switch (addr) {
        case ARM7IOMemoryAddress::DMA0SAD: {
            uint32_t val = arm7ptr->readDMASourceAddress(0);
            writeBits(val, (uint32_t)data, 0, 7);
            arm7ptr->writeDMASourceAddress(0, val);
            break;
        }
        case ARM7IOMemoryAddress::DMA0SAD + 1: {
            uint32_t val = arm7ptr->readDMASourceAddress(0);
            writeBits(val, (uint32_t)data, 8, 15);
            arm7ptr->writeDMASourceAddress(0, val);
            break;
        }
        case ARM7IOMemoryAddress::DMA0SAD + 2: {
            uint32_t val = arm7ptr->readDMASourceAddress(0);
            writeBits(val, (uint32_t)data, 16, 23);
            arm7ptr->writeDMASourceAddress(0, val);
            break;
        }
        case ARM7IOMemoryAddress::DMA0SAD + 3: {
            uint32_t val = arm7ptr->readDMASourceAddress(0);
            writeBits(val, (uint32_t)data, 24, 31);
            arm7ptr->writeDMASourceAddress(0, val);
            break;
        }
        case ARM7IOMemoryAddress::DMA0DAD: {
            uint32_t val = arm7ptr->readDMADestAddress(0);
            writeBits(val, (uint32_t)data, 0, 7);
            arm7ptr->writeDMADestAddress(0, val);
            break;
        }
        case ARM7IOMemoryAddress::DMA0DAD + 1: {
            uint32_t val = arm7ptr->readDMADestAddress(0);
            writeBits(val, (uint32_t)data, 8, 15);
            arm7ptr->writeDMADestAddress(0, val);
            break;
        }
        case ARM7IOMemoryAddress::DMA0DAD + 2: {
            uint32_t val = arm7ptr->readDMADestAddress(0);
            writeBits(val, (uint32_t)data, 16, 23);
            arm7ptr->writeDMADestAddress(0, val);
            break;
        }
        case ARM7IOMemoryAddress::DMA0CNT_L: {
            return arm7ptr->writeDMAControl(0, data, true);
        }
        case ARM7IOMemoryAddress::DMA0CNT_H: {
            return arm7ptr->writeDMAControl(0, data, false);
        }
        case ARM7IOMemoryAddress::DMA1SAD: {
            return arm7ptr->writeDMASourceAddress(1, data);
        }
        case ARM7IOMemoryAddress::DMA1SAD + 1: {
            return arm7ptr->writeDMASourceAddress(1, data);
        }
        case ARM7IOMemoryAddress::DMA1SAD + 2: {
            return arm7ptr->writeDMASourceAddress(1, data);
        }
        case ARM7IOMemoryAddress::DMA1SAD + 3: {
            return arm7ptr->writeDMASourceAddress(1, data);
        }
        case ARM7IOMemoryAddress::DMA1DAD: {
            return arm7ptr->writeDMADestAddress(1, data);
        }
        case ARM7IOMemoryAddress::DMA1DAD + 1: {
            return arm7ptr->writeDMADestAddress(1, data);
        }
        case ARM7IOMemoryAddress::DMA1DAD + 2: {
            return arm7ptr->writeDMADestAddress(1, data);
        }
        case ARM7IOMemoryAddress::DMA1DAD + 3: {
            return arm7ptr->writeDMADestAddress(1, data);
        }
        case ARM7IOMemoryAddress::DMA1CNT_L: {
            return arm7ptr->writeDMAControl(1, data, true);
        }
        case ARM7IOMemoryAddress::DMA1CNT_L + 1: {
            return arm7ptr->writeDMAControl(1, data, true);
        }
        case ARM7IOMemoryAddress::DMA1CNT_H: {
            return arm7ptr->writeDMAControl(1, data, false);
        }
        case ARM7IOMemoryAddress::DMA1CNT_H + 1: {
            return arm7ptr->writeDMAControl(1, data, false);
        }
        case ARM7IOMemoryAddress::DMA2SAD: {
            return arm7ptr->writeDMASourceAddress(2, data);
        }
        case ARM7IOMemoryAddress::DMA2SAD + 1: {
            return arm7ptr->writeDMASourceAddress(2, data);
        }
        case ARM7IOMemoryAddress::DMA2SAD + 2: {
            return arm7ptr->writeDMASourceAddress(2, data);
        }
        case ARM7IOMemoryAddress::DMA2SAD + 3: {
            return arm7ptr->writeDMASourceAddress(2, data);
        }
        case ARM7IOMemoryAddress::DMA2DAD: {
            return arm7ptr->writeDMADestAddress(2, data);
        }
        case ARM7IOMemoryAddress::DMA2DAD + 1: {
            return arm7ptr->writeDMADestAddress(2, data);
        }
        case ARM7IOMemoryAddress::DMA2DAD + 2: {
            return arm7ptr->writeDMADestAddress(2, data);
        }
        case ARM7IOMemoryAddress::DMA2DAD + 3: {
            return arm7ptr->writeDMADestAddress(2, data);
        }
        case ARM7IOMemoryAddress::DMA2CNT_L: {
            return arm7ptr->writeDMAControl(2, data, true);
        }
        case ARM7IOMemoryAddress::DMA2CNT_L + 1: {
            return arm7ptr->writeDMAControl(2, data, true);
        }
        case ARM7IOMemoryAddress::DMA2CNT_H: {
            return arm7ptr->writeDMAControl(2, data, false);
        }
        case ARM7IOMemoryAddress::DMA2CNT_H + 1: {
            return arm7ptr->writeDMAControl(2, data, false);
        }
        case ARM7IOMemoryAddress::DMA3SAD: {
            return arm7ptr->writeDMASourceAddress(3, data);
        }
        case ARM7IOMemoryAddress::DMA3SAD + 1: {
            return arm7ptr->writeDMASourceAddress(3, data);
        }
        case ARM7IOMemoryAddress::DMA3SAD + 2: {
            return arm7ptr->writeDMASourceAddress(3, data);
        }
        case ARM7IOMemoryAddress::DMA3SAD + 3: {
            return arm7ptr->writeDMASourceAddress(3, data);
        }
        case ARM7IOMemoryAddress::DMA3DAD: {
            return arm7ptr->writeDMADestAddress(3, data);
        }
        case ARM7IOMemoryAddress::DMA3DAD + 1: {
            return arm7ptr->writeDMADestAddress(3, data);
        }
        case ARM7IOMemoryAddress::DMA3DAD + 2: {
            return arm7ptr->writeDMADestAddress(3, data);
        }
        case ARM7IOMemoryAddress::DMA3DAD + 3: {
            return arm7ptr->writeDMADestAddress(3, data);
        }
        case ARM7IOMemoryAddress::IPCSYNC: {
            return arm7ptr->writeIPCSYNC(data);
        }
        case ARM7IOMemoryAddress::IME + 1: {
            uint32_t val = arm7ptr->readIME();
            writeBits(val, (uint32_t)data, 8, 15);
            arm7ptr->writeIME(val);
            break;
        }
        case ARM7IOMemoryAddress::IME + 2: {
            uint32_t val = arm7ptr->readIME();
            writeBits(val, (uint32_t)data, 16, 23);
            arm7ptr->writeIME(val);
            break;
        }
        case ARM7IOMemoryAddress::IME + 3: {
            uint32_t val = arm7ptr->readIME();
            writeBits(val, (uint32_t)data, 24, 31);
            arm7ptr->writeIME(val);
            break;
        }
        case ARM7IOMemoryAddress::IE: {
            uint32_t val = arm7ptr->readIE();
            writeBits(val, (uint32_t)data, 0, 7);
            arm7ptr->writeIE(val);
            break;
        }
        case ARM7IOMemoryAddress::IE + 1: {
            uint32_t val = arm7ptr->readIE();
            writeBits(val, (uint32_t)data, 8, 15);
            arm7ptr->writeIE(val);
            break;
        }
        case ARM7IOMemoryAddress::IE + 2: {
            uint32_t val = arm7ptr->readIE();
            writeBits(val, (uint32_t)data, 16, 23);
            arm7ptr->writeIE(val);
            break;
        }
        case ARM7IOMemoryAddress::IE + 3: {
            uint32_t val = arm7ptr->readIE();
            writeBits(val, (uint32_t)data, 24, 31);
            arm7ptr->writeIE(val);
            break;
        }
        case ARM7IOMemoryAddress::IF: {
            uint32_t val = arm7ptr->readIF();
            writeBits(val, (uint32_t)data, 0, 7);
            arm7ptr->writeIF(val);
            break;
        }
        case ARM7IOMemoryAddress::IF + 1: {
            uint32_t val = arm7ptr->readIF();
            writeBits(val, (uint32_t)data, 8, 15);
            arm7ptr->writeIF(val);
            break;
        }
        case ARM7IOMemoryAddress::IF + 2: {
            uint32_t val = arm7ptr->readIF();
            writeBits(val, (uint32_t)data, 16, 23);
            arm7ptr->writeIF(val);
            break;
        }
        case ARM7IOMemoryAddress::IF + 3: {
            uint32_t val = arm7ptr->readIF();
            writeBits(val, (uint32_t)data, 24, 31);
            arm7ptr->writeIF(val);
            break;
        }
        default:
            arm7ptr->DEBUG_pauseExecution();
            LogError("Unsupported ARM7 IO 8-bit write at " << hexString(addr));
            break;
    }
}
// ==================================================================================================
}  // namespace Core
}  // namespace RedPandaDS
