/**
 * @file arm7.cpp
 * @brief Implements ARM7TDMI instuction set
 */
#include "cpu.h"
#include "interconnect.h"

// Control print statements.
#define LOG_LEVEL 1
#include "logger.h"

// https://developer.arm.com/documentation/ddi0406/cb/Application-Level-Architecture/ARM-Instruction-Set-Encoding/Load-store-word-and-unsigned-byte?lang=en
cycles ARM7TDMI::loadStoreDecodeAndExecute(uint32_t instuct, uint8_t cond) {
    return 1;
}