/**
 * @file armInterpreter/thumb_branch.cpp
 * @brief Implements THUMB mode instuction set for instructions in the branch category.
 */
#include "cpu.h"
#include "interconnect.h"

// Control print statements.
#define LOG_LEVEL 2
#include "logger.h"

// ==================================================================================================
// Branch
// ==================================================================================================
cycles ARM::THUMB_B(uint32_t instruct) {
    return ARM_UNDEFINED_INST(instruct);
}
// ==================================================================================================
cycles ARM::THUMB_B_COND(uint32_t instruct) {
    return ARM_UNDEFINED_INST(instruct);
}
// ==================================================================================================
// Branch Link and Exchange
// ==================================================================================================
cycles ARM::THUMB_BX(uint32_t instruct) {
    return ARM_UNDEFINED_INST(instruct);
}
// ==================================================================================================
cycles ARM::THUMB_BLX(uint32_t instruct) {
    return ARM_UNDEFINED_INST(instruct);
}
// ==================================================================================================