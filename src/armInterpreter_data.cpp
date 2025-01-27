/**
 * @file armInterpreter_data.cpp
 * @brief Implements arm data processing instuction set
 */
#include "cpu.h"
#include "interconnect.h"

// Control print statements.
#define LOG_LEVEL 2
#include "logger.h"

cycles ARM::ARM_MOV(uint32_t desReg, uint32_t srcValue) {
    LogDebug("Executing MOV");
    // Move the value into the destination register.
    *activeRegs[desReg] = srcValue;
    return 1;  // An instruction prefetch occurs at the same time as the data operation.
}