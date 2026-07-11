/**
 * @file arm7.cpp
 * @brief Implements ARM7TDMI
 */
#include "cpu.h"
#include "interconnect.h"

// Control print statements.
#define LOG_LEVEL 2
#include "logger.h"

namespace RedPandaDS {
namespace Core {

// ==================================================================================================
ARM7TDMI::ARM7TDMI(BIOS* d_bios) : ARM(d_bios) {
    arm9 = false;
    // Inital SP values.
    reg[SP_REGISTER_NUM] = 0x03002F7C;
    regIRQ[0] = 0x03003F80;
    regSVC[0] = 0x03003FC0;

    // Memory Access timings. Based on https://problemkaputt.de/gbatek.htm#dsmemorytimings
    // BIOS.
    code_nonSequencial32BitAccessTimings[ARM7MemoryRegionNum::BIOS] = 1;
    code_sequencial32BitAccessTimings[ARM7MemoryRegionNum::BIOS] = 1;
    code_nonSequencial16BitAccessTimings[ARM7MemoryRegionNum::BIOS] = 1;
    code_sequencial16BitAccessTimings[ARM7MemoryRegionNum::BIOS] = 1;
    data_nonSequencial32BitAccessTimings[ARM7MemoryRegionNum::BIOS] = 1;
    data_sequencial32BitAccessTimings[ARM7MemoryRegionNum::BIOS] = 1;
    data_nonSequencial16BitAccessTimings[ARM7MemoryRegionNum::BIOS] = 1;
    data_sequencial16BitAccessTimings[ARM7MemoryRegionNum::BIOS] = 1;
    // Main RAM.
    code_nonSequencial32BitAccessTimings[ARM7MemoryRegionNum::MAIN_RAM] = 9;
    code_sequencial32BitAccessTimings[ARM7MemoryRegionNum::MAIN_RAM] = 2;
    code_nonSequencial16BitAccessTimings[ARM7MemoryRegionNum::MAIN_RAM] = 8;
    code_sequencial16BitAccessTimings[ARM7MemoryRegionNum::MAIN_RAM] = 1;
    data_nonSequencial32BitAccessTimings[ARM7MemoryRegionNum::MAIN_RAM] = 10;
    data_sequencial32BitAccessTimings[ARM7MemoryRegionNum::MAIN_RAM] = 2;
    data_nonSequencial16BitAccessTimings[ARM7MemoryRegionNum::MAIN_RAM] = 9;
    data_sequencial16BitAccessTimings[ARM7MemoryRegionNum::MAIN_RAM] = 1;
    // WRAM.
    code_nonSequencial32BitAccessTimings[ARM7MemoryRegionNum::WRAM] = 1;
    code_sequencial32BitAccessTimings[ARM7MemoryRegionNum::WRAM] = 1;
    code_nonSequencial16BitAccessTimings[ARM7MemoryRegionNum::WRAM] = 1;
    code_sequencial16BitAccessTimings[ARM7MemoryRegionNum::WRAM] = 1;
    data_nonSequencial32BitAccessTimings[ARM7MemoryRegionNum::WRAM] = 1;
    data_sequencial32BitAccessTimings[ARM7MemoryRegionNum::WRAM] = 1;
    data_nonSequencial16BitAccessTimings[ARM7MemoryRegionNum::WRAM] = 1;
    data_sequencial16BitAccessTimings[ARM7MemoryRegionNum::WRAM] = 1;
    // I/O.
    code_nonSequencial32BitAccessTimings[ARM7MemoryRegionNum::IO] = 1;
    code_sequencial32BitAccessTimings[ARM7MemoryRegionNum::IO] = 1;
    code_nonSequencial16BitAccessTimings[ARM7MemoryRegionNum::IO] = 1;
    code_sequencial16BitAccessTimings[ARM7MemoryRegionNum::IO] = 1;
    data_nonSequencial32BitAccessTimings[ARM7MemoryRegionNum::IO] = 1;
    data_sequencial32BitAccessTimings[ARM7MemoryRegionNum::IO] = 1;
    data_nonSequencial16BitAccessTimings[ARM7MemoryRegionNum::IO] = 1;
    data_sequencial16BitAccessTimings[ARM7MemoryRegionNum::IO] = 1;
    // VRAM.
    code_nonSequencial32BitAccessTimings[ARM7MemoryRegionNum::VRAM] = 2;
    code_sequencial32BitAccessTimings[ARM7MemoryRegionNum::VRAM] = 2;
    code_nonSequencial16BitAccessTimings[ARM7MemoryRegionNum::VRAM] = 1;
    code_sequencial16BitAccessTimings[ARM7MemoryRegionNum::VRAM] = 1;
    data_nonSequencial32BitAccessTimings[ARM7MemoryRegionNum::VRAM] = 2;
    data_sequencial32BitAccessTimings[ARM7MemoryRegionNum::VRAM] = 2;
    data_nonSequencial16BitAccessTimings[ARM7MemoryRegionNum::VRAM] = 1;
    data_sequencial16BitAccessTimings[ARM7MemoryRegionNum::VRAM] = 1;
    // GBA ROM.
    code_nonSequencial32BitAccessTimings[ARM7MemoryRegionNum::GBAROM1] = 16;
    code_nonSequencial32BitAccessTimings[ARM7MemoryRegionNum::GBAROM2] = 16;
    code_sequencial32BitAccessTimings[ARM7MemoryRegionNum::GBAROM1] = 12;
    code_sequencial32BitAccessTimings[ARM7MemoryRegionNum::GBAROM2] = 12;
    code_nonSequencial16BitAccessTimings[ARM7MemoryRegionNum::GBAROM1] = 10;
    code_nonSequencial16BitAccessTimings[ARM7MemoryRegionNum::GBAROM2] = 10;
    code_sequencial16BitAccessTimings[ARM7MemoryRegionNum::GBAROM1] = 6;
    code_sequencial16BitAccessTimings[ARM7MemoryRegionNum::GBAROM2] = 6;
    data_nonSequencial32BitAccessTimings[ARM7MemoryRegionNum::GBAROM1] = 15;
    data_nonSequencial32BitAccessTimings[ARM7MemoryRegionNum::GBAROM2] = 15;
    data_sequencial32BitAccessTimings[ARM7MemoryRegionNum::GBAROM1] = 12;
    data_sequencial32BitAccessTimings[ARM7MemoryRegionNum::GBAROM2] = 12;
    data_nonSequencial16BitAccessTimings[ARM7MemoryRegionNum::GBAROM1] = 9;
    data_nonSequencial16BitAccessTimings[ARM7MemoryRegionNum::GBAROM2] = 9;
    data_sequencial16BitAccessTimings[ARM7MemoryRegionNum::GBAROM1] = 6;
    data_sequencial16BitAccessTimings[ARM7MemoryRegionNum::GBAROM2] = 6;
    // GBA RAM.
    data_nonSequencial32BitAccessTimings[ARM7MemoryRegionNum::GBARAM] = 9;
    data_sequencial32BitAccessTimings[ARM7MemoryRegionNum::GBARAM] = 10;
    data_nonSequencial16BitAccessTimings[ARM7MemoryRegionNum::GBARAM] = 9;
    data_sequencial16BitAccessTimings[ARM7MemoryRegionNum::GBARAM] = 10;
}
// ==================================================================================================
ARM7TDMI::~ARM7TDMI() {
}
// ==================================================================================================
void ARM7TDMI::reset() {
    ARM::reset();

    // Inital SP values.
    reg[SP_REGISTER_NUM] = 0x0380FD80;
    regIRQ[0] = 0x0380FF80;
    regSVC[0] = 0x0380FFC0;
}
// ==================================================================================================
busPayload ARM7TDMI::readBus(uint32_t address, uint32_t size, bool codeRead) {
    uint32_t& previousAddr = codeRead ? previousCodeAddr : previousDataAddr;
    /**
     * A sequential cycle requests a transfer to or from an address which is one word, or one
     * halfword greater than the address used in the preceding cycle
     */
    bool sequencial = address == previousAddr + 4 || address == previousAddr + 2;
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
busPayload ARM7TDMI::writeBus(uint32_t address, uint32_t data, uint32_t size) {
    // Writes always relate to the previous data.
    uint32_t& previousAddr = previousDataAddr;
    /**
     * A sequential cycle requests a transfer to or from an address which is one word, or one
     * halfword greater than the address used in the preceding cycle
     */
    bool sequencial = address == previousAddr + 4 || address == previousAddr + 2;
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
cycles ARM7TDMI::cycle() {
    cycles cyclesRan = 0;
    justBranched = false;
    justHitBreakpoint = false;

    // CPU is running execute like normal.
    while (currentCycle < targetCycle) {
        // Handle execution limit.
        if (hasExecutionLimit && executionLimit == 0) {
            hasExecutionLimit = false;
            justHitBreakpoint = true;
            break;
        }

        // Handle halted or stopped states.
        if (state != ProcessorStates::Running) {
            cyclesRan = underflowSafeSubtract(targetCycle, currentCycle);
            cyclesElapsed = cyclesRan;
            addCyclesElapsed();
            break;
        }

        // Clear just branched indicator.
        justBranched = false;
        // Clear the just hit a breakpoint indicator.
        justHitBreakpoint = false;

        // Advance instuction pipeline.
        if (instuctionPipeLine[0].inst == NO_INSTRUCT) {
            advanceInstructionPipeline();
        }

        // Check if we hit a break point.
        if (instuctionPipeLine[0].inst != NO_INSTRUCT && !justHitBreakpoint &&
            breakpoints.contains(instuctionPipeLine[0].addr)) {
            justHitBreakpoint = true;
            break;
        }

        // Execute Stage.
        if (executeCooldown == 0 && instuctionPipeLine[0].inst != NO_INSTRUCT &&
            (instuctionPipeLine[1].inst != NO_INSTRUCT ||
             instuctionPipeLine[2].inst != NO_INSTRUCT)) {
            // Instruction is on the execute stage, execute a new instuction.
            executeCooldown = execute();
            // Decrement execution limit.
            if (hasExecutionLimit && executionLimit >= 0) {
                executionLimit--;
            }
        }

        // Fetch Stage.
        if (fetchCooldown == 0 && instuctionPipeLine[2].inst == NO_INSTRUCT &&
            (instuctionPipeLine[1].inst == NO_INSTRUCT ||
             instuctionPipeLine[0].inst == NO_INSTRUCT)) {
            // Instruction pipeline has space, fetch a new instuction.
            fetchCooldown = fetch();
        }

        // Determine how much to progress the CPU's cycle counter.
        if (fetchCooldown > 0 && executeCooldown > 0) {
            cyclesElapsed = std::min(fetchCooldown, executeCooldown);
        } else if (fetchCooldown > 0) {
            cyclesElapsed = fetchCooldown;
        } else if (executeCooldown > 0) {
            cyclesElapsed = executeCooldown;
        } else {
            // Catch cycles where no work is done -> filling pipeline.
            cyclesElapsed = 1;
        }
        // Increment the cpu's current cycles based on the number of cycles elapsed.
        cyclesRan += cyclesElapsed;
        addCyclesElapsed();
    }

    return cyclesRan;
}
// ==================================================================================================

}  // namespace Core
}  // namespace RedPandaDS
