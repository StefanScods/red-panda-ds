#include <gtest/gtest.h>

#include "../src/cpu.h"
#include "../src/interconnect.h"
#include "../src/memoryDefines.h"
#include "../src/utils/armEncode.h"
#include "commonTest.h"

/**
 * @brief Test fixture for the CPU's instructions in the data category.
 */
class TestCPUDataInstructions : public testing::Test {
protected:
    Interconnect bus;
    ARM7TDMI arm7;
    TestCPUDataInstructions() {}
    ~TestCPUDataInstructions() {}

    void SetUp() override {
        bus.init();
        bus.bindARM7(&arm7);
    }

    void TearDown() override {}
};

class TestCPUDataInstructions_MOV : public TestCPUDataInstructions {
protected:
    TestCPUDataInstructions_MOV() {}
    ~TestCPUDataInstructions_MOV() {}

    void SetUp() override { TestCPUDataInstructions::SetUp(); }

    void TearDown() override { TestCPUDataInstructions::TearDown(); }
};

/**
 * @brief Test moving imm values into all possible regs. Small imm values
 * are defined as able to be encoded in 8 bits.
 */
TEST_F(TestCPUDataInstructions_MOV, MOV_SMALL_IMMEDIATE) {
    // Generate test cases.
    std::vector<uint32_t> immValuesToTest = {0, 1, 2, 8, 16, 42, 113, 173, 255};
    std::vector<std::string> instructions = {
        "MOV " + BASE_REG_TOKEN + ", #0",   "MOV " + BASE_REG_TOKEN + ", #1",
        "MOV " + BASE_REG_TOKEN + ", #2",   "MOV " + BASE_REG_TOKEN + ", #8",
        "MOV " + BASE_REG_TOKEN + ", #16",  "MOV " + BASE_REG_TOKEN + ", #42",
        "MOV " + BASE_REG_TOKEN + ", #113", "MOV " + BASE_REG_TOKEN + ", #173",
        "MOV " + BASE_REG_TOKEN + ", #255"};
    std::vector<InstructionTestCase> testCases =
        genInstuctionTestCase(instructions, immValuesToTest, false);

    // Loop over all test cases.
    for (uint32_t i = 0; i < testCases.size(); i++) {
        int testValue = testCases[i].expectedVal;
        // Set a default value to the reg.
        arm7.reset();
        arm7.writeReg(testCases[i].regNum, testValue ? 0 : 1);
        bus.write32ARM7(MAIN_RAM_START, testCases[i].instuction);
        arm7.setPC(MAIN_RAM_START);
        arm7.fetchAndExecute();
        ASSERT_EQ(arm7.readReg(testCases[i].regNum), testValue);
    }
}

TEST_F(TestCPUDataInstructions_MOV, MOV_LARGE_IMMEDIATE) {
    // Generate test cases.
    std::vector<uint32_t> immValuesToTest = {0xFF0,     0xFF00,     0xFF000,   0xFF0000,
                                             0xFF00000, 0xFF000000, 0xF000000F};
    std::vector<std::string> instructions = {
        "MOV " + BASE_REG_TOKEN + ", #0xFF0",      "MOV " + BASE_REG_TOKEN + ", #0xFF00",
        "MOV " + BASE_REG_TOKEN + ", #0xFF000",    "MOV " + BASE_REG_TOKEN + ", #0xFF0000",
        "MOV " + BASE_REG_TOKEN + ", #0xFF00000",  "MOV " + BASE_REG_TOKEN + ", #0xFF000000",
        "MOV " + BASE_REG_TOKEN + ", #0xF000000F",
    };
    std::vector<InstructionTestCase> testCases =
        genInstuctionTestCase(instructions, immValuesToTest, false);

    // Loop over all test cases.
    for (uint32_t i = 0; i < testCases.size(); i++) {
        int testValue = testCases[i].expectedVal;
        // Set a default value to the reg.
        arm7.reset();
        arm7.writeReg(testCases[i].regNum, testValue ? 0 : 1);
        bus.write32ARM7(MAIN_RAM_START, testCases[i].instuction);
        arm7.setPC(MAIN_RAM_START);
        arm7.fetchAndExecute();
        ASSERT_EQ(arm7.readReg(testCases[i].regNum), testValue);
    }
}