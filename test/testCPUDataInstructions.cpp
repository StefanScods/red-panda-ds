#include <gtest/gtest.h>

#include "../src/cpu.h"
#include "../src/interconnect.h"
#include "../src/memoryDefines.h"
#include "../src/utils/armEncode.h"

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
    std::vector<uint32_t> immValuesToTest = {0, 1, 2, 8, 16, 42, 113, 173, 255};
    // Loop over all possible regs.
    for (uint32_t i = 0; i < g_regNames.size(); i++) {
        // Test a bunch of different values.
        for (uint32_t j = 0; j < immValuesToTest.size(); j++) {
            int testValue = immValuesToTest[j];
            // Set a default value to the reg.
            arm7.reset();
            arm7.writeReg(i, testValue ? 0 : 1);

            // Write the instuction and execute it.
            std::string instructionStr = "MOV " + g_regNames[i] + ", #" + std::to_string(testValue);
            bus.write32ARM7(MAIN_RAM_START, armEncodeASM(instructionStr));
            arm7.setPC(MAIN_RAM_START);
            arm7.fetchAndExecute();

            ASSERT_EQ(arm7.readReg(i), testValue);
        }
    }
}

TEST_F(TestCPUDataInstructions_MOV, MOV_LARGE_IMMEDIATE) {
    std::vector<uint32_t> immValuesToTest = {0xFF00,    0xFF00,     0xFF000,   0xFF0000,
                                             0xFF00000, 0xFF000000, 0xF000000F};
    // Loop over all possible regs.
    for (uint32_t i = 0; i < g_regNames.size(); i++) {
        // Test a bunch of different values.
        for (uint32_t j = 0; j < immValuesToTest.size(); j++) {
            int testValue = immValuesToTest[j];
            // Set a default value to the reg.
            arm7.reset();
            arm7.writeReg(i, testValue ? 0 : 1);

            // Write the instuction and execute it.
            std::string instructionStr = "MOV " + g_regNames[i] + ", #" + std::to_string(testValue);
            bus.write32ARM7(MAIN_RAM_START, armEncodeASM(instructionStr));
            arm7.setPC(MAIN_RAM_START);
            arm7.fetchAndExecute();

            ASSERT_EQ(arm7.readReg(i), testValue);
        }
    }
}