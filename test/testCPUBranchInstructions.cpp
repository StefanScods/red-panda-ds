#include <gtest/gtest.h>

#include "../src/cpu.h"
#include "../src/interconnect.h"
#include "../src/memoryDefines.h"
#include "../src/utils/armEncode.h"
#include "commonTest.h"

/**
 * @brief Test fixture for the CPU's instructions in the branch category.
 */
class TestCPUBranchInstructions : public testing::Test {
protected:
    Interconnect bus;
    ARM7TDMI arm7;
    ARM946ES arm9;
    TestCPUBranchInstructions() {}
    ~TestCPUBranchInstructions() {}

    void SetUp() override {
        bus.init();
        bus.bindARM7(&arm7);
        bus.bindARM9(&arm9);
    }

    void TearDown() override {}
};

// ==================================================================================================
// Branch
// ==================================================================================================
class TestCPUBranchInstructions_B : public TestCPUBranchInstructions {
protected:
    TestCPUBranchInstructions_B() {}
    ~TestCPUBranchInstructions_B() {}

    void SetUp() override { TestCPUBranchInstructions::SetUp(); }

    void TearDown() override { TestCPUBranchInstructions::TearDown(); }
};
/**
 * @brief Tests a Branch operation using an immediate.
 */
TEST_F(TestCPUBranchInstructions_B, B_IMMEDIATE) {
    writeProgramToMemory(
        "MOV R1, #0\n"
        "MOV R0, #0\n"
        "B .-4\n"
        "MOV R1, #255",
        MAIN_RAM_START, &bus,
        arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(3);
    ASSERT_EQ(arm7.readReg(PC_REGISTER_NUM), MAIN_RAM_START + 8);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readReg(1), 0x0);
}
/**
 * @brief Tests a conditional Branch EQ operation using an immediate.
 */
TEST_F(TestCPUBranchInstructions_B, BEQ_IMMEDIATE) {
    // Test branch is executed.
    writeProgramToMemory(
        "MOV R1, #0\n"
        "MOVs R0, #0\n"
        "Beq .-4\n"
        "MOV R1, #255",
        MAIN_RAM_START, &bus,
        arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(3);
    ASSERT_EQ(arm7.readReg(PC_REGISTER_NUM), MAIN_RAM_START + 8);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readReg(1), 0x0);

    arm7.reset();
    // Test branch is not executed.
    writeProgramToMemory(
        "MOV R1, #0\n"
        "MOVs R0, #1\n"
        "Beq .-4\n"
        "MOV R1, #255",
        MAIN_RAM_START, &bus,
        arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(3);
    ASSERT_EQ(arm7.readReg(PC_REGISTER_NUM), MAIN_RAM_START + 20);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readReg(1), 255);
}
/**
 * @brief Tests a conditional Branch NE operation using an immediate.
 */
TEST_F(TestCPUBranchInstructions_B, BNE_IMMEDIATE) {
    // Test branch is executed.
    writeProgramToMemory(
        "MOV R1, #0\n"
        "MOVs R0, #1\n"
        "Bne .-4\n"
        "MOV R1, #255",
        MAIN_RAM_START, &bus,
        arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(3);
    ASSERT_EQ(arm7.readReg(PC_REGISTER_NUM), MAIN_RAM_START + 8);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readReg(1), 0x0);

    arm7.reset();
    // Test branch is not executed.
    writeProgramToMemory(
        "MOV R1, #0\n"
        "MOVs R0, #0\n"
        "Bne .-4\n"
        "MOV R1, #255",
        MAIN_RAM_START, &bus,
        arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(3);
    ASSERT_EQ(arm7.readReg(PC_REGISTER_NUM), MAIN_RAM_START + 20);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readReg(1), 255);
}
/**
 * @brief Tests a conditional Branch CS operation using an immediate.
 */
TEST_F(TestCPUBranchInstructions_B, BCS_IMMEDIATE) {
    // Test branch is executed.
    writeProgramToMemory(
        "MOV R1, #0\n"
        "MOV R0, #0\n"
        "CMP R0, #0\n"
        "Bcs .-4\n"
        "MOV R1, #255",
        MAIN_RAM_START, &bus,
        arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(4);
    ASSERT_EQ(arm7.readReg(PC_REGISTER_NUM), MAIN_RAM_START + 12);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readReg(1), 0x0);

    arm7.reset();
    // Test branch is not executed.
    writeProgramToMemory(
        "MOV R1, #0\n"
        "MOV R0, #0\n"
        "CMP R0, #1\n"
        "Bcs .-4\n"
        "MOV R1, #255",
        MAIN_RAM_START, &bus,
        arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(4);
    ASSERT_EQ(arm7.readReg(PC_REGISTER_NUM), MAIN_RAM_START + 24);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readReg(1), 255);
}
/**
 * @brief Tests a conditional Branch CC operation using an immediate.
 */
TEST_F(TestCPUBranchInstructions_B, BCC_IMMEDIATE) {
    // Test branch is executed.
    writeProgramToMemory(
        "MOV R1, #0\n"
        "MOV R0, #0\n"
        "CMP R0, #1\n"
        "Bcc .-4\n"
        "MOV R1, #255",
        MAIN_RAM_START, &bus,
        arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(4);
    ASSERT_EQ(arm7.readReg(PC_REGISTER_NUM), MAIN_RAM_START + 12);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readReg(1), 0x0);

    arm7.reset();
    // Test branch is not executed.
    writeProgramToMemory(
        "MOV R1, #0\n"
        "MOV R0, #0\n"
        "CMP R0, #0\n"
        "Bcc .-4\n"
        "MOV R1, #255",
        MAIN_RAM_START, &bus,
        arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(4);
    ASSERT_EQ(arm7.readReg(PC_REGISTER_NUM), MAIN_RAM_START + 24);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readReg(1), 255);
}
/**
 * @brief Tests a conditional Branch MI operation using an immediate.
 */
TEST_F(TestCPUBranchInstructions_B, BMI_IMMEDIATE) {
    // Test branch is executed.
    writeProgramToMemory(
        "MOV R1, #0\n"
        "MOV R0, #0\n"
        "CMP R0, #1\n"
        "Bmi .-4\n"
        "MOV R1, #255",
        MAIN_RAM_START, &bus,
        arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(4);
    ASSERT_EQ(arm7.readReg(PC_REGISTER_NUM), MAIN_RAM_START + 12);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readReg(1), 0x0);

    arm7.reset();
    // Test branch is not executed.
    writeProgramToMemory(
        "MOV R1, #0\n"
        "MOV R0, #1\n"
        "CMP R0, #0\n"
        "Bmi .-4\n"
        "MOV R1, #255",
        MAIN_RAM_START, &bus,
        arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(4);
    ASSERT_EQ(arm7.readReg(PC_REGISTER_NUM), MAIN_RAM_START + 24);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readReg(1), 255);
}
/**
 * @brief Tests a conditional Branch PL operation using an immediate.
 */
TEST_F(TestCPUBranchInstructions_B, BPL_IMMEDIATE) {
    // Test branch is executed.
    writeProgramToMemory(
        "MOV R1, #0\n"
        "MOV R0, #1\n"
        "CMP R0, #0\n"
        "Bpl .-4\n"
        "MOV R1, #255",
        MAIN_RAM_START, &bus,
        arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(4);
    ASSERT_EQ(arm7.readReg(PC_REGISTER_NUM), MAIN_RAM_START + 12);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readReg(1), 0x0);

    arm7.reset();
    // Test branch is not executed.
    writeProgramToMemory(
        "MOV R1, #0\n"
        "MOV R0, #0\n"
        "CMP R0, #1\n"
        "Bpl .-4\n"
        "MOV R1, #255",
        MAIN_RAM_START, &bus,
        arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(4);
    ASSERT_EQ(arm7.readReg(PC_REGISTER_NUM), MAIN_RAM_START + 24);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readReg(1), 255);
}
/**
 * @brief Tests a conditional Branch VS operation using an immediate.
 */
TEST_F(TestCPUBranchInstructions_B, BVS_IMMEDIATE) {
    // Test branch is executed.
    writeProgramToMemory(
        "MOV R1, #0\n"
        "MOV R0, #0x7FFFFFFF\n"
        "ADDS R0, R0, #1\n"
        "Bvs .-4\n"
        "MOV R1, #255",
        MAIN_RAM_START, &bus,
        arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(4);
    ASSERT_EQ(arm7.readReg(PC_REGISTER_NUM), MAIN_RAM_START + 12);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readReg(1), 0x0);

    arm7.reset();
    // Test branch is not executed.
    writeProgramToMemory(
        "MOV R1, #0\n"
        "MOV R0, #1\n"
        "ADDS R0, R0, #1\n"
        "Bvs .-4\n"
        "MOV R1, #255",
        MAIN_RAM_START, &bus,
        arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(4);
    ASSERT_EQ(arm7.readReg(PC_REGISTER_NUM), MAIN_RAM_START + 24);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readReg(1), 255);
}
/**
 * @brief Tests a conditional Branch VC operation using an immediate.
 */
TEST_F(TestCPUBranchInstructions_B, BVC_IMMEDIATE) {
    // Test branch is executed.
    writeProgramToMemory(
        "MOV R1, #0\n"
        "MOV R0, #1\n"
        "ADDS R0, R0, #1\n"
        "Bvc .-4\n"
        "MOV R1, #255",
        MAIN_RAM_START, &bus,
        arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(4);
    ASSERT_EQ(arm7.readReg(PC_REGISTER_NUM), MAIN_RAM_START + 12);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readReg(1), 0x0);

    arm7.reset();
    // Test branch is not executed.
    writeProgramToMemory(
        "MOV R1, #0\n"
        "MOV R0, #0x7FFFFFFF\n"
        "ADDS R0, R0, #1\n"
        "Bvc .-4\n"
        "MOV R1, #255",
        MAIN_RAM_START, &bus,
        arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(4);
    ASSERT_EQ(arm7.readReg(PC_REGISTER_NUM), MAIN_RAM_START + 24);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readReg(1), 255);
}
/**
 * @brief Tests a conditional Branch HI operation using an immediate.
 */
TEST_F(TestCPUBranchInstructions_B, BHI_IMMEDIATE) {
    // Test branch is executed.
    writeProgramToMemory(
        "MOV R1, #0\n"
        "MOV R0, #2\n"
        "CMP R0, #1\n"
        "Bhi .-4\n"
        "MOV R1, #255",
        MAIN_RAM_START, &bus,
        arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(4);
    ASSERT_EQ(arm7.readReg(PC_REGISTER_NUM), MAIN_RAM_START + 12);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readReg(1), 0x0);

    arm7.reset();
    // Test branch is not executed.
    writeProgramToMemory(
        "MOV R1, #0\n"
        "MOV R0, #1\n"
        "CMP R0, #2\n"
        "Bhi .-4\n"
        "MOV R1, #255",
        MAIN_RAM_START, &bus,
        arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(4);
    ASSERT_EQ(arm7.readReg(PC_REGISTER_NUM), MAIN_RAM_START + 24);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readReg(1), 255);
}
/**
 * @brief Tests a conditional Branch LS operation using an immediate.
 */
TEST_F(TestCPUBranchInstructions_B, BLS_IMMEDIATE) {
    // Test branch is executed.
    writeProgramToMemory(
        "MOV R1, #0\n"
        "MOV R0, #1\n"
        "CMP R0, #2\n"
        "Bls .-4\n"
        "MOV R1, #255",
        MAIN_RAM_START, &bus,
        arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(4);
    ASSERT_EQ(arm7.readReg(PC_REGISTER_NUM), MAIN_RAM_START + 12);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readReg(1), 0x0);

    arm7.reset();
    // Test branch is not executed.
    writeProgramToMemory(
        "MOV R1, #0\n"
        "MOV R0, #2\n"
        "CMP R0, #1\n"
        "Bls .-4\n"
        "MOV R1, #255",
        MAIN_RAM_START, &bus,
        arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(4);
    ASSERT_EQ(arm7.readReg(PC_REGISTER_NUM), MAIN_RAM_START + 24);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readReg(1), 255);
}
/**
 * @brief Tests a conditional Branch GE operation using an immediate.
 */
TEST_F(TestCPUBranchInstructions_B, BGE_IMMEDIATE) {
    // Test branch is executed.
    writeProgramToMemory(
        "MOV R1, #0\n"
        "MOV R0, #1\n"
        "CMP R0, #0\n"
        "Bge .-4\n"
        "MOV R1, #255",
        MAIN_RAM_START, &bus,
        arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(4);
    ASSERT_EQ(arm7.readReg(PC_REGISTER_NUM), MAIN_RAM_START + 12);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readReg(1), 0x0);

    arm7.reset();
    // Test branch is not executed.
    writeProgramToMemory(
        "MOV R1, #0\n"
        "MOV R0, #0\n"
        "CMP R0, #1\n"
        "Bge .-4\n"
        "MOV R1, #255",
        MAIN_RAM_START, &bus,
        arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(4);
    ASSERT_EQ(arm7.readReg(PC_REGISTER_NUM), MAIN_RAM_START + 24);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readReg(1), 255);
}
/**
 * @brief Tests a conditional Branch LT operation using an immediate.
 */
TEST_F(TestCPUBranchInstructions_B, BLT_IMMEDIATE) {
    // Test branch is executed.
    writeProgramToMemory(
        "MOV R1, #0\n"
        "MOV R0, #0\n"
        "CMP R0, #1\n"
        "Blt .-4\n"
        "MOV R1, #255",
        MAIN_RAM_START, &bus,
        arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(4);
    ASSERT_EQ(arm7.readReg(PC_REGISTER_NUM), MAIN_RAM_START + 12);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readReg(1), 0x0);

    arm7.reset();
    // Test branch is not executed.
    writeProgramToMemory(
        "MOV R1, #0\n"
        "MOV R0, #1\n"
        "CMP R0, #0\n"
        "Blt .-4\n"
        "MOV R1, #255",
        MAIN_RAM_START, &bus,
        arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(4);
    ASSERT_EQ(arm7.readReg(PC_REGISTER_NUM), MAIN_RAM_START + 24);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readReg(1), 255);
}
/**
 * @brief Tests a conditional Branch GT operation using an immediate.
 */
TEST_F(TestCPUBranchInstructions_B, BGT_IMMEDIATE) {
    // Test branch is executed.
    writeProgramToMemory(
        "MOV R1, #0\n"
        "MOV R0, #2\n"
        "CMP R0, #1\n"
        "Bgt .-4\n"
        "MOV R1, #255",
        MAIN_RAM_START, &bus,
        arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(4);
    ASSERT_EQ(arm7.readReg(PC_REGISTER_NUM), MAIN_RAM_START + 12);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readReg(1), 0x0);

    arm7.reset();
    // Test branch is not executed.
    writeProgramToMemory(
        "MOV R1, #0\n"
        "MOV R0, #1\n"
        "CMP R0, #2\n"
        "Bgt .-4\n"
        "MOV R1, #255",
        MAIN_RAM_START, &bus,
        arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(4);
    ASSERT_EQ(arm7.readReg(PC_REGISTER_NUM), MAIN_RAM_START + 24);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readReg(1), 255);
}
/**
 * @brief Tests a conditional Branch LE operation using an immediate.
 */
TEST_F(TestCPUBranchInstructions_B, BLE_IMMEDIATE) {
    // Test branch is executed.
    writeProgramToMemory(
        "MOV R1, #0\n"
        "MOV R0, #1\n"
        "CMP R0, #2\n"
        "Ble .-4\n"
        "MOV R1, #255",
        MAIN_RAM_START, &bus,
        arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(4);
    ASSERT_EQ(arm7.readReg(PC_REGISTER_NUM), MAIN_RAM_START + 12);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readReg(1), 0x0);

    arm7.reset();
    // Test branch is not executed.
    writeProgramToMemory(
        "MOV R1, #0\n"
        "MOV R0, #2\n"
        "CMP R0, #1\n"
        "Ble .-4\n"
        "MOV R1, #255",
        MAIN_RAM_START, &bus,
        arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(4);
    ASSERT_EQ(arm7.readReg(PC_REGISTER_NUM), MAIN_RAM_START + 24);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readReg(1), 255);
}

// ==================================================================================================
// Branch Link
// ==================================================================================================
class TestCPUBranchInstructions_BL : public TestCPUBranchInstructions {
protected:
    TestCPUBranchInstructions_BL() {}
    ~TestCPUBranchInstructions_BL() {}

    void SetUp() override { TestCPUBranchInstructions::SetUp(); }

    void TearDown() override { TestCPUBranchInstructions::TearDown(); }
};
/**
 * @brief Tests a Branch Link operation using an immediate.
 */
TEST_F(TestCPUBranchInstructions_BL, BL_IMMEDIATE) {
    writeProgramToMemory(
        "MOV R1, #0\n"
        "MOV R0, #0\n"
        "BL .-4",
        MAIN_RAM_START, &bus,
        arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(3);
    ASSERT_EQ(arm7.readReg(PC_REGISTER_NUM), MAIN_RAM_START + 8);
    ASSERT_EQ(arm7.readReg(LR_REGISTER_NUM), MAIN_RAM_START + 12);
}

// ==================================================================================================
// Branch Link and Exchange
// ==================================================================================================
class TestCPUBranchInstructions_BLX : public TestCPUBranchInstructions {
protected:
    TestCPUBranchInstructions_BLX() {}
    ~TestCPUBranchInstructions_BLX() {}

    void SetUp() override { TestCPUBranchInstructions::SetUp(); }

    void TearDown() override { TestCPUBranchInstructions::TearDown(); }
};
/**
 * @brief Tests a Branch Link and Exchange operation using an immediate.
 */
TEST_F(TestCPUBranchInstructions_BLX, BLX_IMMEDIATE) {
    writeProgramToMemory(
        "MOV R1, #0\n"
        "MOV R0, #0\n"
        "BLX .-4",
        MAIN_RAM_START, &bus,
        arm9.isARM7());
    arm9.setPC(MAIN_RAM_START);
    arm9.fetchAndExecute(3);
    ASSERT_EQ(arm9.readReg(PC_REGISTER_NUM), MAIN_RAM_START + 8);
    ASSERT_EQ(arm9.readReg(LR_REGISTER_NUM), MAIN_RAM_START + 12);
    ASSERT_EQ(arm9.readFlag(T_BIT), 1);
}
/**
 * @brief Tests a Branch Link and Exchange operation using an immediate not aligned to 4.
 */
TEST_F(TestCPUBranchInstructions_BLX, BLX_THUMB_ALIGN_IMMEDIATE) {
    writeProgramToMemory(
        "MOV R1, #0\n"
        "MOV R0, #0\n"
        "BLX .-2",
        MAIN_RAM_START, &bus,
        arm9.isARM7());
    arm9.setPC(MAIN_RAM_START);
    arm9.fetchAndExecute(3);
    ASSERT_EQ(arm9.readReg(PC_REGISTER_NUM), MAIN_RAM_START + 10);
    ASSERT_EQ(arm9.readReg(LR_REGISTER_NUM), MAIN_RAM_START + 12);
    ASSERT_EQ(arm9.readFlag(T_BIT), 1);
}