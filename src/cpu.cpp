#include "cpu.h"

#include "interconnect.h"

// Control print statements.
#define LOG_LEVEL 2
#include "logger.h"
// ==================================================================================================
std::vector<std::string> g_regNames = {"r0", "r1", "r2",  "r3",  "r4",  "r5",  "r6",  "r7",
                                       "r8", "r9", "r10", "r11", "r12", "r13", "r14", "r15"};
// ==================================================================================================
ARM::ARM() {
    reset();
}
// ==================================================================================================
ARM::~ARM() {
    bus = nullptr;
}
// ==================================================================================================
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
// ==================================================================================================
cycles ARM::execute() {
    bool thumbMode = readBit(cpsr, T_BIT);
    return thumbMode ? THUMB_execute() : ARM_execute();
}
// ==================================================================================================
cycles ARM::ARM_execute() {
    // Make space in the pipeline.
    uint32_t nextInstruction = instuctionPipeLine[0];
    instuctionPipeLine[0] = NO_INSTRUCT;
    uint8_t condition = readBits(nextInstruction, 28, 31);
    LogDebug("Executing " << PrintHex(nextInstruction) << " with condition code "
                          << PrintHex(condition) << "!");
    // TODO!!! Remove this if statement once the execute() is less buggy.
    if (nextInstruction == NO_INSTRUCT) {
        LogError("Trying to execute an empty pipeline / bubble in pipeline!");
    }
    // Check the condition code skip opcodes which do not pass.
    if (!checkIfConditionPassed(ConditionMnemonics::ConditionMnemonics(condition))) {
        return 1;
    }
    // Decode and execute the instuction.
    uint8_t opCode = readBits(nextInstruction, 26, 27);
    // https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/ARM-Instruction-Set-Encoding/ARM-instruction-set-encoding?lang=en
    switch (opCode) {
        // Data-processing and miscellaneous instructions.
        case 0b00:
            return ARM_dataProcessingDecodeAndExecute(nextInstruction, condition);
        // Load/store word and unsigned byte.
        case 0b01:
            return ARM_loadStoreDecodeAndExecute(nextInstruction, condition);
        // 	Branch, branch with link, and block data transfer.
        case 0b10:
            return ARM_branchAndBlockDataTransferDecodeAndExecute(nextInstruction, condition);
        // Coprocessor instructions, and Supervisor Call
        case 0b11:
            return ARM_coprocessorAndSupervisorDecodeAndExecute(nextInstruction, condition);
        default:
            break;
    }
    return ARM_UNDEFINED_INST(nextInstruction);
}
// ==================================================================================================
cycles ARM::THUMB_execute() {
    // Make space in the pipeline.
    uint32_t nextInstruction = instuctionPipeLine[0];
    instuctionPipeLine[0] = NO_INSTRUCT;
    // TODO!!! Remove this if statement once the execute() is less buggy.
    if (nextInstruction == NO_INSTRUCT) {
        LogError("Trying to execute an empty pipeline / bubble in pipeline!");
    }
    // Decode and execute the instuction.
    uint8_t opCode = readBits(nextInstruction, 10, 15);
    uint8_t bit9 = readBit(nextInstruction, 9);
    // https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Thumb-Instruction-Set-Encoding/16-bit-Thumb-instruction-encoding?lang=en
    switch (opCode) {
        // Shift (immediate), add, subtract, move, and compare
        // Data-processing
        case 0b000000:
        case 0b000001:
        case 0b000010:
        case 0b000011:
        case 0b000100:
        case 0b000101:
        case 0b000110:
        case 0b000111:
        case 0b001000:
        case 0b001001:
        case 0b001010:
        case 0b001011:
        case 0b001100:
        case 0b001101:
        case 0b001110:
        case 0b001111:
        case 0b010000:
            return THUMB_dataProcessingDecodeAndExecute(nextInstruction);
        // Special data instructions and branch and exchange
        case 0b010001:
            return THUMB_specialDataAndBranchDecodeAndExecute(nextInstruction);
        // LDR (literal)
        case 0b010010:
        case 0b010011:
            return THUMB_LDR_LIT(nextInstruction);
        // Load/store single data item
        case 0b010100:
        case 0b010101:
        case 0b010110:
        case 0b010111:
        case 0b011000:
        case 0b011001:
        case 0b011010:
        case 0b011011:
        case 0b011100:
        case 0b011101:
        case 0b011110:
        case 0b011111:
        case 0b100000:
        case 0b100001:
        case 0b100010:
        case 0b100011:
        case 0b100100:
        case 0b100101:
        case 0b100110:
        case 0b100111:
            return THUMB_loadStoreDecodeAndExecute(nextInstruction);
        // Generate PC-relative address
        case 0b101000:
        case 0b101001:
            return THUMB_ADR(nextInstruction);
        // Generate SP-relative address,
        case 0b101010:
        case 0b101011:
            return THUMB_ADD_SP_IMM(nextInstruction);
        // Miscellaneous 16-bit instructions
        case 0b101100:
        case 0b101101:
        case 0b101110:
        case 0b101111:
            return THUMB_miscDecodeAndExecute(nextInstruction);
        // Store multiple registers
        case 0b110000:
        case 0b110001:
            return THUMB_STM(nextInstruction);
        // Load multiple registers
        case 0b110010:
        case 0b110011:
            return THUMB_LDM(nextInstruction);
        // Conditional branch, and Supervisor Call
        case 0b110100:
        case 0b110101:
        case 0b110110:
        case 0b110111:
            return THUMB_condBranchAndSupervisorCallDecodeAndExecute(nextInstruction);
        // Unconditional Branch
        case 0b111000:
        case 0b111001:
            return THUMB_B(nextInstruction);
        // 32-bit Thumb instructions -> Unsupported on DS's arm7 and arm9 cores.
        case 0b111010:
        case 0b111011:
        case 0b111100:
        case 0b111101:
        case 0b111110:
        case 0b111111:
            return THUMB_UNDEFINED_INST(nextInstruction);
        default:
            break;
    }
    return THUMB_UNDEFINED_INST(nextInstruction);
}
// ==================================================================================================
cycles ARM::fetch() {
    // Fetch the next 32 bits and increment PC.
    LogDebug("Fetching instruction at: " << PrintHex(pc()) << "...");
    bool thumbMode = readBit(cpsr, T_BIT);
    busPayload readResult = readBus(pc(), 32, true);
    LogDebug("Fetched instruction: " << PrintHex(readResult.data) << "!");
    if (thumbMode) {
        // Fill two stages of the pipeline with half words.
        instuctionPipeLine[1] = readResult.data & 0xFFFF;
        instuctionPipeLine[2] = readResult.data >> 8;
    } else {
        instuctionPipeLine[2] = readResult.data;
    }
    pc() += 4;
    // Get the fetch cooldown.
    return readResult.numCycles;
}
// ==================================================================================================
cycles ARM::cycle() {
    LogError("Unimplemented cycle()");
    return 0;
}
// ==================================================================================================
cycles ARM::fetchAndExecute(int numExecutions) {
    cycles cycleCount = 0;
    bool thumbMode = readBit(cpsr, T_BIT);
    while (numExecutions) {
        instuctionPipeLine[0] = instuctionPipeLine[1];
        instuctionPipeLine[1] = instuctionPipeLine[2];
        cycles exeCycles = 0;
        if (instuctionPipeLine[0] != NO_INSTRUCT) {
            exeCycles = execute();
            numExecutions--;
        }
        // Fetch the next instruction.
        // (Always if not in thumb. When in thumb, instuctionPipeLine[1] is empty).
        bool shouldFetch = !thumbMode || instuctionPipeLine[1] != NO_INSTRUCT;
        cycles fetchCycles = 0;
        if (shouldFetch) {
            fetchCycles = fetch();
        }
        cycleCount += std::max(exeCycles, fetchCycles);
    }
    return cycleCount;
}
// ==================================================================================================
busPayload ARM::readBus(uint32_t address, uint32_t size, bool codeRead) {
    LogError("Unimplemented readBus()");
    return {0, 0, 0};
}
// ==================================================================================================
busPayload ARM::writeBus(uint32_t address, uint32_t data, uint32_t size) {
    LogError("Unimplemented writeBus()");
    return {0, 0, 0};
}
// ==================================================================================================
cycles ARM::ARM_UNDEFINED_INST(uint32_t instruct) {
    LogError("Unsupported ARM instuction: " << PrintHex(instruct) << "!");
    return 1;
}
// ==================================================================================================
cycles ARM::THUMB_UNDEFINED_INST(uint32_t instruct) {
    LogError("Unsupported THUMB instuction: " << PrintHex(instruct) << "!");
    return 1;
}
// ==================================================================================================
void ARM::branch(uint32_t dest) {
    // Determine if we are going to thumb mode.
    bool thumb = readBit(dest, 0);
    writeBit(cpsr, thumb, T_BIT);
    // Mask the bottom bit (thumb mode) or bottom 2 bits (arm mode).
    uint32_t pcMask = thumb ? ~(0b1) : ~(0b11);
    LogDebug("Branching - PC currently at: " << PrintHex(pc()) << "...");
    LogDebug("Moving PC to: " << PrintHex(dest & pcMask) << "!");
    LogDebug("Thumb mode after branch: " << thumb << "!");
    pc() = dest & pcMask;
    // Clear the instuction pipeline.
    instuctionPipeLine[0] = NO_INSTRUCT;
    instuctionPipeLine[1] = NO_INSTRUCT;
    instuctionPipeLine[2] = NO_INSTRUCT;
}
// ==================================================================================================
void ARM::fixupIfTargetingPC(uint32_t destReg) {
    if (destReg != PC_REGISTER_NUM) return;
    branch(pc());
}
// ==================================================================================================
bool ARM::checkIfConditionPassed(ConditionMnemonics::ConditionMnemonics condition) {
    bool Z = readBit(cpsr, Z_FLAG);
    bool C = readBit(cpsr, C_FLAG);
    bool N = readBit(cpsr, N_FLAG);
    bool V = readBit(cpsr, V_FLAG);
    switch (condition) {
        case ConditionMnemonics::EQ:
            if (Z) return true;
            LogDebug("Condition EQ not passed!");
            return false;
        case ConditionMnemonics::NE:
            if (!Z) return true;
            LogDebug("Condition NE not passed!");
            return false;
        case ConditionMnemonics::CS:
            if (C) return true;
            LogDebug("Condition CS not passed!");
            return false;
        case ConditionMnemonics::CC:
            if (!C) return true;
            LogDebug("Condition CC not passed!");
            return false;
        case ConditionMnemonics::MI:
            if (N) return true;
            LogDebug("Condition MI not passed!");
            return false;
        case ConditionMnemonics::PL:
            if (!N) return true;
            LogDebug("Condition PL not passed!");
            return false;
        case ConditionMnemonics::VS:
            if (V) return true;
            LogDebug("Condition VS not passed!");
            return false;
        case ConditionMnemonics::VC:
            if (!V) return true;
            LogDebug("Condition VC not passed!");
            return false;
        case ConditionMnemonics::HI:
            if (C && !Z) return true;
            LogDebug("Condition HI not passed!");
            return false;
        case ConditionMnemonics::LS:
            if (!C || Z) return true;
            LogDebug("Condition LS not passed!");
            return false;
        case ConditionMnemonics::GE:
            if (N == V) return true;
            LogDebug("Condition GE not passed!");
            return false;
        case ConditionMnemonics::LT:
            if (N != V) return true;
            LogDebug("Condition LT not passed!");
            return false;
        case ConditionMnemonics::GT:
            if (!Z && N == V) return true;
            LogDebug("Condition GT not passed!");
            return false;
        case ConditionMnemonics::LE:
            if (Z || N != V) return true;
            LogDebug("Condition LE not passed!");
            return false;
        case ConditionMnemonics::AL:
        case ConditionMnemonics::SPECIAL:
            return true;  // Unconditional execution.
        default:
            return false;
    }
    return false;
}
// ==================================================================================================
