#include "cpu.h"

#include <cmath>

#include "interconnect.h"

// Control print statements.
#define LOG_LEVEL 1
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
    spsrFIQ = 0;
    regFIQ[0] = 0;
    regFIQ[1] = 0;
    regFIQ[2] = 0;
    regFIQ[3] = 0;
    regFIQ[4] = 0;
    regFIQ[5] = 0;
    regFIQ[6] = 0;

    // Supervisor Registers.
    spsrSVC = 0;
    regSVC[0] = 0;
    regSVC[1] = 0;

    // Abort Registers.
    spsrABT = 0;
    regABT[0] = 0;
    regABT[1] = 0;

    // Interrupt Registers.
    spsrIRQ = 0;
    regIRQ[0] = 0;
    regIRQ[1] = 0;

    // Undefined Registers.
    spsrUND = 0;
    regUND[0] = 0;
    regUND[1] = 0;

    // Control vars.
    currentCycle = 0;
    cyclesElapsed = 0;
    targetCycle = 0;
    justBranched = false;

    // CPU instruction Pipeline.
    clearInstructionPipeline();

    previousCodeAddr = 0;
    previousDataAddr = 0;

    setProcessorMode(ProcessorModes::User);
}
// ==================================================================================================
cycles ARM::execute() {
    LogDebug("Executing in " << (getThumbMode() ? "THUMB" : "ARM") << " mode!");
    return getThumbMode() ? THUMB_execute() : ARM_execute();
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
    // https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Thumb-Instruction-Set-Encoding/16-bit-Thumb-instruction-encoding?lang=en
    switch (opCode) {
        // Shift (immediate), add, subtract, move, and compare
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
            return THUMB_shiftAddSubtractMoveCompareDecodeAndExecute(nextInstruction);
        // Data-processing
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
            return THUMB_ADD_SP_IMM8(nextInstruction);
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
    if (justBranched) return 1;
    cycles cycleCount = 1;
    if (instructionQueue.empty()) {
        // Fetch the next 32 bits and increment PC.
        LogDebug("Fetching instruction at: " << PrintHex(pc()) << "...");
        busPayload readResult = readBus(pc(), 32, true);
        cycleCount = readResult.numCycles;
        if (getThumbMode()) {
            // Fill the instruction queue with half words.
            instructionQueue.push(readResult.data & 0xFFFF);
            instructionQueue.push(readResult.data >> 16);
        } else {
            instructionQueue.push(readResult.data);
        }
    }
    // Get the next instruction onto the pipeline.
    instuctionPipeLine[2] = instructionQueue.front();
    instructionQueue.pop();
    LogDebug("Fetched instruction: " << PrintHex(instuctionPipeLine[2]) << "!");
    pc() += getThumbMode() ? THUMB_MODE_INST_SIZE : ARM_MODE_INST_SIZE;
    // Get the fetch cooldown.
    return cycleCount;
}
// ==================================================================================================
cycles ARM::cycle() {
    LogError("Unimplemented cycle()");
    return 0;
}
// ==================================================================================================
cycles ARM::fetchAndExecute(int numExecutions) {
    cycles cycleCount = 0;
    while (numExecutions) {
        justBranched = false;
        advanceInstructionPipeline();
        cycles exeCycles = 0;
        if (instuctionPipeLine[0] != NO_INSTRUCT) {
            exeCycles = execute();
            numExecutions--;
        }
        // Fetch the next instruction.
        cycles fetchCycles = fetch();

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
void ARM::setProcessorMode(ProcessorModes::ProcessorModes mode) {
    uint8_t currentMode = readBits(cpsr, MODE_LOWER_BIT, MODE_UPPER_BIT);
    if (currentMode == mode) return;
    writeBits(cpsr, (uint32_t)mode, MODE_LOWER_BIT, MODE_UPPER_BIT);
    handleProcessorModeChange();
}
// ==================================================================================================
void ARM::handleProcessorModeChange() {
    uint8_t currentMode = readBits(cpsr, MODE_LOWER_BIT, MODE_UPPER_BIT);
    // 0 - 8 are always left unchanged. Same With PC (15).
    switch (currentMode) {
        // System and user use the same registers.
        case ProcessorModes::System:
        case ProcessorModes::User: {
            spsr = nullptr;
            activeRegs[8] = &reg[8];
            activeRegs[9] = &reg[9];
            activeRegs[10] = &reg[10];
            activeRegs[11] = &reg[11];
            activeRegs[12] = &reg[12];
            activeRegs[13] = &reg[13];
            activeRegs[14] = &reg[14];
            break;
        }
        case ProcessorModes::FIQ: {
            spsr = &spsrFIQ;
            activeRegs[8] = &regFIQ[0];
            activeRegs[9] = &regFIQ[1];
            activeRegs[10] = &regFIQ[2];
            activeRegs[11] = &regFIQ[3];
            activeRegs[12] = &regFIQ[4];
            activeRegs[13] = &regFIQ[5];
            activeRegs[14] = &regFIQ[6];
            break;
        }
        case ProcessorModes::IRQ: {
            spsr = &spsrIRQ;
            activeRegs[8] = &reg[8];
            activeRegs[9] = &reg[9];
            activeRegs[10] = &reg[10];
            activeRegs[11] = &reg[11];
            activeRegs[12] = &reg[12];
            activeRegs[13] = &regIRQ[0];
            activeRegs[14] = &regIRQ[1];
            break;
        }
        case ProcessorModes::Supervisor: {
            spsr = &spsrSVC;
            activeRegs[8] = &reg[8];
            activeRegs[9] = &reg[9];
            activeRegs[10] = &reg[10];
            activeRegs[11] = &reg[11];
            activeRegs[12] = &reg[12];
            activeRegs[13] = &regSVC[0];
            activeRegs[14] = &regSVC[0];
            break;
        }
        case ProcessorModes::Abort: {
            spsr = &spsrABT;
            activeRegs[8] = &reg[8];
            activeRegs[9] = &reg[9];
            activeRegs[10] = &reg[10];
            activeRegs[11] = &reg[11];
            activeRegs[12] = &reg[12];
            activeRegs[13] = &regABT[0];
            activeRegs[14] = &regABT[1];
            break;
        }
        case ProcessorModes::Undefined: {
            spsr = &spsrUND;
            activeRegs[8] = &reg[8];
            activeRegs[9] = &reg[9];
            activeRegs[10] = &reg[10];
            activeRegs[11] = &reg[11];
            activeRegs[12] = &reg[12];
            activeRegs[13] = &regUND[0];
            activeRegs[14] = &regUND[1];
            break;
        }
        default: {
            LogError("Unsupported Processor Mode: " << PrintHex(currentMode) << "!");
        }
    }
}
// ==================================================================================================
void ARM::branch(uint32_t dest, bool exchange) {
    justBranched = true;
    if (exchange) {  // Determine if we are going to thumb mode.
        bool thumb = readBit(dest, 0);
        setThumbMode(thumb);
        LogDebug("Thumb mode after branch: " << thumb << "!");
    }
    // Mask the bottom bit (thumb mode) or bottom 2 bits (arm mode).
    uint32_t pcMask = getThumbMode() ? ~(0b1) : ~(0b11);
    LogDebug("Branching - PC currently at: " << PrintHex(pc()) << "...");
    LogDebug("Moving PC to: " << PrintHex(dest & pcMask) << "!");
    pc() = dest & pcMask;
    clearInstructionPipeline();
}
// ==================================================================================================
void ARM::clearInstructionPipeline() {
    // Clear the instuction pipeline.
    instuctionPipeLine[0] = NO_INSTRUCT;
    instuctionPipeLine[1] = NO_INSTRUCT;
    instuctionPipeLine[2] = NO_INSTRUCT;
    while (!instructionQueue.empty()) {
        instructionQueue.pop();
    }
    // Reset the previous instruction to force a non-consecutive read.
    uint32_t previousCodeAddr = 0;
    uint32_t previousDataAddr = 0;
}
// ==================================================================================================
void ARM::advanceInstructionPipeline() {
    // Move pipeline.
    instuctionPipeLine[0] = instuctionPipeLine[1];
    instuctionPipeLine[1] = instuctionPipeLine[2];
    instuctionPipeLine[2] = NO_INSTRUCT;
}
// ==================================================================================================
void ARM::setCPSR(uint32_t data) {
    bool modeSwitch = readBits(data, MODE_LOWER_BIT, MODE_UPPER_BIT) != getProcessorMode();
    bool thumbChange = readBit(data, T_BIT) != getThumbMode();
    cpsr = data & 0xF80000FF;  // Protect reserved bits.
    if (modeSwitch) handleProcessorModeChange();
    if (thumbChange) {
        clearInstructionPipeline();
    }
}
// ==================================================================================================
void ARM::fixupIfTargetingPC(uint32_t destReg) {
    if (destReg != PC_REGISTER_NUM) return;
    branch(pc());
}
// ==================================================================================================
// https://developer.arm.com/documentation/ddi0487/maa/-Part-J-Architectural-Pseudocode/-Chapter-J1-A-profile-Architecture-Pseudocode/-J1-3-Shared-pseudocode/-J1-3-672-SignedSatQ?lang=en
int64_t ARM::signedSaturatedQ(int64_t operand, uint8_t bitSize) {
    int64_t max = ((int64_t)std::pow(2, bitSize - 1)) - 1;
    int64_t min = -((int64_t)std::pow(2, bitSize - 1));
    if (operand > max) {
        setFlag(Q_BIT, 1);
        return max;
    }
    if (operand < min) {
        setFlag(Q_BIT, 1);
        return min;
    }
    // Do not clear the Q Flag.
    return operand;
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
            LogError("Unknown condition mnemonic" << condition << "!");
            return false;
    }
    return false;
}
// ==================================================================================================
