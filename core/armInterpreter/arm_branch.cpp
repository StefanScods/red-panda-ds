/**
 * @file armInterpreter/arm_branch.cpp
 * @brief Implements ARM mode instuction set for instructions in the branch category.
 */
#include "cpu.h"
#include "interconnect.h"

// Control print statements.
#define LOG_LEVEL 2
#include "logger.h"

namespace RedPandaDS {
namespace Core {

// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/ARM-Instruction-Set-Encoding/Branch--branch-with-link--and-block-data-transfer?lang=en

// ==================================================================================================
// Branch
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/B?lang=en
// ==================================================================================================
cycles ARM::ARM_B(uint32_t instruct) {
    // Sign extend the bottom 24 bits and align address to 4 bytes.
    uint32_t imm24 = readBits(instruct, 0, 23);
    int32_t offset = ((int32_t)(imm24 << 8)) >> 6;
    branch(pc() + offset, false);
    return 1;  // An instruction prefetch occurs at the same time as the data operation. Branch will
               // trigger one non-sequential + a sequential code read on the following fetches.
}
// ==================================================================================================
// Branch Link and Exchange
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/BLX--register-?lang=en
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/BX?lang=en
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/Instruction-Details/Alphabetical-list-of-instructions/BL--BLX--immediate-?lang=en
// ==================================================================================================
cycles ARM::ARM_BL(uint32_t instruct) {
    lr() = pc() - 4;  // Point to the previous instruction.
    // Sign extend the bottom 24 bits and align address to 4 bytes.
    uint32_t imm24 = readBits(instruct, 0, 23);
    int32_t offset = ((int32_t)(imm24 << 8)) >> 6;
    branch(pc() + offset, false);
    return 1;  // An instruction prefetch occurs at the same time as the data operation.Branch will
               // trigger one non-sequential + a sequential code read on the following fetches.
}
// ==================================================================================================
cycles ARM::ARM_BLX_IMM(uint32_t instruct) {
    lr() = pc() - 4;  // Point to the previous instruction.
    // Sign extend the bottom 24 bits and align address to 4 bytes.
    uint32_t imm24 = readBits(instruct, 0, 23);
    bool H = readBit(instruct, 24);
    int32_t offset = ((int32_t)(imm24 << 8)) >> 6 | (H << 1);
    branch(pc() + offset + 1);  // Offset by + 1 to enter thumb mode.
    return 1;  // An instruction prefetch occurs at the same time as the data operation. Branch will
               // trigger one non-sequential + a sequential code read on the following fetches.
}
// ==================================================================================================
cycles ARM::ARM_BLX_REG(uint32_t instruct) {
    lr() = pc() - 4;  // Point to the previous instruction.
    uint8_t Rm = readBits(instruct, 0, 3);
    branch(*activeRegs[Rm]);
    return 1;  // An instruction prefetch occurs at the same time as the data operation. Branch will
               // trigger one non-sequential + a sequential code read on the following fetches.
}
// ==================================================================================================
cycles ARM::ARM_BX(uint32_t instruct) {
    uint8_t Rm = readBits(instruct, 0, 3);
    branch(*activeRegs[Rm]);
    return 1;  // An instruction prefetch occurs at the same time as the data operation. Branch will
               // trigger one non-sequential + a sequential code read on the following fetches.
}
// ==================================================================================================

}  // namespace Core
}  // namespace RedPandaDS
