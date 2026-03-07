/**
 * @file armInterpreter/arm_data.cpp
 * @brief Implements ARM mode instuction set for instructions in the data category.
 */
#include <bit>

#include "cpu.h"
#include "interconnect.h"

// Control print statements.
#define LOG_LEVEL 2
#include "logger.h"

// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/ARM-Instruction-Set-Encoding/Data-processing-and-miscellaneous-instructions?lang=en

// ==================================================================================================
// Data-processing
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/ARM-Instruction-Set-Encoding/Data-processing-and-miscellaneous-instructions/Data-processing--register-?lang=en
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/ARM-Instruction-Set-Encoding/Data-processing-and-miscellaneous-instructions/Data-processing--register-shifted-register-?lang=en
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/ARM-Instruction-Set-Encoding/Data-processing-and-miscellaneous-instructions/Data-processing--immediate-?lang=en
// ==================================================================================================
// ==================================================================================================
// AND
// ==================================================================================================
cycles ARM::ARM_AND(uint32_t desReg, uint32_t opp1, uint32_t opp2, bool carry, bool setFlags) {
    LogDebug("Executing AND");
    uint32_t result = opp1 & opp2;
    if (setFlags) {
        LogDebug("Updating CPU flags...");
        writeBit(cpsr, readBit(result, 31), N_FLAG);
        writeBit(cpsr, result == 0, Z_FLAG);
        writeBit(cpsr, carry, C_FLAG);
    }
    *activeRegs[desReg] = result;
    fixupIfTargetingPC(desReg);
    return 1;  // An instruction prefetch occurs at the same time as the data operation.
}
cycles ARM::ARM_AND_REG(uint32_t instruct) {
    // S - Set Condition Codes
    bool S = readBit(instruct, 20);
    // 1st Operand Register.
    uint8_t Rn = readBits(instruct, 16, 19);
    // Destination Register.
    uint8_t Rd = readBits(instruct, 12, 15);
    // Process the second operand.
    uint8_t imm5 = readBits(instruct, 7, 11);
    uint8_t type = readBits(instruct, 5, 6);
    decodeShiftResult shiftOp = ARMDecodeImmShift(type, imm5);
    uint8_t Rm = readBits(instruct, 0, 3);
    u32AndBool operandShifted = ARMShift(*activeRegs[Rm], shiftOp, readBit(cpsr, C_FLAG));
    // Data processing arguments.
    uint32_t operand1 = *activeRegs[Rn];
    uint32_t operand2 = operandShifted.data_u32;
    bool carry = operandShifted.data_bool;
    return ARM_AND(Rd, operand1, operand2, carry, S);
}
cycles ARM::ARM_AND_REG_SHIFT(uint32_t instruct) {
    // S - Set Condition Codes
    bool S = readBit(instruct, 20);
    // 1st Operand Register.
    uint8_t Rn = readBits(instruct, 16, 19);
    // Destination Register.
    uint8_t Rd = readBits(instruct, 12, 15);
    // Process the second operand.
    uint8_t Rs = readBits(instruct, 8, 11);
    uint8_t type = readBits(instruct, 5, 6);
    uint8_t Rm = readBits(instruct, 0, 3);
    uint8_t shiftAmount = (*activeRegs[Rs]) & 0xFF;
    u32AndBool operandShifted = ARMShift(type, *activeRegs[Rm], shiftAmount, readBit(cpsr, C_FLAG));
    // Data processing arguments.
    uint32_t operand1 = *activeRegs[Rn];
    uint32_t operand2 = operandShifted.data_u32;
    bool carry = operandShifted.data_bool;
    return ARM_AND(Rd, operand1, operand2, carry, S);
}
cycles ARM::ARM_AND_IMM(uint32_t instruct) {
    // S - Set Condition Codes
    bool S = readBit(instruct, 20);
    // 1st Operand Register.
    uint8_t Rn = readBits(instruct, 16, 19);
    // Destination Register.
    uint8_t Rd = readBits(instruct, 12, 15);
    // Process the second operand.
    uint8_t Rs = readBits(instruct, 8, 11);
    uint8_t type = readBits(instruct, 5, 6);
    uint8_t Rm = readBits(instruct, 0, 3);
    uint8_t shiftAmount = (*activeRegs[Rs]) & 0xFF;
    u32AndBool operandShifted = ARMShift(type, *activeRegs[Rm], shiftAmount, readBit(cpsr, C_FLAG));
    // Intermediate encodings.
    uint32_t imm12 = readBits(instruct, 0, 11);
    u32AndBool immDecoded = ARMExpandImm_C(imm12, readBit(cpsr, C_FLAG));
    // Data processing arguments.
    uint32_t operand1 = *activeRegs[Rn];
    uint32_t operand2 = immDecoded.data_u32;
    bool carry = immDecoded.data_bool;
    return ARM_AND(Rd, operand1, operand2, carry, S);
}
// ==================================================================================================
// EOR
// ==================================================================================================
cycles ARM::ARM_EOR(uint32_t desReg, uint32_t opp1, uint32_t opp2, bool carry, bool setFlags) {
    LogDebug("Executing EOR");
    uint32_t result = opp1 ^ opp2;
    if (setFlags) {
        LogDebug("Updating CPU flags...");
        writeBit(cpsr, readBit(result, 31), N_FLAG);
        writeBit(cpsr, result == 0, Z_FLAG);
        writeBit(cpsr, carry, C_FLAG);
    }
    *activeRegs[desReg] = result;
    fixupIfTargetingPC(desReg);
    return 1;  // An instruction prefetch occurs at the same time as the data operation.
}
cycles ARM::ARM_EOR_REG(uint32_t instruct) {
    // S - Set Condition Codes
    bool S = readBit(instruct, 20);
    // 1st Operand Register.
    uint8_t Rn = readBits(instruct, 16, 19);
    // Destination Register.
    uint8_t Rd = readBits(instruct, 12, 15);
    // Process the second operand.
    uint8_t imm5 = readBits(instruct, 7, 11);
    uint8_t type = readBits(instruct, 5, 6);
    decodeShiftResult shiftOp = ARMDecodeImmShift(type, imm5);
    uint8_t Rm = readBits(instruct, 0, 3);
    u32AndBool operandShifted = ARMShift(*activeRegs[Rm], shiftOp, readBit(cpsr, C_FLAG));
    // Data processing arguments.
    uint32_t operand1 = *activeRegs[Rn];
    uint32_t operand2 = operandShifted.data_u32;
    bool carry = operandShifted.data_bool;
    return ARM_EOR(Rd, operand1, operand2, carry, S);
}
cycles ARM::ARM_EOR_REG_SHIFT(uint32_t instruct) {
    // S - Set Condition Codes
    bool S = readBit(instruct, 20);
    // 1st Operand Register.
    uint8_t Rn = readBits(instruct, 16, 19);
    // Destination Register.
    uint8_t Rd = readBits(instruct, 12, 15);
    // Process the second operand.
    uint8_t Rs = readBits(instruct, 8, 11);
    uint8_t type = readBits(instruct, 5, 6);
    uint8_t Rm = readBits(instruct, 0, 3);
    uint8_t shiftAmount = (*activeRegs[Rs]) & 0xFF;
    u32AndBool operandShifted = ARMShift(type, *activeRegs[Rm], shiftAmount, readBit(cpsr, C_FLAG));
    // Data processing arguments.
    uint32_t operand1 = *activeRegs[Rn];
    uint32_t operand2 = operandShifted.data_u32;
    bool carry = operandShifted.data_bool;
    return ARM_EOR(Rd, operand1, operand2, carry, S);
}
cycles ARM::ARM_EOR_IMM(uint32_t instruct) {
    // S - Set Condition Codes
    bool S = readBit(instruct, 20);
    // 1st Operand Register.
    uint8_t Rn = readBits(instruct, 16, 19);
    // Destination Register.
    uint8_t Rd = readBits(instruct, 12, 15);
    // Process the second operand.
    uint8_t Rs = readBits(instruct, 8, 11);
    uint8_t type = readBits(instruct, 5, 6);
    uint8_t Rm = readBits(instruct, 0, 3);
    uint8_t shiftAmount = (*activeRegs[Rs]) & 0xFF;
    u32AndBool operandShifted = ARMShift(type, *activeRegs[Rm], shiftAmount, readBit(cpsr, C_FLAG));
    // Intermediate encodings.
    uint32_t imm12 = readBits(instruct, 0, 11);
    u32AndBool immDecoded = ARMExpandImm_C(imm12, readBit(cpsr, C_FLAG));
    // Data processing arguments.
    uint32_t operand1 = *activeRegs[Rn];
    uint32_t operand2 = immDecoded.data_u32;
    bool carry = immDecoded.data_bool;
    return ARM_EOR(Rd, operand1, operand2, carry, S);
}
// ==================================================================================================
// SUB
// ==================================================================================================
cycles ARM::ARM_SUB(uint32_t desReg, uint32_t opp1, uint32_t opp2, bool setFlags) {
    LogDebug("Executing SUB");
    u32WithCarryAndOverflow result = ARMAddWithCarry(opp1, ~opp2, 1);
    if (setFlags) {
        LogDebug("Updating CPU flags...");
        writeBit(cpsr, readBit(result.data_u32, 31), N_FLAG);
        writeBit(cpsr, result.data_u32 == 0, Z_FLAG);
        writeBit(cpsr, result.carry, C_FLAG);
        writeBit(cpsr, result.overflow, V_FLAG);
    }
    *activeRegs[desReg] = result.data_u32;
    fixupIfTargetingPC(desReg);
    return 1;  // An instruction prefetch occurs at the same time as the data operation.
}
cycles ARM::ARM_SUB_REG(uint32_t instruct) {
    // S - Set Condition Codes
    bool S = readBit(instruct, 20);
    // 1st Operand Register.
    uint8_t Rn = readBits(instruct, 16, 19);
    // Destination Register.
    uint8_t Rd = readBits(instruct, 12, 15);
    // Process the second operand.
    uint8_t imm5 = readBits(instruct, 7, 11);
    uint8_t type = readBits(instruct, 5, 6);
    decodeShiftResult shiftOp = ARMDecodeImmShift(type, imm5);
    uint8_t Rm = readBits(instruct, 0, 3);
    u32AndBool operandShifted = ARMShift(*activeRegs[Rm], shiftOp, readBit(cpsr, C_FLAG));
    // Data processing arguments.
    uint32_t operand1 = *activeRegs[Rn];
    uint32_t operand2 = operandShifted.data_u32;
    bool carry = operandShifted.data_bool;
    return ARM_SUB(Rd, operand1, operand2, S);
}
cycles ARM::ARM_SUB_REG_SHIFT(uint32_t instruct) {
    // S - Set Condition Codes
    bool S = readBit(instruct, 20);
    // 1st Operand Register.
    uint8_t Rn = readBits(instruct, 16, 19);
    // Destination Register.
    uint8_t Rd = readBits(instruct, 12, 15);
    // Process the second operand.
    uint8_t Rs = readBits(instruct, 8, 11);
    uint8_t type = readBits(instruct, 5, 6);
    uint8_t Rm = readBits(instruct, 0, 3);
    uint8_t shiftAmount = (*activeRegs[Rs]) & 0xFF;
    u32AndBool operandShifted = ARMShift(type, *activeRegs[Rm], shiftAmount, readBit(cpsr, C_FLAG));
    // Data processing arguments.
    uint32_t operand1 = *activeRegs[Rn];
    uint32_t operand2 = operandShifted.data_u32;
    bool carry = operandShifted.data_bool;
    return ARM_SUB(Rd, operand1, operand2, S);
}
cycles ARM::ARM_SUB_IMM(uint32_t instruct) {
    // S - Set Condition Codes
    bool S = readBit(instruct, 20);
    // 1st Operand Register.
    uint8_t Rn = readBits(instruct, 16, 19);
    // Destination Register.
    uint8_t Rd = readBits(instruct, 12, 15);
    // Process the second operand.
    uint8_t Rs = readBits(instruct, 8, 11);
    uint8_t type = readBits(instruct, 5, 6);
    uint8_t Rm = readBits(instruct, 0, 3);
    uint8_t shiftAmount = (*activeRegs[Rs]) & 0xFF;
    u32AndBool operandShifted = ARMShift(type, *activeRegs[Rm], shiftAmount, readBit(cpsr, C_FLAG));
    // Intermediate encodings.
    uint32_t imm12 = readBits(instruct, 0, 11);
    u32AndBool immDecoded = ARMExpandImm_C(imm12, readBit(cpsr, C_FLAG));
    // Data processing arguments.
    uint32_t operand1 = *activeRegs[Rn];
    uint32_t operand2 = immDecoded.data_u32;
    bool carry = immDecoded.data_bool;
    return ARM_SUB(Rd, operand1, operand2, S);
}
// ==================================================================================================
// RSB
// ==================================================================================================
cycles ARM::ARM_RSB(uint32_t desReg, uint32_t opp1, uint32_t opp2, bool setFlags) {
    LogDebug("Executing RSB");
    u32WithCarryAndOverflow result = ARMAddWithCarry(~opp1, opp2, 1);
    if (setFlags) {
        LogDebug("Updating CPU flags...");
        writeBit(cpsr, readBit(result.data_u32, 31), N_FLAG);
        writeBit(cpsr, result.data_u32 == 0, Z_FLAG);
        writeBit(cpsr, result.carry, C_FLAG);
        writeBit(cpsr, result.overflow, V_FLAG);
    }
    *activeRegs[desReg] = result.data_u32;
    fixupIfTargetingPC(desReg);
    return 1;  // An instruction prefetch occurs at the same time as the data operation.
}
cycles ARM::ARM_RSB_REG(uint32_t instruct) {
    // S - Set Condition Codes
    bool S = readBit(instruct, 20);
    // 1st Operand Register.
    uint8_t Rn = readBits(instruct, 16, 19);
    // Destination Register.
    uint8_t Rd = readBits(instruct, 12, 15);
    // Process the second operand.
    uint8_t imm5 = readBits(instruct, 7, 11);
    uint8_t type = readBits(instruct, 5, 6);
    decodeShiftResult shiftOp = ARMDecodeImmShift(type, imm5);
    uint8_t Rm = readBits(instruct, 0, 3);
    u32AndBool operandShifted = ARMShift(*activeRegs[Rm], shiftOp, readBit(cpsr, C_FLAG));
    // Data processing arguments.
    uint32_t operand1 = *activeRegs[Rn];
    uint32_t operand2 = operandShifted.data_u32;
    bool carry = operandShifted.data_bool;
    return ARM_RSB(Rd, operand1, operand2, S);
}
cycles ARM::ARM_RSB_REG_SHIFT(uint32_t instruct) {
    // S - Set Condition Codes
    bool S = readBit(instruct, 20);
    // 1st Operand Register.
    uint8_t Rn = readBits(instruct, 16, 19);
    // Destination Register.
    uint8_t Rd = readBits(instruct, 12, 15);
    // Process the second operand.
    uint8_t Rs = readBits(instruct, 8, 11);
    uint8_t type = readBits(instruct, 5, 6);
    uint8_t Rm = readBits(instruct, 0, 3);
    uint8_t shiftAmount = (*activeRegs[Rs]) & 0xFF;
    u32AndBool operandShifted = ARMShift(type, *activeRegs[Rm], shiftAmount, readBit(cpsr, C_FLAG));
    // Data processing arguments.
    uint32_t operand1 = *activeRegs[Rn];
    uint32_t operand2 = operandShifted.data_u32;
    bool carry = operandShifted.data_bool;
    return ARM_RSB(Rd, operand1, operand2, S);
}
cycles ARM::ARM_RSB_IMM(uint32_t instruct) {
    // S - Set Condition Codes
    bool S = readBit(instruct, 20);
    // 1st Operand Register.
    uint8_t Rn = readBits(instruct, 16, 19);
    // Destination Register.
    uint8_t Rd = readBits(instruct, 12, 15);
    // Process the second operand.
    uint8_t Rs = readBits(instruct, 8, 11);
    uint8_t type = readBits(instruct, 5, 6);
    uint8_t Rm = readBits(instruct, 0, 3);
    uint8_t shiftAmount = (*activeRegs[Rs]) & 0xFF;
    u32AndBool operandShifted = ARMShift(type, *activeRegs[Rm], shiftAmount, readBit(cpsr, C_FLAG));
    // Intermediate encodings.
    uint32_t imm12 = readBits(instruct, 0, 11);
    u32AndBool immDecoded = ARMExpandImm_C(imm12, readBit(cpsr, C_FLAG));
    // Data processing arguments.
    uint32_t operand1 = *activeRegs[Rn];
    uint32_t operand2 = immDecoded.data_u32;
    bool carry = immDecoded.data_bool;
    return ARM_RSB(Rd, operand1, operand2, S);
}
// ==================================================================================================
// ADD
// ==================================================================================================
cycles ARM::ARM_ADD(uint32_t desReg, uint32_t opp1, uint32_t opp2, bool setFlags) {
    LogDebug("Executing ADD");
    u32WithCarryAndOverflow result = ARMAddWithCarry(opp1, opp2, 0);
    if (setFlags) {
        LogDebug("Updating CPU flags...");
        writeBit(cpsr, readBit(result.data_u32, 31), N_FLAG);
        writeBit(cpsr, result.data_u32 == 0, Z_FLAG);
        writeBit(cpsr, result.carry, C_FLAG);
        writeBit(cpsr, result.overflow, V_FLAG);
    }
    *activeRegs[desReg] = result.data_u32;
    fixupIfTargetingPC(desReg);
    return 1;  // An instruction prefetch occurs at the same time as the data operation.
}
cycles ARM::ARM_ADD_REG(uint32_t instruct) {
    // S - Set Condition Codes
    bool S = readBit(instruct, 20);
    // 1st Operand Register.
    uint8_t Rn = readBits(instruct, 16, 19);
    // Destination Register.
    uint8_t Rd = readBits(instruct, 12, 15);
    // Process the second operand.
    uint8_t imm5 = readBits(instruct, 7, 11);
    uint8_t type = readBits(instruct, 5, 6);
    decodeShiftResult shiftOp = ARMDecodeImmShift(type, imm5);
    uint8_t Rm = readBits(instruct, 0, 3);
    u32AndBool operandShifted = ARMShift(*activeRegs[Rm], shiftOp, readBit(cpsr, C_FLAG));
    // Data processing arguments.
    uint32_t operand1 = *activeRegs[Rn];
    uint32_t operand2 = operandShifted.data_u32;
    bool carry = operandShifted.data_bool;
    return ARM_ADD(Rd, operand1, operand2, S);
}
cycles ARM::ARM_ADD_REG_SHIFT(uint32_t instruct) {
    // S - Set Condition Codes
    bool S = readBit(instruct, 20);
    // 1st Operand Register.
    uint8_t Rn = readBits(instruct, 16, 19);
    // Destination Register.
    uint8_t Rd = readBits(instruct, 12, 15);
    // Process the second operand.
    uint8_t Rs = readBits(instruct, 8, 11);
    uint8_t type = readBits(instruct, 5, 6);
    uint8_t Rm = readBits(instruct, 0, 3);
    uint8_t shiftAmount = (*activeRegs[Rs]) & 0xFF;
    u32AndBool operandShifted = ARMShift(type, *activeRegs[Rm], shiftAmount, readBit(cpsr, C_FLAG));
    // Data processing arguments.
    uint32_t operand1 = *activeRegs[Rn];
    uint32_t operand2 = operandShifted.data_u32;
    bool carry = operandShifted.data_bool;
    return ARM_ADD(Rd, operand1, operand2, S);
}
cycles ARM::ARM_ADD_IMM(uint32_t instruct) {
    // S - Set Condition Codes
    bool S = readBit(instruct, 20);
    // 1st Operand Register.
    uint8_t Rn = readBits(instruct, 16, 19);
    // Destination Register.
    uint8_t Rd = readBits(instruct, 12, 15);
    // Process the second operand.
    uint8_t Rs = readBits(instruct, 8, 11);
    uint8_t type = readBits(instruct, 5, 6);
    uint8_t Rm = readBits(instruct, 0, 3);
    uint8_t shiftAmount = (*activeRegs[Rs]) & 0xFF;
    u32AndBool operandShifted = ARMShift(type, *activeRegs[Rm], shiftAmount, readBit(cpsr, C_FLAG));
    // Intermediate encodings.
    uint32_t imm12 = readBits(instruct, 0, 11);
    u32AndBool immDecoded = ARMExpandImm_C(imm12, readBit(cpsr, C_FLAG));
    // Data processing arguments.
    uint32_t operand1 = *activeRegs[Rn];
    uint32_t operand2 = immDecoded.data_u32;
    bool carry = immDecoded.data_bool;
    return ARM_ADD(Rd, operand1, operand2, S);
}
// ==================================================================================================
// ADC
// ==================================================================================================
cycles ARM::ARM_ADC(uint32_t desReg, uint32_t opp1, uint32_t opp2, bool carry, bool setFlags) {
    LogDebug("Executing ADC");
    u32WithCarryAndOverflow result = ARMAddWithCarry(opp1, opp2, carry);
    if (setFlags) {
        LogDebug("Updating CPU flags...");
        writeBit(cpsr, readBit(result.data_u32, 31), N_FLAG);
        writeBit(cpsr, result.data_u32 == 0, Z_FLAG);
        writeBit(cpsr, result.carry, C_FLAG);
        writeBit(cpsr, result.overflow, V_FLAG);
    }
    *activeRegs[desReg] = result.data_u32;
    fixupIfTargetingPC(desReg);
    return 1;  // An instruction prefetch occurs at the same time as the data operation.
}
cycles ARM::ARM_ADC_REG(uint32_t instruct) {
    // S - Set Condition Codes
    bool S = readBit(instruct, 20);
    // 1st Operand Register.
    uint8_t Rn = readBits(instruct, 16, 19);
    // Destination Register.
    uint8_t Rd = readBits(instruct, 12, 15);
    // Process the second operand.
    uint8_t imm5 = readBits(instruct, 7, 11);
    uint8_t type = readBits(instruct, 5, 6);
    decodeShiftResult shiftOp = ARMDecodeImmShift(type, imm5);
    uint8_t Rm = readBits(instruct, 0, 3);
    u32AndBool operandShifted = ARMShift(*activeRegs[Rm], shiftOp, readBit(cpsr, C_FLAG));
    // Data processing arguments.
    uint32_t operand1 = *activeRegs[Rn];
    uint32_t operand2 = operandShifted.data_u32;
    bool carry = operandShifted.data_bool;
    return ARM_ADC(Rd, operand1, operand2, carry, S);
}
cycles ARM::ARM_ADC_REG_SHIFT(uint32_t instruct) {
    // S - Set Condition Codes
    bool S = readBit(instruct, 20);
    // 1st Operand Register.
    uint8_t Rn = readBits(instruct, 16, 19);
    // Destination Register.
    uint8_t Rd = readBits(instruct, 12, 15);
    // Process the second operand.
    uint8_t Rs = readBits(instruct, 8, 11);
    uint8_t type = readBits(instruct, 5, 6);
    uint8_t Rm = readBits(instruct, 0, 3);
    uint8_t shiftAmount = (*activeRegs[Rs]) & 0xFF;
    u32AndBool operandShifted = ARMShift(type, *activeRegs[Rm], shiftAmount, readBit(cpsr, C_FLAG));
    // Data processing arguments.
    uint32_t operand1 = *activeRegs[Rn];
    uint32_t operand2 = operandShifted.data_u32;
    bool carry = operandShifted.data_bool;
    return ARM_ADC(Rd, operand1, operand2, carry, S);
}
cycles ARM::ARM_ADC_IMM(uint32_t instruct) {
    // S - Set Condition Codes
    bool S = readBit(instruct, 20);
    // 1st Operand Register.
    uint8_t Rn = readBits(instruct, 16, 19);
    // Destination Register.
    uint8_t Rd = readBits(instruct, 12, 15);
    // Process the second operand.
    uint8_t Rs = readBits(instruct, 8, 11);
    uint8_t type = readBits(instruct, 5, 6);
    uint8_t Rm = readBits(instruct, 0, 3);
    uint8_t shiftAmount = (*activeRegs[Rs]) & 0xFF;
    u32AndBool operandShifted = ARMShift(type, *activeRegs[Rm], shiftAmount, readBit(cpsr, C_FLAG));
    // Intermediate encodings.
    uint32_t imm12 = readBits(instruct, 0, 11);
    u32AndBool immDecoded = ARMExpandImm_C(imm12, readBit(cpsr, C_FLAG));
    // Data processing arguments.
    uint32_t operand1 = *activeRegs[Rn];
    uint32_t operand2 = immDecoded.data_u32;
    bool carry = immDecoded.data_bool;
    return ARM_ADC(Rd, operand1, operand2, carry, S);
}
// ==================================================================================================
// SBC
// ==================================================================================================
cycles ARM::ARM_SBC(uint32_t desReg, uint32_t opp1, uint32_t opp2, bool carry, bool setFlags) {
    LogDebug("Executing SBC");
    u32WithCarryAndOverflow result = ARMAddWithCarry(opp1, ~opp2, carry);
    if (setFlags) {
        LogDebug("Updating CPU flags...");
        writeBit(cpsr, readBit(result.data_u32, 31), N_FLAG);
        writeBit(cpsr, result.data_u32 == 0, Z_FLAG);
        writeBit(cpsr, result.carry, C_FLAG);
        writeBit(cpsr, result.overflow, V_FLAG);
    }
    *activeRegs[desReg] = result.data_u32;
    fixupIfTargetingPC(desReg);
    return 1;  // An instruction prefetch occurs at the same time as the data operation.
}
cycles ARM::ARM_SBC_REG(uint32_t instruct) {
    // S - Set Condition Codes
    bool S = readBit(instruct, 20);
    // 1st Operand Register.
    uint8_t Rn = readBits(instruct, 16, 19);
    // Destination Register.
    uint8_t Rd = readBits(instruct, 12, 15);
    // Process the second operand.
    uint8_t imm5 = readBits(instruct, 7, 11);
    uint8_t type = readBits(instruct, 5, 6);
    decodeShiftResult shiftOp = ARMDecodeImmShift(type, imm5);
    uint8_t Rm = readBits(instruct, 0, 3);
    u32AndBool operandShifted = ARMShift(*activeRegs[Rm], shiftOp, readBit(cpsr, C_FLAG));
    // Data processing arguments.
    uint32_t operand1 = *activeRegs[Rn];
    uint32_t operand2 = operandShifted.data_u32;
    bool carry = operandShifted.data_bool;
    return ARM_SBC(Rd, operand1, operand2, carry, S);
}
cycles ARM::ARM_SBC_REG_SHIFT(uint32_t instruct) {
    // S - Set Condition Codes
    bool S = readBit(instruct, 20);
    // 1st Operand Register.
    uint8_t Rn = readBits(instruct, 16, 19);
    // Destination Register.
    uint8_t Rd = readBits(instruct, 12, 15);
    // Process the second operand.
    uint8_t Rs = readBits(instruct, 8, 11);
    uint8_t type = readBits(instruct, 5, 6);
    uint8_t Rm = readBits(instruct, 0, 3);
    uint8_t shiftAmount = (*activeRegs[Rs]) & 0xFF;
    u32AndBool operandShifted = ARMShift(type, *activeRegs[Rm], shiftAmount, readBit(cpsr, C_FLAG));
    // Data processing arguments.
    uint32_t operand1 = *activeRegs[Rn];
    uint32_t operand2 = operandShifted.data_u32;
    bool carry = operandShifted.data_bool;
    return ARM_SBC(Rd, operand1, operand2, carry, S);
}
cycles ARM::ARM_SBC_IMM(uint32_t instruct) {
    // S - Set Condition Codes
    bool S = readBit(instruct, 20);
    // 1st Operand Register.
    uint8_t Rn = readBits(instruct, 16, 19);
    // Destination Register.
    uint8_t Rd = readBits(instruct, 12, 15);
    // Process the second operand.
    uint8_t Rs = readBits(instruct, 8, 11);
    uint8_t type = readBits(instruct, 5, 6);
    uint8_t Rm = readBits(instruct, 0, 3);
    uint8_t shiftAmount = (*activeRegs[Rs]) & 0xFF;
    u32AndBool operandShifted = ARMShift(type, *activeRegs[Rm], shiftAmount, readBit(cpsr, C_FLAG));
    // Intermediate encodings.
    uint32_t imm12 = readBits(instruct, 0, 11);
    u32AndBool immDecoded = ARMExpandImm_C(imm12, readBit(cpsr, C_FLAG));
    // Data processing arguments.
    uint32_t operand1 = *activeRegs[Rn];
    uint32_t operand2 = immDecoded.data_u32;
    bool carry = immDecoded.data_bool;
    return ARM_SBC(Rd, operand1, operand2, carry, S);
}
// ==================================================================================================
// RSC
// ==================================================================================================
cycles ARM::ARM_RSC(uint32_t desReg, uint32_t opp1, uint32_t opp2, bool carry, bool setFlags) {
    LogDebug("Executing RSC");
    u32WithCarryAndOverflow result = ARMAddWithCarry(~opp1, opp2, carry);
    if (setFlags) {
        LogDebug("Updating CPU flags...");
        writeBit(cpsr, readBit(result.data_u32, 31), N_FLAG);
        writeBit(cpsr, result.data_u32 == 0, Z_FLAG);
        writeBit(cpsr, result.carry, C_FLAG);
        writeBit(cpsr, result.overflow, V_FLAG);
    }
    *activeRegs[desReg] = result.data_u32;
    fixupIfTargetingPC(desReg);
    return 1;  // An instruction prefetch occurs at the same time as the data operation.
}
cycles ARM::ARM_RSC_REG(uint32_t instruct) {
    // S - Set Condition Codes
    bool S = readBit(instruct, 20);
    // 1st Operand Register.
    uint8_t Rn = readBits(instruct, 16, 19);
    // Destination Register.
    uint8_t Rd = readBits(instruct, 12, 15);
    // Process the second operand.
    uint8_t imm5 = readBits(instruct, 7, 11);
    uint8_t type = readBits(instruct, 5, 6);
    decodeShiftResult shiftOp = ARMDecodeImmShift(type, imm5);
    uint8_t Rm = readBits(instruct, 0, 3);
    u32AndBool operandShifted = ARMShift(*activeRegs[Rm], shiftOp, readBit(cpsr, C_FLAG));
    // Data processing arguments.
    uint32_t operand1 = *activeRegs[Rn];
    uint32_t operand2 = operandShifted.data_u32;
    bool carry = operandShifted.data_bool;
    return ARM_RSC(Rd, operand1, operand2, carry, S);
}
cycles ARM::ARM_RSC_REG_SHIFT(uint32_t instruct) {
    // S - Set Condition Codes
    bool S = readBit(instruct, 20);
    // 1st Operand Register.
    uint8_t Rn = readBits(instruct, 16, 19);
    // Destination Register.
    uint8_t Rd = readBits(instruct, 12, 15);
    // Process the second operand.
    uint8_t Rs = readBits(instruct, 8, 11);
    uint8_t type = readBits(instruct, 5, 6);
    uint8_t Rm = readBits(instruct, 0, 3);
    uint8_t shiftAmount = (*activeRegs[Rs]) & 0xFF;
    u32AndBool operandShifted = ARMShift(type, *activeRegs[Rm], shiftAmount, readBit(cpsr, C_FLAG));
    // Data processing arguments.
    uint32_t operand1 = *activeRegs[Rn];
    uint32_t operand2 = operandShifted.data_u32;
    bool carry = operandShifted.data_bool;
    return ARM_RSC(Rd, operand1, operand2, carry, S);
}
cycles ARM::ARM_RSC_IMM(uint32_t instruct) {
    // S - Set Condition Codes
    bool S = readBit(instruct, 20);
    // 1st Operand Register.
    uint8_t Rn = readBits(instruct, 16, 19);
    // Destination Register.
    uint8_t Rd = readBits(instruct, 12, 15);
    // Process the second operand.
    uint8_t Rs = readBits(instruct, 8, 11);
    uint8_t type = readBits(instruct, 5, 6);
    uint8_t Rm = readBits(instruct, 0, 3);
    uint8_t shiftAmount = (*activeRegs[Rs]) & 0xFF;
    u32AndBool operandShifted = ARMShift(type, *activeRegs[Rm], shiftAmount, readBit(cpsr, C_FLAG));
    // Intermediate encodings.
    uint32_t imm12 = readBits(instruct, 0, 11);
    u32AndBool immDecoded = ARMExpandImm_C(imm12, readBit(cpsr, C_FLAG));
    // Data processing arguments.
    uint32_t operand1 = *activeRegs[Rn];
    uint32_t operand2 = immDecoded.data_u32;
    bool carry = immDecoded.data_bool;
    return ARM_RSC(Rd, operand1, operand2, carry, S);
}
// ==================================================================================================
// TST
// ==================================================================================================
cycles ARM::ARM_TST(uint32_t opp1, uint32_t opp2, bool carry) {
    LogDebug("Executing TST");
    LogDebug("Updating CPU flags...");
    uint32_t result = opp1 & opp2;
    writeBit(cpsr, readBit(result, 31), N_FLAG);
    writeBit(cpsr, result == 0, Z_FLAG);
    writeBit(cpsr, carry, C_FLAG);
    return 1;  // An instruction prefetch occurs at the same time as the data operation.
}
cycles ARM::ARM_TST_REG(uint32_t instruct) {
    // 1st Operand Register.
    uint8_t Rn = readBits(instruct, 16, 19);
    // Process the second operand.
    uint8_t imm5 = readBits(instruct, 7, 11);
    uint8_t type = readBits(instruct, 5, 6);
    decodeShiftResult shiftOp = ARMDecodeImmShift(type, imm5);
    uint8_t Rm = readBits(instruct, 0, 3);
    u32AndBool operandShifted = ARMShift(*activeRegs[Rm], shiftOp, readBit(cpsr, C_FLAG));
    // Data processing arguments.
    uint32_t operand1 = *activeRegs[Rn];
    uint32_t operand2 = operandShifted.data_u32;
    bool carry = operandShifted.data_bool;
    return ARM_TST(operand1, operand2, carry);
}
cycles ARM::ARM_TST_REG_SHIFT(uint32_t instruct) {
    // 1st Operand Register.
    uint8_t Rn = readBits(instruct, 16, 19);
    // Process the second operand.
    uint8_t Rs = readBits(instruct, 8, 11);
    uint8_t type = readBits(instruct, 5, 6);
    uint8_t Rm = readBits(instruct, 0, 3);
    uint8_t shiftAmount = (*activeRegs[Rs]) & 0xFF;
    u32AndBool operandShifted = ARMShift(type, *activeRegs[Rm], shiftAmount, readBit(cpsr, C_FLAG));
    // Data processing arguments.
    uint32_t operand1 = *activeRegs[Rn];
    uint32_t operand2 = operandShifted.data_u32;
    bool carry = operandShifted.data_bool;
    return ARM_TST(operand1, operand2, carry);
}
cycles ARM::ARM_TST_IMM(uint32_t instruct) {
    // 1st Operand Register.
    uint8_t Rn = readBits(instruct, 16, 19);
    // Process the second operand.
    uint8_t Rs = readBits(instruct, 8, 11);
    uint8_t type = readBits(instruct, 5, 6);
    uint8_t Rm = readBits(instruct, 0, 3);
    uint8_t shiftAmount = (*activeRegs[Rs]) & 0xFF;
    u32AndBool operandShifted = ARMShift(type, *activeRegs[Rm], shiftAmount, readBit(cpsr, C_FLAG));
    // Intermediate encodings.
    uint32_t imm12 = readBits(instruct, 0, 11);
    u32AndBool immDecoded = ARMExpandImm_C(imm12, readBit(cpsr, C_FLAG));
    // Data processing arguments.
    uint32_t operand1 = *activeRegs[Rn];
    uint32_t operand2 = immDecoded.data_u32;
    bool carry = immDecoded.data_bool;
    return ARM_TST(operand1, operand2, carry);
}
// ==================================================================================================
// TEQ
// ==================================================================================================
cycles ARM::ARM_TEQ(uint32_t opp1, uint32_t opp2, bool carry) {
    LogDebug("Executing TEQ");
    LogDebug("Updating CPU flags...");
    uint32_t result = opp1 ^ opp2;
    writeBit(cpsr, readBit(result, 31), N_FLAG);
    writeBit(cpsr, result == 0, Z_FLAG);
    writeBit(cpsr, carry, C_FLAG);
    return 1;  // An instruction prefetch occurs at the same time as the data operation.
}
cycles ARM::ARM_TEQ_REG(uint32_t instruct) {
    // 1st Operand Register.
    uint8_t Rn = readBits(instruct, 16, 19);
    // Process the second operand.
    uint8_t imm5 = readBits(instruct, 7, 11);
    uint8_t type = readBits(instruct, 5, 6);
    decodeShiftResult shiftOp = ARMDecodeImmShift(type, imm5);
    uint8_t Rm = readBits(instruct, 0, 3);
    u32AndBool operandShifted = ARMShift(*activeRegs[Rm], shiftOp, readBit(cpsr, C_FLAG));
    // Data processing arguments.
    uint32_t operand1 = *activeRegs[Rn];
    uint32_t operand2 = operandShifted.data_u32;
    bool carry = operandShifted.data_bool;
    return ARM_TEQ(operand1, operand2, carry);
}
cycles ARM::ARM_TEQ_REG_SHIFT(uint32_t instruct) {
    // 1st Operand Register.
    uint8_t Rn = readBits(instruct, 16, 19);
    // Process the second operand.
    uint8_t Rs = readBits(instruct, 8, 11);
    uint8_t type = readBits(instruct, 5, 6);
    uint8_t Rm = readBits(instruct, 0, 3);
    uint8_t shiftAmount = (*activeRegs[Rs]) & 0xFF;
    u32AndBool operandShifted = ARMShift(type, *activeRegs[Rm], shiftAmount, readBit(cpsr, C_FLAG));
    // Data processing arguments.
    uint32_t operand1 = *activeRegs[Rn];
    uint32_t operand2 = operandShifted.data_u32;
    bool carry = operandShifted.data_bool;
    return ARM_TEQ(operand1, operand2, carry);
}
cycles ARM::ARM_TEQ_IMM(uint32_t instruct) {
    // 1st Operand Register.
    uint8_t Rn = readBits(instruct, 16, 19);
    // Process the second operand.
    uint8_t Rs = readBits(instruct, 8, 11);
    uint8_t type = readBits(instruct, 5, 6);
    uint8_t Rm = readBits(instruct, 0, 3);
    uint8_t shiftAmount = (*activeRegs[Rs]) & 0xFF;
    u32AndBool operandShifted = ARMShift(type, *activeRegs[Rm], shiftAmount, readBit(cpsr, C_FLAG));
    // Intermediate encodings.
    uint32_t imm12 = readBits(instruct, 0, 11);
    u32AndBool immDecoded = ARMExpandImm_C(imm12, readBit(cpsr, C_FLAG));
    // Data processing arguments.
    uint32_t operand1 = *activeRegs[Rn];
    uint32_t operand2 = immDecoded.data_u32;
    bool carry = immDecoded.data_bool;
    return ARM_TEQ(operand1, operand2, carry);
}
// ==================================================================================================
// CMP
// ==================================================================================================
cycles ARM::ARM_CMP(uint32_t opp1, uint32_t opp2) {
    LogDebug("Executing CMP");
    u32WithCarryAndOverflow result = ARMAddWithCarry(opp1, ~opp2, 1);
    LogDebug("Updating CPU flags...");
    writeBit(cpsr, readBit(result.data_u32, 31), N_FLAG);
    writeBit(cpsr, result.data_u32 == 0, Z_FLAG);
    writeBit(cpsr, result.carry, C_FLAG);
    writeBit(cpsr, result.overflow, V_FLAG);
    return 1;  // An instruction prefetch occurs at the same time as the data operation.
}
cycles ARM::ARM_CMP_REG(uint32_t instruct) {
    // 1st Operand Register.
    uint8_t Rn = readBits(instruct, 16, 19);
    // Process the second operand.
    uint8_t imm5 = readBits(instruct, 7, 11);
    uint8_t type = readBits(instruct, 5, 6);
    decodeShiftResult shiftOp = ARMDecodeImmShift(type, imm5);
    uint8_t Rm = readBits(instruct, 0, 3);
    u32AndBool operandShifted = ARMShift(*activeRegs[Rm], shiftOp, readBit(cpsr, C_FLAG));
    // Data processing arguments.
    uint32_t operand1 = *activeRegs[Rn];
    uint32_t operand2 = operandShifted.data_u32;
    bool carry = operandShifted.data_bool;
    return ARM_CMP(operand1, operand2);
}
cycles ARM::ARM_CMP_REG_SHIFT(uint32_t instruct) {
    // 1st Operand Register.
    uint8_t Rn = readBits(instruct, 16, 19);
    // Process the second operand.
    uint8_t Rs = readBits(instruct, 8, 11);
    uint8_t type = readBits(instruct, 5, 6);
    uint8_t Rm = readBits(instruct, 0, 3);
    uint8_t shiftAmount = (*activeRegs[Rs]) & 0xFF;
    u32AndBool operandShifted = ARMShift(type, *activeRegs[Rm], shiftAmount, readBit(cpsr, C_FLAG));
    // Data processing arguments.
    uint32_t operand1 = *activeRegs[Rn];
    uint32_t operand2 = operandShifted.data_u32;
    bool carry = operandShifted.data_bool;
    return ARM_CMP(operand1, operand2);
}
cycles ARM::ARM_CMP_IMM(uint32_t instruct) {
    // 1st Operand Register.
    uint8_t Rn = readBits(instruct, 16, 19);
    // Process the second operand.
    uint8_t Rs = readBits(instruct, 8, 11);
    uint8_t type = readBits(instruct, 5, 6);
    uint8_t Rm = readBits(instruct, 0, 3);
    uint8_t shiftAmount = (*activeRegs[Rs]) & 0xFF;
    u32AndBool operandShifted = ARMShift(type, *activeRegs[Rm], shiftAmount, readBit(cpsr, C_FLAG));
    // Intermediate encodings.
    uint32_t imm12 = readBits(instruct, 0, 11);
    u32AndBool immDecoded = ARMExpandImm_C(imm12, readBit(cpsr, C_FLAG));
    // Data processing arguments.
    uint32_t operand1 = *activeRegs[Rn];
    uint32_t operand2 = immDecoded.data_u32;
    bool carry = immDecoded.data_bool;
    return ARM_CMP(operand1, operand2);
}
// ==================================================================================================
// CMN
// ==================================================================================================
cycles ARM::ARM_CMN(uint32_t opp1, uint32_t opp2) {
    LogDebug("Executing CMN");
    u32WithCarryAndOverflow result = ARMAddWithCarry(opp1, opp2, 0);
    LogDebug("Updating CPU flags...");
    writeBit(cpsr, readBit(result.data_u32, 31), N_FLAG);
    writeBit(cpsr, result.data_u32 == 0, Z_FLAG);
    writeBit(cpsr, result.carry, C_FLAG);
    writeBit(cpsr, result.overflow, V_FLAG);
    return 1;  // An instruction prefetch occurs at the same time as the data operation.
}
cycles ARM::ARM_CMN_REG(uint32_t instruct) {
    // 1st Operand Register.
    uint8_t Rn = readBits(instruct, 16, 19);
    // Process the second operand.
    uint8_t imm5 = readBits(instruct, 7, 11);
    uint8_t type = readBits(instruct, 5, 6);
    decodeShiftResult shiftOp = ARMDecodeImmShift(type, imm5);
    uint8_t Rm = readBits(instruct, 0, 3);
    u32AndBool operandShifted = ARMShift(*activeRegs[Rm], shiftOp, readBit(cpsr, C_FLAG));
    // Data processing arguments.
    uint32_t operand1 = *activeRegs[Rn];
    uint32_t operand2 = operandShifted.data_u32;
    bool carry = operandShifted.data_bool;
    return ARM_CMN(operand1, operand2);
}
cycles ARM::ARM_CMN_REG_SHIFT(uint32_t instruct) {
    // 1st Operand Register.
    uint8_t Rn = readBits(instruct, 16, 19);
    // Process the second operand.
    uint8_t Rs = readBits(instruct, 8, 11);
    uint8_t type = readBits(instruct, 5, 6);
    uint8_t Rm = readBits(instruct, 0, 3);
    uint8_t shiftAmount = (*activeRegs[Rs]) & 0xFF;
    u32AndBool operandShifted = ARMShift(type, *activeRegs[Rm], shiftAmount, readBit(cpsr, C_FLAG));
    // Data processing arguments.
    uint32_t operand1 = *activeRegs[Rn];
    uint32_t operand2 = operandShifted.data_u32;
    bool carry = operandShifted.data_bool;
    return ARM_CMN(operand1, operand2);
}
cycles ARM::ARM_CMN_IMM(uint32_t instruct) {
    // 1st Operand Register.
    uint8_t Rn = readBits(instruct, 16, 19);
    // Process the second operand.
    uint8_t Rs = readBits(instruct, 8, 11);
    uint8_t type = readBits(instruct, 5, 6);
    uint8_t Rm = readBits(instruct, 0, 3);
    uint8_t shiftAmount = (*activeRegs[Rs]) & 0xFF;
    u32AndBool operandShifted = ARMShift(type, *activeRegs[Rm], shiftAmount, readBit(cpsr, C_FLAG));
    // Intermediate encodings.
    uint32_t imm12 = readBits(instruct, 0, 11);
    u32AndBool immDecoded = ARMExpandImm_C(imm12, readBit(cpsr, C_FLAG));
    // Data processing arguments.
    uint32_t operand1 = *activeRegs[Rn];
    uint32_t operand2 = immDecoded.data_u32;
    bool carry = immDecoded.data_bool;
    return ARM_CMN(operand1, operand2);
}
// ==================================================================================================
// ORR
// ==================================================================================================
cycles ARM::ARM_ORR(uint32_t desReg, uint32_t opp1, uint32_t opp2, bool carry, bool setFlags) {
    LogDebug("Executing ORR");
    uint32_t result = opp1 | opp2;
    if (setFlags) {
        LogDebug("Updating CPU flags...");
        writeBit(cpsr, readBit(result, 31), N_FLAG);
        writeBit(cpsr, result == 0, Z_FLAG);
        writeBit(cpsr, carry, C_FLAG);
    }
    *activeRegs[desReg] = result;
    fixupIfTargetingPC(desReg);
    return 1;  // An instruction prefetch occurs at the same time as the data operation.
}
cycles ARM::ARM_ORR_REG(uint32_t instruct) {
    // S - Set Condition Codes
    bool S = readBit(instruct, 20);
    // 1st Operand Register.
    uint8_t Rn = readBits(instruct, 16, 19);
    // Destination Register.
    uint8_t Rd = readBits(instruct, 12, 15);
    // Process the second operand.
    uint8_t imm5 = readBits(instruct, 7, 11);
    uint8_t type = readBits(instruct, 5, 6);
    decodeShiftResult shiftOp = ARMDecodeImmShift(type, imm5);
    uint8_t Rm = readBits(instruct, 0, 3);
    u32AndBool operandShifted = ARMShift(*activeRegs[Rm], shiftOp, readBit(cpsr, C_FLAG));
    // Data processing arguments.
    uint32_t operand1 = *activeRegs[Rn];
    uint32_t operand2 = operandShifted.data_u32;
    bool carry = operandShifted.data_bool;
    return ARM_ORR(Rd, operand1, operand2, carry, S);
}
cycles ARM::ARM_ORR_REG_SHIFT(uint32_t instruct) {
    // S - Set Condition Codes
    bool S = readBit(instruct, 20);
    // 1st Operand Register.
    uint8_t Rn = readBits(instruct, 16, 19);
    // Destination Register.
    uint8_t Rd = readBits(instruct, 12, 15);
    // Process the second operand.
    uint8_t Rs = readBits(instruct, 8, 11);
    uint8_t type = readBits(instruct, 5, 6);
    uint8_t Rm = readBits(instruct, 0, 3);
    uint8_t shiftAmount = (*activeRegs[Rs]) & 0xFF;
    u32AndBool operandShifted = ARMShift(type, *activeRegs[Rm], shiftAmount, readBit(cpsr, C_FLAG));
    // Data processing arguments.
    uint32_t operand1 = *activeRegs[Rn];
    uint32_t operand2 = operandShifted.data_u32;
    bool carry = operandShifted.data_bool;
    return ARM_ORR(Rd, operand1, operand2, carry, S);
}
cycles ARM::ARM_ORR_IMM(uint32_t instruct) {
    // S - Set Condition Codes
    bool S = readBit(instruct, 20);
    // 1st Operand Register.
    uint8_t Rn = readBits(instruct, 16, 19);
    // Destination Register.
    uint8_t Rd = readBits(instruct, 12, 15);
    // Process the second operand.
    uint8_t Rs = readBits(instruct, 8, 11);
    uint8_t type = readBits(instruct, 5, 6);
    uint8_t Rm = readBits(instruct, 0, 3);
    uint8_t shiftAmount = (*activeRegs[Rs]) & 0xFF;
    u32AndBool operandShifted = ARMShift(type, *activeRegs[Rm], shiftAmount, readBit(cpsr, C_FLAG));
    // Intermediate encodings.
    uint32_t imm12 = readBits(instruct, 0, 11);
    u32AndBool immDecoded = ARMExpandImm_C(imm12, readBit(cpsr, C_FLAG));
    // Data processing arguments.
    uint32_t operand1 = *activeRegs[Rn];
    uint32_t operand2 = immDecoded.data_u32;
    bool carry = immDecoded.data_bool;
    return ARM_ORR(Rd, operand1, operand2, carry, S);
}
// ==================================================================================================
// MOV
// ==================================================================================================
cycles ARM::ARM_MOV(uint32_t desReg, uint32_t srcValue, bool carry, bool setFlags) {
    LogDebug("Executing MOV");
    if (setFlags) {
        LogDebug("Updating CPU flags...");
        writeBit(cpsr, readBit(srcValue, 31), N_FLAG);
        writeBit(cpsr, srcValue == 0, Z_FLAG);
        writeBit(cpsr, carry, C_FLAG);
    }
    *activeRegs[desReg] = srcValue;
    fixupIfTargetingPC(desReg);
    return 1;  // An instruction prefetch occurs at the same time as the data operation.
}
cycles ARM::ARM_MOV_REG(uint32_t instruct) {
    // S - Set Condition Codes
    bool S = readBit(instruct, 20);
    // Destination Register.
    uint8_t Rd = readBits(instruct, 12, 15);
    // Process the operand.
    uint8_t imm5 = readBits(instruct, 7, 11);
    uint8_t type = readBits(instruct, 5, 6);
    decodeShiftResult shiftOp = ARMDecodeImmShift(type, imm5);
    uint8_t Rm = readBits(instruct, 0, 3);
    u32AndBool operandShifted = ARMShift(*activeRegs[Rm], shiftOp, readBit(cpsr, C_FLAG));
    // Data processing arguments.
    uint32_t operand = operandShifted.data_u32;
    bool carry = operandShifted.data_bool;
    return ARM_MOV(Rd, operand, carry, S);
}
cycles ARM::ARM_MOV_REG_SHIFT(uint32_t instruct) {
    // S - Set Condition Codes
    bool S = readBit(instruct, 20);
    // Destination Register.
    uint8_t Rd = readBits(instruct, 12, 15);
    // Process the operand.
    uint8_t Rs = readBits(instruct, 8, 11);
    uint8_t type = readBits(instruct, 5, 6);
    uint8_t Rm = readBits(instruct, 0, 3);
    uint8_t shiftAmount = (*activeRegs[Rs]) & 0xFF;
    u32AndBool operandShifted = ARMShift(type, *activeRegs[Rm], shiftAmount, readBit(cpsr, C_FLAG));
    // Data processing arguments.
    uint32_t operand = operandShifted.data_u32;
    bool carry = operandShifted.data_bool;
    return ARM_MOV(Rd, operand, carry, S);
}
cycles ARM::ARM_MOV_IMM(uint32_t instruct) {
    // S - Set Condition Codes
    bool S = readBit(instruct, 20);
    // Destination Register.
    uint8_t Rd = readBits(instruct, 12, 15);
    // Process the operand.
    uint8_t Rs = readBits(instruct, 8, 11);
    uint8_t type = readBits(instruct, 5, 6);
    uint8_t Rm = readBits(instruct, 0, 3);
    uint8_t shiftAmount = (*activeRegs[Rs]) & 0xFF;
    u32AndBool operandShifted = ARMShift(type, *activeRegs[Rm], shiftAmount, readBit(cpsr, C_FLAG));
    // Intermediate encodings.
    uint32_t imm12 = readBits(instruct, 0, 11);
    u32AndBool immDecoded = ARMExpandImm_C(imm12, readBit(cpsr, C_FLAG));
    // Data processing arguments.
    uint32_t operand = immDecoded.data_u32;
    bool carry = immDecoded.data_bool;
    return ARM_MOV(Rd, operand, carry, S);
}
// ==================================================================================================
// BIC
// ==================================================================================================
cycles ARM::ARM_BIC(uint32_t desReg, uint32_t opp1, uint32_t opp2, bool carry, bool setFlags) {
    LogDebug("Executing BIC");
    uint32_t result = opp1 & ~opp2;
    if (setFlags) {
        LogDebug("Updating CPU flags...");
        writeBit(cpsr, readBit(result, 31), N_FLAG);
        writeBit(cpsr, result == 0, Z_FLAG);
        writeBit(cpsr, carry, C_FLAG);
    }
    *activeRegs[desReg] = result;
    fixupIfTargetingPC(desReg);
    return 1;  // An instruction prefetch occurs at the same time as the data operation.
}
cycles ARM::ARM_BIC_REG(uint32_t instruct) {
    // S - Set Condition Codes
    bool S = readBit(instruct, 20);
    // 1st Operand Register.
    uint8_t Rn = readBits(instruct, 16, 19);
    // Destination Register.
    uint8_t Rd = readBits(instruct, 12, 15);
    // Process the second operand.
    uint8_t imm5 = readBits(instruct, 7, 11);
    uint8_t type = readBits(instruct, 5, 6);
    decodeShiftResult shiftOp = ARMDecodeImmShift(type, imm5);
    uint8_t Rm = readBits(instruct, 0, 3);
    u32AndBool operandShifted = ARMShift(*activeRegs[Rm], shiftOp, readBit(cpsr, C_FLAG));
    // Data processing arguments.
    uint32_t operand1 = *activeRegs[Rn];
    uint32_t operand2 = operandShifted.data_u32;
    bool carry = operandShifted.data_bool;
    return ARM_BIC(Rd, operand1, operand2, carry, S);
}
cycles ARM::ARM_BIC_REG_SHIFT(uint32_t instruct) {
    // S - Set Condition Codes
    bool S = readBit(instruct, 20);
    // 1st Operand Register.
    uint8_t Rn = readBits(instruct, 16, 19);
    // Destination Register.
    uint8_t Rd = readBits(instruct, 12, 15);
    // Process the second operand.
    uint8_t Rs = readBits(instruct, 8, 11);
    uint8_t type = readBits(instruct, 5, 6);
    uint8_t Rm = readBits(instruct, 0, 3);
    uint8_t shiftAmount = (*activeRegs[Rs]) & 0xFF;
    u32AndBool operandShifted = ARMShift(type, *activeRegs[Rm], shiftAmount, readBit(cpsr, C_FLAG));
    // Data processing arguments.
    uint32_t operand1 = *activeRegs[Rn];
    uint32_t operand2 = operandShifted.data_u32;
    bool carry = operandShifted.data_bool;
    return ARM_BIC(Rd, operand1, operand2, carry, S);
}
cycles ARM::ARM_BIC_IMM(uint32_t instruct) {
    // S - Set Condition Codes
    bool S = readBit(instruct, 20);
    // 1st Operand Register.
    uint8_t Rn = readBits(instruct, 16, 19);
    // Destination Register.
    uint8_t Rd = readBits(instruct, 12, 15);
    // Process the second operand.
    uint8_t Rs = readBits(instruct, 8, 11);
    uint8_t type = readBits(instruct, 5, 6);
    uint8_t Rm = readBits(instruct, 0, 3);
    uint8_t shiftAmount = (*activeRegs[Rs]) & 0xFF;
    u32AndBool operandShifted = ARMShift(type, *activeRegs[Rm], shiftAmount, readBit(cpsr, C_FLAG));
    // Intermediate encodings.
    uint32_t imm12 = readBits(instruct, 0, 11);
    u32AndBool immDecoded = ARMExpandImm_C(imm12, readBit(cpsr, C_FLAG));
    // Data processing arguments.
    uint32_t operand1 = *activeRegs[Rn];
    uint32_t operand2 = immDecoded.data_u32;
    bool carry = immDecoded.data_bool;
    return ARM_BIC(Rd, operand1, operand2, carry, S);
}
// ==================================================================================================
// MVN
// ==================================================================================================
cycles ARM::ARM_MVN(uint32_t desReg, uint32_t opp1, bool carry, bool setFlags) {
    LogDebug("Executing MVN");
    uint32_t result = ~opp1;
    if (setFlags) {
        LogDebug("Updating CPU flags...");
        writeBit(cpsr, readBit(result, 31), N_FLAG);
        writeBit(cpsr, result == 0, Z_FLAG);
        writeBit(cpsr, carry, C_FLAG);
    }
    *activeRegs[desReg] = result;
    fixupIfTargetingPC(desReg);
    return 1;  // An instruction prefetch occurs at the same time as the data operation.
}
cycles ARM::ARM_MVN_REG(uint32_t instruct) {
    // S - Set Condition Codes
    bool S = readBit(instruct, 20);
    // Destination Register.
    uint8_t Rd = readBits(instruct, 12, 15);
    // Process the operand.
    uint8_t imm5 = readBits(instruct, 7, 11);
    uint8_t type = readBits(instruct, 5, 6);
    decodeShiftResult shiftOp = ARMDecodeImmShift(type, imm5);
    uint8_t Rm = readBits(instruct, 0, 3);
    u32AndBool operandShifted = ARMShift(*activeRegs[Rm], shiftOp, readBit(cpsr, C_FLAG));
    // Data processing arguments.
    uint32_t operand = operandShifted.data_u32;
    bool carry = operandShifted.data_bool;
    return ARM_MVN(Rd, operand, carry, S);
}
cycles ARM::ARM_MVN_REG_SHIFT(uint32_t instruct) {
    // S - Set Condition Codes
    bool S = readBit(instruct, 20);
    // Destination Register.
    uint8_t Rd = readBits(instruct, 12, 15);
    // Process the operand.
    uint8_t Rs = readBits(instruct, 8, 11);
    uint8_t type = readBits(instruct, 5, 6);
    uint8_t Rm = readBits(instruct, 0, 3);
    uint8_t shiftAmount = (*activeRegs[Rs]) & 0xFF;
    u32AndBool operandShifted = ARMShift(type, *activeRegs[Rm], shiftAmount, readBit(cpsr, C_FLAG));
    // Data processing arguments.;
    uint32_t operand = operandShifted.data_u32;
    bool carry = operandShifted.data_bool;
    return ARM_MVN(Rd, operand, carry, S);
}
cycles ARM::ARM_MVN_IMM(uint32_t instruct) {
    // S - Set Condition Codes
    bool S = readBit(instruct, 20);
    // Destination Register.
    uint8_t Rd = readBits(instruct, 12, 15);
    // Process the operand.
    uint8_t Rs = readBits(instruct, 8, 11);
    uint8_t type = readBits(instruct, 5, 6);
    uint8_t Rm = readBits(instruct, 0, 3);
    uint8_t shiftAmount = (*activeRegs[Rs]) & 0xFF;
    u32AndBool operandShifted = ARMShift(type, *activeRegs[Rm], shiftAmount, readBit(cpsr, C_FLAG));
    // Intermediate encodings.
    uint32_t imm12 = readBits(instruct, 0, 11);
    u32AndBool immDecoded = ARMExpandImm_C(imm12, readBit(cpsr, C_FLAG));
    // Data processing arguments.
    uint32_t operand = immDecoded.data_u32;
    bool carry = immDecoded.data_bool;
    return ARM_MVN(Rd, operand, carry, S);
}
// ==================================================================================================
// Multiply and Multiply Accumulate
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/ARM-Instruction-Set-Encoding/Data-processing-and-miscellaneous-instructions/Multiply-and-multiply-accumulate?lang=en
// ==================================================================================================
cycles ARM::ARM_MUL(uint32_t instruct) {
    // S - Set Condition Codes
    bool S = readBit(instruct, 20);
    // Destination Register.
    uint8_t Rd = readBits(instruct, 16, 19);
    // Operand Resigners.
    uint8_t Rm = readBits(instruct, 8, 11);
    uint8_t Rn = readBits(instruct, 0, 3);
    // Compute the result using the operand registers.
    uint32_t operand1 = *activeRegs[Rn];
    uint32_t operand2 = *activeRegs[Rm];
    uint32_t result = operand1 * operand2;
    // Update the flags and save the result.
    if (S) {
        LogDebug("Updating CPU flags...");
        writeBit(cpsr, readBit(result, 31), N_FLAG);
        writeBit(cpsr, result == 0, Z_FLAG);
    }
    *activeRegs[Rd] = result;
    fixupIfTargetingPC(Rd);
    return 1;
}
cycles ARM::ARM_MLA(uint32_t instruct) {
    // S - Set Condition Codes
    bool S = readBit(instruct, 20);
    // Destination Register.
    uint8_t Rd = readBits(instruct, 16, 19);
    // Operand Resigners.
    uint8_t Rs = readBits(instruct, 12, 15);
    uint8_t Rm = readBits(instruct, 8, 11);
    uint8_t Rn = readBits(instruct, 0, 3);
    // Compute the result using the operand registers.
    uint32_t operand1 = *activeRegs[Rn];
    uint32_t operand2 = *activeRegs[Rm];
    uint32_t addend = *activeRegs[Rs];
    uint32_t result = operand1 * operand2 + addend;
    // Update the flags and save the result.
    if (S) {
        LogDebug("Updating CPU flags...");
        writeBit(cpsr, readBit(result, 31), N_FLAG);
        writeBit(cpsr, result == 0, Z_FLAG);
    }
    *activeRegs[Rd] = result;
    fixupIfTargetingPC(Rd);
    return 1;
}
cycles ARM::ARM_UMAAL(uint32_t instruct) {
    // Note: Not supported by the DS's arm7 or arm9 cores.
    // Operand Resigners.
    uint8_t RdHi = readBits(instruct, 16, 19);
    uint8_t RdLo = readBits(instruct, 12, 15);
    uint8_t Rm = readBits(instruct, 8, 11);
    uint8_t Rn = readBits(instruct, 0, 3);
    // Compute the result using the operand registers.
    uint64_t operand1 = *activeRegs[Rn];
    uint64_t operand2 = *activeRegs[Rm];
    uint64_t addend1 = *activeRegs[RdHi];
    uint64_t addend2 = *activeRegs[RdLo];
    uint64_t result = operand1 * operand2 + addend1 + addend2;
    // Save the result.
    LogDebug(result);
    *activeRegs[RdHi] = (uint32_t)(result >> 32);
    *activeRegs[RdLo] = (uint32_t)(result);
    fixupIfTargetingPC(RdHi);
    fixupIfTargetingPC(RdLo);
    return 1;
}
cycles ARM::ARM_MLS(uint32_t instruct) {
    // Note: Not supported by the DS's arm7 or arm9 cores.
    // Destination Register.
    uint8_t Rd = readBits(instruct, 16, 19);
    // Operand Resigners.
    uint8_t Rs = readBits(instruct, 12, 15);
    uint8_t Rm = readBits(instruct, 8, 11);
    uint8_t Rn = readBits(instruct, 0, 3);
    // Compute the result using the operand registers.
    uint32_t operand1 = *activeRegs[Rn];
    uint32_t operand2 = *activeRegs[Rm];
    uint32_t addend = *activeRegs[Rs];
    uint32_t result = addend - operand1 * operand2;
    // Save the result.
    *activeRegs[Rd] = result;
    fixupIfTargetingPC(Rd);
    return 1;
}
cycles ARM::ARM_UMULL(uint32_t instruct) {
    // S - Set Condition Codes
    bool S = readBit(instruct, 20);
    // Operand Resigners.
    uint8_t RdHi = readBits(instruct, 16, 19);
    uint8_t RdLo = readBits(instruct, 12, 15);
    uint8_t Rm = readBits(instruct, 8, 11);
    uint8_t Rn = readBits(instruct, 0, 3);
    // Compute the result using the operand registers.
    uint64_t operand1 = *activeRegs[Rn];
    uint64_t operand2 = *activeRegs[Rm];
    uint64_t result = operand1 * operand2;
    // Update the flags and save the result.
    if (S) {
        LogDebug("Updating CPU flags...");
        writeBit(cpsr, readBit(result, 63), N_FLAG);
        writeBit(cpsr, result == 0, Z_FLAG);
    }
    *activeRegs[RdHi] = (uint32_t)(result >> 32);
    *activeRegs[RdLo] = (uint32_t)(result);
    fixupIfTargetingPC(RdHi);
    fixupIfTargetingPC(RdLo);
    return 1;
}
cycles ARM::ARM_UMLAL(uint32_t instruct) {
    // S - Set Condition Codes
    bool S = readBit(instruct, 20);
    // Operand Resigners.
    uint8_t RdHi = readBits(instruct, 16, 19);
    uint8_t RdLo = readBits(instruct, 12, 15);
    uint8_t Rm = readBits(instruct, 8, 11);
    uint8_t Rn = readBits(instruct, 0, 3);
    // Compute the result using the operand registers.
    uint64_t operand1 = *activeRegs[Rn];
    uint64_t operand2 = *activeRegs[Rm];
    uint64_t addend1 = *activeRegs[RdHi];
    uint64_t addend2 = *activeRegs[RdLo];
    uint64_t result = operand1 * operand2 + (addend1 << 32 | addend2);
    // Update the flags and save the result.
    if (S) {
        LogDebug("Updating CPU flags...");
        writeBit(cpsr, readBit(result, 63), N_FLAG);
        writeBit(cpsr, result == 0, Z_FLAG);
    }
    *activeRegs[RdHi] = (uint32_t)(result >> 32);
    *activeRegs[RdLo] = (uint32_t)(result);
    fixupIfTargetingPC(RdHi);
    fixupIfTargetingPC(RdLo);
    return 1;
}
cycles ARM::ARM_SMULL(uint32_t instruct) {
    // S - Set Condition Codes
    bool S = readBit(instruct, 20);
    // Operand Resigners.
    uint8_t RdHi = readBits(instruct, 16, 19);
    uint8_t RdLo = readBits(instruct, 12, 15);
    uint8_t Rm = readBits(instruct, 8, 11);
    uint8_t Rn = readBits(instruct, 0, 3);
    // Compute the result using the operand registers.
    int64_t operand1 = (int32_t)(*activeRegs[Rn]);
    int64_t operand2 = (int32_t)(*activeRegs[Rm]);
    int64_t result = operand1 * operand2;
    // Update the flags and save the result.
    if (S) {
        LogDebug("Updating CPU flags...");
        writeBit(cpsr, readBit(result, 63), N_FLAG);
        writeBit(cpsr, result == 0, Z_FLAG);
    }
    *activeRegs[RdHi] = (int32_t)(result >> 32);
    *activeRegs[RdLo] = (int32_t)(result);
    fixupIfTargetingPC(RdHi);
    fixupIfTargetingPC(RdLo);
    return 1;
}
cycles ARM::ARM_SMLAL(uint32_t instruct) {
    // S - Set Condition Codes
    bool S = readBit(instruct, 20);
    // Operand Resigners.
    uint8_t RdHi = readBits(instruct, 16, 19);
    uint8_t RdLo = readBits(instruct, 12, 15);
    uint8_t Rm = readBits(instruct, 8, 11);
    uint8_t Rn = readBits(instruct, 0, 3);
    // Compute the result using the operand registers.
    int64_t operand1 = (int32_t)(*activeRegs[Rn]);
    int64_t operand2 = (int32_t)(*activeRegs[Rm]);
    uint64_t addend1 = *activeRegs[RdHi];
    uint64_t addend2 = *activeRegs[RdLo];
    int64_t addend = std::bit_cast<int64_t>((addend1 << 32) | addend2);
    int64_t result = operand1 * operand2 + addend;
    // Update the flags and save the result.
    if (S) {
        LogDebug("Updating CPU flags...");
        writeBit(cpsr, readBit(result, 63), N_FLAG);
        writeBit(cpsr, result == 0, Z_FLAG);
    }
    *activeRegs[RdHi] = (int32_t)(result >> 32);
    *activeRegs[RdLo] = (int32_t)(result);
    fixupIfTargetingPC(RdHi);
    fixupIfTargetingPC(RdLo);
    return 1;
}
// ==================================================================================================
// Halfword multiply and multiply accumulate
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/ARM-Instruction-Set-Encoding/Data-processing-and-miscellaneous-instructions/Halfword-multiply-and-multiply-accumulate?lang=en
// ==================================================================================================
// ==================================================================================================
// SMLA
// ==================================================================================================
cycles ARM::ARM_SMLABB(uint32_t instruct) {
    // Note: Only supported by the DS's arm9 core.
    // Destination Register.
    uint8_t Rd = readBits(instruct, 16, 19);
    // Operand Resigners.
    uint8_t Ra = readBits(instruct, 12, 15);
    uint8_t Rm = readBits(instruct, 8, 11);
    uint8_t Rn = readBits(instruct, 0, 3);
    // Process Operands.
    int32_t operand1 = (int16_t)(*activeRegs[Rn]);
    int32_t operand2 = (int16_t)(*activeRegs[Rm]);
    int32_t addend = (*activeRegs[Ra]);
    return ARM_SMLA(Rd, operand1, operand2, addend);
}
cycles ARM::ARM_SMLABT(uint32_t instruct) {
    // Note: Only supported by the DS's arm9 core.
    // Destination Register.
    uint8_t Rd = readBits(instruct, 16, 19);
    // Operand Resigners.
    uint8_t Ra = readBits(instruct, 12, 15);
    uint8_t Rm = readBits(instruct, 8, 11);
    uint8_t Rn = readBits(instruct, 0, 3);
    // Process Operands.
    int32_t operand1 = (int16_t)(*activeRegs[Rn]);
    int32_t operand2 = (int16_t)(*activeRegs[Rm] >> 16);
    int32_t addend = (*activeRegs[Ra]);
    return ARM_SMLA(Rd, operand1, operand2, addend);
}
cycles ARM::ARM_SMLATB(uint32_t instruct) {
    // Note: Only supported by the DS's arm9 core.
    // Destination Register.
    uint8_t Rd = readBits(instruct, 16, 19);
    // Operand Resigners.
    uint8_t Ra = readBits(instruct, 12, 15);
    uint8_t Rm = readBits(instruct, 8, 11);
    uint8_t Rn = readBits(instruct, 0, 3);
    // Process Operands.
    int32_t operand1 = (int16_t)(*activeRegs[Rn] >> 16);
    int32_t operand2 = (int16_t)(*activeRegs[Rm]);
    int32_t addend = (*activeRegs[Ra]);
    return ARM_SMLA(Rd, operand1, operand2, addend);
}
cycles ARM::ARM_SMLATT(uint32_t instruct) {
    // Note: Only supported by the DS's arm9 core.
    // Destination Register.
    uint8_t Rd = readBits(instruct, 16, 19);
    // Operand Resigners.
    uint8_t Ra = readBits(instruct, 12, 15);
    uint8_t Rm = readBits(instruct, 8, 11);
    uint8_t Rn = readBits(instruct, 0, 3);
    // Process Operands.
    int32_t operand1 = (int16_t)(*activeRegs[Rn] >> 16);
    int32_t operand2 = (int16_t)(*activeRegs[Rm] >> 16);
    int32_t addend = (*activeRegs[Ra]);
    return ARM_SMLA(Rd, operand1, operand2, addend);
}
cycles ARM::ARM_SMLAWB(uint32_t instruct) {
    // Note: Only supported by the DS's arm9 core.
    // Destination Register.
    uint8_t Rd = readBits(instruct, 16, 19);
    // Operand Resigners.
    uint8_t Ra = readBits(instruct, 12, 15);
    uint8_t Rm = readBits(instruct, 8, 11);
    uint8_t Rn = readBits(instruct, 0, 3);
    // Process Operands.
    int32_t operand1 = (*activeRegs[Rn]);
    int32_t operand2 = (int16_t)(*activeRegs[Rm]);
    int64_t addend = ((int32_t)(*activeRegs[Ra]) << 16);
    // Compute the result.
    int64_t result = operand1 * operand2 + addend;
    *activeRegs[Rd] = (result >> 16);
    fixupIfTargetingPC(Rd);
    // Set Q Flag.
    if ((result >> 16) != ((int32_t)*activeRegs[Rd])) {
        setFlag(Q_FLAG, 1);
    }
    return 1;
}
cycles ARM::ARM_SMLAWT(uint32_t instruct) {
    // Note: Only supported by the DS's arm9 core.
    // Destination Register.
    uint8_t Rd = readBits(instruct, 16, 19);
    // Operand Resigners.
    uint8_t Ra = readBits(instruct, 12, 15);
    uint8_t Rm = readBits(instruct, 8, 11);
    uint8_t Rn = readBits(instruct, 0, 3);
    // Process Operands.
    int32_t operand1 = (*activeRegs[Rn]);
    int32_t operand2 = (int16_t)(*activeRegs[Rm] >> 16);
    int64_t addend = ((int32_t)(*activeRegs[Ra]) << 16);
    // Compute the result.
    int64_t result = operand1 * operand2 + addend;
    *activeRegs[Rd] = (result >> 16);
    fixupIfTargetingPC(Rd);
    // Set Q Flag.
    if ((result >> 16) != ((int32_t)*activeRegs[Rd])) {
        setFlag(Q_FLAG, 1);
    }
    return 1;
}
cycles ARM::ARM_SMLA(uint32_t desReg, int32_t opp1, int32_t opp2, int32_t addend) {
    // Note: Only supported by the DS's arm9 core.
    int64_t result = (int64_t)opp1 * (int64_t)opp2 + (int64_t)addend;
    *activeRegs[desReg] = result;
    fixupIfTargetingPC(desReg);
    // Set Q Flag.
    if (result != ((int32_t)result)) {
        setFlag(Q_FLAG, 1);
    }
    return 1;
}
// ==================================================================================================
// SMUL
// ==================================================================================================
cycles ARM::ARM_SMULBB(uint32_t instruct) {
    // Note: Only supported by the DS's arm9 core.
    // Destination Register.
    uint8_t Rd = readBits(instruct, 16, 19);
    // Operand Resigners.
    uint8_t Rm = readBits(instruct, 8, 11);
    uint8_t Rn = readBits(instruct, 0, 3);
    // Process Operands.
    int32_t operand1 = (int16_t)(*activeRegs[Rn]);
    int32_t operand2 = (int16_t)(*activeRegs[Rm]);
    return ARM_SMUL(Rd, operand1, operand2);
}
cycles ARM::ARM_SMULBT(uint32_t instruct) {
    // Note: Only supported by the DS's arm9 core.
    // Destination Register.
    uint8_t Rd = readBits(instruct, 16, 19);
    // Operand Resigners.
    uint8_t Rm = readBits(instruct, 8, 11);
    uint8_t Rn = readBits(instruct, 0, 3);
    // Process Operands.
    int32_t operand1 = (int16_t)(*activeRegs[Rn]);
    int32_t operand2 = (int16_t)(*activeRegs[Rm] >> 16);
    return ARM_SMUL(Rd, operand1, operand2);
}
cycles ARM::ARM_SMULTB(uint32_t instruct) {
    // Note: Only supported by the DS's arm9 core.
    // Destination Register.
    uint8_t Rd = readBits(instruct, 16, 19);
    // Operand Resigners.
    uint8_t Rm = readBits(instruct, 8, 11);
    uint8_t Rn = readBits(instruct, 0, 3);
    // Process Operands.
    int32_t operand1 = (int16_t)(*activeRegs[Rn] >> 16);
    int32_t operand2 = (int16_t)(*activeRegs[Rm]);
    return ARM_SMUL(Rd, operand1, operand2);
}
cycles ARM::ARM_SMULTT(uint32_t instruct) {
    // Note: Only supported by the DS's arm9 core.
    // Destination Register.
    uint8_t Rd = readBits(instruct, 16, 19);
    // Operand Resigners.
    uint8_t Rm = readBits(instruct, 8, 11);
    uint8_t Rn = readBits(instruct, 0, 3);
    // Process Operands.
    int32_t operand1 = (int16_t)(*activeRegs[Rn] >> 16);
    int32_t operand2 = (int16_t)(*activeRegs[Rm] >> 16);
    return ARM_SMUL(Rd, operand1, operand2);
}
cycles ARM::ARM_SMULWB(uint32_t instruct) {
    // Note: Only supported by the DS's arm9 core.
    // Destination Register.
    uint8_t Rd = readBits(instruct, 16, 19);
    // Operand Resigners.
    uint8_t Rm = readBits(instruct, 8, 11);
    uint8_t Rn = readBits(instruct, 0, 3);
    // Process Operands.
    int64_t operand1 = (int16_t)(*activeRegs[Rn]);
    int64_t operand2 = (int16_t)(*activeRegs[Rm]);
    int64_t result = operand1 * operand2;
    *activeRegs[Rd] = (result >> 16);
    fixupIfTargetingPC(Rd);
    // Q Overflow cannot occur here.
    return 1;
}
cycles ARM::ARM_SMULWT(uint32_t instruct) {
    // Note: Only supported by the DS's arm9 core.
    // Destination Register.
    uint8_t Rd = readBits(instruct, 16, 19);
    // Operand Resigners.
    uint8_t Rm = readBits(instruct, 8, 11);
    uint8_t Rn = readBits(instruct, 0, 3);
    // Process Operands.
    int64_t operand1 = (int16_t)(*activeRegs[Rn]);
    int64_t operand2 = (int16_t)(*activeRegs[Rm] >> 16);
    int64_t result = operand1 * operand2;
    *activeRegs[Rd] = (result >> 16);
    fixupIfTargetingPC(Rd);
    // Q Overflow cannot occur here.
    return 1;
}
cycles ARM::ARM_SMUL(uint32_t desReg, int32_t opp1, int32_t opp2) {
    // Note: Only supported by the DS's arm9 core.
    int32_t result = opp1 * opp2;
    *activeRegs[desReg] = result;
    fixupIfTargetingPC(desReg);
    // Q Overflow cannot occur here.
    return 1;
}
// ==================================================================================================
// SMLAL
// ==================================================================================================
cycles ARM::ARM_SMLALBB(uint32_t instruct) {
    // Note: Only supported by the DS's arm9 core.
    // Operand Resigners.
    uint8_t RdHi = readBits(instruct, 16, 19);
    uint8_t RdLo = readBits(instruct, 12, 15);
    uint8_t Rm = readBits(instruct, 8, 11);
    uint8_t Rn = readBits(instruct, 0, 3);
    // Process Operands.
    int64_t operand1 = (int32_t)(*activeRegs[Rn]);
    int64_t operand2 = (int32_t)(*activeRegs[Rm]);
    uint64_t addend1 = *activeRegs[RdHi];
    uint64_t addend2 = *activeRegs[RdLo];
    int64_t addend = std::bit_cast<int64_t>((addend1 << 32) | addend2);
    return ARM_SMLAL(RdLo, RdHi, operand1, operand2, addend);
}
cycles ARM::ARM_SMLALBT(uint32_t instruct) {
    // Note: Only supported by the DS's arm9 core.
    // Operand Resigners.
    uint8_t RdHi = readBits(instruct, 16, 19);
    uint8_t RdLo = readBits(instruct, 12, 15);
    uint8_t Rm = readBits(instruct, 8, 11);
    uint8_t Rn = readBits(instruct, 0, 3);
    // Process Operands.
    int64_t operand1 = (int32_t)(*activeRegs[Rn]);
    int64_t operand2 = (int32_t)(*activeRegs[Rm] >> 16);
    uint64_t addend1 = *activeRegs[RdHi];
    uint64_t addend2 = *activeRegs[RdLo];
    int64_t addend = std::bit_cast<int64_t>((addend1 << 32) | addend2);
    return ARM_SMLAL(RdLo, RdHi, operand1, operand2, addend);
}
cycles ARM::ARM_SMLALTB(uint32_t instruct) {
    // Note: Only supported by the DS's arm9 core.
    // Operand Resigners.
    uint8_t RdHi = readBits(instruct, 16, 19);
    uint8_t RdLo = readBits(instruct, 12, 15);
    uint8_t Rm = readBits(instruct, 8, 11);
    uint8_t Rn = readBits(instruct, 0, 3);
    // Process Operands.
    int64_t operand1 = (int32_t)(*activeRegs[Rn] >> 16);
    int64_t operand2 = (int32_t)(*activeRegs[Rm]);
    uint64_t addend1 = *activeRegs[RdHi];
    uint64_t addend2 = *activeRegs[RdLo];
    int64_t addend = std::bit_cast<int64_t>((addend1 << 32) | addend2);
    return ARM_SMLAL(RdLo, RdHi, operand1, operand2, addend);
}
cycles ARM::ARM_SMLALTT(uint32_t instruct) {
    // Note: Only supported by the DS's arm9 core.
    // Operand Resigners.
    uint8_t RdHi = readBits(instruct, 16, 19);
    uint8_t RdLo = readBits(instruct, 12, 15);
    uint8_t Rm = readBits(instruct, 8, 11);
    uint8_t Rn = readBits(instruct, 0, 3);
    // Process Operands.
    int64_t operand1 = (int32_t)(*activeRegs[Rn] >> 16);
    int64_t operand2 = (int32_t)(*activeRegs[Rm] >> 16);
    uint64_t addend1 = *activeRegs[RdHi];
    uint64_t addend2 = *activeRegs[RdLo];
    int64_t addend = std::bit_cast<int64_t>((addend1 << 32) | addend2);
    return ARM_SMLAL(RdLo, RdHi, operand1, operand2, addend);
}
cycles ARM::ARM_SMLAL(uint32_t desRegLow, uint32_t desRegHigh, int64_t opp1, int64_t opp2,
                      int64_t addend) {
    // Note: Only supported by the DS's arm9 core.
    int64_t result = opp1 * opp2 + addend;
    *activeRegs[desRegHigh] = (int32_t)(result >> 32);
    *activeRegs[desRegLow] = (int32_t)(result);
    fixupIfTargetingPC(desRegHigh);
    fixupIfTargetingPC(desRegLow);
    // Q Overflow cannot occur here.
    return 1;
}
// ==================================================================================================
// Saturating addition and subtraction
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/ARM-Instruction-Set-Encoding/Data-processing-and-miscellaneous-instructions/Saturating-addition-and-subtraction?lang=en
// ==================================================================================================
// ==================================================================================================
// QUAD
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/QADD?lang=en
// ==================================================================================================
cycles ARM::ARM_QADD(uint32_t instruct) {
    // Destination Register.
    uint8_t Rd = readBits(instruct, 12, 15);
    // Operand Resigners.
    uint8_t Rn = readBits(instruct, 16, 19);
    uint8_t Rm = readBits(instruct, 0, 3);
    int64_t operand1 = (int32_t)(*activeRegs[Rm]);
    int64_t operand2 = (int32_t)(*activeRegs[Rn]);
    *activeRegs[Rd] = (int32_t)signedSaturatedQ(operand1 + operand2, 32);
    fixupIfTargetingPC(Rd);
    return 1;
}
// ==================================================================================================
// QSUB
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/QSUB?lang=en
// ==================================================================================================
cycles ARM::ARM_QSUB(uint32_t instruct) {
    // Destination Register.
    uint8_t Rd = readBits(instruct, 12, 15);
    // Operand Resigners.
    uint8_t Rn = readBits(instruct, 16, 19);
    uint8_t Rm = readBits(instruct, 0, 3);
    int64_t operand1 = (int32_t)(*activeRegs[Rm]);
    int64_t operand2 = (int32_t)(*activeRegs[Rn]);
    *activeRegs[Rd] = (int32_t)signedSaturatedQ(operand1 - operand2, 32);
    fixupIfTargetingPC(Rd);
    return 1;
}
// ==================================================================================================
// QDADD
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/QDADD?lang=en
// ==================================================================================================
cycles ARM::ARM_QDADD(uint32_t instruct) {
    // Destination Register.
    uint8_t Rd = readBits(instruct, 12, 15);
    // Operand Resigners.
    uint8_t Rn = readBits(instruct, 16, 19);
    uint8_t Rm = readBits(instruct, 0, 3);
    int64_t operand1 = (int32_t)(*activeRegs[Rm]);
    int64_t operand2 = (int32_t)(*activeRegs[Rn]);
    int64_t doubled = signedSaturatedQ(2 * operand2, 32);
    *activeRegs[Rd] = (int32_t)signedSaturatedQ(operand1 + doubled, 32);
    fixupIfTargetingPC(Rd);
    return 1;
}
// ==================================================================================================
// QDSUB
// ==================================================================================================
cycles ARM::ARM_QDSUB(uint32_t instruct) {
    // Destination Register.
    uint8_t Rd = readBits(instruct, 12, 15);
    // Operand Resigners.
    uint8_t Rn = readBits(instruct, 16, 19);
    uint8_t Rm = readBits(instruct, 0, 3);
    int64_t operand1 = (int32_t)(*activeRegs[Rm]);
    int64_t operand2 = (int32_t)(*activeRegs[Rn]);
    int64_t doubled = signedSaturatedQ(2 * operand2, 32);
    *activeRegs[Rd] = (int32_t)signedSaturatedQ(operand1 - doubled, 32);
    fixupIfTargetingPC(Rd);
    return 1;
}
// ==================================================================================================
// Other.
// ==================================================================================================
// ==================================================================================================
// SWP
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/SWP--SWPB?lang=en
// ==================================================================================================
cycles ARM::ARM_SWP(uint32_t instruct) {
    uint8_t Rn = readBits(instruct, 16, 19);
    uint8_t Rt = readBits(instruct, 12, 15);
    uint8_t Rt2 = readBits(instruct, 0, 3);
    busPayload dataRead = readBus(*activeRegs[Rn]);
    busPayload dataWrite = writeBus(*activeRegs[Rn], *activeRegs[Rt2]);
    *activeRegs[Rt] = dataRead.data;
    fixupIfTargetingPC(Rt);
    return 1;
}
// ==================================================================================================
cycles ARM::ARM_SWPB(uint32_t instruct) {
    uint8_t Rn = readBits(instruct, 16, 19);
    uint8_t Rt = readBits(instruct, 12, 15);
    uint8_t Rt2 = readBits(instruct, 0, 3);
    busPayload dataRead = readBus(*activeRegs[Rn], 8);
    busPayload dataWrite = writeBus(*activeRegs[Rn], (*activeRegs[Rt2] & 0xFF), 8);
    *activeRegs[Rt] = (dataRead.data & 0xFF);
    fixupIfTargetingPC(Rt);
    return 1;
}
// ==================================================================================================
// MRS
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/MRS?lang=en
// https://developer.arm.com/documentation/ddi0406/cb/System-Level-Architecture/System-Instructions/Alphabetical-list-of-instructions/MRS?lang=en
// ==================================================================================================
cycles ARM::ARM_MRS(uint32_t instruct) {
    bool R = readBit(instruct, 22);
    uint32_t Rd = readBits(instruct, 12, 15);
    uint32_t dataRead = 0;
    if (R) {
        if (spsr == nullptr) {
            LogError("Cannot read from SPSR in current processor Mode!");
            return 1;
        }
        dataRead = *spsr;
    } else {
        dataRead = cpsr;
    }
    *activeRegs[Rd] = dataRead;
    fixupIfTargetingPC(Rd);
    return 1;
}
// ==================================================================================================
// MSR
// ==================================================================================================
cycles ARM::ARM_MSR_REG(uint32_t instruct) {
    bool R = readBit(instruct, 22);
    uint32_t Rn = readBits(instruct, 0, 3);
    uint32_t maskBits = readBits(instruct, 16, 19);
    return ARM_MSR(R, *activeRegs[Rn], maskBits);
}
// ==================================================================================================
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/MSR--immediate-?lang=en
// https://developer.arm.com/documentation/ddi0406/cb/System-Level-Architecture/System-Instructions/Alphabetical-list-of-instructions/MSR--immediate-?lang=en
// https://developer.arm.com/documentation/ddi0406/cb/System-Level-Architecture/System-Instructions/Alphabetical-list-of-instructions/MSR--immediate-?lang=en
cycles ARM::ARM_MSR_IMM(uint32_t instruct) {
    bool R = readBit(instruct, 22);
    uint32_t imm12 = readBits(instruct, 0, 11);
    u32AndBool immDecoded = ARMExpandImm_C(imm12, 0);
    uint32_t maskBits = readBits(instruct, 16, 19);
    return ARM_MSR(R, immDecoded.data_u32, maskBits);
}
// ==================================================================================================
cycles ARM::ARM_MSR(bool readSPSR, uint32_t value, uint32_t maskBits) {
    uint32_t mask = 0;
    if (readBit(maskBits, 0)) mask |= 0x000000FF;
    if (readBit(maskBits, 1)) mask |= 0x0000FF00;
    if (readBit(maskBits, 2)) mask |= 0x00FF0000;
    if (readBit(maskBits, 3)) mask |= 0xFF000000;
    // Handle processor modes.
    if (getProcessorMode() == ProcessorModes::User) {
        mask &= 0xFF000000;  // Only support writing to the flags in user mode.
    }
    if (readSPSR) {
        if (spsr == nullptr) {
            LogError("Cannot write to SPSR in current processor Mode!");
            return 1;
        }
        *spsr = value & mask;
    } else {
        setCPSR(value & mask);
    }
    return 1;
}
// ==================================================================================================
// CLZ
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/CLZ?lang=en
// ==================================================================================================
cycles ARM::ARM_CLZ(uint32_t instruct) {
    uint8_t Rd = readBits(instruct, 12, 15);
    uint8_t Rm = readBits(instruct, 0, 3);
    uint32_t data = *activeRegs[Rm];
    uint32_t count = 0;
    // Count leading zeros.
    while ((!readBit(data, 31)) && count < 32) {
        count++;
        data <<= 1;
    }
    *activeRegs[Rd] = (count);
    fixupIfTargetingPC(Rd);
    return 1;
}
// ==================================================================================================