/**
 * @file armInterpreter/thumb_loadStore.cpp
 * @brief Implements THUMB mode instuction set for instructions in the load and store category.
 */
#include "cpu.h"
#include "interconnect.h"

// Control print statements.
#define LOG_LEVEL 2
#include "logger.h"

// ==================================================================================================
// STR
// ==================================================================================================
cycles ARM::THUMB_STR(uint32_t srcReg, uint32_t baseReg, uint32_t offset) {
    return ARM_UNDEFINED_INST(0);
}
// ==================================================================================================
cycles ARM::THUMB_STRH(uint32_t srcReg, uint32_t baseReg, uint32_t offset) {
    return ARM_UNDEFINED_INST(0);
}
// ==================================================================================================
cycles ARM::THUMB_STRB(uint32_t srcReg, uint32_t baseReg, uint32_t offset) {
    return ARM_UNDEFINED_INST(0);
}
// ==================================================================================================
// LDR
// ==================================================================================================
cycles ARM::THUMB_LDR(uint32_t desReg, uint32_t baseReg, uint32_t offset) {
    return ARM_UNDEFINED_INST(0);
}
// ==================================================================================================
cycles ARM::THUMB_LDRH(uint32_t desReg, uint32_t baseReg, uint32_t offset) {
    return ARM_UNDEFINED_INST(0);
}
// ==================================================================================================
cycles ARM::THUMB_LDRB(uint32_t desReg, uint32_t baseReg, uint32_t offset) {
    return ARM_UNDEFINED_INST(0);
}
// ==================================================================================================
cycles ARM::THUMB_LDRSH(uint32_t desReg, uint32_t baseReg, uint32_t offset) {
    return ARM_UNDEFINED_INST(0);
}
// ==================================================================================================
cycles ARM::THUMB_LDRSB(uint32_t desReg, uint32_t baseReg, uint32_t offset) {
    return ARM_UNDEFINED_INST(0);
}
// ==================================================================================================
// STM
// ==================================================================================================
cycles ARM::THUMB_STM(uint32_t instruct) {
    return ARM_UNDEFINED_INST(instruct);
}
// ==================================================================================================
// LDR
// ==================================================================================================
cycles ARM::THUMB_LDM(uint32_t instruct) {
    return ARM_UNDEFINED_INST(instruct);
}
// ==================================================================================================
cycles ARM::THUMB_LDR_LIT(uint32_t instruct) {
    return ARM_UNDEFINED_INST(instruct);
}
// ==================================================================================================
// PUSH
// ==================================================================================================
cycles ARM::THUMB_PUSH(uint32_t instruct) {
    return ARM_UNDEFINED_INST(instruct);
}
// ==================================================================================================
// POP
// ==================================================================================================
cycles ARM::THUMB_POP(uint32_t instruct) {
    return ARM_UNDEFINED_INST(instruct);
}
// ==================================================================================================
