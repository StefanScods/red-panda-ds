#include "interconnect.h"

#include "cpu.h"

// Control print statements.
#define LOG_LEVEL 1
#include "logger.h"

Interconnect::Interconnect() {
    // Malloc Data.

    // RAM.
    mainRAM = new uint8_t[MAIN_RAM_SIZE];
    if (mainRAM == nullptr) {
        LogError("Could not malloc main RAM!");
        exit(1);
    }
}

Interconnect::~Interconnect() {
    arm7 = nullptr;

    delete[] mainRAM;
    mainRAM = nullptr;
}

void Interconnect::bindARM7(ARM* cpu) {
    arm7 = cpu;
    arm7->connectToInterconnect(this);
}

uint32_t Interconnect::read32ARM7(uint32_t addr) {
    // Byte align mem accesses.
    addr &= ~0x3;
    // Memory map.
    uint8_t memRegion = (addr & 0xFF000000) >> 24;
    switch (memRegion) {
        case (MAIN_RAM_MEM_REGION_NUM): {
            uint32_t val = *reinterpret_cast<uint32_t*>(mainRAM + (addr & MAIN_RAM_MASK));
            LogDebug("Main RAM - 32 bit read at " << hexString(addr) << ": " << hexString(val));
            return val;
        }

        default:
            LogError("Unsupported memory access at " << hexString(addr));
            return INVALID_MEM;
    }
    // Unreachable.
    return INVALID_MEM;
}

uint16_t Interconnect::read16ARM7(uint32_t addr) {
    return 0;
}

void Interconnect::write32ARM7(uint32_t addr, uint32_t data) {
}

void Interconnect::write16ARM7(uint32_t addr, uint16_t data) {
}
