#ifndef TEST_E2E_COMMON_H
#define TEST_E2E_COMMON_H

#include <gtest/gtest.h>

#include "core/core.h"

using namespace RedPandaDS::Core;

/**
 * @brief Test fixture for full end to end test running actual ROMs.
 */
class TestE2E : public testing::Test {
protected:
    DSEmuCore core;
    Interconnect* bus = nullptr;

    TestE2E() { bus = core.getInterconnect(); }
    ~TestE2E() {}

    void SetUp() override { core.reset(); }

    void TearDown() override {}
};
// =================================================================================================
/**
 * @brief Returns the full path to a testing ROM file.
 *
 * @param name Name of the ROM test case.
 * @return `std::string`
 */
std::string getTestROMFile(std::string name);
// =================================================================================================
/**
 * @brief Pretty prints a test result.
 *
 * @param result The result to print.
 * @param core The core which produced the result.
 */
void printTestResult(E2ETestResult& result, DSEmuCore& core);
// =================================================================================================

#endif
