#include <gtest/gtest.h>

#include "commonTest.h"
#include "core/cpu.h"
#include "core/interconnect.h"
#include "core/memoryDefines.h"
#include "core/utils/armEncode.h"

using namespace RedPandaDS::Core;

/**
 * @brief Test fixture for the CPU's instructions in the branch category.
 */
class TestCPU_THUMB_BranchInstructions : public testing::Test {
protected:
    Interconnect bus;
    ARM7TDMI arm7;
    ARM946ES arm9;
    TestCPU_THUMB_BranchInstructions() {}
    ~TestCPU_THUMB_BranchInstructions() {}

    void SetUp() override {
        bus.init();
        bus.bindARM7(&arm7);
        bus.bindARM9(&arm9);

        arm7.setThumbMode(1);
        arm9.setThumbMode(1);
    }

    void TearDown() override {}
};

// ==================================================================================================
// Branch
// ==================================================================================================
class TestCPU_THUMB_BranchInstructions_B : public TestCPU_THUMB_BranchInstructions {
protected:
    TestCPU_THUMB_BranchInstructions_B() {}
    ~TestCPU_THUMB_BranchInstructions_B() {}

    void SetUp() override { TestCPU_THUMB_BranchInstructions::SetUp(); }

    void TearDown() override { TestCPU_THUMB_BranchInstructions::TearDown(); }
};
/**
 * @brief Tests a Branch operation using an immediate.
 */
TEST_F(TestCPU_THUMB_BranchInstructions_B, B_IMMEDIATE) {
    writeProgramToMemory(
        ".thumb\n"
        "B .\n"
        "MOV R1, #255",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readReg(PC_REGISTER_NUM), MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readReg(1), 0x0);
}

/**
 * @brief Tests a conditional Branch EQ operation using an immediate.
 */
TEST_F(TestCPU_THUMB_BranchInstructions_B, BEQ_IMMEDIATE) {
    // Test branch is executed.
    arm7.setFlag(Z_FLAG, 1);
    writeProgramToMemory(
        ".thumb\n"
        "Beq .\n"
        "MOV R1, #255",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readReg(PC_REGISTER_NUM), MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readReg(1), 0x0);

    // Test branch is not executed.
    arm7.setFlag(Z_FLAG, 0);
    writeProgramToMemory(
        ".thumb\n"
        "Beq .\n"
        "MOV R1, #255",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readReg(PC_REGISTER_NUM), MAIN_RAM_START + 6);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readReg(1), 255);
}
/**
 * @brief Tests a conditional Branch NE operation using an immediate.
 */
TEST_F(TestCPU_THUMB_BranchInstructions_B, BNE_IMMEDIATE) {
    // Test branch is executed.
    arm7.setFlag(Z_FLAG, 0);
    writeProgramToMemory(
        ".thumb\n"
        "Bne .\n"
        "MOV R1, #255",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readReg(PC_REGISTER_NUM), MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readReg(1), 0x0);

    // Test branch is not executed.
    arm7.setFlag(Z_FLAG, 1);
    writeProgramToMemory(
        ".thumb\n"
        "Bne .\n"
        "MOV R1, #255",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readReg(PC_REGISTER_NUM), MAIN_RAM_START + 6);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readReg(1), 255);
}
/**
 * @brief Tests a conditional Branch CS operation using an immediate.
 */
TEST_F(TestCPU_THUMB_BranchInstructions_B, BCS_IMMEDIATE) {
    // Test branch is executed.
    arm7.setFlag(C_FLAG, 1);
    writeProgramToMemory(
        ".thumb\n"
        "Bcs .\n"
        "MOV R1, #255",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readReg(PC_REGISTER_NUM), MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readReg(1), 0x0);

    // Test branch is not executed.
    arm7.setFlag(C_FLAG, 0);
    writeProgramToMemory(
        ".thumb\n"
        "Bcs .\n"
        "MOV R1, #255",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readReg(PC_REGISTER_NUM), MAIN_RAM_START + 6);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readReg(1), 255);
}
/**
 * @brief Tests a conditional Branch CC operation using an immediate.
 */
TEST_F(TestCPU_THUMB_BranchInstructions_B, BCC_IMMEDIATE) {
    // Test branch is executed.
    arm7.setFlag(C_FLAG, 0);
    writeProgramToMemory(
        ".thumb\n"
        "Bcc .\n"
        "MOV R1, #255",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readReg(PC_REGISTER_NUM), MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readReg(1), 0x0);

    // Test branch is not executed.
    arm7.setFlag(C_FLAG, 1);
    writeProgramToMemory(
        ".thumb\n"
        "Bcc .\n"
        "MOV R1, #255",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readReg(PC_REGISTER_NUM), MAIN_RAM_START + 6);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readReg(1), 255);
}
/**
 * @brief Tests a conditional Branch MI operation using an immediate.
 */
TEST_F(TestCPU_THUMB_BranchInstructions_B, BMI_IMMEDIATE) {
    // Test branch is executed.
    arm7.setFlag(N_FLAG, 1);
    writeProgramToMemory(
        ".thumb\n"
        "Bmi .\n"
        "MOV R1, #255",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readReg(PC_REGISTER_NUM), MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readReg(1), 0x0);

    // Test branch is not executed.
    arm7.setFlag(N_FLAG, 0);
    writeProgramToMemory(
        ".thumb\n"
        "Bmi .\n"
        "MOV R1, #255",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readReg(PC_REGISTER_NUM), MAIN_RAM_START + 6);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readReg(1), 255);
}
/**
 * @brief Tests a conditional Branch PL operation using an immediate.
 */
TEST_F(TestCPU_THUMB_BranchInstructions_B, BPL_IMMEDIATE) {
    // Test branch is executed.
    arm7.setFlag(N_FLAG, 0);
    writeProgramToMemory(
        ".thumb\n"
        "Bpl .\n"
        "MOV R1, #255",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readReg(PC_REGISTER_NUM), MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readReg(1), 0x0);

    // Test branch is not executed.
    arm7.setFlag(N_FLAG, 1);
    writeProgramToMemory(
        ".thumb\n"
        "Bpl .\n"
        "MOV R1, #255",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readReg(PC_REGISTER_NUM), MAIN_RAM_START + 6);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readReg(1), 255);
}
/**
 * @brief Tests a conditional Branch VS operation using an immediate.
 */
TEST_F(TestCPU_THUMB_BranchInstructions_B, BVS_IMMEDIATE) {
    // Test branch is executed.
    arm7.setFlag(V_FLAG, 1);
    writeProgramToMemory(
        ".thumb\n"
        "Bvs .\n"
        "MOV R1, #255",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readReg(PC_REGISTER_NUM), MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readReg(1), 0x0);

    // Test branch is not executed.
    arm7.setFlag(V_FLAG, 0);
    writeProgramToMemory(
        ".thumb\n"
        "Bvs .\n"
        "MOV R1, #255",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readReg(PC_REGISTER_NUM), MAIN_RAM_START + 6);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readReg(1), 255);
}
/**
 * @brief Tests a conditional Branch VC operation using an immediate.
 */
TEST_F(TestCPU_THUMB_BranchInstructions_B, BVC_IMMEDIATE) {
    // Test branch is executed.
    arm7.setFlag(V_FLAG, 0);
    writeProgramToMemory(
        ".thumb\n"
        "Bvc .\n"
        "MOV R1, #255",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readReg(PC_REGISTER_NUM), MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readReg(1), 0x0);

    // Test branch is not executed.
    arm7.setFlag(V_FLAG, 1);
    writeProgramToMemory(
        ".thumb\n"
        "Bvc .\n"
        "MOV R1, #255",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readReg(PC_REGISTER_NUM), MAIN_RAM_START + 6);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readReg(1), 255);
}
/**
 * @brief Tests a conditional Branch HI operation using an immediate.
 */
TEST_F(TestCPU_THUMB_BranchInstructions_B, BHI_IMMEDIATE) {
    // Test branch is executed.
    arm7.setFlag(C_FLAG, 1);
    arm7.setFlag(Z_FLAG, 0);
    writeProgramToMemory(
        ".thumb\n"
        "Bhi .\n"
        "MOV R1, #255",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readReg(PC_REGISTER_NUM), MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readReg(1), 0x0);

    // Test branch is not executed.
    arm7.setFlag(C_FLAG, 0);
    writeProgramToMemory(
        ".thumb\n"
        "Bhi .\n"
        "MOV R1, #255",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readReg(PC_REGISTER_NUM), MAIN_RAM_START + 6);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readReg(1), 255);
}
/**
 * @brief Tests a conditional Branch LS operation using an immediate.
 */
TEST_F(TestCPU_THUMB_BranchInstructions_B, BLS_IMMEDIATE) {
    // Test branch is executed.
    arm7.setFlag(C_FLAG, 0);
    writeProgramToMemory(
        ".thumb\n"
        "Bls .\n"
        "MOV R1, #255",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readReg(PC_REGISTER_NUM), MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readReg(1), 0x0);

    // Test branch is not executed.
    arm7.setFlag(C_FLAG, 1);
    arm7.setFlag(Z_FLAG, 0);
    writeProgramToMemory(
        ".thumb\n"
        "Bls .\n"
        "MOV R1, #255",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readReg(PC_REGISTER_NUM), MAIN_RAM_START + 6);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readReg(1), 255);
}
/**
 * @brief Tests a conditional Branch GE operation using an immediate.
 */
TEST_F(TestCPU_THUMB_BranchInstructions_B, BGE_IMMEDIATE) {
    // Test branch is executed.
    arm7.setFlag(N_FLAG, 0);
    writeProgramToMemory(
        ".thumb\n"
        "Bge .\n"
        "MOV R1, #255",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readReg(PC_REGISTER_NUM), MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readReg(1), 0x0);

    // Test branch is not executed.
    arm7.setFlag(N_FLAG, 1);
    writeProgramToMemory(
        ".thumb\n"
        "Bge .\n"
        "MOV R1, #255",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readReg(PC_REGISTER_NUM), MAIN_RAM_START + 6);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readReg(1), 255);
}
/**
 * @brief Tests a conditional Branch LT operation using an immediate.
 */
TEST_F(TestCPU_THUMB_BranchInstructions_B, BLT_IMMEDIATE) {
    // Test branch is executed.
    arm7.setFlag(N_FLAG, 1);
    writeProgramToMemory(
        ".thumb\n"
        "Blt .\n"
        "MOV R1, #255",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readReg(PC_REGISTER_NUM), MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readReg(1), 0x0);

    // Test branch is not executed.
    arm7.setFlag(N_FLAG, 0);
    writeProgramToMemory(
        ".thumb\n"
        "Blt .\n"
        "MOV R1, #255",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readReg(PC_REGISTER_NUM), MAIN_RAM_START + 6);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readReg(1), 255);
}
/**
 * @brief Tests a conditional Branch GT operation using an immediate.
 */
TEST_F(TestCPU_THUMB_BranchInstructions_B, BGT_IMMEDIATE) {
    // Test branch is executed.
    arm7.setFlag(Z_FLAG, 0);
    arm7.setFlag(N_FLAG, 0);
    writeProgramToMemory(
        ".thumb\n"
        "Bgt .\n"
        "MOV R1, #255",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readReg(PC_REGISTER_NUM), MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readReg(1), 0x0);

    // Test branch is not executed.
    arm7.setFlag(Z_FLAG, 1);
    writeProgramToMemory(
        ".thumb\n"
        "Bgt .\n"
        "MOV R1, #255",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readReg(PC_REGISTER_NUM), MAIN_RAM_START + 6);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readReg(1), 255);
}
/**
 * @brief Tests a conditional Branch LE operation using an immediate.
 */
TEST_F(TestCPU_THUMB_BranchInstructions_B, BLE_IMMEDIATE) {
    // Test branch is executed.
    arm7.setFlag(Z_FLAG, 1);
    writeProgramToMemory(
        ".thumb\n"
        "Ble .\n"
        "MOV R1, #255",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readReg(PC_REGISTER_NUM), MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readReg(1), 0x0);

    // Test branch is not executed.
    arm7.setFlag(Z_FLAG, 0);
    arm7.setFlag(N_FLAG, 0);
    writeProgramToMemory(
        ".thumb\n"
        "Ble .\n"
        "MOV R1, #255",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readReg(PC_REGISTER_NUM), MAIN_RAM_START + 6);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readReg(1), 255);
}
// ==================================================================================================
// Branch and Exchange
// ==================================================================================================
class TestCPU_THUMB_BranchInstructions_BX : public TestCPU_THUMB_BranchInstructions {
protected:
    TestCPU_THUMB_BranchInstructions_BX() {}
    ~TestCPU_THUMB_BranchInstructions_BX() {}

    void SetUp() override { TestCPU_THUMB_BranchInstructions::SetUp(); }

    void TearDown() override { TestCPU_THUMB_BranchInstructions::TearDown(); }
};
/**
 * @brief Tests a Branch and Exchange operation using a register to switch to ARM mode
 */
TEST_F(TestCPU_THUMB_BranchInstructions_BX, BX_ARM_REG) {
    uint32_t target = MAIN_RAM_START + 0x200;
    arm7.writeReg(3, target);
    arm7.setPC(MAIN_RAM_START);

    writeProgramToMemory(".thumb\nBX R3", MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readReg(PC_REGISTER_NUM), target);
    ASSERT_EQ(arm7.getThumbMode(), 0);
}
/**
 * @brief Tests a Branch and Exchange operation using a register to stay in THUMB mode.
 */
TEST_F(TestCPU_THUMB_BranchInstructions_BX, BX_THUMB_REG) {
    uint32_t target = MAIN_RAM_START + 0x200;
    arm7.writeReg(3, target | 1);
    arm7.setPC(MAIN_RAM_START);

    writeProgramToMemory(".thumb\nBX R3", MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readReg(PC_REGISTER_NUM), target);
    ASSERT_EQ(arm7.getThumbMode(), 1);
}
// ==================================================================================================
// Branch Link and Exchange
// ==================================================================================================
class TestCPU_THUMB_BranchInstructions_BLX : public TestCPU_THUMB_BranchInstructions {
protected:
    TestCPU_THUMB_BranchInstructions_BLX() {}
    ~TestCPU_THUMB_BranchInstructions_BLX() {}

    void SetUp() override { TestCPU_THUMB_BranchInstructions::SetUp(); }

    void TearDown() override { TestCPU_THUMB_BranchInstructions::TearDown(); }
};
/**
 * @brief Tests a Branch Link and Exchange operation using a register to switch to ARM mode
 */
TEST_F(TestCPU_THUMB_BranchInstructions_BLX, BLX_ARM_REG) {
    uint32_t target = MAIN_RAM_START + 0x200;
    arm9.writeReg(3, target);
    arm9.setPC(MAIN_RAM_START);

    writeProgramToMemory(".thumb\nBLX R3", MAIN_RAM_START, &bus, arm9.isARM7());
    arm9.fetchAndExecute(1);
    ASSERT_EQ(arm9.readReg(PC_REGISTER_NUM), target);
    ASSERT_EQ(arm9.readReg(LR_REGISTER_NUM), MAIN_RAM_START + 3);
    ASSERT_EQ(arm9.getThumbMode(), 0);
}
/**
 * @brief Tests a Branch Link and Exchange operation using a register to stay in THUMB mode.
 */
TEST_F(TestCPU_THUMB_BranchInstructions_BLX, BLX_THUMB_REG) {
    uint32_t target = MAIN_RAM_START + 0x200;
    arm9.writeReg(3, target | 1);
    arm9.setPC(MAIN_RAM_START);

    writeProgramToMemory(".thumb\nBLX R3", MAIN_RAM_START, &bus, arm9.isARM7());
    arm9.fetchAndExecute(1);
    ASSERT_EQ(arm9.readReg(PC_REGISTER_NUM), target);
    ASSERT_EQ(arm9.readReg(LR_REGISTER_NUM), MAIN_RAM_START + 3);
    ASSERT_EQ(arm9.getThumbMode(), 1);
}
