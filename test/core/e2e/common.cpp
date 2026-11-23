#include "common.h"

#include <iostream>

using namespace RedPandaDS::Core;

// =================================================================================================
std::string getTestROMFile(std::string name) {
#ifdef TEST_ROM_OUTPUT
    return std::string(TEST_ROM_OUTPUT) + "/" + name + ".nds";
#else
    return name + ".nds";
#endif
}
// =================================================================================================
void printTestResult(E2ETestResult& result, DSEmuCore& core) {
#define ANSI_RESET "\033[0m"
#define ANSI_BOLD "\033[1m"
#define ANSI_RED "\033[31m"
#define ANSI_GREEN "\033[32m"

    std::cout << "================================================================================="
              << std::endl;

    std::cout << ANSI_BOLD << "E2E Test Result: " << ANSI_RESET
              << core.getNDS_Cartridge()->getLoadedROMFilePath() << std::endl;
    std::cout << std::endl;
    std::cout << ANSI_BOLD << "Status: " << ANSI_RESET << (result.valid ? ANSI_GREEN : ANSI_RED)
              << (result.valid ? "Completed" : "Failed") << ANSI_RESET << std::endl;
    std::cout << ANSI_BOLD << "Number of Executions: " << ANSI_RESET << result.numIterations
              << std::endl;
    std::cout << ANSI_BOLD << "Number of Cycles: " << ANSI_RESET << result.numCycles << std::endl;
    std::cout << ANSI_BOLD << "Result Value: " << ANSI_RESET << result.result << std::endl;

    std::cout << "================================================================================="
              << std::endl;
}
// =================================================================================================
