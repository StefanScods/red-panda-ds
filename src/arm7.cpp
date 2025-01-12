/**
 * @file arm7.cpp
 * @brief Implements ARM7TDMI
 */
#include "cpu.h"
#include "interconnect.h"

// Control print statements.
#include "logger.h"

ARM7TDMI::ARM7TDMI() {
    // Memory Access timings. Based on https://problemkaputt.de/gbatek.htm#dsmemorytimings
    // TODO!!! finish this.
    // Main RAM.
    code_nonSequencial32BitAccessTimings[ARM7MemoryRegionNum::MAIN_RAM] = 9;
    code_sequencial32BitAccessTimings[ARM7MemoryRegionNum::MAIN_RAM] = 2;
    code_nonSequencial16BitAccessTimings[ARM7MemoryRegionNum::MAIN_RAM] = 8;
    code_sequencial16BitAccessTimings[ARM7MemoryRegionNum::MAIN_RAM] = 1;
    data_nonSequencial32BitAccessTimings[ARM7MemoryRegionNum::MAIN_RAM] = 10;
    data_sequencial32BitAccessTimings[ARM7MemoryRegionNum::MAIN_RAM] = 2;
    data_nonSequencial16BitAccessTimings[ARM7MemoryRegionNum::MAIN_RAM] = 9;
    data_sequencial16BitAccessTimings[ARM7MemoryRegionNum::MAIN_RAM] = 1;
}

ARM7TDMI::~ARM7TDMI() {
}

busPayload ARM7TDMI::readBus(uint32_t address, uint32_t size, bool codeRead) {
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
        case (8):
        default:
            LogError(size << " bit reads are currently unsupported");
            return {0, 0, 0};
    }

    // Update the previous address to this read to keep track of sequencial accesses.
    previousAddr = address;
    return {data, sequencial ? cycleMapSequential[memRegion] : cycleMapNonSequential[memRegion],
            size};
}

busPayload ARM7TDMI::writeBus(uint32_t address, uint32_t data, uint32_t size) {
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
            bus->write16ARM7(address, data);
            cycleMapSequential = data_sequencial16BitAccessTimings;
            cycleMapNonSequential = data_nonSequencial16BitAccessTimings;
            break;
        case (8):
        default:
            LogError(size << " bit writes are currently unsupported");
            return {0, 0, 0};
    }

    // Update the previous address to this write to keep track of sequencial accesses.
    previousAddr = address;
    return {data, sequencial ? cycleMapSequential[memRegion] : cycleMapNonSequential[memRegion],
            size};
}

cycles ARM7TDMI::execute() {
    // Make space in the pipeline.
    uint32_t nextInstruction = instuctionPipeLine[0];
    instuctionPipeLine[0] = NO_INSTRUCT;
    // Decode and execute the instuction.
    uint8_t opCode = readBits(nextInstruction, 25, 27);
    uint8_t condition = readBits(nextInstruction, 28, 31);
    // https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/ARM-Instruction-Set-Encoding/ARM-instruction-set-encoding?lang=en
    switch (opCode) {
        case 0b000:
        case 0b001:
            return ARM::dataProcessingDecodeAndExecute(nextInstruction, condition);
        // Load/store word and unsigned byte.
        case 0b010:
        case 0b011:
            return ARM::loadStoreDecodeAndExecute(nextInstruction, condition);
        default:
            LogError("Unsupported OpCode: " << opCode
                                            << "! Full instruction data: " << nextInstruction);
            return 1;
    }
}

cycles ARM7TDMI::fetch() {
    // Fetch the next 32 bits and increment PC.
    busPayload readResult = readBus(pc, 32, true);
    instuctionPipeLine[2] = readResult.data;
    pc += 4;
    // Get the fetch cooldown.
    return readResult.numCycles;
}

cycles ARM7TDMI::cycle() {
    cycles cyclesRan = 0;
    while (currentCycle < targetCycle) {
        // Perform fetches and executes in parallel.
        if (fetchCooldown == 0) {
            // Instruction pipeline has space, fetch a new instuction.
            if (instuctionPipeLine[2] == NO_INSTRUCT) {
                fetchCooldown = fetch();
            }
        }
        if (executeCooldown == 0) {
            // Move pipeline.
            instuctionPipeLine[0] = instuctionPipeLine[1];
            instuctionPipeLine[1] = instuctionPipeLine[2];
            instuctionPipeLine[2] = NO_INSTRUCT;
            if (instuctionPipeLine[0] != NO_INSTRUCT) {
                executeCooldown = execute();
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