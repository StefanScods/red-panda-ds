/**
 * @file armInterpreter_loadStore.cpp
 * @brief Implements arm load store instruction set
 */
#include "cpu.h"
#include "interconnect.h"

// Control print statements.
#define LOG_LEVEL 2
#include "logger.h"

// TODO!!! Handle T instructions - privilege vs unprivilege access
// TODO!!! Handle special reg case - maybe sp

// ==================================================================================================
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/ARM-Instruction-Set-Encoding/Load-store-word-and-unsigned-byte?lang=en
cycles ARM::loadStoreDecodeAndExecute(uint32_t instruct, uint8_t cond) {
    // Extract useful parts of the instruction in order to decode.
    // P - Pre/Post (0=post; add offset after transfer, 1=pre; before trans.)
    bool P = readBit(instruct, 24);
    // U - Up/Down Bit (0=down; subtract offset from base, 1=up; add to base)
    bool U = readBit(instruct, 23);
    /**
     * When above Bit 24 P=0 (Post-indexing, write-back is ALWAYS enabled):
     *   21     T - Memory Management (0=Normal, 1=Force non-privileged access)
     * When above Bit 24 P=1 (Pre-indexing, write-back is optional):
     *   21     W - Write-back bit (0=no write-back, 1=write address into base)
     */
    bool bit21 = readBit(instruct, 21);
    // 1st Operand Register.
    uint8_t Rn = readBits(instruct, 16, 19);
    // Destination / target Register.
    uint8_t Rd = readBits(instruct, 12, 15);
    uint8_t op = readBits(instruct, 20, 24);
    // I - Immediate Offset Flag (0=Immediate, 1=Shifted Register)
    bool I = readBit(instruct, 25);
    // Get the offset.
    uint32_t offset;
    if (I) {  // 1=Shifted Register
        // Shift type.
        uint8_t type = readBits(instruct, 5, 6);
        // Offset Register.
        uint8_t Rm = readBits(instruct, 0, 3);
        assert(Rm != 15);
        // Shift amount.
        uint8_t shiftAmount = readBits(instruct, 7, 11);
        // Calculate the offset via a shift operation.
        decodeShiftResult shiftOp = ARMDecodeImmShift(type, shiftAmount);
        offset = ARMShift(*activeRegs[Rm], shiftOp, readBit(cpsr, C_FLAG)).data_u32;
    } else {  // 0=Immediate.
        offset = readBits(instruct, 0, 11);
    }
    switch (op) {
        // STR (immediate, register)
        case 0b00000:
        case 0b01000:
        case 0b10000:
        case 0b10010:
        case 0b11000:
        case 0b11010:
            return ARM_STR(Rd, Rn, offset, P, U, (!P || bit21));
        // STR - T (immediate, register)
        case 0b00010:
        case 0b01010:
            return ARM_STRT(Rd, Rn, offset, U);
        // LDR (immediate, register)
        case 0b00001:
        case 0b01001:
        case 0b10001:
        case 0b10011:
        case 0b11001:
        case 0b11011:
            return ARM_LDR(Rd, Rn, offset, P, U, (!P || bit21));
        // LDR - T (immediate, register)
        case 0b00011:
        case 0b01011:
            return ARM_LDRT(Rd, Rn, offset, U);
        // STR - Byte (immediate, register)
        case 0b00100:
        case 0b01100:
        case 0b10100:
        case 0b10110:
        case 0b11100:
        case 0b11110:
            return ARM_STRB(Rd, Rn, offset, P, U, (!P || bit21));
        // STR - Byte - T (immediate, register)
        case 0b00110:
        case 0b01110:
            return ARM_STRBT(Rd, Rn, offset, U);
        // LDR - Byte (immediate, register)
        case 0b00101:
        case 0b01101:
        case 0b10101:
        case 0b10111:
        case 0b11101:
        case 0b11111:
            return ARM_LDRB(Rd, Rn, offset, P, U, (!P || bit21));
        // LDR - Byte - T (immediate, register)
        case 0b00111:
        case 0b01111:
            return ARM_LDRBT(Rd, Rn, offset, U);
    }

    return ARM_UNDEFINED_INST(instruct);
}
// ==================================================================================================
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/STR--register-?lang=en
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/STR--immediate--ARM-?lang=en
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
cycles ARM::ARM_STRT(uint32_t srcReg, uint32_t baseReg, uint32_t offset, bool add) {
    LogDebug("Executing STRT");
    LogWarning("Memory access privilege currently unchecked.");
    return ARM_STR(srcReg, baseReg, offset, false, add, true);
}
// ==================================================================================================
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/STRB--immediate--ARM-?lang=en
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/STRB--register-?lang=en
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
    if (!arm9) payload.numCycles += data_nonSequencial32BitAccessTimings[(targetAddress) >> 24];
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
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/LDR--literal-?lang=en
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/LDR--immediate--ARM-?lang=en
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/LDR--register--ARM-?lang=en
cycles ARM::ARM_LDR(uint32_t desReg, uint32_t baseReg, uint32_t offset, bool pre, bool add,
                    bool wback) {
    LogDebug("Executing LDR");
    uint32_t address = *activeRegs[baseReg];
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
cycles ARM::ARM_LDRT(uint32_t srcReg, uint32_t baseReg, uint32_t offset, bool add) {
    LogDebug("Executing LDRT");
    LogWarning("Memory access privilege currently unchecked.");
    return ARM_LDR(srcReg, baseReg, offset, false, add, true);
}
// ==================================================================================================
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/LDRB--immediate--ARM-?lang=en
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/LDRB--literal-?lang=en
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/LDRB--register-?lang=en
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
    payload.numCycles += data_nonSequencial32BitAccessTimings[(targetAddress) >> 24] + 1;
    *activeRegs[desReg] = payload.data;
    fixupIfTargetingPC(desReg);
    return payload.numCycles;
}
// ==================================================================================================
// hhttps://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/LDRBT?lang=en
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
    uint32_t baseAddress = *activeRegs[baseReg];
    uint32_t numStores = 0;
    cycles totalCycles = 0;
    // Align the base address to 4.
    uint32_t targetAddress = (baseAddress & ~(0b11));
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
    return 1;
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
    uint32_t baseAddress = *activeRegs[baseReg];
    uint32_t numStores = 0;
    cycles totalCycles = 0;
    // Align the base address to 4.
    uint32_t targetAddress = (baseAddress & ~(0b11));
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
    return 1;
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
    uint32_t baseAddress = *activeRegs[baseReg];
    uint32_t numLoads = 0;
    cycles totalCycles = 0;
    // Align the base address to 4.
    uint32_t targetAddress = (baseAddress & ~(0b11));
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
    return 1;
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
    uint32_t baseAddress = *activeRegs[baseReg];
    uint32_t numLoads = 0;
    cycles totalCycles = 0;
    // Align the base address to 4.
    uint32_t targetAddress = (baseAddress & ~(0b11));
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
    return 1;
}
// ==================================================================================================