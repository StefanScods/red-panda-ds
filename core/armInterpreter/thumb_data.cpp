/**
 * @file armInterpreter/thumb_data.cpp
 * @brief Implements THUMB mode instuction set for instructions in the data category.
 */
#include <bit>

#include "cpu.h"
#include "interconnect.h"

// Control print statements.
#define LOG_LEVEL 2
#include "logger.h"

namespace RedPandaDS {
namespace Core {

// ==================================================================================================
// MOV
// ==================================================================================================
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/MOV--register--Thumb-?lang=en
cycles ARM::THUMB_MOV_REG_HIGH(uint32_t instruct) {
    uint32_t Rd = readBits(instruct, 0, 3) | readBit(instruct, 7) << 3;
    uint32_t Rm = readBits(instruct, 3, 6);
    return THUMB_MOV(Rd, *activeRegs[Rm], readFlag(C_FLAG), false);
}
// ==================================================================================================
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/MOV--register--Thumb-?lang=en
cycles ARM::THUMB_MOV_REG_LOW(uint32_t instruct) {
    uint32_t Rd = readBits(instruct, 0, 3) | readBit(instruct, 7) << 3;
    uint32_t Rm = readBits(instruct, 3, 6);
    return THUMB_MOV(Rd, *activeRegs[Rm], readFlag(C_FLAG), false);
}
// ==================================================================================================
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/MOV--immediate-?lang=en
cycles ARM::THUMB_MOV_IMM(uint32_t instruct) {
    uint32_t Rd = readBits(instruct, 8, 10);
    uint32_t imm32 = readBits(instruct, 0, 7);
    return THUMB_MOV(Rd, imm32, readFlag(C_FLAG), true);
}
// ==================================================================================================
cycles ARM::THUMB_MOV(uint32_t desReg, uint32_t srcValue, bool carry, bool setFlags) {
    LogDebug("Executing MOV");
    if (setFlags) {
        setFlag(N_FLAG, readBit(srcValue, 31));
        setFlag(Z_FLAG, srcValue == 0);
        setFlag(C_FLAG, carry);
    }
    *activeRegs[desReg] = srcValue;
    fixupIfTargetingPC(desReg);
    return 1;  // One I cycle.
}
// ==================================================================================================
// AND
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/AND--register-?lang=en
// ==================================================================================================
cycles ARM::THUMB_AND_REG(uint32_t instruct) {
    LogDebug("Executing AND");
    uint32_t Rdn = readBits(instruct, 0, 2);
    uint32_t Rm = readBits(instruct, 3, 5);
    uint32_t result = (*activeRegs[Rdn]) & (*activeRegs[Rm]);
    setFlag(N_FLAG, readBit(result, 31));
    setFlag(Z_FLAG, result == 0);
    // Carry is unchanged.
    *activeRegs[Rdn] = result;
    fixupIfTargetingPC(Rdn);
    return 1;  // One I cycle.
}
// ==================================================================================================
// EOR
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/EOR--register-?lang=en
// ==================================================================================================
cycles ARM::THUMB_EOR_REG(uint32_t instruct) {
    LogDebug("Executing EOR");
    uint32_t Rdn = readBits(instruct, 0, 2);
    uint32_t Rm = readBits(instruct, 3, 5);
    uint32_t result = (*activeRegs[Rdn]) ^ (*activeRegs[Rm]);
    setFlag(N_FLAG, readBit(result, 31));
    setFlag(Z_FLAG, result == 0);
    // Carry is unchanged.
    *activeRegs[Rdn] = result;
    fixupIfTargetingPC(Rdn);
    return 1;  // One I cycle.
}
// ==================================================================================================
// LSL
// ==================================================================================================
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/LSL--register-?lang=en
cycles ARM::THUMB_LSL_REG(uint32_t instruct) {
    uint32_t Rdn = readBits(instruct, 0, 2);
    uint32_t Rm = readBits(instruct, 3, 5);
    return THUMB_LSL(Rdn, *activeRegs[Rdn], (uint8_t)(*activeRegs[Rm] & 0xFF), readFlag(C_FLAG));
}
// ==================================================================================================
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/LSL--immediate-?lang=en
cycles ARM::THUMB_LSL_IMM(uint32_t instruct) {
    uint32_t Rd = readBits(instruct, 0, 2);
    uint32_t Rm = readBits(instruct, 3, 5);
    uint32_t imm5 = readBits(instruct, 6, 10);
    decodeShiftResult shiftOp{ARM_SHIFT_LSL, (uint8_t)readBits(instruct, 6, 10)};
    return THUMB_LSL(Rd, *activeRegs[Rm], imm5, readBit(cpsr, C_FLAG));
}
// ==================================================================================================
cycles ARM::THUMB_LSL(uint32_t desReg, uint32_t operand, uint8_t amount, bool carry) {
    LogDebug("Executing LSL");
    decodeShiftResult shiftOp{ARM_SHIFT_LSL, amount};
    u32AndBool result = ARMShift(operand, shiftOp, carry);

    setFlag(N_FLAG, readBit(result.data_u32, 31));
    setFlag(Z_FLAG, result.data_u32 == 0);
    setFlag(C_FLAG, result.data_bool);

    *activeRegs[desReg] = result.data_u32;
    fixupIfTargetingPC(desReg);

    return 1;  // One I cycle.
}
// ==================================================================================================
// LSR
// ==================================================================================================
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/LSR--register-?lang=en
cycles ARM::THUMB_LSR_REG(uint32_t instruct) {
    uint32_t Rdn = readBits(instruct, 0, 2);
    uint32_t Rm = readBits(instruct, 3, 5);
    return THUMB_LSR(Rdn, *activeRegs[Rdn], (uint8_t)(*activeRegs[Rm] & 0xFF), readFlag(C_FLAG));
}
// ==================================================================================================
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/LSR--immediate-?lang=en
cycles ARM::THUMB_LSR_IMM(uint32_t instruct) {
    uint32_t Rd = readBits(instruct, 0, 2);
    uint32_t Rm = readBits(instruct, 3, 5);
    uint32_t imm5 = readBits(instruct, 6, 10);
    decodeShiftResult shiftOp{ARM_SHIFT_LSL, (uint8_t)readBits(instruct, 6, 10)};
    return THUMB_LSR(Rd, *activeRegs[Rm], imm5, readBit(cpsr, C_FLAG));
}
// ==================================================================================================
cycles ARM::THUMB_LSR(uint32_t desReg, uint32_t operand, uint8_t amount, bool carry) {
    LogDebug("Executing LSR");
    decodeShiftResult shiftOp{ARM_SHIFT_LSR, amount};
    u32AndBool result = ARMShift(operand, shiftOp, carry);

    setFlag(N_FLAG, readBit(result.data_u32, 31));
    setFlag(Z_FLAG, result.data_u32 == 0);
    setFlag(C_FLAG, result.data_bool);

    *activeRegs[desReg] = result.data_u32;
    fixupIfTargetingPC(desReg);

    return 1;  // One I cycle.
}
// ==================================================================================================
// ASR
// ==================================================================================================
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/LSR--register-?lang=en
cycles ARM::THUMB_ASR_REG(uint32_t instruct) {
    uint32_t Rdn = readBits(instruct, 0, 2);
    uint32_t Rm = readBits(instruct, 3, 5);
    return THUMB_ASR(Rdn, *activeRegs[Rdn], (uint8_t)(*activeRegs[Rm] & 0xFF), readFlag(C_FLAG));
}
// ==================================================================================================
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/ASR--immediate-?lang=en
cycles ARM::THUMB_ASR_IMM(uint32_t instruct) {
    uint32_t Rd = readBits(instruct, 0, 2);
    uint32_t Rm = readBits(instruct, 3, 5);
    uint32_t imm5 = readBits(instruct, 6, 10);
    return THUMB_ASR(Rd, *activeRegs[Rm], imm5, readFlag(C_FLAG));
}
// ==================================================================================================
cycles ARM::THUMB_ASR(uint32_t desReg, uint32_t operand, uint8_t amount, bool carry) {
    LogDebug("Executing ASR");
    decodeShiftResult shiftOp{ARM_SHIFT_ASR, amount};
    u32AndBool result = ARMShift(operand, shiftOp, carry);

    setFlag(N_FLAG, readBit(result.data_u32, 31));
    setFlag(Z_FLAG, result.data_u32 == 0);
    setFlag(C_FLAG, result.data_bool);

    *activeRegs[desReg] = result.data_u32;
    fixupIfTargetingPC(desReg);

    return 1;  // One I cycle.
}
// ==================================================================================================
// ADD
// ==================================================================================================
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/ADD--register--Thumb-?lang=en
cycles ARM::THUMB_ADD_REG(uint32_t instruct) {
    uint32_t Rd = readBits(instruct, 0, 2);
    uint32_t Rn = readBits(instruct, 3, 5);
    uint32_t Rm = readBits(instruct, 6, 8);
    return THUMB_ADD(Rd, *activeRegs[Rn], *activeRegs[Rm], 0, true);
}
// ==================================================================================================
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/ADD--register--Thumb-?lang=en
cycles ARM::THUMB_ADD_REG_HIGH(uint32_t instruct) {
    uint32_t Rdn = readBits(instruct, 0, 3) | readBit(instruct, 7) << 3;
    uint32_t Rm = readBits(instruct, 3, 6);
    return THUMB_ADD(Rdn, *activeRegs[Rdn], *activeRegs[Rm], 0, false);
}
// ==================================================================================================
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/ADD--register--Thumb-?lang=en
cycles ARM::THUMB_ADD_REG_LOW(uint32_t instruct) {
    uint32_t Rdn = readBits(instruct, 0, 3) | readBit(instruct, 7) << 3;
    uint32_t Rm = readBits(instruct, 3, 6);
    return THUMB_ADD(Rdn, *activeRegs[Rdn], *activeRegs[Rm], 0, false);
}
// ==================================================================================================
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/ADD--immediate--Thumb-?lang=en
cycles ARM::THUMB_ADD_IMM3(uint32_t instruct) {
    uint32_t Rd = readBits(instruct, 0, 2);
    uint32_t Rn = readBits(instruct, 3, 5);
    uint32_t imm3 = readBits(instruct, 6, 8);
    return THUMB_ADD(Rd, *activeRegs[Rn], imm3, 0, true);
}
// ==================================================================================================
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/ADD--immediate--Thumb-?lang=en
cycles ARM::THUMB_ADD_IMM(uint32_t instruct) {
    uint32_t Rdn = readBits(instruct, 8, 10);
    uint32_t imm8 = readBits(instruct, 0, 7);
    return THUMB_ADD(Rdn, *activeRegs[Rdn], imm8, 0, true);
}
// ==================================================================================================
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/ADD--SP-plus-immediate-?lang=en
cycles ARM::THUMB_ADD_SP_IMM7(uint32_t instruct) {
    uint32_t imm7 = readBits(instruct, 0, 6);
    return THUMB_ADD(SP_REGISTER_NUM, *activeRegs[SP_REGISTER_NUM], imm7 << 2, 0, false);
}
// ==================================================================================================
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/ADD--SP-plus-immediate-?lang=en
cycles ARM::THUMB_ADD_SP_IMM8(uint32_t instruct) {
    uint32_t Rd = readBits(instruct, 8, 10);
    uint32_t imm8 = readBits(instruct, 0, 7);
    return THUMB_ADD(Rd, *activeRegs[SP_REGISTER_NUM], imm8 << 2, 0, false);
}
// ==================================================================================================
cycles ARM::THUMB_ADD(uint32_t desReg, uint32_t opp1, uint32_t opp2, bool carry, bool setFlags) {
    LogDebug("Executing ADD");
    u32WithCarryAndOverflow result = ARMAddWithCarry(opp1, opp2, carry);

    if (setFlags) {
        setFlag(N_FLAG, readBit(result.data_u32, 31));
        setFlag(Z_FLAG, result.data_u32 == 0);
        setFlag(C_FLAG, result.carry);
        setFlag(V_FLAG, result.overflow);
    }

    *activeRegs[desReg] = result.data_u32;
    fixupIfTargetingPC(desReg);
    return 1;  // One I cycle.
}
// ==================================================================================================
// SBC
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/SBC--register-?lang=en
// ==================================================================================================
cycles ARM::THUMB_SBC_REG(uint32_t instruct) {
    uint32_t Rdn = readBits(instruct, 0, 2);
    uint32_t Rm = readBits(instruct, 3, 5);
    return THUMB_SUB(Rdn, *activeRegs[Rdn], *activeRegs[Rm], readFlag(C_FLAG), true);
}
// ==================================================================================================
// ASC
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/ADC--register-?lang=en
// ==================================================================================================
cycles ARM::THUMB_ADC_REG(uint32_t instruct) {
    uint32_t Rdn = readBits(instruct, 0, 2);
    uint32_t Rm = readBits(instruct, 3, 5);
    return THUMB_ADD(Rdn, *activeRegs[Rdn], *activeRegs[Rm], readFlag(C_FLAG), true);
}
// ==================================================================================================
// SUB
// ==================================================================================================
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/SUB--immediate--Thumb-?lang=en
cycles ARM::THUMB_SUB_IMM(uint32_t instruct) {
    uint32_t Rdn = readBits(instruct, 8, 10);
    uint32_t imm8 = readBits(instruct, 0, 7);
    return THUMB_SUB(Rdn, *activeRegs[Rdn], imm8, 1, true);
}
// ==================================================================================================
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/SUB--register-?lang=en
cycles ARM::THUMB_SUB_REG(uint32_t instruct) {
    uint32_t Rd = readBits(instruct, 0, 2);
    uint32_t Rn = readBits(instruct, 3, 5);
    uint32_t Rm = readBits(instruct, 6, 8);
    return THUMB_SUB(Rd, *activeRegs[Rn], *activeRegs[Rm], 1, true);
}
// ==================================================================================================
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/SUB--immediate--Thumb-?lang=en
cycles ARM::THUMB_SUB_IMM3(uint32_t instruct) {
    uint32_t Rd = readBits(instruct, 0, 2);
    uint32_t Rn = readBits(instruct, 3, 5);
    uint32_t imm3 = readBits(instruct, 6, 8);
    return THUMB_SUB(Rd, *activeRegs[Rn], imm3, 1, true);
}
// ==================================================================================================
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/SUB--SP-minus-immediate-?lang=en
cycles ARM::THUMB_SUB_SP_IMM7(uint32_t instruct) {
    uint32_t imm7 = readBits(instruct, 0, 6);
    return THUMB_SUB(SP_REGISTER_NUM, *activeRegs[SP_REGISTER_NUM], imm7 << 2, 1, false);
}
// ==================================================================================================
cycles ARM::THUMB_SUB(uint32_t desReg, uint32_t opp1, uint32_t opp2, bool carry, bool setFlags) {
    LogDebug("Executing SUB");
    u32WithCarryAndOverflow result = ARMAddWithCarry(opp1, ~opp2, carry);

    setFlag(N_FLAG, readBit(result.data_u32, 31));
    setFlag(Z_FLAG, result.data_u32 == 0);
    setFlag(C_FLAG, result.carry);
    setFlag(V_FLAG, result.overflow);

    *activeRegs[desReg] = result.data_u32;
    fixupIfTargetingPC(desReg);
    return 1;  // One I cycle.
}
// ==================================================================================================
// ROR
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/ROR--register-?lang=en
// ==================================================================================================
cycles ARM::THUMB_ROR_REG(uint32_t instruct) {
    LogDebug("Executing ROR");

    uint32_t Rdn = readBits(instruct, 0, 2);
    uint32_t Rm = readBits(instruct, 3, 5);

    decodeShiftResult shiftOp{ARM_SHIFT_ROR, (uint8_t)(*activeRegs[Rm] & 0xFF)};
    u32AndBool result = ARMShift(*activeRegs[Rdn], shiftOp, readBit(cpsr, C_FLAG));

    setFlag(N_FLAG, readBit(result.data_u32, 31));
    setFlag(Z_FLAG, result.data_u32 == 0);
    setFlag(C_FLAG, result.data_bool);

    *activeRegs[Rdn] = result.data_u32;
    fixupIfTargetingPC(Rdn);

    return 1;  // One I cycle.
}
// ==================================================================================================
// TST
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/TST--register-?lang=en
// ==================================================================================================
cycles ARM::THUMB_TST_REG(uint32_t instruct) {
    LogDebug("Executing TST");

    uint32_t Rn = readBits(instruct, 0, 2);
    uint32_t Rm = readBits(instruct, 3, 5);

    uint32_t result = *activeRegs[Rn] & *activeRegs[Rm];
    writeBit(cpsr, readBit(result, 31), N_FLAG);
    writeBit(cpsr, result == 0, Z_FLAG);
    return 1;  // One I cycle.
}
// ==================================================================================================
// RSB
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/RSB--immediate-?lang=en
// ==================================================================================================
cycles ARM::THUMB_RSB_REG(uint32_t instruct) {
    LogDebug("Executing RSB");

    uint32_t Rd = readBits(instruct, 0, 2);
    uint32_t Rn = readBits(instruct, 3, 5);

    u32WithCarryAndOverflow result = ARMAddWithCarry(~(*activeRegs[Rn]), 0, 1);

    setFlag(N_FLAG, readBit(result.data_u32, 31));
    setFlag(Z_FLAG, result.data_u32 == 0);
    setFlag(C_FLAG, result.carry);
    setFlag(V_FLAG, result.overflow);

    *activeRegs[Rd] = result.data_u32;
    fixupIfTargetingPC(Rd);
    return 1;  // One I cycle.
}
// ==================================================================================================
// CMP
// ==================================================================================================
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/CMP--register-?lang=en
cycles ARM::THUMB_CMP_REG(uint32_t instruct) {
    uint32_t Rn = readBits(instruct, 0, 2);
    uint32_t Rm = readBits(instruct, 3, 5);
    return THUMB_CMP(*activeRegs[Rn], *activeRegs[Rm]);
}
// ==================================================================================================
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/CMP--register-?lang=en
cycles ARM::THUMB_CMP_REG_HIGH(uint32_t instruct) {
    uint32_t Rn = readBits(instruct, 0, 3) | readBit(instruct, 7) << 3;
    uint32_t Rm = readBits(instruct, 3, 6);
    return THUMB_CMP(*activeRegs[Rn], *activeRegs[Rm]);
}
// ==================================================================================================
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/CMP--immediate-?lang=en
cycles ARM::THUMB_CMP_IMM(uint32_t instruct) {
    uint32_t Rn = readBits(instruct, 8, 10);
    uint32_t imm8 = readBits(instruct, 0, 7);
    return THUMB_CMP(*activeRegs[Rn], imm8);
}
// ==================================================================================================
cycles ARM::THUMB_CMP(uint32_t opp1, uint32_t opp2) {
    LogDebug("Executing CMP");

    u32WithCarryAndOverflow result = ARMAddWithCarry(opp1, ~opp2, 1);
    setFlag(N_FLAG, readBit(result.data_u32, 31));
    setFlag(Z_FLAG, result.data_u32 == 0);
    setFlag(C_FLAG, result.carry);
    setFlag(V_FLAG, result.overflow);

    return 1;  // One I cycle.
}
// ==================================================================================================
// CMN
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/CMN--register-?lang=en
// ==================================================================================================
cycles ARM::THUMB_CMN_REG(uint32_t instruct) {
    LogDebug("Executing CMN");

    uint32_t Rn = readBits(instruct, 0, 2);
    uint32_t Rm = readBits(instruct, 3, 5);

    u32WithCarryAndOverflow result = ARMAddWithCarry(*activeRegs[Rn], *activeRegs[Rm], 0);
    setFlag(N_FLAG, readBit(result.data_u32, 31));
    setFlag(Z_FLAG, result.data_u32 == 0);
    setFlag(C_FLAG, result.carry);
    setFlag(V_FLAG, result.overflow);

    return 1;  // One I cycle.
}
// ==================================================================================================
// ORR
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/ORR--register-?lang=en
// ==================================================================================================
cycles ARM::THUMB_ORR_REG(uint32_t instruct) {
    LogDebug("Executing ORR");
    uint32_t Rdn = readBits(instruct, 0, 2);
    uint32_t Rm = readBits(instruct, 3, 5);
    uint32_t result = (*activeRegs[Rdn]) | (*activeRegs[Rm]);
    setFlag(N_FLAG, readBit(result, 31));
    setFlag(Z_FLAG, result == 0);
    // Carry is unchanged.
    *activeRegs[Rdn] = result;
    fixupIfTargetingPC(Rdn);
    return 1;  // One I cycle.
}
// ==================================================================================================
// MUL
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/MUL?lang=en
// ==================================================================================================
cycles ARM::THUMB_MUL(uint32_t instruct) {
    LogDebug("Executing MUL");
    uint32_t Rdm = readBits(instruct, 0, 2);
    uint32_t Rn = readBits(instruct, 3, 5);

    uint32_t operand1 = *activeRegs[Rn];
    uint32_t operand2 = *activeRegs[Rdm];
    uint32_t result = operand1 * operand2;

    setFlag(N_FLAG, readBit(result, 31));
    setFlag(Z_FLAG, result == 0);

    *activeRegs[Rdm] = result;
    fixupIfTargetingPC(Rdm);

    cycles numCycles = 1;
    // On ARM7, the number of cycles depends on operand2.
    if (!arm9) {
        if (operand2 & 0xFF000000) {
            numCycles = 4;
        } else if (operand2 & 0x00FF0000) {
            numCycles = 3;
        } else if (operand2 & 0x0000FF00) {
            numCycles = 2;
        } else {
            numCycles = 1;
        }
    }
    return numCycles;
}
// ==================================================================================================
// BIC
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/BIC--register-?lang=en
// ==================================================================================================
cycles ARM::THUMB_BIC_REG(uint32_t instruct) {
    LogDebug("Executing BIC");
    uint32_t Rdn = readBits(instruct, 0, 2);
    uint32_t Rm = readBits(instruct, 3, 5);
    uint32_t result = *activeRegs[Rdn] & ~(*activeRegs[Rm]);

    setFlag(N_FLAG, readBit(result, 31));
    setFlag(Z_FLAG, result == 0);
    // Carry is unchanged.

    *activeRegs[Rdn] = result;
    fixupIfTargetingPC(Rdn);

    return 1;  // One I cycle.
}
// ==================================================================================================
// MVN
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/MVN--register-?lang=en
// ==================================================================================================
cycles ARM::THUMB_MVN_REG(uint32_t instruct) {
    LogDebug("Executing NVM");
    uint32_t Rd = readBits(instruct, 0, 2);
    uint32_t Rm = readBits(instruct, 3, 5);
    uint32_t result = ~(*activeRegs[Rm]);

    setFlag(N_FLAG, readBit(result, 31));
    setFlag(Z_FLAG, result == 0);
    // Carry is unchanged.

    *activeRegs[Rd] = result;
    fixupIfTargetingPC(Rd);

    return 1;  // One I cycle.
}
// ==================================================================================================
// ADR
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/ADR?lang=en
// ==================================================================================================
cycles ARM::THUMB_ADR(uint32_t instruct) {
    LogDebug("Executing ADR");
    uint32_t Rd = readBits(instruct, 8, 10);
    uint32_t imm8 = readBits(instruct, 0, 7);
    uint32_t result = pc() + (imm8 << 2);
    *activeRegs[Rd] = result;
    fixupIfTargetingPC(Rd);
    return 1;  // One I cycle.
}
// ==================================================================================================
// MSR
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/MSR--register-?lang=en
// https://developer.arm.com/documentation/ddi0406/cb/System-Level-Architecture/System-Instructions/Alphabetical-list-of-instructions/MSR--register-?lang=en
// ==================================================================================================
cycles ARM::THUMB_MSR(uint32_t instruct) {
    if (readBit(instruct, 5)) return THUMB_UNDEFINED_INST(instruct);
    if (readBits(instruct, 21, 26) != 0b011100) return THUMB_UNDEFINED_INST(instruct);
    bool writeSPSR = readBit(instruct, 20);
    uint32_t Rn = readBits(instruct, 16, 19);
    uint32_t value = *activeRegs[Rn];
    uint32_t maskBits = readBits(instruct, 8, 11);

    uint32_t mask = 0;
    if (readBit(maskBits, 0)) mask |= 0x000000FF;
    if (readBit(maskBits, 1)) mask |= 0x0000FF00;
    if (readBit(maskBits, 2)) mask |= 0x00FF0000;
    if (readBit(maskBits, 3)) mask |= 0xFF000000;
    // Handle processor modes.
    if (getProcessorMode() == ProcessorModes::User) {
        mask &= 0xFF000000;  // Only support writing to the flags in user mode.
    }
    if (writeSPSR) {
        if (spsr == nullptr) {
            LogError("Cannot write to SPSR in current processor Mode!");
            return 1;
        }
        *spsr = value & mask;
    } else {
        setCPSR(value & mask);
    }
    return 1;  // Only one I Cycle.
}
// ==================================================================================================
}  // namespace Core
}  // namespace RedPandaDS
