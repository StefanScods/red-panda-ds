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
// ==================================================================================================
cycles ARM::THUMB_BX(uint32_t instruct) {
    uint8_t Rm = readBits(instruct, 3, 6);
    branch(*activeRegs[Rm]);
    return 1;  // An instruction prefetch occurs at the same time as the operation. Branch will
               // trigger one non-sequential + a sequential code read on the following fetches.
}
// ==================================================================================================
cycles ARM::THUMB_BLX(uint32_t instruct) {
    lr() = pc() - 2 | 0b1;  // Point to the previous instruction in thumb mode.
    uint8_t Rm = readBits(instruct, 3, 6);
    branch(*activeRegs[Rm]);
    return 1;  // An instruction prefetch occurs at the same time as the operation. Branch will
               // trigger one non-sequential + a sequential code read on the following fetches.
}
// ==================================================================================================

}  // namespace Core
}  // namespace RedPandaDS
