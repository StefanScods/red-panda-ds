
#include "commonTest.h"

// Control print statements.
#define LOG_LEVEL 1
#include "logger.h"

void writeProgramToMemory(std::string program, uint32_t startAddress, Interconnect* bus,
                          bool arm7) {
    std::vector<uint32_t> instuctionEncodings = armEncodeASM(program, arm7);
    LogDebug("Writing program to 0x" << std::hex << startAddress << std::dec << "...");
    for (int i = 0; i < instuctionEncodings.size(); i++) {
        uint32_t address = startAddress + i * ARM_WORD_SIZE;
        arm7 ? bus->write32ARM7(address, instuctionEncodings[i])
             : bus->write32ARM7(address, instuctionEncodings[i]);
    }
    LogDebug("Finished writing program! - " << instuctionEncodings.size() << " instructions - "
                                            << instuctionEncodings.size() * ARM_WORD_SIZE
                                            << " byte(s)");
}

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
