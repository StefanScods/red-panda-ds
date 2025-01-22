#ifndef COMMONTEST_H
#define COMMONTEST_H

#include <cassert>
#include <iostream>
#include <regex>

#include "../src/cpu.h"
#include "../src/utils/armEncode.h"
#include "../src/utils/utils.h"

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
                                                       bool skipPC) {
    std::vector<InstructionTestCase> returnValue;
    std::vector<uint32_t> instuctionEncodings;
    std::string instructionStr = "";
    assert(instruction.size() == expectedVals.size());
    for (int i = 0; i < g_regNames.size() - skipPC; i++) {
        for (int j = 0; j < instruction.size(); j++) {
            instructionStr += std::regex_replace(instruction[j], std::regex("\\" + BASE_REG_TOKEN),
                                                 g_regNames[i]) +
                              "\n";
        }
    }
    instuctionEncodings = armEncodeASM(instructionStr);
    int count = 0;
    for (uint8_t i = 0; i < g_regNames.size() - skipPC; i++) {
        for (int j = 0; j < instruction.size(); j++) {
            returnValue.push_back({instuctionEncodings[count], i, expectedVals[j]});
            count++;
        }
    }

    return returnValue;
}

#endif