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

TEST_F(TestCPUDataInstructions_MOV, MOV_IMMEDIATE_NEGATIVE_FLAG) {
    // N = true
    writeProgramToMemory("MOV R0, #0xFF000000\n", MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readReg(0), 0xFF000000);
    ASSERT_EQ(arm7.readFlag(N_FLAG), 1);

    // N = false
    arm7.reset();
    writeProgramToMemory("MOV R0, #1\n", MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readReg(0), 1);
    ASSERT_EQ(arm7.readFlag(N_FLAG), 0);
}

TEST_F(TestCPUDataInstructions_MOV, MOV_IMMEDIATE_ZERO_FLAG) {
    // Z = true
    writeProgramToMemory("MOV R0, #0\n", MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readReg(0), 0);
    ASSERT_EQ(arm7.readFlag(Z_FLAG), 1);

    // Z = false
    arm7.reset();
    writeProgramToMemory("MOV R0, #1\n", MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readReg(0), 1);
    ASSERT_EQ(arm7.readFlag(Z_FLAG), 0);
}

TEST_F(TestCPUDataInstructions_MOV, MOV_IMMEDIATE_CARRY_FLAG) {
    // C = true
    writeProgramToMemory("MOV R0, #0xF0000001\n", MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readReg(0), 0xF0000001);
    ASSERT_EQ(arm7.readFlag(C_FLAG), 1);

    // C = false
    arm7.reset();
    writeProgramToMemory("MOV R0, #1\n", MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readReg(0), 1);
    ASSERT_EQ(arm7.readFlag(C_FLAG), 0);
}

class TestCPUDataInstructions_ADD : public TestCPUDataInstructions {
protected:
    TestCPUDataInstructions_ADD() {}
    ~TestCPUDataInstructions_ADD() {}

    void SetUp() override { TestCPUDataInstructions::SetUp(); }

    void TearDown() override { TestCPUDataInstructions::TearDown(); }
};

TEST_F(TestCPUDataInstructions_ADD, ADD_IMMEDIATE) {
    writeProgramToMemory(
        "MOV R1, #100\n"
        "ADD R0, R1, #10\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(2);
    ASSERT_EQ(arm7.readReg(0), 110);
}

TEST_F(TestCPUDataInstructions_ADD, ADD_IMMEDIATE_NEGATIVE_FLAG) {
    // N = true
    writeProgramToMemory(
        "MOV R1, #0xFF000000\n"
        "ADD R0, R1, #0xFF\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(2);
    ASSERT_EQ(arm7.readReg(0), 0xFF0000FF);
    ASSERT_EQ(arm7.readFlag(N_FLAG), 1);

    // N = false
    arm7.reset();
    writeProgramToMemory(
        "MOV R1, #0x0FF00000\n"
        "ADD R0, R1, #0xFF\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(2);
    ASSERT_EQ(arm7.readReg(0), 0x0FF000FF);
    ASSERT_EQ(arm7.readFlag(N_FLAG), 0);
}

TEST_F(TestCPUDataInstructions_ADD, ADD_IMMEDIATE_ZERO_FLAG) {
    // Z = true
    writeProgramToMemory(
        "MOV R9, #0\n"
        "ADD R10, R9, #0\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(2);
    ASSERT_EQ(arm7.readReg(10), 0);
    ASSERT_EQ(arm7.readFlag(Z_FLAG), 1);

    // Z = false
    arm7.reset();
    writeProgramToMemory(
        "MOV R5, #1\n"
        "ADD R4, R5, #7\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(2);
    ASSERT_EQ(arm7.readReg(4), 8);
    ASSERT_EQ(arm7.readFlag(Z_FLAG), 0);
}

TEST_F(TestCPUDataInstructions_ADD, ADD_IMMEDIATE_CARRY_FLAG) {
    // C = true
    writeProgramToMemory(
        "MOV R1, #0xFF\n"
        "ADD R1, R1, #0xFF00\n"
        "ADD R1, R1, #0xFF0000\n"
        "ADD R1, R1, #0xFF000000\n"
        "ADD R1, R1, #1\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(5);
    ASSERT_EQ(arm7.readReg(1), 0);
    ASSERT_EQ(arm7.readFlag(C_FLAG), 1);

    // C = false
    arm7.reset();
    writeProgramToMemory(
        "MOV R1, #0xFF\n"
        "ADD R1, R1, #0xFF00\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(2);
    ASSERT_EQ(arm7.readReg(1), 0xFFFF);
    ASSERT_EQ(arm7.readFlag(C_FLAG), 0);
}

TEST_F(TestCPUDataInstructions_ADD, ADD_IMMEDIATE_OVERFLOW_FLAG) {
    // V = true
    writeProgramToMemory(
        "MOV R1, #0xFF\n"
        "ADD R1, R1, #0xFF00\n"
        "ADD R1, R1, #0xFF0000\n"
        "ADD R1, R1, #0x7F000000\n"
        "ADD R1, R1, #1\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(5);
    ASSERT_EQ(arm7.readReg(1), 0x80000000);
    ASSERT_EQ(arm7.readFlag(V_FLAG), 1);

    // V = false
    arm7.reset();
    writeProgramToMemory(
        "MOV R9, #1\n"
        "ADD R7, R9, #7\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(2);
    ASSERT_EQ(arm7.readReg(7), 8);
    ASSERT_EQ(arm7.readFlag(V_FLAG), 0);
}
