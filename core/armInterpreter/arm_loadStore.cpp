/**
 * @file armInterpreter/arm_loadStore.cpp
 * @brief Implements ARM mode instuction set for instructions in the load and store category.
 */
#include "cpu.h"
#include "interconnect.h"

// Control print statements.
#define LOG_LEVEL 2
#include "logger.h"

namespace RedPandaDS {
namespace Core {

// TODO!!! Handle T instructions - privilege vs unprivilege access

// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/ARM-Instruction-Set-Encoding/Load-store-word-and-unsigned-byte?lang=en

// ==================================================================================================
// STR
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/STR--register-?lang=en
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/STR--immediate--ARM-?lang=en
// ==================================================================================================
cycles ARM::ARM_STR(uint32_t srcReg, uint32_t baseReg, uint32_t offset, bool pre, bool add,
                    bool wback) {
    LogDebug("Executing STR");
    uint32_t address = *activeRegs[baseReg];
    uint32_t offset_address = add ? address + offset : address - offset;
    uint32_t targetAddress = pre ? offset_address : address;
    busPayload payload = writeBus(targetAddress, *activeRegs[srcReg], 32);
    if (wback) {
        *activeRegs[baseReg] = offset_address;
        fixupIfTargetingPC(baseReg);
    }
    // Add an additional 1N cycle for arm7.
    if (!arm9) payload.numCycles += data_nonSequencial32BitAccessTimings[(targetAddress) >> 24];
    return payload.numCycles;
}
// ==================================================================================================
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/STRT?lang=en
// ==================================================================================================
cycles ARM::ARM_STRT(uint32_t srcReg, uint32_t baseReg, uint32_t offset, bool add) {
    LogDebug("Executing STRT");
    LogWarning("Memory access privilege currently unchecked.");
    return ARM_STR(srcReg, baseReg, offset, false, add, true);
}
// ==================================================================================================
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/STRB--immediate--ARM-?lang=en
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/STRB--register-?lang=en
// ==================================================================================================
cycles ARM::ARM_STRB(uint32_t srcReg, uint32_t baseReg, uint32_t offset, bool pre, bool add,
                     bool wback) {
    LogDebug("Executing STRB");
    uint32_t address = *activeRegs[baseReg];
    uint32_t offset_address = add ? address + offset : address - offset;
    uint32_t targetAddress = pre ? offset_address : address;
    busPayload payload = writeBus(targetAddress, *activeRegs[srcReg], 8);
    if (wback) {
        *activeRegs[baseReg] = offset_address;
        fixupIfTargetingPC(baseReg);
    }
    // Add an additional 1N cycle for arm7.
    if (!arm9) payload.numCycles += data_nonSequencial16BitAccessTimings[(targetAddress) >> 24];
    return payload.numCycles;
}
// ==================================================================================================
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/STRBT?lang=en
cycles ARM::ARM_STRBT(uint32_t srcReg, uint32_t baseReg, uint32_t offset, bool add) {
    LogDebug("Executing STRBT");
    LogWarning("Memory access privilege currently unchecked.");
    return ARM_STRB(srcReg, baseReg, offset, false, add, true);
}
// ==================================================================================================
// LDR
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/LDR--literal-?lang=en
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/LDR--immediate--ARM-?lang=en
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/LDR--register--ARM-?lang=en
// ==================================================================================================
cycles ARM::ARM_LDR(uint32_t desReg, uint32_t baseReg, uint32_t offset, bool pre, bool add,
                    bool wback) {
    LogDebug("Executing LDR");
    uint32_t address = *activeRegs[baseReg];
    LogDebug("Base address " << PrintHex(address));
    uint32_t offset_address = add ? address + offset : address - offset;
    uint32_t targetAddress = pre ? offset_address : address;
    busPayload payload = readBus(targetAddress, 32);
    if (wback) {
        *activeRegs[baseReg] = offset_address;
        fixupIfTargetingPC(baseReg);
    }
    // Add an additional 1N + 1I cycles
    payload.numCycles += data_nonSequencial32BitAccessTimings[(targetAddress) >> 24] + 1;
    *activeRegs[desReg] = payload.data;
    fixupIfTargetingPC(desReg);
    return payload.numCycles;
}
// ==================================================================================================
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/LDRT?lang=en
// ==================================================================================================
cycles ARM::ARM_LDRT(uint32_t srcReg, uint32_t baseReg, uint32_t offset, bool add) {
    LogDebug("Executing LDRT");
    LogWarning("Memory access privilege currently unchecked.");
    return ARM_LDR(srcReg, baseReg, offset, false, add, true);
}
// ==================================================================================================
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/LDRB--immediate--ARM-?lang=en
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/LDRB--literal-?lang=en
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/LDRB--register-?lang=en
// ==================================================================================================
cycles ARM::ARM_LDRB(uint32_t desReg, uint32_t baseReg, uint32_t offset, bool pre, bool add,
                     bool wback) {
    LogDebug("Executing LDRB");
    uint32_t address = *activeRegs[baseReg];
    uint32_t offset_address = add ? address + offset : address - offset;
    uint32_t targetAddress = pre ? offset_address : address;
    busPayload payload = readBus(targetAddress, 8);
    if (wback) {
        *activeRegs[baseReg] = offset_address;
        fixupIfTargetingPC(baseReg);
    }
    // Add an additional 1N + 1I cycles
    payload.numCycles += data_nonSequencial16BitAccessTimings[(targetAddress) >> 24] + 1;
    *activeRegs[desReg] = payload.data;
    fixupIfTargetingPC(desReg);
    return payload.numCycles;
}
// ==================================================================================================
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/LDRBT?lang=en
// ==================================================================================================
cycles ARM::ARM_LDRBT(uint32_t srcReg, uint32_t baseReg, uint32_t offset, bool add) {
    LogDebug("Executing LDRBT");
    LogWarning("Memory access privilege currently unchecked.");
    return ARM_LDRB(srcReg, baseReg, offset, false, add, true);
}
// ==================================================================================================
// STM
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/STM--STMIA--STMEA-?lang=en
// https://developer.arm.com/documentation/ddi0406/cb/System-Level-Architecture/System-Instructions/Alphabetical-list-of-instructions/STM--User-registers-?lang=en
// ==================================================================================================
cycles ARM::ARM_STMIA(uint32_t instruct) {
    bool W = readBit(instruct, 21);
    uint32_t Rn = readBits(instruct, 16, 19);
    return ARM_STMI(Rn, instruct, false, W);
}
// ==================================================================================================
cycles ARM::ARM_STMIB(uint32_t instruct) {
    bool W = readBit(instruct, 21);
    uint32_t Rn = readBits(instruct, 16, 19);
    return ARM_STMI(Rn, instruct, true, W);
}
// ==================================================================================================
cycles ARM::ARM_STMI(uint32_t baseReg, uint32_t registerList, bool pre, bool wback) {
    LogDebug("Executing STM with base R" << baseReg);
    // Align the base address to 4.
    uint32_t baseAddress = (*activeRegs[baseReg]) & ~(0b11);
    uint32_t numStores = 0;
    cycles totalCycles = 0;
    uint32_t targetAddress = baseAddress;
    // Acount for inc / dec before.
    if (pre) targetAddress = targetAddress + 4;
    for (int i = 0; i <= PC_REGISTER_NUM; i++) {
        bool shouldStore = registerList & (0b1 << i);
        if (!shouldStore) continue;
        // Store to the target address.
        LogDebug("Storing R" << i << " to memory address " << PrintHex(targetAddress) << "");
        busPayload payload = writeBus(targetAddress, *activeRegs[i], 32);
        totalCycles += payload.numCycles;
        // Increment the store address.
        targetAddress += 4;
        numStores++;
    }
    if (wback) {
        *activeRegs[baseReg] = baseAddress + (4 * numStores);
        fixupIfTargetingPC(baseReg);
    }
    // Add an additional 1N cycle for arm7.
    if (!arm9) totalCycles += data_nonSequencial32BitAccessTimings[(targetAddress) >> 24];
    return totalCycles;
}
// ==================================================================================================
cycles ARM::ARM_STMDA(uint32_t instruct) {
    bool W = readBit(instruct, 21);
    uint32_t Rn = readBits(instruct, 16, 19);
    return ARM_STMD(Rn, instruct, false, W);
}
// ==================================================================================================
cycles ARM::ARM_STMDB(uint32_t instruct) {
    bool W = readBit(instruct, 21);
    uint32_t Rn = readBits(instruct, 16, 19);
    return ARM_STMD(Rn, instruct, true, W);
}
// ==================================================================================================
cycles ARM::ARM_STMD(uint32_t baseReg, uint32_t registerList, bool pre, bool wback) {
    LogDebug("Executing STM with base R" << baseReg);
    // Align the base address to 4.
    uint32_t baseAddress = (*activeRegs[baseReg]) & ~(0b11);
    uint32_t numStores = 0;
    cycles totalCycles = 0;
    uint32_t targetAddress = baseAddress;
    // Acount for inc / dec before.
    if (pre) targetAddress = targetAddress - 4;
    for (int i = PC_REGISTER_NUM; i >= 0; i--) {
        bool shouldStore = registerList & (0b1 << i);
        if (!shouldStore) continue;
        // Store to the target address.
        LogDebug("Storing R" << i << " to memory address " << PrintHex(targetAddress) << "");
        busPayload payload = writeBus(targetAddress, *activeRegs[i], 32);
        totalCycles += payload.numCycles;
        // Decrement the store address.
        targetAddress -= 4;
        numStores++;
    }
    if (wback) {
        *activeRegs[baseReg] = baseAddress - (4 * numStores);
        fixupIfTargetingPC(baseReg);
    }
    // Add an additional 1N cycle for arm7.
    if (!arm9) totalCycles += data_nonSequencial32BitAccessTimings[(targetAddress) >> 24];
    return totalCycles;
}
// ==================================================================================================
// https://developer.arm.com/documentation/ddi0406/cb/System-Level-Architecture/System-Instructions/Alphabetical-list-of-instructions/STM--User-registers-?lang=en
cycles ARM::ARM_STM_USER_REG(uint32_t instruct) {
    uint32_t Rn = readBits(instruct, 16, 19);
    bool wordHigher = readBit(instruct, 24);
    bool increment = readBit(instruct, 23);
    uint32_t numStores = 0;
    cycles totalCycles = 0;
    // Align the base address to 4.
    uint32_t targetAddress = (*activeRegs[Rn] & ~(0b11));
    // Acount for inc / dec before.
    if (wordHigher) targetAddress = increment ? (targetAddress + 4) : (targetAddress - 4);
    // Determine how to loop based increment.
    for (int32_t i = increment ? 0 : PC_REGISTER_NUM; increment ? (i <= PC_REGISTER_NUM) : (i >= 0);
         increment ? i += 1 : i -= 1) {
        bool shouldStore = instruct & (0b1 << i);
        if (!shouldStore) continue;
        // Store to the target address.
        LogDebug("Storing User R" << i << " to memory address " << PrintHex(targetAddress) << "");
        busPayload payload = writeBus(targetAddress, reg[i], 32);
        totalCycles += payload.numCycles;
        targetAddress = increment ? (targetAddress + 4) : (targetAddress - 4);
        numStores++;
    }
    // Add an additional 1N cycle for arm7.
    if (!arm9) totalCycles += data_nonSequencial32BitAccessTimings[(targetAddress) >> 24];
    return totalCycles;
}
// ==================================================================================================
// LDM
// https://developer.arm.com/documentation/ddi0406/cb/System-Level-Architecture/System-Instructions/Alphabetical-list-of-instructions/LDM--User-registers-?lang=en
// ==================================================================================================
cycles ARM::ARM_LDMIA(uint32_t instruct) {
    bool W = readBit(instruct, 21);
    uint32_t Rn = readBits(instruct, 16, 19);
    return ARM_LDMI(Rn, instruct, false, W);
}
// ==================================================================================================
cycles ARM::ARM_LDMIB(uint32_t instruct) {
    bool W = readBit(instruct, 21);
    uint32_t Rn = readBits(instruct, 16, 19);
    return ARM_LDMI(Rn, instruct, true, W);
}
// ==================================================================================================
cycles ARM::ARM_LDMI(uint32_t baseReg, uint32_t registerList, bool pre, bool wback) {
    LogDebug("Executing LDM with base R" << baseReg);
    // Align the base address to 4.
    uint32_t baseAddress = (*activeRegs[baseReg]) & ~(0b11);
    uint32_t numLoads = 0;
    cycles totalCycles = 0;
    uint32_t targetAddress = baseAddress;
    // Acount for inc / dec before.
    if (pre) targetAddress = targetAddress + 4;
    for (int i = 0; i < PC_REGISTER_NUM; i++) {
        bool shouldLoad = registerList & (0b1 << i);
        if (!shouldLoad) continue;
        // Load from the target address.
        LogDebug("Loading R" << i << " from memory address " << PrintHex(targetAddress) << "");
        busPayload payload = readBus(targetAddress, 32);
        *activeRegs[i] = payload.data;
        totalCycles += payload.numCycles;
        // Increment the load address.
        targetAddress += 4;
        numLoads++;
    }
    if (readBit(registerList, PC_REGISTER_NUM)) {
        // Use the loaded address to branch.
        busPayload payload = readBus(targetAddress, 32);
        totalCycles += payload.numCycles;
        branch(payload.data);
        targetAddress += 4;
        numLoads++;
    }
    if (wback) {
        *activeRegs[baseReg] = baseAddress + (4 * numLoads);
        fixupIfTargetingPC(baseReg);
    }
    // Add an additional 1N cycle for arm7.
    if (!arm9) totalCycles += data_nonSequencial32BitAccessTimings[(targetAddress) >> 24];
    return totalCycles;
}
// ==================================================================================================
cycles ARM::ARM_LDMDA(uint32_t instruct) {
    bool W = readBit(instruct, 21);
    uint32_t Rn = readBits(instruct, 16, 19);
    return ARM_LDMD(Rn, instruct, false, W);
}
// ==================================================================================================
cycles ARM::ARM_LDMDB(uint32_t instruct) {
    bool W = readBit(instruct, 21);
    uint32_t Rn = readBits(instruct, 16, 19);
    return ARM_LDMD(Rn, instruct, true, W);
}
// ==================================================================================================
cycles ARM::ARM_LDMD(uint32_t baseReg, uint32_t registerList, bool pre, bool wback) {
    LogDebug("Executing LDM with base R" << baseReg);
    // Align the base address to 4.
    uint32_t baseAddress = (*activeRegs[baseReg]) & ~(0b11);
    uint32_t numLoads = 0;
    cycles totalCycles = 0;
    uint32_t targetAddress = baseAddress;
    // Acount for inc / dec before.
    if (pre) targetAddress = targetAddress - 4;
    if (readBit(registerList, PC_REGISTER_NUM)) {
        // Use the loaded address to branch.
        busPayload payload = readBus(targetAddress, 32);
        totalCycles += payload.numCycles;
        branch(payload.data);
        targetAddress -= 4;
        numLoads++;
    }
    for (int i = PC_REGISTER_NUM - 1; i >= 0; i--) {
        bool shouldLoad = registerList & (0b1 << i);
        if (!shouldLoad) continue;
        // Load from the target address.
        LogDebug("Loading R" << i << " from memory address " << PrintHex(targetAddress) << "");
        busPayload payload = readBus(targetAddress, 32);
        *activeRegs[i] = payload.data;
        totalCycles += payload.numCycles;
        // Decrement the load address.
        targetAddress -= 4;
        numLoads++;
    }
    if (wback) {
        *activeRegs[baseReg] = baseAddress - (4 * numLoads);
        fixupIfTargetingPC(baseReg);
    }
    // Add an additional 1N cycle for arm7.
    if (!arm9) totalCycles += data_nonSequencial32BitAccessTimings[(targetAddress) >> 24];
    return totalCycles;
}
// ==================================================================================================
// https://developer.arm.com/documentation/ddi0406/cb/System-Level-Architecture/System-Instructions/Alphabetical-list-of-instructions/LDM--User-registers-?lang=en
// https://developer.arm.com/documentation/ddi0406/cb/System-Level-Architecture/System-Instructions/Alphabetical-list-of-instructions/LDM--exception-return-?lang=en
cycles ARM::ARM_LDM_USER_REG(uint32_t instruct) {
    uint32_t Rn = readBits(instruct, 16, 19);
    bool wordHigher = readBit(instruct, 24);
    bool increment = readBit(instruct, 23);
    bool writeback = readBit(instruct, 21) && readBit(instruct, 15);
    uint32_t numLoads = 0;
    cycles totalCycles = 0;
    // Align the base address to 4.
    uint32_t baseAddress = (*activeRegs[Rn]) & ~(0b11);
    uint32_t targetAddress = baseAddress;
    // Acount for inc / dec before.
    if (wordHigher) targetAddress = increment ? (targetAddress + 4) : (targetAddress - 4);
    // Determine how to loop based increment.
    for (int32_t i = increment ? 0 : PC_REGISTER_NUM; increment ? (i <= PC_REGISTER_NUM) : (i >= 0);
         increment ? i += 1 : i -= 1) {
        bool shouldLoad = instruct & (0b1 << i);
        if (!shouldLoad) continue;
        // Load from the target address.
        LogDebug("Loading User R" << i << " from memory address " << PrintHex(targetAddress) << "");
        busPayload payload = readBus(targetAddress, 32);
        reg[i] = payload.data;
        totalCycles += payload.numCycles;
        if (i == PC_REGISTER_NUM) {
            // Branch and restore CPSR.
            branch(payload.data);
            assert(spsr != nullptr && "ARM_LDM_USER_REG() called in user mode!");
            setCPSR(*spsr);
        }
        targetAddress = increment ? (targetAddress + 4) : (targetAddress - 4);
        numLoads++;
    }
    if (writeback) {
        *activeRegs[Rn] =
            increment ? (baseAddress + (4 * numLoads)) : (baseAddress - (4 * numLoads));
        fixupIfTargetingPC(Rn);
    }
    // Add an additional 1N cycle for arm7.
    if (!arm9) totalCycles += data_nonSequencial32BitAccessTimings[(targetAddress) >> 24];
    return totalCycles;
}
// ==================================================================================================
// STRH
// ==================================================================================================
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/STRH--register-?lang=en
cycles ARM::ARM_STRH_REG(uint32_t instruct) {
    bool P = readBit(instruct, 24);
    bool U = readBit(instruct, 23);
    bool W = readBit(instruct, 21);
    uint8_t Rn = readBits(instruct, 16, 19);
    uint8_t Rt = readBits(instruct, 12, 15);
    uint8_t Rm = readBits(instruct, 0, 3);
    bool writeback = P == 0 || W;
    return ARM_STRH(Rt, Rn, *activeRegs[Rm], P, U, writeback);
}
// ==================================================================================================
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/STRH--immediate--ARM-?lang=en
cycles ARM::ARM_STRH_IMM(uint32_t instruct) {
    bool P = readBit(instruct, 24);
    bool U = readBit(instruct, 23);
    bool W = readBit(instruct, 21);
    uint8_t Rn = readBits(instruct, 16, 19);
    uint8_t Rt = readBits(instruct, 12, 15);
    uint8_t imm4High = readBits(instruct, 8, 11);
    uint8_t imm4Low = readBits(instruct, 0, 3);
    bool writeback = P == 0 || W;
    return ARM_STRH(Rt, Rn, imm4High << 4 | imm4Low, P, U, writeback);
}
// ==================================================================================================
cycles ARM::ARM_STRH(uint32_t srcReg, uint32_t baseReg, uint32_t offset, bool pre, bool add,
                     bool wback) {
    LogDebug("Executing STRH");
    uint32_t address = *activeRegs[baseReg];
    uint32_t offset_address = add ? address + offset : address - offset;
    uint32_t targetAddress = pre ? offset_address : address;
    busPayload payload = writeBus(targetAddress, *activeRegs[srcReg], 16);
    if (wback) {
        *activeRegs[baseReg] = offset_address;
        fixupIfTargetingPC(baseReg);
    }
    // Add an additional 1N cycle for arm7.
    if (!arm9) payload.numCycles += data_nonSequencial16BitAccessTimings[(targetAddress) >> 24];
    return payload.numCycles;
}
// ==================================================================================================
// STRD
// ==================================================================================================
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/STRD--register-?lang=en
cycles ARM::ARM_STRD_REG(uint32_t instruct) {
    bool P = readBit(instruct, 24);
    bool U = readBit(instruct, 23);
    bool W = readBit(instruct, 21);
    uint8_t Rn = readBits(instruct, 16, 19);
    uint8_t Rt = readBits(instruct, 12, 15);
    uint8_t Rm = readBits(instruct, 0, 3);
    bool writeback = P == 0 || W;
    return ARM_STRD(Rt, Rt + 1, Rn, *activeRegs[Rm], P, U, writeback);
}
// ==================================================================================================
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/STRD--immediate-?lang=en
cycles ARM::ARM_STRD_IMM(uint32_t instruct) {
    bool P = readBit(instruct, 24);
    bool U = readBit(instruct, 23);
    bool W = readBit(instruct, 21);
    uint8_t Rn = readBits(instruct, 16, 19);
    uint8_t Rt = readBits(instruct, 12, 15);
    uint8_t imm4High = readBits(instruct, 8, 11);
    uint8_t imm4Low = readBits(instruct, 0, 3);
    bool writeback = P == 0 || W;
    return ARM_STRD(Rt, Rt + 1, Rn, imm4High << 4 | imm4Low, P, U, writeback);
}
// ==================================================================================================
cycles ARM::ARM_STRD(uint32_t srcReg1, uint32_t srcReg2, uint32_t baseReg, uint32_t offset,
                     bool pre, bool add, bool wback) {
    LogDebug("Executing STRD");
    uint32_t address = *activeRegs[baseReg];
    uint32_t offset_address = add ? address + offset : address - offset;
    uint32_t targetAddress = pre ? offset_address : address;
    busPayload payload1 = writeBus(targetAddress, *activeRegs[srcReg1], 32);
    busPayload payload2 = writeBus(targetAddress + 4, *activeRegs[srcReg2], 32);
    if (wback) {
        *activeRegs[baseReg] = offset_address;
        fixupIfTargetingPC(baseReg);
    }
    cycles totalCycles = payload1.numCycles + payload2.numCycles;
    // Add an additional 1N cycle for arm7.
    if (!arm9) totalCycles += data_nonSequencial32BitAccessTimings[(targetAddress) >> 24];
    return totalCycles;
}
// ==================================================================================================
// LDRH
// ==================================================================================================
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/LDRH--register-?lang=en
cycles ARM::ARM_LDRH_REG(uint32_t instruct) {
    bool P = readBit(instruct, 24);
    bool U = readBit(instruct, 23);
    bool W = readBit(instruct, 21);
    uint8_t Rn = readBits(instruct, 16, 19);
    uint8_t Rt = readBits(instruct, 12, 15);
    uint8_t Rm = readBits(instruct, 0, 3);
    bool writeback = P == 0 || W;
    return ARM_LDRH(Rt, Rn, *activeRegs[Rm], P, U, writeback);
}
// ==================================================================================================
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/LDRH--immediate--ARM-?lang=en
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/LDRH--literal-?lang=en
cycles ARM::ARM_LDRH_IMM(uint32_t instruct) {
    bool P = readBit(instruct, 24);
    bool U = readBit(instruct, 23);
    bool W = readBit(instruct, 21);
    uint8_t Rn = readBits(instruct, 16, 19);
    uint8_t Rt = readBits(instruct, 12, 15);
    uint8_t imm4High = readBits(instruct, 8, 11);
    uint8_t imm4Low = readBits(instruct, 0, 3);
    bool writeback = P == 0 || W;
    return ARM_LDRH(Rt, Rn, imm4High << 4 | imm4Low, P, U, writeback);
}
// ==================================================================================================
cycles ARM::ARM_LDRH(uint32_t desReg, uint32_t baseReg, uint32_t offset, bool pre, bool add,
                     bool wback) {
    LogDebug("Executing LDRH");
    uint32_t address = *activeRegs[baseReg];
    uint32_t offset_address = add ? address + offset : address - offset;
    uint32_t targetAddress = pre ? offset_address : address;
    busPayload payload = readBus(targetAddress, 16);
    if (wback) {
        *activeRegs[baseReg] = offset_address;
        fixupIfTargetingPC(baseReg);
    }
    *activeRegs[desReg] = payload.data;
    fixupIfTargetingPC(desReg);
    // Add an additional 1N cycle for arm7.
    if (!arm9) payload.numCycles += data_nonSequencial16BitAccessTimings[(targetAddress) >> 24];
    return payload.numCycles;
}
// ==================================================================================================
// LDRD
// ==================================================================================================
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/LDRD--register-?lang=en
cycles ARM::ARM_LDRD_REG(uint32_t instruct) {
    bool P = readBit(instruct, 24);
    bool U = readBit(instruct, 23);
    bool W = readBit(instruct, 21);
    uint8_t Rn = readBits(instruct, 16, 19);
    uint8_t Rt = readBits(instruct, 12, 15);
    uint8_t Rm = readBits(instruct, 0, 3);
    bool writeback = P == 0 || W;
    return ARM_LDRD(Rt, Rt + 1, Rn, *activeRegs[Rm], P, U, writeback);
}
// ==================================================================================================
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/LDRD--immediate-?lang=en
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/LDRD--literal-?lang=en
cycles ARM::ARM_LDRD_IMM(uint32_t instruct) {
    bool P = readBit(instruct, 24);
    bool U = readBit(instruct, 23);
    bool W = readBit(instruct, 21);
    uint8_t Rn = readBits(instruct, 16, 19);
    uint8_t Rt = readBits(instruct, 12, 15);
    uint8_t imm4High = readBits(instruct, 8, 11);
    uint8_t imm4Low = readBits(instruct, 0, 3);
    bool writeback = P == 0 || W;
    return ARM_LDRD(Rt, Rt + 1, Rn, imm4High << 4 | imm4Low, P, U, writeback);
}
// ==================================================================================================
cycles ARM::ARM_LDRD(uint32_t desReg1, uint32_t desReg2, uint32_t baseReg, uint32_t offset,
                     bool pre, bool add, bool wback) {
    LogDebug("Executing LDRD");
    uint32_t address = *activeRegs[baseReg];
    uint32_t offset_address = add ? address + offset : address - offset;
    uint32_t targetAddress = pre ? offset_address : address;
    busPayload payload1 = readBus(targetAddress, 32);
    busPayload payload2 = readBus(targetAddress + 4, 32);
    if (wback) {
        *activeRegs[baseReg] = offset_address;
        fixupIfTargetingPC(baseReg);
    }
    *activeRegs[desReg1] = payload1.data;
    fixupIfTargetingPC(desReg1);
    *activeRegs[desReg2] = payload2.data;
    fixupIfTargetingPC(desReg2);
    cycles totalCycles = payload1.numCycles + payload2.numCycles;
    // Add an additional 1N cycle for arm7.
    if (!arm9) totalCycles += data_nonSequencial32BitAccessTimings[(targetAddress) >> 24];
    return totalCycles;
}
// ==================================================================================================
// LDRSB
// ==================================================================================================
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/LDRSB--register-?lang=en
cycles ARM::ARM_LDRSB_REG(uint32_t instruct) {
    bool P = readBit(instruct, 24);
    bool U = readBit(instruct, 23);
    bool W = readBit(instruct, 21);
    uint8_t Rn = readBits(instruct, 16, 19);
    uint8_t Rt = readBits(instruct, 12, 15);
    uint8_t Rm = readBits(instruct, 0, 3);
    bool writeback = P == 0 || W;
    return ARM_LDRSB(Rt, Rn, *activeRegs[Rm], P, U, writeback);
}
// ==================================================================================================
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/LDRSB--immediate-?lang=en
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/LDRSB--literal-?lang=en
cycles ARM::ARM_LDRSB_IMM(uint32_t instruct) {
    bool P = readBit(instruct, 24);
    bool U = readBit(instruct, 23);
    bool W = readBit(instruct, 21);
    uint8_t Rn = readBits(instruct, 16, 19);
    uint8_t Rt = readBits(instruct, 12, 15);
    uint8_t imm4High = readBits(instruct, 8, 11);
    uint8_t imm4Low = readBits(instruct, 0, 3);
    bool writeback = P == 0 || W;
    return ARM_LDRSB(Rt, Rn, imm4High << 4 | imm4Low, P, U, writeback);
}
// ==================================================================================================
cycles ARM::ARM_LDRSB(uint32_t desReg, uint32_t baseReg, uint32_t offset, bool pre, bool add,
                      bool wback) {
    LogDebug("Executing LDRSB");
    uint32_t address = *activeRegs[baseReg];
    uint32_t offset_address = add ? address + offset : address - offset;
    uint32_t targetAddress = pre ? offset_address : address;
    busPayload payload = readBus(targetAddress, 8);
    if (wback) {
        *activeRegs[baseReg] = offset_address;
        fixupIfTargetingPC(baseReg);
    }
    uint32_t data = payload.data | (readBit(payload.data, 7) ? 0xFFFFFF00 : 0x0);
    *activeRegs[desReg] = data;
    fixupIfTargetingPC(desReg);
    // Add an additional 1N cycle for arm7.
    if (!arm9) payload.numCycles += data_nonSequencial16BitAccessTimings[(targetAddress) >> 24];
    return payload.numCycles;
}
// ==================================================================================================
// LDRSH
// ==================================================================================================
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/LDRSH--register-?lang=en
cycles ARM::ARM_LDRSH_REG(uint32_t instruct) {
    bool P = readBit(instruct, 24);
    bool U = readBit(instruct, 23);
    bool W = readBit(instruct, 21);
    uint8_t Rn = readBits(instruct, 16, 19);
    uint8_t Rt = readBits(instruct, 12, 15);
    uint8_t Rm = readBits(instruct, 0, 3);
    bool writeback = P == 0 || W;
    return ARM_LDRSH(Rt, Rn, *activeRegs[Rm], P, U, writeback);
}
// ==================================================================================================
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/LDRSH--immediate-?lang=en
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/LDRSH--literal-?lang=en
cycles ARM::ARM_LDRSH_IMM(uint32_t instruct) {
    bool P = readBit(instruct, 24);
    bool U = readBit(instruct, 23);
    bool W = readBit(instruct, 21);
    uint8_t Rn = readBits(instruct, 16, 19);
    uint8_t Rt = readBits(instruct, 12, 15);
    uint8_t imm4High = readBits(instruct, 8, 11);
    uint8_t imm4Low = readBits(instruct, 0, 3);
    bool writeback = P == 0 || W;
    return ARM_LDRSH(Rt, Rn, imm4High << 4 | imm4Low, P, U, writeback);
}
// ==================================================================================================
cycles ARM::ARM_LDRSH(uint32_t desReg, uint32_t baseReg, uint32_t offset, bool pre, bool add,
                      bool wback) {
    LogDebug("Executing LDRSB");
    uint32_t address = *activeRegs[baseReg];
    uint32_t offset_address = add ? address + offset : address - offset;
    uint32_t targetAddress = pre ? offset_address : address;
    busPayload payload = readBus(targetAddress, 16);
    if (wback) {
        *activeRegs[baseReg] = offset_address;
        fixupIfTargetingPC(baseReg);
    }
    uint32_t data = payload.data | (readBit(payload.data, 15) ? 0xFFFF0000 : 0x0);
    *activeRegs[desReg] = data;
    fixupIfTargetingPC(desReg);
    // Add an additional 1N cycle for arm7.
    if (!arm9) payload.numCycles += data_nonSequencial16BitAccessTimings[(targetAddress) >> 24];
    return payload.numCycles;
}
// ==================================================================================================

}  // namespace Core
}  // namespace RedPandaDS
