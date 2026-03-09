/**
 * @file arm9.cpp
 * @brief Implements ARM946ES
 */
#include "cpu.h"
#include "interconnect.h"

// Control print statements.
#define LOG_LEVEL 2
#include "logger.h"

// ==================================================================================================
ARM946ES::ARM946ES() {
    arm9 = true;
    // Memory Access timings. Based on https://problemkaputt.de/gbatek.htm#dsmemorytimings
    // TODO!!! finish this.
    // Main RAM.
    code_nonSequencial32BitAccessTimings[ARM9MemoryRegionNum::MAIN_RAM] = 18;
    code_sequencial32BitAccessTimings[ARM9MemoryRegionNum::MAIN_RAM] = 18;
    code_nonSequencial16BitAccessTimings[ARM9MemoryRegionNum::MAIN_RAM] = 9;
    code_sequencial16BitAccessTimings[ARM9MemoryRegionNum::MAIN_RAM] = 9;
    data_nonSequencial32BitAccessTimings[ARM9MemoryRegionNum::MAIN_RAM] = 20;
    data_sequencial32BitAccessTimings[ARM9MemoryRegionNum::MAIN_RAM] = 4;
    data_nonSequencial16BitAccessTimings[ARM9MemoryRegionNum::MAIN_RAM] = 18;
    data_sequencial16BitAccessTimings[ARM9MemoryRegionNum::MAIN_RAM] = 2;
}
// ==================================================================================================
ARM946ES::~ARM946ES() {
}
// ==================================================================================================
busPayload ARM946ES::readBus(uint32_t address, uint32_t size, bool codeRead) {
    uint32_t& previousAddr = codeRead ? previousCodeAddr : previousDataAddr;
    /**
     * A sequential cycle requests a transfer to or from an address which is either the
     * same, one word, or one halfword greater than the address used in the preceding
     * cycle
     */
    bool sequencial =
        address == previousAddr || address == previousAddr + 4 || address == previousAddr + 2;
    uint8_t memRegion = (address) >> 24;

    // Preform the read and determine the cycle map to read.
    cycles* cycleMapSequential = nullptr;
    cycles* cycleMapNonSequential = nullptr;
    uint32_t data;
    switch (size) {
        case (32):  // 32 Bit read.
            data = bus->read32ARM7(address);
            cycleMapSequential =
                codeRead ? code_sequencial32BitAccessTimings : data_sequencial32BitAccessTimings;
            cycleMapNonSequential = codeRead ? code_nonSequencial32BitAccessTimings
                                             : data_nonSequencial32BitAccessTimings;
            break;
        case (16):  // 16 Bit read.
            data = bus->read16ARM7(address);
            cycleMapSequential =
                codeRead ? code_sequencial16BitAccessTimings : data_sequencial16BitAccessTimings;
            cycleMapNonSequential = codeRead ? code_nonSequencial16BitAccessTimings
                                             : data_nonSequencial16BitAccessTimings;
            break;
        case (8):  // 8 Bit read.
            data = bus->read8ARM7(address);
            //  8bit data accesses have the same timings as 16bit data.
            // There are no code 8 bit accesses.
            cycleMapSequential = data_sequencial16BitAccessTimings;
            cycleMapNonSequential = data_nonSequencial16BitAccessTimings;
            break;
        default:
            LogError(size << " bit reads are currently unsupported");
            return {0, 0, 0};
    }

    // Update the previous address to this read to keep track of sequencial accesses.
    previousAddr = address;
    return {data, sequencial ? cycleMapSequential[memRegion] : cycleMapNonSequential[memRegion],
            size};
}
// ==================================================================================================
busPayload ARM946ES::writeBus(uint32_t address, uint32_t data, uint32_t size) {
    // Writes always relate to the previous data.
    uint32_t& previousAddr = previousDataAddr;
    /**
     * A sequential cycle requests a transfer to or from an address which is either the
     * same, one word, or one halfword greater than the address used in the preceding
     * cycle
     */
    bool sequencial =
        address == previousAddr || address == previousAddr + 4 || address == previousAddr + 2;
    uint8_t memRegion = (address) >> 24;

    // Preform the read and determine the cycle map to read.
    cycles* cycleMapSequential = nullptr;
    cycles* cycleMapNonSequential = nullptr;
    switch (size) {
        case (32):  // 32 Bit write.
            bus->write32ARM7(address, data);
            cycleMapSequential = data_sequencial32BitAccessTimings;
            cycleMapNonSequential = data_nonSequencial32BitAccessTimings;
            break;
        case (16):  // 16 Bit write.
            bus->write16ARM7(address, data & 0xFFFF);
            cycleMapSequential = data_sequencial16BitAccessTimings;
            cycleMapNonSequential = data_nonSequencial16BitAccessTimings;
            break;
        case (8):
            bus->write8ARM7(address, data & 0xFF);
            //  8bit data accesses have the same timings as 16bit data.
            cycleMapSequential = data_sequencial16BitAccessTimings;
            cycleMapNonSequential = data_nonSequencial16BitAccessTimings;
            break;
        default:
            LogError(size << " bit writes are currently unsupported");
            return {0, 0, 0};
    }

    // Update the previous address to this write to keep track of sequencial accesses.
    previousAddr = address;
    return {data, sequencial ? cycleMapSequential[memRegion] : cycleMapNonSequential[memRegion],
            size};
}
// ==================================================================================================
cycles ARM946ES::cycle() {
    cycles cyclesRan = 0;
    while (currentCycle < targetCycle) {
        // Perform fetches and executes in parallel.
        if (executeCooldown == 0) {
            // Move pipeline.
            instuctionPipeLine[0] = instuctionPipeLine[1];
            instuctionPipeLine[1] = instuctionPipeLine[2];
            instuctionPipeLine[2] = NO_INSTRUCT;
            if (instuctionPipeLine[0] != NO_INSTRUCT) {
                executeCooldown = execute();
            }
        }
        if (fetchCooldown == 0) {
            bool shouldFetch = getThumbMode() ? instuctionPipeLine[1] == NO_INSTRUCT
                                              : instuctionPipeLine[2] == NO_INSTRUCT;
            // Instruction pipeline has space, fetch a new instuction.
            if (shouldFetch) {
                fetchCooldown = fetch();
            }
        }

        // Determine how much to progress the CPU's cycle counter.
        if (fetchCooldown > 0)
            cyclesElapsed = std::min(fetchCooldown, executeCooldown);
        else
            cyclesElapsed = executeCooldown;
        // Catch cycles where no work is done -> filling pipeline.
        if (cyclesElapsed == 0) {
            cyclesElapsed = 1;
        }
        // Increment the cpu's current cycles based on the number of cycles elapsed.
        cyclesRan += cyclesElapsed;
        addCyclesElapsed();
    }

    return cyclesRan;
}
// ==================================================================================================