/**
 * @file armInterpreter_data.cpp
 * @brief Implements arm data processing instuction set
 */
#include "cpu.h"
#include "interconnect.h"

// Control print statements.
#define LOG_LEVEL 1
#include "logger.h"

cycles ARM::mov32bit(uint32_t desReg, uint32_t src) {
    // Move the value into the destination register.
    *activeRegs[desReg] = src;
    return 1;  // An instruction prefetch occurs at the same time as the data operation.
}