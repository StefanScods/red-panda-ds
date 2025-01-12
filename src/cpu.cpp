#include "cpu.h"

#include "interconnect.h"

// Control print statements.
#define LOG_LEVEL 1
#include "logger.h"

ARM::ARM() {
    reset();
}

ARM::~ARM() {
    bus = nullptr;
}

void ARM::reset() {
    // Standard Registers.
    reg[0] = 0;
    reg[1] = 0;
    reg[2] = 0;
    reg[3] = 0;
    reg[4] = 0;
    reg[5] = 0;
    reg[6] = 0;
    reg[7] = 0;
    reg[8] = 0;
    reg[9] = 0;
    reg[10] = 0;
    reg[11] = 0;
    reg[12] = 0;
    reg[13] = 0;
    reg[14] = 0;
    reg[15] = 0;  // PC.
    cpsr = 0;

    // Fast interrupt Registers.
    regFIQ[0] = 0;
    regFIQ[1] = 0;
    regFIQ[2] = 0;
    regFIQ[3] = 0;
    regFIQ[4] = 0;
    regFIQ[5] = 0;
    regFIQ[6] = 0;
    regFIQ[7] = 0;

    // Supervisor Registers.
    regSVC[0] = 0;
    regSVC[1] = 0;
    regSVC[2] = 0;

    // Abort Registers.
    regABT[0] = 0;
    regABT[1] = 0;
    regABT[2] = 0;

    // Interrupt Registers.
    regIRQ[0] = 0;
    regIRQ[1] = 0;
    regIRQ[2] = 0;

    // Undefined Registers.
    regUND[0] = 0;
    regUND[1] = 0;
    regUND[2] = 0;

    // Control vars.
    currentCycle = 0;
    cyclesElapsed = 0;
    targetCycle = 0;

    // CPU instruction Pipeline.
    instuctionPipeLine[0] = NO_INSTRUCT;
    instuctionPipeLine[1] = NO_INSTRUCT;
    instuctionPipeLine[2] = NO_INSTRUCT;

    previousCodeAddr = 0;
    previousDataAddr = 0;
}

cycles ARM::execute() {
    LogError("Unimplemented execute()");
    return 0;
}

cycles ARM::fetch() {
    LogError("Unimplemented fetch()");
    return 0;
}

cycles ARM::cycle() {
    LogError("Unimplemented cycle()");
    return 0;
}

cycles ARM::fetchAndExecute() {
    cycles cycleCount = 0;
    cycleCount += fillInstuctionPipeline();
    cycleCount += execute();
    return cycleCount;
}

cycles ARM::fillInstuctionPipeline() {
    cycles cycleCount = 0;
    // Fill pipeline.
    while (instuctionPipeLine[0] == NO_INSTRUCT) {
        instuctionPipeLine[0] = instuctionPipeLine[1];
        instuctionPipeLine[1] = instuctionPipeLine[2];
        cycleCount += fetch();
    }
    return cycleCount;
}

busPayload ARM::readBus(uint32_t address, uint32_t size, bool codeRead) {
    LogError("Unimplemented readBus()");
    return {0, 0, 0};
}

busPayload ARM::writeBus(uint32_t address, uint32_t data, uint32_t size) {
    LogError("Unimplemented writeBus()");
    return {0, 0, 0};
}

ARM946ES::ARM946ES() {
}

ARM946ES::~ARM946ES() {
}