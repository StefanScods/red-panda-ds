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

// ==================================================================================================
// BKPT
// ==================================================================================================
cycles ARM::THUMB_BKPT(uint32_t instruct) {
    return ARM_UNDEFINED_INST(instruct);
}
// ==================================================================================================
// SVC
// ==================================================================================================
cycles ARM::THUMB_SVC(uint32_t instruct) {
    return ARM_UNDEFINED_INST(instruct);
}
// ==================================================================================================
