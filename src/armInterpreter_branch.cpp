/**
 * @file armInterpreter_branch.cpp
 * @brief Implements arm branch instuction set
 */
#include "cpu.h"
#include "interconnect.h"

// Control print statements.
#define LOG_LEVEL 2
#include "logger.h"

// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/ARM-Instruction-Set-Encoding/Branch--branch-with-link--and-block-data-transfer?lang=en
cycles ARM::branchDecodeAndExecute(uint32_t instuct, uint8_t cond) {
    LogError("Unsupported instruction: " << instuct << "!");
    return 1;
}
