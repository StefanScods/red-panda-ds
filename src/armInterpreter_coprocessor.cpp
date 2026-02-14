/**
 * @file armInterpreter.cpp
 * @brief Implements arm instuction set decoding
 */
#include <cassert>
#include <cstdint>

#include "cpu.h"
#include "interconnect.h"

// Control print statements.
#define LOG_LEVEL 2
#include "logger.h"

// ==================================================================================================
// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/ARM-Instruction-Set-Encoding/Coprocessor-instructions--and-Supervisor-Call?lang=en
cycles ARM::coprocessorAndSupervisorDecodeAndExecute(uint32_t instruct, uint8_t cond) {
    return ARM_UNDEFINED_INST(instruct);
}
// ==================================================================================================
