/**
 * @file armInterpreter/thumb_branch.cpp
 * @brief Implements THUMB mode instuction set for instructions in the branch category.
 */
#include "cpu.h"
#include "interconnect.h"

// Control print statements.
#define LOG_LEVEL 2
#include "logger.h"

namespace RedPandaDS {
namespace Core {

// ==================================================================================================
// Branch
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/B?lang=en
// ==================================================================================================
cycles ARM::THUMB_B(uint32_t instruct) {
    uint32_t imm11 = readBits(instruct, 0, 10);
    // Sign extend the bottom 11 bits and align address to 2 bytes.
    int32_t offset = ((int32_t)(imm11 << 21)) >> 20;
    branch(pc() + offset, false);
    return 1;  // An instruction prefetch occurs at the same time as the operation. Branch will
               // trigger one non-sequential + a sequential code read on the following fetches.
}
// ==================================================================================================
cycles ARM::THUMB_B_COND(uint32_t instruct) {
    uint8_t condition = readBits(instruct, 8, 11);
    if (checkIfConditionPassed(ConditionMnemonics::ConditionMnemonics(condition))) {
        uint32_t imm8 = readBits(instruct, 0, 7);
        // Sign extend the bottom 8 bits and align address to 2 bytes.
        int32_t offset = ((int32_t)(imm8 << 24)) >> 23;
        branch(pc() + offset, false);
    }
    return 1;  // An instruction prefetch occurs at the same time as the operation. Branch will
               // trigger one non-sequential + a sequential code read on the following fetches.
}
// ==================================================================================================
// Branch Link and Exchange
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/BX?lang=en
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/BLX--register-?lang=en
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/BL--BLX--immediate-?lang=en
// ==================================================================================================
cycles ARM::THUMB_BX(uint32_t instruct) {
    uint8_t Rm = readBits(instruct, 3, 6);
    branch(*activeRegs[Rm]);
    return 1;  // An instruction prefetch occurs at the same time as the operation. Branch will
               // trigger one non-sequential + a sequential code read on the following fetches.
}
// ==================================================================================================
cycles ARM::THUMB_BLX_REG(uint32_t instruct) {
    lr() = pc() - 2 | 0b1;  // Point to the previous instruction in thumb mode.
    uint8_t Rm = readBits(instruct, 3, 6);
    branch(*activeRegs[Rm]);
    return 1;  // An instruction prefetch occurs at the same time as the operation. Branch will
               // trigger one non-sequential + a sequential code read on the following fetches.
}
cycles ARM::THUMB_BLX_IMM(uint32_t instruct) {
    uint32_t pcVal = pc() - 2;  // PC calculation is off. Easier to fix here than fix emu.
    lr() = pcVal - 2 | 0b1;     // Point to the previous instruction in thumb mode.
    bool S = readBit(instruct, 26);
    bool I1 = !(readBit(instruct, 13) ^ S);
    bool I2 = !(readBit(instruct, 11) ^ S);
    if (readBit(instruct, 12)) {
        uint32_t imm = (S << 23) | (I1 << 22) | (I2 << 21) | (readBits(instruct, 16, 25) << 11) |
                       readBits(instruct, 0, 10);
        int32_t offset = (((int32_t)(imm << 8)) >> 7);
        branch(pcVal + offset, false);
    } else {
        uint32_t imm = (S << 22) | (I1 << 21) | (I2 << 20) | (readBits(instruct, 16, 25) << 10) |
                       readBits(instruct, 1, 10);
        int32_t offset = (((int32_t)(imm << 9)) >> 7);
        branch(pcVal + offset);
    }
    return 1;  // An instruction prefetch occurs at the same time as the operation. Branch will
               // trigger one non-sequential + a sequential code read on the following fetches.
}
// ==================================================================================================

}  // namespace Core
}  // namespace RedPandaDS
