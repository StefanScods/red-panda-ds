#include <gtest/gtest.h>

#include "commonTest.h"
#include "core/cpu.h"
#include "core/interconnect.h"
#include "core/memoryDefines.h"
#include "core/utils/armEncode.h"

/**
 * @brief Test fixture for the CPU's instructions in the branch category.
 */
class TestCPU_ARM_BranchInstructions : public testing::Test {
protected:
    Interconnect bus;
    ARM7TDMI arm7;
    ARM946ES arm9;
    TestCPU_ARM_BranchInstructions() {}
    ~TestCPU_ARM_BranchInstructions() {}

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
class TestCPU_ARM_BranchInstructions_B : public TestCPU_ARM_BranchInstructions {
protected:
    TestCPU_ARM_BranchInstructions_B() {}
    ~TestCPU_ARM_BranchInstructions_B() {}

    void SetUp() override { TestCPU_ARM_BranchInstructions::SetUp(); }

    void TearDown() override { TestCPU_ARM_BranchInstructions::TearDown(); }
};
/**
 * @brief Tests a Branch operation using an immediate.
 */
TEST_F(TestCPU_ARM_BranchInstructions_B, B_IMMEDIATE) {
    writeProgramToMemory(
        "MOV R1, #0\n"
        "MOV R0, #0\n"
        "B .\n"
        "MOV R1, #255",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(3);
    ASSERT_EQ(arm7.readReg(PC_REGISTER_NUM), MAIN_RAM_START + 8);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readReg(1), 0x0);
}
/**
 * @brief Tests a conditional Branch EQ operation using an immediate.
 */
TEST_F(TestCPU_ARM_BranchInstructions_B, BEQ_IMMEDIATE) {
    // Test branch is executed.
    writeProgramToMemory(
        "MOV R1, #0\n"
        "MOVs R0, #0\n"
        "Beq .\n"
        "MOV R1, #255",
        MAIN_RAM_START, &bus, arm7.isARM7());
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
        "Beq .\n"
        "MOV R1, #255",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(3);
    ASSERT_EQ(arm7.readReg(PC_REGISTER_NUM), MAIN_RAM_START + 20);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readReg(1), 255);
}
/**
 * @brief Tests a conditional Branch NE operation using an immediate.
 */
TEST_F(TestCPU_ARM_BranchInstructions_B, BNE_IMMEDIATE) {
    // Test branch is executed.
    writeProgramToMemory(
        "MOV R1, #0\n"
        "MOVs R0, #1\n"
        "Bne .\n"
        "MOV R1, #255",
        MAIN_RAM_START, &bus, arm7.isARM7());
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
        "Bne .\n"
        "MOV R1, #255",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(3);
    ASSERT_EQ(arm7.readReg(PC_REGISTER_NUM), MAIN_RAM_START + 20);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readReg(1), 255);
}
/**
 * @brief Tests a conditional Branch CS operation using an immediate.
 */
TEST_F(TestCPU_ARM_BranchInstructions_B, BCS_IMMEDIATE) {
    // Test branch is executed.
    writeProgramToMemory(
        "MOV R1, #0\n"
        "MOV R0, #0\n"
        "CMP R0, #0\n"
        "Bcs .\n"
        "MOV R1, #255",
        MAIN_RAM_START, &bus, arm7.isARM7());
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
        "Bcs .\n"
        "MOV R1, #255",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(4);
    ASSERT_EQ(arm7.readReg(PC_REGISTER_NUM), MAIN_RAM_START + 24);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readReg(1), 255);
}
/**
 * @brief Tests a conditional Branch CC operation using an immediate.
 */
TEST_F(TestCPU_ARM_BranchInstructions_B, BCC_IMMEDIATE) {
    // Test branch is executed.
    writeProgramToMemory(
        "MOV R1, #0\n"
        "MOV R0, #0\n"
        "CMP R0, #1\n"
        "Bcc .\n"
        "MOV R1, #255",
        MAIN_RAM_START, &bus, arm7.isARM7());
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
        "Bcc .\n"
        "MOV R1, #255",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(4);
    ASSERT_EQ(arm7.readReg(PC_REGISTER_NUM), MAIN_RAM_START + 24);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readReg(1), 255);
}
/**
 * @brief Tests a conditional Branch MI operation using an immediate.
 */
TEST_F(TestCPU_ARM_BranchInstructions_B, BMI_IMMEDIATE) {
    // Test branch is executed.
    writeProgramToMemory(
        "MOV R1, #0\n"
        "MOV R0, #0\n"
        "CMP R0, #1\n"
        "Bmi .\n"
        "MOV R1, #255",
        MAIN_RAM_START, &bus, arm7.isARM7());
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
        "Bmi .\n"
        "MOV R1, #255",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(4);
    ASSERT_EQ(arm7.readReg(PC_REGISTER_NUM), MAIN_RAM_START + 24);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readReg(1), 255);
}
/**
 * @brief Tests a conditional Branch PL operation using an immediate.
 */
TEST_F(TestCPU_ARM_BranchInstructions_B, BPL_IMMEDIATE) {
    // Test branch is executed.
    writeProgramToMemory(
        "MOV R1, #0\n"
        "MOV R0, #1\n"
        "CMP R0, #0\n"
        "Bpl .\n"
        "MOV R1, #255",
        MAIN_RAM_START, &bus, arm7.isARM7());
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
        "Bpl .\n"
        "MOV R1, #255",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(4);
    ASSERT_EQ(arm7.readReg(PC_REGISTER_NUM), MAIN_RAM_START + 24);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readReg(1), 255);
}
/**
 * @brief Tests a conditional Branch VS operation using an immediate.
 */
TEST_F(TestCPU_ARM_BranchInstructions_B, BVS_IMMEDIATE) {
    // Test branch is executed.
    writeProgramToMemory(
        "MOV R1, #0\n"
        "MOV R0, #0x7FFFFFFF\n"
        "ADDS R0, R0, #1\n"
        "Bvs .\n"
        "MOV R1, #255",
        MAIN_RAM_START, &bus, arm7.isARM7());
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
        "Bvs .\n"
        "MOV R1, #255",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(4);
    ASSERT_EQ(arm7.readReg(PC_REGISTER_NUM), MAIN_RAM_START + 24);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readReg(1), 255);
}
/**
 * @brief Tests a conditional Branch VC operation using an immediate.
 */
TEST_F(TestCPU_ARM_BranchInstructions_B, BVC_IMMEDIATE) {
    // Test branch is executed.
    writeProgramToMemory(
        "MOV R1, #0\n"
        "MOV R0, #1\n"
        "ADDS R0, R0, #1\n"
        "Bvc .\n"
        "MOV R1, #255",
        MAIN_RAM_START, &bus, arm7.isARM7());
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
        "Bvc .\n"
        "MOV R1, #255",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(4);
    ASSERT_EQ(arm7.readReg(PC_REGISTER_NUM), MAIN_RAM_START + 24);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readReg(1), 255);
}
/**
 * @brief Tests a conditional Branch HI operation using an immediate.
 */
TEST_F(TestCPU_ARM_BranchInstructions_B, BHI_IMMEDIATE) {
    // Test branch is executed.
    writeProgramToMemory(
        "MOV R1, #0\n"
        "MOV R0, #2\n"
        "CMP R0, #1\n"
        "Bhi .\n"
        "MOV R1, #255",
        MAIN_RAM_START, &bus, arm7.isARM7());
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
        "Bhi .\n"
        "MOV R1, #255",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(4);
    ASSERT_EQ(arm7.readReg(PC_REGISTER_NUM), MAIN_RAM_START + 24);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readReg(1), 255);
}
/**
 * @brief Tests a conditional Branch LS operation using an immediate.
 */
TEST_F(TestCPU_ARM_BranchInstructions_B, BLS_IMMEDIATE) {
    // Test branch is executed.
    writeProgramToMemory(
        "MOV R1, #0\n"
        "MOV R0, #1\n"
        "CMP R0, #2\n"
        "Bls .\n"
        "MOV R1, #255",
        MAIN_RAM_START, &bus, arm7.isARM7());
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
        "Bls .\n"
        "MOV R1, #255",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(4);
    ASSERT_EQ(arm7.readReg(PC_REGISTER_NUM), MAIN_RAM_START + 24);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readReg(1), 255);
}
/**
 * @brief Tests a conditional Branch GE operation using an immediate.
 */
TEST_F(TestCPU_ARM_BranchInstructions_B, BGE_IMMEDIATE) {
    // Test branch is executed.
    writeProgramToMemory(
        "MOV R1, #0\n"
        "MOV R0, #1\n"
        "CMP R0, #0\n"
        "Bge .\n"
        "MOV R1, #255",
        MAIN_RAM_START, &bus, arm7.isARM7());
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
        "Bge .\n"
        "MOV R1, #255",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(4);
    ASSERT_EQ(arm7.readReg(PC_REGISTER_NUM), MAIN_RAM_START + 24);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readReg(1), 255);
}
/**
 * @brief Tests a conditional Branch LT operation using an immediate.
 */
TEST_F(TestCPU_ARM_BranchInstructions_B, BLT_IMMEDIATE) {
    // Test branch is executed.
    writeProgramToMemory(
        "MOV R1, #0\n"
        "MOV R0, #0\n"
        "CMP R0, #1\n"
        "Blt .\n"
        "MOV R1, #255",
        MAIN_RAM_START, &bus, arm7.isARM7());
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
        "Blt .\n"
        "MOV R1, #255",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(4);
    ASSERT_EQ(arm7.readReg(PC_REGISTER_NUM), MAIN_RAM_START + 24);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readReg(1), 255);
}
/**
 * @brief Tests a conditional Branch GT operation using an immediate.
 */
TEST_F(TestCPU_ARM_BranchInstructions_B, BGT_IMMEDIATE) {
    // Test branch is executed.
    writeProgramToMemory(
        "MOV R1, #0\n"
        "MOV R0, #2\n"
        "CMP R0, #1\n"
        "Bgt .\n"
        "MOV R1, #255",
        MAIN_RAM_START, &bus, arm7.isARM7());
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
        "Bgt .\n"
        "MOV R1, #255",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(4);
    ASSERT_EQ(arm7.readReg(PC_REGISTER_NUM), MAIN_RAM_START + 24);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readReg(1), 255);
}
/**
 * @brief Tests a conditional Branch LE operation using an immediate.
 */
TEST_F(TestCPU_ARM_BranchInstructions_B, BLE_IMMEDIATE) {
    // Test branch is executed.
    writeProgramToMemory(
        "MOV R1, #0\n"
        "MOV R0, #1\n"
        "CMP R0, #2\n"
        "Ble .\n"
        "MOV R1, #255",
        MAIN_RAM_START, &bus, arm7.isARM7());
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
        "Ble .\n"
        "MOV R1, #255",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(4);
    ASSERT_EQ(arm7.readReg(PC_REGISTER_NUM), MAIN_RAM_START + 24);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readReg(1), 255);
}

// ==================================================================================================
// Branch Link
// ==================================================================================================
class TestCPU_ARM_BranchInstructions_BL : public TestCPU_ARM_BranchInstructions {
protected:
    TestCPU_ARM_BranchInstructions_BL() {}
    ~TestCPU_ARM_BranchInstructions_BL() {}

    void SetUp() override { TestCPU_ARM_BranchInstructions::SetUp(); }

    void TearDown() override { TestCPU_ARM_BranchInstructions::TearDown(); }
};
/**
 * @brief Tests a Branch Link operation using an immediate.
 */
TEST_F(TestCPU_ARM_BranchInstructions_BL, BL_IMMEDIATE) {
    writeProgramToMemory(
        "MOV R1, #0\n"
        "MOV R0, #0\n"
        "BL .",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(3);
    ASSERT_EQ(arm7.readReg(PC_REGISTER_NUM), MAIN_RAM_START + 8);
    ASSERT_EQ(arm7.readReg(LR_REGISTER_NUM), MAIN_RAM_START + 12);
}

// ==================================================================================================
// Branch Exchange
// ==================================================================================================
class TestCPU_ARM_BranchInstructions_BX : public TestCPU_ARM_BranchInstructions {
protected:
    TestCPU_ARM_BranchInstructions_BX() {}
    ~TestCPU_ARM_BranchInstructions_BX() {}

    void SetUp() override { TestCPU_ARM_BranchInstructions::SetUp(); }

    void TearDown() override { TestCPU_ARM_BranchInstructions::TearDown(); }
};
/**
 * @brief Tests a Branch Exchange operation using a register.
 */
TEST_F(TestCPU_ARM_BranchInstructions_BX, BX_REG) {
    uint32_t target = MAIN_RAM_START + 0x200;
    arm9.writeReg(3, target);
    arm9.setPC(MAIN_RAM_START);

    writeProgramToMemory("BX R3", MAIN_RAM_START, &bus, arm9.isARM7());
    arm9.fetchAndExecute(1);
    ASSERT_EQ(arm9.readReg(PC_REGISTER_NUM), target);
    ASSERT_EQ(arm9.getThumbMode(), 0);
}
/**
 * @brief Tests a Branch Exchange operation using a register to THUMB mode.
 */
TEST_F(TestCPU_ARM_BranchInstructions_BX, BX_REG_TO_THUMB) {
    uint32_t target = MAIN_RAM_START + 0x200;
    arm9.writeReg(3, target | 1);
    arm9.setPC(MAIN_RAM_START);

    writeProgramToMemory("BX R3", MAIN_RAM_START, &bus, arm9.isARM7());
    arm9.fetchAndExecute(1);
    ASSERT_EQ(arm9.readReg(PC_REGISTER_NUM), target);
    ASSERT_EQ(arm9.getThumbMode(), 1);
}
// ==================================================================================================
// Branch Link and Exchange
// ==================================================================================================
class TestCPU_ARM_BranchInstructions_BLX : public TestCPU_ARM_BranchInstructions {
protected:
    TestCPU_ARM_BranchInstructions_BLX() {}
    ~TestCPU_ARM_BranchInstructions_BLX() {}

    void SetUp() override { TestCPU_ARM_BranchInstructions::SetUp(); }

    void TearDown() override { TestCPU_ARM_BranchInstructions::TearDown(); }
};
/**
 * @brief Tests a Branch Link and Exchange operation using an immediate.
 */
TEST_F(TestCPU_ARM_BranchInstructions_BLX, BLX_IMMEDIATE) {
    writeProgramToMemory(
        "MOV R1, #0\n"
        "MOV R0, #0\n"
        "BLX .",
        MAIN_RAM_START, &bus, arm9.isARM7());
    arm9.setPC(MAIN_RAM_START);
    arm9.fetchAndExecute(3);
    ASSERT_EQ(arm9.readReg(PC_REGISTER_NUM), MAIN_RAM_START + 8);
    ASSERT_EQ(arm9.readReg(LR_REGISTER_NUM), MAIN_RAM_START + 12);
    ASSERT_EQ(arm9.getThumbMode(), 1);
}
/**
 * @brief Tests a Branch Link and Exchange operation using an immediate not aligned to 4.
 */
TEST_F(TestCPU_ARM_BranchInstructions_BLX, BLX_THUMB_ALIGN_IMMEDIATE) {
    writeProgramToMemory(
        "MOV R1, #0\n"
        "MOV R0, #0\n"
        "BLX .+2",
        MAIN_RAM_START, &bus, arm9.isARM7());
    arm9.setPC(MAIN_RAM_START);
    arm9.fetchAndExecute(3);
    ASSERT_EQ(arm9.readReg(PC_REGISTER_NUM), MAIN_RAM_START + 10);
    ASSERT_EQ(arm9.readReg(LR_REGISTER_NUM), MAIN_RAM_START + 12);
    ASSERT_EQ(arm9.getThumbMode(), 1);
}
/**
 * @brief Tests a Branch Link and Exchange operation using a register.
 */
TEST_F(TestCPU_ARM_BranchInstructions_BLX, BLX_REG) {
    uint32_t target = MAIN_RAM_START + 0x200;
    arm9.writeReg(3, target);
    arm9.setPC(MAIN_RAM_START);

    writeProgramToMemory("BLX R3", MAIN_RAM_START, &bus, arm9.isARM7());
    arm9.fetchAndExecute(1);
    ASSERT_EQ(arm9.readReg(PC_REGISTER_NUM), target);
    ASSERT_EQ(arm9.readReg(LR_REGISTER_NUM), MAIN_RAM_START + 4);
    ASSERT_EQ(arm9.getThumbMode(), 0);
}
/**
 * @brief Tests a Branch Link and Exchange operation using a register not aligned to 4.
 */
TEST_F(TestCPU_ARM_BranchInstructions_BLX, BLX_THUMB_ALIGN_REG) {
    uint32_t target = MAIN_RAM_START + 0x200;
    arm9.writeReg(3, target | 1);
    arm9.setPC(MAIN_RAM_START);

    writeProgramToMemory("BLX R3", MAIN_RAM_START, &bus, arm9.isARM7());
    arm9.fetchAndExecute(1);
    ASSERT_EQ(arm9.readReg(PC_REGISTER_NUM), target);
    ASSERT_EQ(arm9.readReg(LR_REGISTER_NUM), MAIN_RAM_START + 4);
    ASSERT_EQ(arm9.getThumbMode(), 1);
}
