/**
 * @file armInterpreter/thumb_misc.cpp
 * @brief Implements THUMB mode instuction set for instructions which do not fit in any other
 * category.
 */
#include "cpu.h"
#include "interconnect.h"

// Control print statements.
#define LOG_LEVEL 2
#include "logger.h"

namespace RedPandaDS {
namespace Core {

// ==================================================================================================
// BKPT
// ==================================================================================================
cycles ARM::THUMB_BKPT(uint32_t instruct) {
    return THUMB_UNDEFINED_INST(instruct);
}
// ==================================================================================================
// SVC
// ==================================================================================================
cycles ARM::THUMB_SVC(uint32_t instruct) {
    uint32_t imm8 = readBits(instruct, 0, 7);
    cycles executionTime = bios->handleSWI(imm8);
    return 1 + executionTime;
}
// ==================================================================================================

}  // namespace Core
}  // namespace RedPandaDS
