/**
 * @file arm7.cpp
 * @brief Implements ARM7TDMI
 */
#include "cpu.h"
#include "interconnect.h"

// Control print statements.
#define LOG_LEVEL 1
#include "logger.h"

uint16_t ARM7TDMI::read16Bits(uint32_t addr) {
    return bus->read16ARM7(addr);
}
uint32_t ARM7TDMI::read32Bits(uint32_t addr) {
    return bus->read32ARM7(addr);
}
void ARM7TDMI::write16Bits(uint32_t addr, uint16_t data) {
    bus->write16ARM7(addr, data);
}
void ARM7TDMI::write32Bits(uint32_t addr, uint32_t data) {
    bus->write32ARM7(addr, data);
}

ARM7TDMI::ARM7TDMI() {
}

ARM7TDMI::~ARM7TDMI() {
}

cycles ARM7TDMI::execute() {
    return 0;
}

cycles ARM7TDMI::fetch() {
    return 0;
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

        if (fetchCooldown > 0)
            cyclesElapsed = std::min(fetchCooldown, executeCooldown);
        else
            cyclesElapsed = executeCooldown;

        // TODO!!! is this possible???
        if (cyclesElapsed == 0) {
            cyclesElapsed = 1;
        }

        // Increment the cpu's current cycles based on the number of cycles elapsed.
        cyclesRan += cyclesElapsed;
        addCyclesElapsed();
    }

    return cyclesRan;
}