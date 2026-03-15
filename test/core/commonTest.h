#ifndef COMMONTEST_H
#define COMMONTEST_H

#include <cassert>
#include <iostream>
#include <regex>

#include "core/cpu.h"
#include "core/interconnect.h"
#include "core/utils/armEncode.h"
#include "core/utils/utils.h"

using namespace RedPandaDS::Core;

const std::string BASE_REG_TOKEN = "{BASE_REG}";

struct InstructionTestCase {
    uint32_t instuction;
    uint8_t regNum;
    uint32_t expectedVal;
} typedef InstructionTestCase;

/**
 * @brief Creates a bunch of test cases with only one call to the assembler.
 *
 * @param instruction The string instructions to generate encodings for, an encoding
 * will be generated for all base (or source) register. The token BASE_REG_TOKEN within
 * the passed string will be replaced by that register.
 *
 * @param expectedVals The values expected after the instuction executes. There should be an
 * expected value for each instruction in the passed instruction array.
 * @param skipPC If true, encodings will only be generated for registers 1-14. Else 15 is also
 * included.
 * @return std::vector<InstructionTestCase>
 */
std::vector<InstructionTestCase> genInstuctionTestCase(std::vector<std::string> instruction,
                                                       std::vector<uint32_t> expectedVals,
                                                       bool skipPC);
#endif
