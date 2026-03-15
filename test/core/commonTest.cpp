
#include "commonTest.h"

// Control print statements.
#define LOG_LEVEL 1
#include "logger.h"

using namespace RedPandaDS::Core;

std::vector<InstructionTestCase> genInstuctionTestCase(std::vector<std::string> instruction,
                                                       std::vector<uint32_t> expectedVals,
                                                       bool skipPC) {
    std::vector<InstructionTestCase> returnValue;
    std::vector<Encoding> instuctionEncodings;
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
            returnValue.push_back({instuctionEncodings[count].instruction, i, expectedVals[j]});
            count++;
        }
    }

    return returnValue;
}
