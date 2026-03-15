/**
 * @file armInterpreter/thumb_loadStore.cpp
 * @brief Implements THUMB mode instuction set for instructions in the load and store category.
 */
#include "cpu.h"
#include "interconnect.h"

// Control print statements.
#define LOG_LEVEL 2
#include "logger.h"

namespace RedPandaDS {
namespace Core {

// ==================================================================================================
// STR
// ==================================================================================================
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/STR--register-?lang=en
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/STR--immediate--Thumb-?lang=en
cycles ARM::THUMB_STR(uint32_t srcReg, uint32_t baseReg, uint32_t offset) {
    LogDebug("Executing STR");
    uint32_t address = *activeRegs[baseReg];
    uint32_t targetAddress = address + offset;
    busPayload payload = writeBus(targetAddress, *activeRegs[srcReg], 32);
    // Add an additional 1N cycle for arm7.
    if (!arm9) payload.numCycles += data_nonSequencial32BitAccessTimings[(targetAddress) >> 24];
    return payload.numCycles;
}
// ==================================================================================================
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/STRH--register-?lang=en
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/STRH--immediate--Thumb-?lang=en
cycles ARM::THUMB_STRH(uint32_t srcReg, uint32_t baseReg, uint32_t offset) {
    LogDebug("Executing STRH");
    uint32_t address = *activeRegs[baseReg];
    uint32_t targetAddress = address + offset;
    busPayload payload = writeBus(targetAddress, *activeRegs[srcReg], 16);
    // Add an additional 1N cycle for arm7.
    if (!arm9) payload.numCycles += data_nonSequencial16BitAccessTimings[(targetAddress) >> 24];
    return payload.numCycles;
}
// ==================================================================================================
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/STRB--register-?lang=en
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/STRB--immediate--Thumb-?lang=en
cycles ARM::THUMB_STRB(uint32_t srcReg, uint32_t baseReg, uint32_t offset) {
    LogDebug("Executing STRB");
    uint32_t address = *activeRegs[baseReg];
    uint32_t targetAddress = address + offset;
    busPayload payload = writeBus(targetAddress, *activeRegs[srcReg], 8);
    // Add an additional 1N cycle for arm7.
    if (!arm9) payload.numCycles += data_nonSequencial16BitAccessTimings[(targetAddress) >> 24];
    return payload.numCycles;
}
// ==================================================================================================
// LDR
// ==================================================================================================
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/LDR--register--Thumb-?lang=en
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/LDR--immediate--Thumb-?lang=en
cycles ARM::THUMB_LDR(uint32_t desReg, uint32_t baseReg, uint32_t offset) {
    LogDebug("Executing LDR");
    uint32_t address = *activeRegs[baseReg];
    uint32_t targetAddress = address + offset;
    busPayload payload = readBus(targetAddress, 32);
    *activeRegs[desReg] = payload.data;
    fixupIfTargetingPC(desReg);
    // Add an additional 1N + 1I cycles
    payload.numCycles += data_nonSequencial32BitAccessTimings[(targetAddress) >> 24] + 1;
    return payload.numCycles;
}
// ==================================================================================================
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/LDRH--register-?lang=en
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/LDRH--immediate--Thumb-?lang=en
cycles ARM::THUMB_LDRH(uint32_t desReg, uint32_t baseReg, uint32_t offset) {
    LogDebug("Executing LDRH");
    uint32_t address = *activeRegs[baseReg];
    uint32_t targetAddress = address + offset;
    busPayload payload = readBus(targetAddress, 16);
    *activeRegs[desReg] = payload.data;
    fixupIfTargetingPC(desReg);
    // Add an additional 1N + 1I cycles
    payload.numCycles += data_nonSequencial16BitAccessTimings[(targetAddress) >> 24] + 1;
    return payload.numCycles;
}
// ==================================================================================================
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/LDRB--register-?lang=en
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/LDRB--immediate--Thumb-?lang=en
cycles ARM::THUMB_LDRB(uint32_t desReg, uint32_t baseReg, uint32_t offset) {
    LogDebug("Executing LDRB");
    uint32_t address = *activeRegs[baseReg];
    uint32_t targetAddress = address + offset;
    busPayload payload = readBus(targetAddress, 8);
    *activeRegs[desReg] = payload.data;
    fixupIfTargetingPC(desReg);
    // Add an additional 1N + 1I cycles
    payload.numCycles += data_nonSequencial16BitAccessTimings[(targetAddress) >> 24] + 1;
    return payload.numCycles;
}
// ==================================================================================================
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/LDRSH--register-?lang=en
cycles ARM::THUMB_LDRSH(uint32_t desReg, uint32_t baseReg, uint32_t offset) {
    LogDebug("Executing LDRSH");
    uint32_t address = *activeRegs[baseReg];
    uint32_t targetAddress = address + offset;
    busPayload payload = readBus(targetAddress, 16);
    uint32_t data = payload.data | (readBit(payload.data, 15) ? 0xFFFF0000 : 0x0);
    *activeRegs[desReg] = data;
    fixupIfTargetingPC(desReg);
    // Add an additional 1N + 1I cycles
    payload.numCycles += data_nonSequencial16BitAccessTimings[(targetAddress) >> 24] + 1;
    return payload.numCycles;
}
// ==================================================================================================
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/LDRSB--register-?lang=en
cycles ARM::THUMB_LDRSB(uint32_t desReg, uint32_t baseReg, uint32_t offset) {
    LogDebug("Executing LDRB");
    uint32_t address = *activeRegs[baseReg];
    uint32_t targetAddress = address + offset;
    busPayload payload = readBus(targetAddress, 8);
    uint32_t data = payload.data | (readBit(payload.data, 7) ? 0xFFFFFF00 : 0x0);
    *activeRegs[desReg] = data;
    fixupIfTargetingPC(desReg);
    // Add an additional 1N + 1I cycles
    payload.numCycles += data_nonSequencial16BitAccessTimings[(targetAddress) >> 24] + 1;
    return payload.numCycles;
}
// ==================================================================================================
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/LDR--literal-?lang=en
cycles ARM::THUMB_LDR_LIT(uint32_t instruct) {
    uint32_t imm32 = readBits(instruct, 0, 7) << 2;
    uint32_t desReg = readBits(instruct, 8, 10);
    return THUMB_LDR(desReg, PC_REGISTER_NUM, imm32);
}
// ==================================================================================================
// STM
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/STM--STMIA--STMEA-?lang=en
// ==================================================================================================
cycles ARM::THUMB_STM(uint32_t instruct) {
    uint8_t baseReg = readBits(instruct, 8, 10);
    uint32_t registerList = readBits(instruct, 0, 7);
    LogDebug("Executing STM with base R" << baseReg);
    // Align the base address to 4.
    uint32_t baseAddress = *activeRegs[baseReg] & ~(0b11);
    uint32_t numStores = 0;
    cycles totalCycles = 0;
    uint32_t targetAddress = baseAddress;
    for (int i = 0; i <= 7; i++) {
        bool shouldStore = registerList & (0b1 << i);
        if (!shouldStore) continue;
        // Store to the target address.
        LogDebug("Storing R" << i << " to memory address " << PrintHex(targetAddress));
        busPayload payload = writeBus(targetAddress, *activeRegs[i], 32);
        totalCycles += payload.numCycles;
        // Increment the store address.
        targetAddress += 4;
        numStores++;
    }
    // Write back the new address.
    *activeRegs[baseReg] = baseAddress + (4 * numStores);
    fixupIfTargetingPC(baseReg);
    // Add an additional 1N cycle for arm7.
    if (!arm9) totalCycles += data_nonSequencial32BitAccessTimings[(baseAddress) >> 24];
    return totalCycles;
}
// ==================================================================================================
// LDM
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/LDM-LDMIA-LDMFD--Thumb-?lang=en
// ==================================================================================================
cycles ARM::THUMB_LDM(uint32_t instruct) {
    uint8_t baseReg = readBits(instruct, 8, 10);
    uint32_t registerList = readBits(instruct, 0, 7);
    LogDebug("Executing LDM with base R" << baseReg);
    // Align the base address to 4.
    uint32_t baseAddress = *activeRegs[baseReg] & ~(0b11);
    uint32_t numLoads = 0;
    cycles totalCycles = 0;
    uint32_t targetAddress = baseAddress;
    for (int i = 0; i <= 7; i++) {
        bool shouldLoad = registerList & (0b1 << i);
        if (!shouldLoad) continue;
        // Load from the target address.
        LogDebug("Loading R" << i << " from memory address " << PrintHex(targetAddress));
        busPayload payload = readBus(targetAddress, 32);
        totalCycles += payload.numCycles;
        *activeRegs[i] = payload.data;
        fixupIfTargetingPC(i);
        // Increment the load address.
        targetAddress += 4;
        numLoads++;
    }
    if (!readBit(registerList, baseReg)) {
        // Write back the new address.
        *activeRegs[baseReg] = baseAddress + (4 * numLoads);
        fixupIfTargetingPC(baseReg);
    }
    // Add an additional 1N cycle for arm7.
    if (!arm9) totalCycles += data_nonSequencial32BitAccessTimings[(baseAddress) >> 24];
    return totalCycles;
}
// ==================================================================================================
// PUSH
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/PUSH?lang=en
// ==================================================================================================
cycles ARM::THUMB_PUSH(uint32_t instruct) {
    LogDebug("Executing PUSH");
    uint8_t m = readBit(instruct, 8);
    uint32_t registerList = readBits(instruct, 0, 7);
    writeBit(registerList, m, LR_REGISTER_NUM);
    // Align the base address to 4.
    uint32_t baseAddress = *activeRegs[SP_REGISTER_NUM] & ~(0b11);
    uint32_t numStores = 0;
    cycles totalCycles = 0;
    uint32_t targetAddress = baseAddress - 4;
    for (int i = LR_REGISTER_NUM; i >= 0; i--) {
        bool shouldStore = registerList & (0b1 << i);
        if (!shouldStore) continue;
        // Store to the target address.
        LogDebug("Storing R" << i << " to memory address " << PrintHex(targetAddress));
        busPayload payload = writeBus(targetAddress, *activeRegs[i], 32);
        totalCycles += payload.numCycles;
        // Decrement the store address.
        targetAddress -= 4;
        numStores++;
    }
    // Write back the new address.
    *activeRegs[SP_REGISTER_NUM] = baseAddress - (4 * numStores);
    // Add an additional 1N cycle for arm7.
    if (!arm9) totalCycles += data_nonSequencial32BitAccessTimings[(baseAddress) >> 24];
    return totalCycles;
}
// ==================================================================================================
// POP
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/POP--Thumb-?lang=en
// ==================================================================================================
cycles ARM::THUMB_POP(uint32_t instruct) {
    LogDebug("Executing POP");
    uint8_t p = readBit(instruct, 8);
    uint32_t registerList = readBits(instruct, 0, 7);
    writeBit(registerList, p, PC_REGISTER_NUM);
    // Align the base address to 4.
    uint32_t baseAddress = *activeRegs[SP_REGISTER_NUM] & ~(0b11);
    uint32_t numLoads = 0;
    cycles totalCycles = 0;
    uint32_t targetAddress = baseAddress;
    for (int i = 0; i <= PC_REGISTER_NUM; i++) {
        bool shouldLoad = registerList & (0b1 << i);
        if (!shouldLoad) continue;
        // Load from the target address.
        LogDebug("Loading R" << i << " from memory address " << PrintHex(targetAddress));
        busPayload payload = readBus(targetAddress, 32);
        totalCycles += payload.numCycles;
        *activeRegs[i] = payload.data;
        fixupIfTargetingPC(i);
        // Increment the load address.
        targetAddress += 4;
        numLoads++;
    }
    // Write back the new address.
    *activeRegs[SP_REGISTER_NUM] = baseAddress + (4 * numLoads);
    // Add an additional 1N cycle for arm7.
    if (!arm9) totalCycles += data_nonSequencial32BitAccessTimings[(baseAddress) >> 24];
    return totalCycles;
}
// ==================================================================================================

}  // namespace Core
}  // namespace RedPandaDS
