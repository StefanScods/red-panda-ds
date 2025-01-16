/**
 * @file armInterpreter_loadStore.cpp
 * @brief Implements arm load store instuction set
 */
#include "cpu.h"
#include "interconnect.h"

// Control print statements.
#define LOG_LEVEL 2
#include "logger.h"

// TODO!!! Handle T instructions - privilege vs unprivilege access
// TODO!!! Handle special reg case - PC maybe sp

// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/STR--register-?lang=en
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/STR--immediate--ARM-?lang=en
cycles ARM::ARM_STR(uint32_t srcReg, uint32_t baseReg, uint32_t offset, bool pre, bool add,
                    bool wback) {
    uint32_t address = *activeRegs[baseReg];
    uint32_t offset_address = add ? address + offset : address - offset;
    uint32_t targetAddress = pre ? offset_address : address;
    busPayload payload = writeBus(targetAddress, *activeRegs[srcReg], 32);
    if (wback) *activeRegs[baseReg] = offset_address;
    // Add an additional 1N cycle for arm7.
    if (!arm9) payload.numCycles += data_nonSequencial32BitAccessTimings[(targetAddress) >> 24];
    return payload.numCycles;
}

// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/STRT?lang=en
cycles ARM::ARM_STRT(uint32_t srcReg, uint32_t baseReg, uint32_t offset, bool add) {
    LogWarning("Memory access privilege currently unchecked.");
    return ARM_STR(srcReg, baseReg, offset, false, add, true);
}

// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/STRB--immediate--ARM-?lang=en
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/STRB--register-?lang=en
cycles ARM::ARM_STRB(uint32_t srcReg, uint32_t baseReg, uint32_t offset, bool pre, bool add,
                     bool wback) {
    uint32_t address = *activeRegs[baseReg];
    uint32_t offset_address = add ? address + offset : address - offset;
    uint32_t targetAddress = pre ? offset_address : address;
    busPayload payload = writeBus(targetAddress, *activeRegs[srcReg], 8);
    if (wback) *activeRegs[baseReg] = offset_address;
    // Add an additional 1N cycle for arm7.
    if (!arm9) payload.numCycles += data_nonSequencial32BitAccessTimings[(targetAddress) >> 24];
    return payload.numCycles;
}

// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/STRBT?lang=en
cycles ARM::ARM_STRBT(uint32_t srcReg, uint32_t baseReg, uint32_t offset, bool add) {
    LogWarning("Memory access privilege currently unchecked.");
    return ARM_STRB(srcReg, baseReg, offset, false, add, true);
}

// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/LDR--literal-?lang=en
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/LDR--immediate--ARM-?lang=en
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/LDR--register--ARM-?lang=en
cycles ARM::ARM_LDR(uint32_t desReg, uint32_t baseReg, uint32_t offset, bool pre, bool add,
                    bool wback) {
    uint32_t address = *activeRegs[baseReg];
    uint32_t offset_address = add ? address + offset : address - offset;
    uint32_t targetAddress = pre ? offset_address : address;
    busPayload payload = readBus(targetAddress, 32);
    if (wback) *activeRegs[baseReg] = offset_address;
    // Add an additional 1N + 1I cycles
    payload.numCycles += data_nonSequencial32BitAccessTimings[(targetAddress) >> 24] + 1;
    *activeRegs[desReg] = payload.data;
    return payload.numCycles;
}

// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/LDRT?lang=en
cycles ARM::ARM_LDRT(uint32_t srcReg, uint32_t baseReg, uint32_t offset, bool add) {
    LogWarning("Memory access privilege currently unchecked.");
    return ARM_LDR(srcReg, baseReg, offset, false, add, true);
}

// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/LDRB--immediate--ARM-?lang=en
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/LDRB--literal-?lang=en
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/LDRB--register-?lang=en
cycles ARM::ARM_LDRB(uint32_t desReg, uint32_t baseReg, uint32_t offset, bool pre, bool add,
                     bool wback) {
    uint32_t address = *activeRegs[baseReg];
    uint32_t offset_address = add ? address + offset : address - offset;
    uint32_t targetAddress = pre ? offset_address : address;
    busPayload payload = readBus(targetAddress, 8);
    if (wback) *activeRegs[baseReg] = offset_address;
    // Add an additional 1N + 1I cycles
    payload.numCycles += data_nonSequencial32BitAccessTimings[(targetAddress) >> 24] + 1;
    *activeRegs[desReg] = payload.data;
    return payload.numCycles;
}

// hhttps://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/LDRBT?lang=en
cycles ARM::ARM_LDRBT(uint32_t srcReg, uint32_t baseReg, uint32_t offset, bool add) {
    LogWarning("Memory access privilege currently unchecked.");
    return ARM_LDRB(srcReg, baseReg, offset, false, add, true);
}
