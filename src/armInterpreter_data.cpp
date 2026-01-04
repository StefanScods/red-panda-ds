/**
 * @file armInterpreter_data.cpp
 * @brief Implements arm data processing instuction set
 */
#include "cpu.h"
#include "interconnect.h"

// Control print statements.
#define LOG_LEVEL 2
#include "logger.h"

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
    return 1;  // An instruction prefetch occurs at the same time as the data operation.
}

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
    return 1;  // An instruction prefetch occurs at the same time as the data operation.
}

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
    return 1;  // An instruction prefetch occurs at the same time as the data operation.
}

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
    return 1;  // An instruction prefetch occurs at the same time as the data operation.
}

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
    return 1;  // An instruction prefetch occurs at the same time as the data operation.
}

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
    return 1;  // An instruction prefetch occurs at the same time as the data operation.
}

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
    return 1;  // An instruction prefetch occurs at the same time as the data operation.
}

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
    return 1;  // An instruction prefetch occurs at the same time as the data operation.
}

cycles ARM::ARM_TST(uint32_t opp1, uint32_t opp2, bool carry) {
    LogDebug("Executing TST");
    LogDebug("Updating CPU flags...");
    uint32_t result = opp1 & opp2;
    writeBit(cpsr, readBit(result, 31), N_FLAG);
    writeBit(cpsr, result == 0, Z_FLAG);
    writeBit(cpsr, carry, C_FLAG);
    return 1;  // An instruction prefetch occurs at the same time as the data operation.
}

cycles ARM::ARM_TEQ(uint32_t opp1, uint32_t opp2, bool carry) {
    LogDebug("Executing TEQ");
    LogDebug("Updating CPU flags...");
    uint32_t result = opp1 ^ opp2;
    writeBit(cpsr, readBit(result, 31), N_FLAG);
    writeBit(cpsr, result == 0, Z_FLAG);
    writeBit(cpsr, carry, C_FLAG);
    return 1;  // An instruction prefetch occurs at the same time as the data operation.
}

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
    return 1;  // An instruction prefetch occurs at the same time as the data operation.
}

cycles ARM::ARM_MOV(uint32_t desReg, uint32_t srcValue, bool carry, bool setFlags) {
    LogDebug("Executing MOV");
    if (setFlags) {
        LogDebug("Updating CPU flags...");
        writeBit(cpsr, readBit(srcValue, 31), N_FLAG);
        writeBit(cpsr, srcValue == 0, Z_FLAG);
        writeBit(cpsr, carry, C_FLAG);
    }
    *activeRegs[desReg] = srcValue;
    return 1;  // An instruction prefetch occurs at the same time as the data operation.
}

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
    return 1;  // An instruction prefetch occurs at the same time as the data operation.
}

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
    return 1;  // An instruction prefetch occurs at the same time as the data operation.
}
