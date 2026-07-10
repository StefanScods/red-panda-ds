#include <gtest/gtest.h>

#include "commonTest.h"
#include "core/cpu.h"
#include "core/interconnect.h"
#include "core/memoryDefines.h"
#include "core/utils/armEncode.h"

using namespace RedPandaDS::Core;

/**
 * @brief Test fixture for the CPU's instructions in the data category.
 */
class TestCPU_THUMB_DataInstructions : public testing::Test {
protected:
    Interconnect bus;
    ARM7TDMI arm7{new NDS_ARM7_BIOS()};
    ARM946ES arm9{new NDS_ARM9_BIOS()};
    TestCPU_THUMB_DataInstructions() {}
    ~TestCPU_THUMB_DataInstructions() {}

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
// MOV
// ==================================================================================================
class TestCPU_THUMB_DataInstructions_MOV : public TestCPU_THUMB_DataInstructions {
protected:
    TestCPU_THUMB_DataInstructions_MOV() {}
    ~TestCPU_THUMB_DataInstructions_MOV() {}

    void SetUp() override { TestCPU_THUMB_DataInstructions::SetUp(); }
    void TearDown() override { TestCPU_THUMB_DataInstructions::TearDown(); }
};
/**
 * @brief Test MOV with small number regs.
 */
TEST_F(TestCPU_THUMB_DataInstructions_MOV, MOV_REG_SMALL) {
    arm7.writeReg(7, 0xFFFFFFFF);
    writeProgramToMemory(
        ".thumb\n"
        "MOV R3, R7\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_EQ(arm7.readReg(3), 0xFFFFFFFF);
}
/**
 * @brief Test MOV with large number regs.
 */
TEST_F(TestCPU_THUMB_DataInstructions_MOV, MOV_REG_LARGE) {
    arm7.writeReg(10, 0xFFFFFFFF);
    writeProgramToMemory(
        ".thumb\n"
        "MOV R14, R10\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_EQ(arm7.readReg(14), 0xFFFFFFFF);
}
/**
 * @brief Test that MOV with large number regs does not edit flags.
 */
TEST_F(TestCPU_THUMB_DataInstructions_MOV, MOV_REG_LARGE_NO_FLAG_CHANGE) {
    arm7.writeReg(10, 0);
    arm7.setFlag(Z_FLAG, 1);
    writeProgramToMemory(
        ".thumb\n"
        "MOVs R14, R10\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_EQ(arm7.readReg(14), 0);
    EXPECT_TRUE(arm7.readFlag(Z_FLAG));
}
/**
 * @brief Test MOV with an immediate value.
 */
TEST_F(TestCPU_THUMB_DataInstructions_MOV, MOV_IMM) {
    writeProgramToMemory(
        ".thumb\n"
        "MOVs R3, #0xFF\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_EQ(arm7.readReg(3), 0xFF);
}
/**
 * @brief Test MOV's zero flag behaviour.
 */
TEST_F(TestCPU_THUMB_DataInstructions_MOV, MOV_ZERO_FLAG) {
    // Z == 1.
    arm7.setFlag(Z_FLAG, 0);
    writeProgramToMemory(
        ".thumb\n"
        "MOVS R3, #0\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_EQ(arm7.readReg(3), 0);
    EXPECT_TRUE(arm7.readFlag(Z_FLAG));

    // Z == 0.
    arm7.setFlag(Z_FLAG, 1);
    writeProgramToMemory(
        ".thumb\n"
        "MOVS R3, #1\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_EQ(arm7.readReg(3), 1);
    EXPECT_FALSE(arm7.readFlag(Z_FLAG));
}
/**
 * @brief Test MOV's negative flag behaviour.
 */
TEST_F(TestCPU_THUMB_DataInstructions_MOV, MOV_NEGATIVE_FLAG) {
    // N == 0.
    arm7.setFlag(N_FLAG, 1);
    writeProgramToMemory(
        ".thumb\n"
        "MOVS R3, #0x0\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_EQ(arm7.readReg(3), 0x0);
    EXPECT_FALSE(arm7.readFlag(N_FLAG));
}

/**
 * @brief Test MOV's carry flag behaviour.
 */
TEST_F(TestCPU_THUMB_DataInstructions_MOV, MOV_CARRY_FLAG) {
    // C == 1.
    arm7.setFlag(C_FLAG, 1);
    writeProgramToMemory(
        ".thumb\n"
        "MOVS R3, #0x01\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_EQ(arm7.readReg(3), 0x01);
    EXPECT_TRUE(arm7.readFlag(C_FLAG));  // Flag is unchanged.

    // C == 0.
    arm7.setFlag(C_FLAG, 0);
    writeProgramToMemory(
        ".thumb\n"
        "MOVS R3, #0x01\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_EQ(arm7.readReg(3), 0x01);
    EXPECT_FALSE(arm7.readFlag(C_FLAG));  // Flag is unchanged.
}

/**
 * @brief Test MOV's overflow flag behaviour.
 */
TEST_F(TestCPU_THUMB_DataInstructions_MOV, MOV_OVERFLOW_FLAG) {
    // V == 1.
    arm7.setFlag(V_FLAG, 1);
    writeProgramToMemory(
        ".thumb\n"
        "MOVS R3, #0x01\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_EQ(arm7.readReg(3), 0x01);
    EXPECT_TRUE(arm7.readFlag(V_FLAG));  // Flag is unchanged.

    // V == 0.
    arm7.setFlag(V_FLAG, 0);
    writeProgramToMemory(
        ".thumb\n"
        "MOVS R3, #0x01\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_EQ(arm7.readReg(3), 0x01);
    EXPECT_FALSE(arm7.readFlag(V_FLAG));  // Flag is unchanged.
}
// ==================================================================================================
// AND
// ==================================================================================================
class TestCPU_THUMB_DataInstructions_AND : public TestCPU_THUMB_DataInstructions {
protected:
    TestCPU_THUMB_DataInstructions_AND() {}
    ~TestCPU_THUMB_DataInstructions_AND() {}

    void SetUp() override { TestCPU_THUMB_DataInstructions::SetUp(); }
    void TearDown() override { TestCPU_THUMB_DataInstructions::TearDown(); }
};
/**
 * @brief Test AND with small-number regs.
 */
TEST_F(TestCPU_THUMB_DataInstructions_AND, AND_REG_SMALL) {
    arm7.writeReg(1, 0xFFFFFFFF);
    arm7.writeReg(2, 0x0F0F0F0F);

    writeProgramToMemory(
        ".thumb\n"
        "AND R1, R2\n",  // R1 := R1 & R2
        MAIN_RAM_START, &bus, arm7.isARM7());

    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);

    EXPECT_EQ(arm7.readReg(1), 0x0F0F0F0F);
}
/**
 * @brief Test AND where result is zero.
 */
TEST_F(TestCPU_THUMB_DataInstructions_AND, AND_REG_ZERO_RESULT) {
    arm7.writeReg(3, 0x00FF00FF);
    arm7.writeReg(4, 0xFF00FF00);

    writeProgramToMemory(
        ".thumb\n"
        "AND R3, R4\n",
        MAIN_RAM_START, &bus, arm7.isARM7());

    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);

    EXPECT_EQ(arm7.readReg(3), 0x00000000);
}
/**
 * @brief Test AND's zero flag behaviour.
 */
TEST_F(TestCPU_THUMB_DataInstructions_AND, AND_ZERO_FLAG) {
    // Z == 1.
    arm7.setFlag(Z_FLAG, 0);
    arm7.writeReg(1, 0x00000000);
    arm7.writeReg(2, 0xFFFFFFFF);
    writeProgramToMemory(
        ".thumb\n"
        "AND R1, R2\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_EQ(arm7.readReg(1), 0);
    EXPECT_TRUE(arm7.readFlag(Z_FLAG));

    // Z == 0.
    arm7.setFlag(Z_FLAG, 1);
    arm7.writeReg(1, 0xFFFFFFFF);
    arm7.writeReg(2, 0x00000001);
    writeProgramToMemory(
        ".thumb\n"
        "AND R1, R2\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_EQ(arm7.readReg(1), 1);
    EXPECT_FALSE(arm7.readFlag(Z_FLAG));
}
/**
 * @brief Test AND's negative flag behaviour.
 */
TEST_F(TestCPU_THUMB_DataInstructions_AND, AND_NEGATIVE_FLAG) {
    // N == 0.
    arm7.setFlag(N_FLAG, 1);
    arm7.writeReg(3, 0x7FFFFFFF);  // MSB=0
    arm7.writeReg(4, 0xFFFFFFFF);
    writeProgramToMemory(
        ".thumb\n"
        "AND R3, R4\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_EQ(arm7.readReg(3), 0x7FFFFFFF);
    EXPECT_FALSE(arm7.readFlag(N_FLAG));

    // N == 1.
    arm7.setFlag(N_FLAG, 0);
    arm7.writeReg(3, 0x80000000);  // MSB=1
    arm7.writeReg(4, 0xFFFFFFFF);
    writeProgramToMemory(
        ".thumb\n"
        "AND R3, R4\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_EQ(arm7.readReg(3), 0x80000000);
    EXPECT_TRUE(arm7.readFlag(N_FLAG));
}
/**
 * @brief Test AND's carry flag behaviour.
 */
TEST_F(TestCPU_THUMB_DataInstructions_AND, AND_CARRY_FLAG) {
    // C == 1.
    arm7.setFlag(C_FLAG, 1);
    arm7.writeReg(5, 0xFFFFFFFF);
    arm7.writeReg(6, 0x00000001);
    writeProgramToMemory(
        ".thumb\n"
        "AND R5, R6\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_EQ(arm7.readReg(5), 0x00000001);
    EXPECT_TRUE(arm7.readFlag(C_FLAG));  // Flag is unchanged.

    // C == 0.
    arm7.setFlag(C_FLAG, 0);
    arm7.writeReg(5, 0xFFFFFFFF);
    arm7.writeReg(6, 0x00000001);
    writeProgramToMemory(
        ".thumb\n"
        "AND R5, R6\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_EQ(arm7.readReg(5), 0x00000001);
    EXPECT_FALSE(arm7.readFlag(C_FLAG));  // Flag is unchanged.
}
/**
 * @brief Test AND's overflow flag behaviour.
 */
TEST_F(TestCPU_THUMB_DataInstructions_AND, AND_OVERFLOW_FLAG) {
    // V == 1.
    arm7.setFlag(V_FLAG, 1);
    arm7.writeReg(5, 0xFFFFFFFF);
    arm7.writeReg(6, 0x00000001);
    writeProgramToMemory(
        ".thumb\n"
        "AND R5, R6\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_EQ(arm7.readReg(5), 0x00000001);
    EXPECT_TRUE(arm7.readFlag(V_FLAG));  // Flag is unchanged.

    // V == 0.
    arm7.setFlag(V_FLAG, 0);
    arm7.writeReg(5, 0xFFFFFFFF);
    arm7.writeReg(6, 0x00000001);
    writeProgramToMemory(
        ".thumb\n"
        "AND R5, R6\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_EQ(arm7.readReg(5), 0x00000001);
    EXPECT_FALSE(arm7.readFlag(V_FLAG));  // Flag is unchanged.
}
// ==================================================================================================
// EOR
// ==================================================================================================
class TestCPU_THUMB_DataInstructions_EOR : public TestCPU_THUMB_DataInstructions {
protected:
    TestCPU_THUMB_DataInstructions_EOR() {}
    ~TestCPU_THUMB_DataInstructions_EOR() {}

    void SetUp() override { TestCPU_THUMB_DataInstructions::SetUp(); }
    void TearDown() override { TestCPU_THUMB_DataInstructions::TearDown(); }
};
/**
 * @brief Test EOR with small-number regs.
 */
TEST_F(TestCPU_THUMB_DataInstructions_EOR, EOR_REG_SMALL) {
    arm7.writeReg(1, 0xFFFFFFFF);
    arm7.writeReg(2, 0x0F0F0F0F);

    writeProgramToMemory(
        ".thumb\n"
        "EOR R1, R2\n",  // R1 := R1 ^ R2
        MAIN_RAM_START, &bus, arm7.isARM7());

    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);

    EXPECT_EQ(arm7.readReg(1), 0xF0F0F0F0);
}
/**
 * @brief Test EOR where result is zero.
 */
TEST_F(TestCPU_THUMB_DataInstructions_EOR, EOR_REG_ZERO_RESULT) {
    arm7.writeReg(3, 0x00FF00FF);
    arm7.writeReg(4, 0x00FF00FF);

    writeProgramToMemory(
        ".thumb\n"
        "EOR R3, R4\n",
        MAIN_RAM_START, &bus, arm7.isARM7());

    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);

    EXPECT_EQ(arm7.readReg(3), 0x00000000);
}
/**
 * @brief Test EOR's zero flag behaviour.
 */
TEST_F(TestCPU_THUMB_DataInstructions_EOR, EOR_ZERO_FLAG) {
    // Z == 1.
    arm7.setFlag(Z_FLAG, 0);
    arm7.writeReg(1, 0xFFFFFFFF);
    arm7.writeReg(2, 0xFFFFFFFF);
    writeProgramToMemory(
        ".thumb\n"
        "EOR R1, R2\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_EQ(arm7.readReg(1), 0);
    EXPECT_TRUE(arm7.readFlag(Z_FLAG));

    // Z == 0.
    arm7.setFlag(Z_FLAG, 1);
    arm7.writeReg(1, 0xFFFFFFFF);
    arm7.writeReg(2, 0xFFFFFFFE);
    writeProgramToMemory(
        ".thumb\n"
        "EOR R1, R2\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_EQ(arm7.readReg(1), 1);
    EXPECT_FALSE(arm7.readFlag(Z_FLAG));
}
/**
 * @brief Test EOR's negative flag behaviour.
 */
TEST_F(TestCPU_THUMB_DataInstructions_EOR, EOR_NEGATIVE_FLAG) {
    // N == 0.
    arm7.setFlag(N_FLAG, 1);
    arm7.writeReg(3, 0x7FFFFFFF);  // MSB=0
    arm7.writeReg(4, 0xFFFFFFFF);
    writeProgramToMemory(
        ".thumb\n"
        "EOR R3, R4\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_EQ(arm7.readReg(3), 0x80000000);
    EXPECT_TRUE(arm7.readFlag(N_FLAG));

    // N == 1.
    arm7.setFlag(N_FLAG, 0);
    arm7.writeReg(3, 0x80000000);  // MSB=1
    arm7.writeReg(4, 0xFFFFFFFF);
    writeProgramToMemory(
        ".thumb\n"
        "EOR R3, R4\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_EQ(arm7.readReg(3), 0x7FFFFFFF);
    EXPECT_FALSE(arm7.readFlag(N_FLAG));
}
/**
 * @brief Test EOR's carry flag behaviour.
 */
TEST_F(TestCPU_THUMB_DataInstructions_EOR, EOR_CARRY_FLAG) {
    // C == 1.
    arm7.setFlag(C_FLAG, 1);
    arm7.writeReg(5, 0xFFFFFFFF);
    arm7.writeReg(6, 0x00000001);
    writeProgramToMemory(
        ".thumb\n"
        "EOR R5, R6\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_EQ(arm7.readReg(5), 0xFFFFFFFE);
    EXPECT_TRUE(arm7.readFlag(C_FLAG));  // Flag is unchanged.

    // C == 0.
    arm7.setFlag(C_FLAG, 0);
    arm7.writeReg(5, 0xFFFFFFFF);
    arm7.writeReg(6, 0x00000001);
    writeProgramToMemory(
        ".thumb\n"
        "EOR R5, R6\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_EQ(arm7.readReg(5), 0xFFFFFFFE);
    EXPECT_FALSE(arm7.readFlag(C_FLAG));  // Flag is unchanged.
}
/**
 * @brief Test EOR's overflow flag behaviour.
 */
TEST_F(TestCPU_THUMB_DataInstructions_EOR, EOR_OVERFLOW_FLAG) {
    // V == 1.
    arm7.setFlag(V_FLAG, 1);
    arm7.writeReg(5, 0xFFFFFFFF);
    arm7.writeReg(6, 0x00000001);
    writeProgramToMemory(
        ".thumb\n"
        "EOR R5, R6\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_EQ(arm7.readReg(5), 0xFFFFFFFE);
    EXPECT_TRUE(arm7.readFlag(V_FLAG));  // Flag is unchanged.

    // V == 0.
    arm7.setFlag(V_FLAG, 0);
    arm7.writeReg(5, 0xFFFFFFFF);
    arm7.writeReg(6, 0x00000001);
    writeProgramToMemory(
        ".thumb\n"
        "EOR R5, R6\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_EQ(arm7.readReg(5), 0xFFFFFFFE);
    EXPECT_FALSE(arm7.readFlag(V_FLAG));  // Flag is unchanged.
}
// ==================================================================================================
// LSL
// ==================================================================================================
class TestCPU_THUMB_DataInstructions_LSL : public TestCPU_THUMB_DataInstructions {
protected:
    TestCPU_THUMB_DataInstructions_LSL() {}
    ~TestCPU_THUMB_DataInstructions_LSL() {}

    void SetUp() override { TestCPU_THUMB_DataInstructions::SetUp(); }
    void TearDown() override { TestCPU_THUMB_DataInstructions::TearDown(); }
};
/**
 * @brief Test LSL with small-number regs.
 */
TEST_F(TestCPU_THUMB_DataInstructions_LSL, LSL_REG_SMALL) {
    arm7.writeReg(1, 0x00000001);
    arm7.writeReg(2, 4);

    writeProgramToMemory(
        ".thumb\n"
        "LSL R1, R2\n",
        MAIN_RAM_START, &bus, arm7.isARM7());

    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);

    EXPECT_EQ(arm7.readReg(1), 0x00000010);
}
/**
 * @brief Test LSL where result is zero.
 */
TEST_F(TestCPU_THUMB_DataInstructions_LSL, LSL_REG_ZERO_RESULT) {
    arm7.writeReg(3, 0x00000000);
    arm7.writeReg(4, 8);

    writeProgramToMemory(
        ".thumb\n"
        "LSL R3, R4\n",
        MAIN_RAM_START, &bus, arm7.isARM7());

    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);

    EXPECT_EQ(arm7.readReg(3), 0x00000000);
}
/**
 * @brief Test LSL immediate.
 */
TEST_F(TestCPU_THUMB_DataInstructions_LSL, LSL_IMM) {
    writeProgramToMemory(
        ".thumb\n"
        "LSL R1, #4\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.writeReg(1, 0x00000001);

    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);

    EXPECT_EQ(arm7.readReg(1), 0x00000010);
}
/**
 * @brief Test LSL's zero flag behaviour.
 */
TEST_F(TestCPU_THUMB_DataInstructions_LSL, LSL_ZERO_FLAG) {
    // Z == 1.
    arm7.setFlag(Z_FLAG, 0);
    arm7.writeReg(1, 0x00000000);
    arm7.writeReg(2, 1);
    writeProgramToMemory(
        ".thumb\n"
        "LSL R1, R2\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_EQ(arm7.readReg(1), 0);
    EXPECT_TRUE(arm7.readFlag(Z_FLAG));

    // Z == 0.
    arm7.setFlag(Z_FLAG, 1);
    arm7.writeReg(1, 0x00000001);
    arm7.writeReg(2, 2);
    writeProgramToMemory(
        ".thumb\n"
        "LSL R1, R2\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_EQ(arm7.readReg(1), 0x00000004);
    EXPECT_FALSE(arm7.readFlag(Z_FLAG));
}
/**
 * @brief Test LSL's negative flag behaviour.
 */
TEST_F(TestCPU_THUMB_DataInstructions_LSL, LSL_NEGATIVE_FLAG) {
    // N == 0.
    arm7.setFlag(N_FLAG, 1);
    arm7.writeReg(3, 0x40000000);  // shift brings 0 to MSB
    arm7.writeReg(4, 1);
    writeProgramToMemory(
        ".thumb\n"
        "LSL R3, R4\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_EQ(arm7.readReg(3), 0x80000000);
    EXPECT_TRUE(arm7.readFlag(N_FLAG));

    // N == 1.
    arm7.setFlag(N_FLAG, 0);
    arm7.writeReg(3, 0x20000000);  // shift brings 0 to MSB
    arm7.writeReg(4, 2);
    writeProgramToMemory(
        ".thumb\n"
        "LSL R3, R4\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_EQ(arm7.readReg(3), 0x80000000);
    EXPECT_TRUE(arm7.readFlag(N_FLAG));
}
/**
 * @brief Test LSL's carry flag behaviour.
 */
TEST_F(TestCPU_THUMB_DataInstructions_LSL, LSL_CARRY_FLAG) {
    // C == 1 (bit shifted out).
    arm7.setFlag(C_FLAG, 0);
    arm7.writeReg(5, 0x80000000);  // MSB=1 shifted out
    arm7.writeReg(6, 1);
    writeProgramToMemory(
        ".thumb\n"
        "LSL R5, R6\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_EQ(arm7.readReg(5), 0x00000000);
    EXPECT_TRUE(arm7.readFlag(C_FLAG));  // Flag set by bit 31 out.

    // C == 0 (no bit shifted out).
    arm7.setFlag(C_FLAG, 1);
    arm7.writeReg(5, 0x40000000);  // MSB=0 shifted out
    arm7.writeReg(6, 1);
    writeProgramToMemory(
        ".thumb\n"
        "LSL R5, R6\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_EQ(arm7.readReg(5), 0x80000000);
    EXPECT_FALSE(arm7.readFlag(C_FLAG));  // Flag cleared by bit 31 out.
}
/**
 * @brief Test LSL's overflow flag behaviour.
 */
TEST_F(TestCPU_THUMB_DataInstructions_LSL, LSL_OVERFLOW_FLAG) {
    // V == 1.
    arm7.setFlag(V_FLAG, 1);
    arm7.writeReg(5, 0x40000000);
    arm7.writeReg(6, 1);
    writeProgramToMemory(
        ".thumb\n"
        "LSL R5, R6\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_EQ(arm7.readReg(5), 0x80000000);
    EXPECT_TRUE(arm7.readFlag(V_FLAG));  // Flag is unchanged.

    // V == 0.
    arm7.setFlag(V_FLAG, 0);
    arm7.writeReg(5, 0x40000000);
    arm7.writeReg(6, 1);
    writeProgramToMemory(
        ".thumb\n"
        "LSL R5, R6\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_EQ(arm7.readReg(5), 0x80000000);
    EXPECT_FALSE(arm7.readFlag(V_FLAG));  // Flag is unchanged.
}
// ==================================================================================================
// LSR
// ==================================================================================================
class TestCPU_THUMB_DataInstructions_LSR : public TestCPU_THUMB_DataInstructions {
protected:
    TestCPU_THUMB_DataInstructions_LSR() {}
    ~TestCPU_THUMB_DataInstructions_LSR() {}

    void SetUp() override { TestCPU_THUMB_DataInstructions::SetUp(); }
    void TearDown() override { TestCPU_THUMB_DataInstructions::TearDown(); }
};
/**
 * @brief Test LSR with small-number regs.
 */
TEST_F(TestCPU_THUMB_DataInstructions_LSR, LSR_REG_SMALL) {
    arm7.writeReg(1, 0x00000010);
    arm7.writeReg(2, 4);

    writeProgramToMemory(
        ".thumb\n"
        "LSR R1, R2\n",
        MAIN_RAM_START, &bus, arm7.isARM7());

    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);

    EXPECT_EQ(arm7.readReg(1), 0x00000001);
}
/**
 * @brief Test LSR where result is zero.
 */
TEST_F(TestCPU_THUMB_DataInstructions_LSR, LSR_REG_ZERO_RESULT) {
    arm7.writeReg(3, 0x000000FF);
    arm7.writeReg(4, 8);

    writeProgramToMemory(
        ".thumb\n"
        "LSR R3, R4\n",
        MAIN_RAM_START, &bus, arm7.isARM7());

    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);

    EXPECT_EQ(arm7.readReg(3), 0x00000000);
}
/**
 * @brief Test LSR immediate.
 */
TEST_F(TestCPU_THUMB_DataInstructions_LSR, LSR_IMM) {
    writeProgramToMemory(
        ".thumb\n"
        "LSR R1, #4\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.writeReg(1, 0x00000010);

    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);

    EXPECT_EQ(arm7.readReg(1), 0x00000001);
}
/**
 * @brief Test LSR's zero flag behaviour.
 */
TEST_F(TestCPU_THUMB_DataInstructions_LSR, LSR_ZERO_FLAG) {
    // Z == 1.
    arm7.setFlag(Z_FLAG, 0);
    arm7.writeReg(1, 0x00000000);
    arm7.writeReg(2, 1);
    writeProgramToMemory(
        ".thumb\n"
        "LSR R1, R2\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_EQ(arm7.readReg(1), 0);
    EXPECT_TRUE(arm7.readFlag(Z_FLAG));

    // Z == 0.
    arm7.setFlag(Z_FLAG, 1);
    arm7.writeReg(1, 0x00000080);
    arm7.writeReg(2, 7);
    writeProgramToMemory(
        ".thumb\n"
        "LSR R1, R2\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_EQ(arm7.readReg(1), 0x00000001);
    EXPECT_FALSE(arm7.readFlag(Z_FLAG));
}
/**
 * @brief Test LSR's negative flag behaviour.
 */
TEST_F(TestCPU_THUMB_DataInstructions_LSR, LSR_NEGATIVE_FLAG) {
    // N == 0.
    arm7.setFlag(N_FLAG, 1);
    arm7.writeReg(3, 0x80000000);
    arm7.writeReg(4, 1);
    writeProgramToMemory(
        ".thumb\n"
        "LSR R3, R4\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_EQ(arm7.readReg(3), 0x40000000);
    EXPECT_FALSE(arm7.readFlag(N_FLAG));

    // N == 1.
    arm7.setFlag(N_FLAG, 0);
    arm7.writeReg(3, 0xFFFFFFFF);
    arm7.writeReg(4, 0);
    writeProgramToMemory(
        ".thumb\n"
        "LSR R3, R4\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_EQ(arm7.readReg(3), 0xFFFFFFFF);
    EXPECT_TRUE(arm7.readFlag(N_FLAG));
}
/**
 * @brief Test LSR's carry flag behaviour.
 */
TEST_F(TestCPU_THUMB_DataInstructions_LSR, LSR_CARRY_FLAG) {
    // C == 1 (bit shifted out).
    arm7.setFlag(C_FLAG, 0);
    arm7.writeReg(5, 0x00000001);
    arm7.writeReg(6, 1);
    writeProgramToMemory(
        ".thumb\n"
        "LSR R5, R6\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_EQ(arm7.readReg(5), 0x00000000);
    EXPECT_TRUE(arm7.readFlag(C_FLAG));  // Flag set by bit shifted out.

    // C == 0 (no bit shifted out).
    arm7.setFlag(C_FLAG, 1);
    arm7.writeReg(5, 0x00000000);
    arm7.writeReg(6, 1);
    writeProgramToMemory(
        ".thumb\n"
        "LSR R5, R6\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_EQ(arm7.readReg(5), 0x00000000);
    EXPECT_FALSE(arm7.readFlag(C_FLAG));  // Flag cleared by bit shifted out.
}
/**
 * @brief Test LSR's overflow flag behaviour.
 */
TEST_F(TestCPU_THUMB_DataInstructions_LSR, LSR_OVERFLOW_FLAG) {
    // V == 1.
    arm7.setFlag(V_FLAG, 1);
    arm7.writeReg(5, 0x00000001);
    arm7.writeReg(6, 1);
    writeProgramToMemory(
        ".thumb\n"
        "LSR R5, R6\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_EQ(arm7.readReg(5), 0x00000000);
    EXPECT_TRUE(arm7.readFlag(V_FLAG));  // Flag is unchanged.

    // V == 0.
    arm7.setFlag(V_FLAG, 0);
    arm7.writeReg(5, 0x00000001);
    arm7.writeReg(6, 1);
    writeProgramToMemory(
        ".thumb\n"
        "LSR R5, R6\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_EQ(arm7.readReg(5), 0x00000000);
    EXPECT_FALSE(arm7.readFlag(V_FLAG));  // Flag is unchanged.
}
// ==================================================================================================
// ASR
// ==================================================================================================
class TestCPU_THUMB_DataInstructions_ASR : public TestCPU_THUMB_DataInstructions {
protected:
    TestCPU_THUMB_DataInstructions_ASR() {}
    ~TestCPU_THUMB_DataInstructions_ASR() {}

    void SetUp() override { TestCPU_THUMB_DataInstructions::SetUp(); }
    void TearDown() override { TestCPU_THUMB_DataInstructions::TearDown(); }
};
/**
 * @brief Test ASR with small-number regs.
 */
TEST_F(TestCPU_THUMB_DataInstructions_ASR, ASR_REG_SMALL) {
    arm7.writeReg(1, 0x00000010);
    arm7.writeReg(2, 4);

    writeProgramToMemory(
        ".thumb\n"
        "ASR R1, R2\n",
        MAIN_RAM_START, &bus, arm7.isARM7());

    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);

    EXPECT_EQ(arm7.readReg(1), 0x00000001);
}
/**
 * @brief Test ASR where result is zero.
 */
TEST_F(TestCPU_THUMB_DataInstructions_ASR, ASR_REG_ZERO_RESULT) {
    arm7.writeReg(3, 0x0000000F);
    arm7.writeReg(4, 4);

    writeProgramToMemory(
        ".thumb\n"
        "ASR R3, R4\n",
        MAIN_RAM_START, &bus, arm7.isARM7());

    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);

    EXPECT_EQ(arm7.readReg(3), 0x00000000);
}
/**
 * @brief Test ASR immediate.
 */
TEST_F(TestCPU_THUMB_DataInstructions_ASR, ASR_IMM) {
    writeProgramToMemory(
        ".thumb\n"
        "ASR R1, #4\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.writeReg(1, 0x00000010);

    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);

    EXPECT_EQ(arm7.readReg(1), 0x00000001);
}
/**
 * @brief Test ASR's zero flag behaviour.
 */
TEST_F(TestCPU_THUMB_DataInstructions_ASR, ASR_ZERO_FLAG) {
    // Z == 1.
    arm7.setFlag(Z_FLAG, 0);
    arm7.writeReg(1, 0x00000000);
    arm7.writeReg(2, 1);
    writeProgramToMemory(
        ".thumb\n"
        "ASR R1, R2\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_EQ(arm7.readReg(1), 0);
    EXPECT_TRUE(arm7.readFlag(Z_FLAG));

    // Z == 0.
    arm7.setFlag(Z_FLAG, 1);
    arm7.writeReg(1, 0x00000080);
    arm7.writeReg(2, 7);
    writeProgramToMemory(
        ".thumb\n"
        "ASR R1, R2\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_EQ(arm7.readReg(1), 0x00000001);
    EXPECT_FALSE(arm7.readFlag(Z_FLAG));
}
/**
 * @brief Test ASR's negative flag behaviour.
 */
TEST_F(TestCPU_THUMB_DataInstructions_ASR, ASR_NEGATIVE_FLAG) {
    // N == 0.
    arm7.setFlag(N_FLAG, 1);
    arm7.writeReg(3, 0x40000000);
    arm7.writeReg(4, 1);
    writeProgramToMemory(
        ".thumb\n"
        "ASR R3, R4\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_EQ(arm7.readReg(3), 0x20000000);
    EXPECT_FALSE(arm7.readFlag(N_FLAG));

    // N == 1.
    arm7.setFlag(N_FLAG, 0);
    arm7.writeReg(3, 0x80000000);
    arm7.writeReg(4, 1);
    writeProgramToMemory(
        ".thumb\n"
        "ASR R3, R4\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_EQ(arm7.readReg(3), 0xC0000000);
    EXPECT_TRUE(arm7.readFlag(N_FLAG));
}
/**
 * @brief Test ASR's carry flag behaviour.
 */
TEST_F(TestCPU_THUMB_DataInstructions_ASR, ASR_CARRY_FLAG) {
    // C == 1 (bit shifted out).
    arm7.setFlag(C_FLAG, 0);
    arm7.writeReg(5, 0x00000001);
    arm7.writeReg(6, 1);
    writeProgramToMemory(
        ".thumb\n"
        "ASR R5, R6\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_EQ(arm7.readReg(5), 0x00000000);
    EXPECT_TRUE(arm7.readFlag(C_FLAG));  // Flag set by bit shifted out.

    // C == 0 (no bit shifted out).
    arm7.setFlag(C_FLAG, 1);
    arm7.writeReg(5, 0x00000000);
    arm7.writeReg(6, 1);
    writeProgramToMemory(
        ".thumb\n"
        "ASR R5, R6\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_EQ(arm7.readReg(5), 0x00000000);
    EXPECT_FALSE(arm7.readFlag(C_FLAG));  // Flag cleared by bit shifted out.
}
/**
 * @brief Test ASR's overflow flag behaviour.
 */
TEST_F(TestCPU_THUMB_DataInstructions_ASR, ASR_OVERFLOW_FLAG) {
    // V == 1.
    arm7.setFlag(V_FLAG, 1);
    arm7.writeReg(5, 0x00000001);
    arm7.writeReg(6, 1);
    writeProgramToMemory(
        ".thumb\n"
        "ASR R5, R6\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_EQ(arm7.readReg(5), 0x00000000);
    EXPECT_TRUE(arm7.readFlag(V_FLAG));  // Flag is unchanged.

    // V == 0.
    arm7.setFlag(V_FLAG, 0);
    arm7.writeReg(5, 0x00000001);
    arm7.writeReg(6, 1);
    writeProgramToMemory(
        ".thumb\n"
        "ASR R5, R6\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_EQ(arm7.readReg(5), 0x00000000);
    EXPECT_FALSE(arm7.readFlag(V_FLAG));  // Flag is unchanged.
}
// ==================================================================================================
// ADD
// ==================================================================================================
class TestCPU_THUMB_DataInstructions_ADD : public TestCPU_THUMB_DataInstructions {
protected:
    TestCPU_THUMB_DataInstructions_ADD() {}
    ~TestCPU_THUMB_DataInstructions_ADD() {}

    void SetUp() override { TestCPU_THUMB_DataInstructions::SetUp(); }
    void TearDown() override { TestCPU_THUMB_DataInstructions::TearDown(); }
};
/**
 * @brief Test ADD with small num regs.
 */
TEST_F(TestCPU_THUMB_DataInstructions_ADD, ADD_REG_SMALL) {
    arm7.writeReg(1, 0x00000001);
    arm7.writeReg(2, 0x00000002);

    writeProgramToMemory(
        ".thumb\n"
        "ADD R1, R2\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);

    EXPECT_EQ(arm7.readReg(1), 0x00000003);
}
/**
 * @brief Test ADD with large num regs.(flags unchanged).
 */
TEST_F(TestCPU_THUMB_DataInstructions_ADD, ADD_REG_LARGE_REG) {
    arm7.writeReg(8, 0x00000100);
    arm7.writeReg(10, 0x00000200);

    arm7.setFlag(N_FLAG, 1);
    arm7.setFlag(Z_FLAG, 1);

    writeProgramToMemory(
        ".thumb\n"
        "ADD R10, R8\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);

    EXPECT_EQ(arm7.readReg(10), 0x00000300);
    EXPECT_TRUE(arm7.readFlag(N_FLAG));
    EXPECT_TRUE(arm7.readFlag(Z_FLAG));
}
/**
 * @brief Test ADD low reg immediate 3-bit.
 */
TEST_F(TestCPU_THUMB_DataInstructions_ADD, ADD_REG_IMM3) {
    arm7.writeReg(3, 0x00000010);

    writeProgramToMemory(
        ".thumb\n"
        "ADD R3, #4\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);

    EXPECT_EQ(arm7.readReg(3), 0x00000014);
}
/**
 * @brief Test ADD low reg immediate 8-bit.
 */
TEST_F(TestCPU_THUMB_DataInstructions_ADD, ADD_REG_IMM8) {
    arm7.writeReg(5, 0x00001234);
    writeProgramToMemory(
        ".thumb\n"
        "ADD R5, #0xFF\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);

    EXPECT_EQ(arm7.readReg(5), 0x00001333);
}
/**
 * @brief Test ADD SP + immediate (flags unchanged).
 */
TEST_F(TestCPU_THUMB_DataInstructions_ADD, ADD_SP_IMM7) {
    arm7.writeReg(SP_REGISTER_NUM, 0x00010000);

    arm7.setFlag(N_FLAG, 1);
    arm7.setFlag(Z_FLAG, 1);

    writeProgramToMemory(
        ".thumb\n"
        "ADD SP, #0x40\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);

    EXPECT_EQ(arm7.readReg(SP_REGISTER_NUM), 0x00010040);
    EXPECT_TRUE(arm7.readFlag(N_FLAG));
    EXPECT_TRUE(arm7.readFlag(Z_FLAG));
}
/**
 * @brief Test ADD SP + immediate (flags unchanged).
 */
TEST_F(TestCPU_THUMB_DataInstructions_ADD, ADD_SP_IMM8) {
    arm7.writeReg(SP_REGISTER_NUM, 0x00010000);

    arm7.setFlag(N_FLAG, 1);
    arm7.setFlag(Z_FLAG, 1);

    writeProgramToMemory(
        ".thumb\n"
        "ADD R0, SP, #0x3FC\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);

    EXPECT_EQ(arm7.readReg(0), 0x000103FC);
    EXPECT_TRUE(arm7.readFlag(N_FLAG));
    EXPECT_TRUE(arm7.readFlag(Z_FLAG));
}
/**
 * @brief Test ADD's zero flag behaviour.
 */
TEST_F(TestCPU_THUMB_DataInstructions_ADD, ADD_ZERO_FLAG) {
    // Z == 1.
    arm7.setFlag(Z_FLAG, 0);
    arm7.writeReg(6, 0x00000000);
    arm7.writeReg(7, 0x00000000);
    writeProgramToMemory(
        ".thumb\n"
        "ADD R6, R7\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_EQ(arm7.readReg(6), 0);
    EXPECT_TRUE(arm7.readFlag(Z_FLAG));

    // Z == 0.
    arm7.setFlag(Z_FLAG, 1);
    arm7.writeReg(6, 0x00000001);
    arm7.writeReg(7, 0x00000001);
    writeProgramToMemory(
        ".thumb\n"
        "ADD R6, R7\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_EQ(arm7.readReg(6), 2);
    EXPECT_FALSE(arm7.readFlag(Z_FLAG));
}
/**
 * @brief Test ADD's negative flag behaviour.
 */
TEST_F(TestCPU_THUMB_DataInstructions_ADD, ADD_NEGATIVE_FLAG) {
    // N == 0.
    arm7.setFlag(N_FLAG, 1);
    arm7.writeReg(3, 0x7FFFFFF0);  // MSB=0
    arm7.writeReg(4, 0x00000010);
    writeProgramToMemory(
        ".thumb\n"
        "ADD R3, R4\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_EQ(arm7.readReg(3), 0x80000000);
    EXPECT_TRUE(arm7.readFlag(N_FLAG));

    // N == 1.
    arm7.setFlag(N_FLAG, 0);
    arm7.writeReg(3, 0x10000000);
    arm7.writeReg(4, 0x20000000);
    writeProgramToMemory(
        ".thumb\n"
        "ADD R3, R4\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_EQ(arm7.readReg(3), 0x30000000);
    EXPECT_FALSE(arm7.readFlag(N_FLAG));
}
/**
 * @brief Test ADD's carry flag behaviour.
 */
TEST_F(TestCPU_THUMB_DataInstructions_ADD, ADD_CARRY_FLAG) {
    // C == 1.
    arm7.setFlag(C_FLAG, 0);
    arm7.writeReg(5, 0xFFFFFFFF);
    arm7.writeReg(6, 0x00000001);
    writeProgramToMemory(
        ".thumb\n"
        "ADD R5, R6\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_EQ(arm7.readReg(5), 0x00000000);
    EXPECT_TRUE(arm7.readFlag(C_FLAG));  // Flag is set.

    // C == 0.
    arm7.setFlag(C_FLAG, 1);
    arm7.writeReg(5, 0x7FFFFFFF);
    arm7.writeReg(6, 0x00000001);
    writeProgramToMemory(
        ".thumb\n"
        "ADD R5, R6\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_EQ(arm7.readReg(5), 0x80000000);
    EXPECT_FALSE(arm7.readFlag(C_FLAG));  // Flag is cleared.
}
/**
 * @brief Test ADD's overflow flag behaviour.
 */
TEST_F(TestCPU_THUMB_DataInstructions_ADD, ADD_OVERFLOW_FLAG) {
    // V == 1.
    arm7.setFlag(V_FLAG, 0);
    arm7.writeReg(5, 0x7FFFFFFF);
    arm7.writeReg(6, 0x00000001);
    writeProgramToMemory(
        ".thumb\n"
        "ADD R5, R6\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_EQ(arm7.readReg(5), 0x80000000);
    EXPECT_TRUE(arm7.readFlag(V_FLAG));  // Flag is set.

    // V == 0.
    arm7.setFlag(V_FLAG, 1);
    arm7.writeReg(5, 0x40000000);
    arm7.writeReg(6, 0x20000000);
    writeProgramToMemory(
        ".thumb\n"
        "ADD R5, R6\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_EQ(arm7.readReg(5), 0x60000000);
    EXPECT_FALSE(arm7.readFlag(V_FLAG));  // Flag is cleared.
}
// ==================================================================================================
// SBC
// ==================================================================================================
class TestCPU_THUMB_DataInstructions_SBC : public TestCPU_THUMB_DataInstructions {
protected:
    TestCPU_THUMB_DataInstructions_SBC() {}
    ~TestCPU_THUMB_DataInstructions_SBC() {}

    void SetUp() override { TestCPU_THUMB_DataInstructions::SetUp(); }
    void TearDown() override { TestCPU_THUMB_DataInstructions::TearDown(); }
};
/**
 * @brief Test SBC with small num regs.
 */
TEST_F(TestCPU_THUMB_DataInstructions_SBC, SBC_REG_SMALL) {
    arm7.writeReg(1, 0x00000003);
    arm7.writeReg(2, 0x00000001);
    arm7.setFlag(C_FLAG, 1);

    writeProgramToMemory(
        ".thumb\n"
        "SBC R1, R2\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);

    EXPECT_EQ(arm7.readReg(1), 0x00000002);
}
/**
 * @brief Test SBC where result is zero.
 */
TEST_F(TestCPU_THUMB_DataInstructions_SBC, SBC_REG_ZERO_RESULT) {
    arm7.writeReg(3, 0x00000001);
    arm7.writeReg(4, 0x00000000);
    arm7.setFlag(C_FLAG, 0);

    writeProgramToMemory(
        ".thumb\n"
        "SBC R3, R4\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);

    EXPECT_EQ(arm7.readReg(3), 0x00000000);
}
/**
 * @brief Test SBC's zero flag behaviour.
 */
TEST_F(TestCPU_THUMB_DataInstructions_SBC, SBC_ZERO_FLAG) {
    // Z == 1.
    arm7.setFlag(Z_FLAG, 0);
    arm7.writeReg(6, 0x00000000);
    arm7.writeReg(7, 0x00000000);
    arm7.setFlag(C_FLAG, 1);
    writeProgramToMemory(
        ".thumb\n"
        "SBC R6, R7\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_EQ(arm7.readReg(6), 0);
    EXPECT_TRUE(arm7.readFlag(Z_FLAG));

    // Z == 0.
    arm7.setFlag(Z_FLAG, 1);
    arm7.writeReg(6, 0x00000003);
    arm7.writeReg(7, 0x00000001);
    arm7.setFlag(C_FLAG, 1);
    writeProgramToMemory(
        ".thumb\n"
        "SBC R6, R7\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_EQ(arm7.readReg(6), 0x00000002);
    EXPECT_FALSE(arm7.readFlag(Z_FLAG));
}
/**
 * @brief Test SBC's negative flag behaviour.
 */
TEST_F(TestCPU_THUMB_DataInstructions_SBC, SBC_NEGATIVE_FLAG) {
    // N == 0.
    arm7.writeReg(3, 0x00000010);
    arm7.writeReg(4, 0x00000005);
    arm7.setFlag(C_FLAG, 1);
    writeProgramToMemory(
        ".thumb\n"
        "SBC R3, R4\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_EQ(arm7.readReg(3), 0x0000000B);
    EXPECT_FALSE(arm7.readFlag(N_FLAG));

    // N == 1.
    arm7.writeReg(3, 0x00000000);
    arm7.writeReg(4, 0x00000001);
    arm7.setFlag(C_FLAG, 1);
    writeProgramToMemory(
        ".thumb\n"
        "SBC R3, R4\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_EQ(arm7.readReg(3), 0xFFFFFFFF);
    EXPECT_TRUE(arm7.readFlag(N_FLAG));
}
/**
 * @brief Test SBC's carry flag behaviour.
 */
TEST_F(TestCPU_THUMB_DataInstructions_SBC, SBC_CARRY_FLAG) {
    // C == 1.
    arm7.writeReg(5, 0x00000002);
    arm7.writeReg(6, 0x00000001);
    arm7.setFlag(C_FLAG, 1);
    writeProgramToMemory(
        ".thumb\n"
        "SBC R5, R6\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_EQ(arm7.readReg(5), 0x00000001);
    EXPECT_TRUE(arm7.readFlag(C_FLAG));  // No borrow.

    // C == 0.
    arm7.writeReg(5, 0x00000000);
    arm7.writeReg(6, 0x00000001);
    arm7.setFlag(C_FLAG, 1);
    writeProgramToMemory(
        ".thumb\n"
        "SBC R5, R6\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_EQ(arm7.readReg(5), 0xFFFFFFFF);
    EXPECT_FALSE(arm7.readFlag(C_FLAG));  // Borrow occurred.
}
/**
 * @brief Test SBC's overflow flag behaviour.
 */
TEST_F(TestCPU_THUMB_DataInstructions_SBC, SBC_OVERFLOW_FLAG) {
    // V == 1.
    arm7.writeReg(5, 0x80000000);
    arm7.writeReg(6, 0x00000001);
    arm7.setFlag(C_FLAG, 1);

    writeProgramToMemory(
        ".thumb\n"
        "SBC R5, R6\n",
        MAIN_RAM_START, &bus, arm7.isARM7());

    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);

    EXPECT_EQ(arm7.readReg(5), 0x7FFFFFFF);
    EXPECT_TRUE(arm7.readFlag(V_FLAG));

    // V == 0.
    arm7.writeReg(5, 0x00000002);
    arm7.writeReg(6, 0x00000001);
    arm7.setFlag(C_FLAG, 1);

    writeProgramToMemory(
        ".thumb\n"
        "SBC R5, R6\n",
        MAIN_RAM_START, &bus, arm7.isARM7());

    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);

    EXPECT_EQ(arm7.readReg(5), 0x00000001);
    EXPECT_FALSE(arm7.readFlag(V_FLAG));
}
// ==================================================================================================
// ADC
// ==================================================================================================
class TestCPU_THUMB_DataInstructions_ADC : public TestCPU_THUMB_DataInstructions {
protected:
    TestCPU_THUMB_DataInstructions_ADC() {}
    ~TestCPU_THUMB_DataInstructions_ADC() {}

    void SetUp() override { TestCPU_THUMB_DataInstructions::SetUp(); }
    void TearDown() override { TestCPU_THUMB_DataInstructions::TearDown(); }
};
/**
 * @brief Test ADC with small num regs.
 */
TEST_F(TestCPU_THUMB_DataInstructions_ADC, ADC_REG_SMALL) {
    arm7.writeReg(1, 0x00000003);
    arm7.writeReg(2, 0x00000001);
    arm7.setFlag(C_FLAG, 1);

    writeProgramToMemory(
        ".thumb\n"
        "ADC R1, R2\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);

    EXPECT_EQ(arm7.readReg(1), 0x00000005);
}
/**
 * @brief Test ADC where result is zero.
 */
TEST_F(TestCPU_THUMB_DataInstructions_ADC, ADC_REG_ZERO_RESULT) {
    arm7.writeReg(3, 0x00000000);
    arm7.writeReg(4, 0x00000000);
    arm7.setFlag(C_FLAG, 0);

    writeProgramToMemory(
        ".thumb\n"
        "ADC R3, R4\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);

    EXPECT_EQ(arm7.readReg(3), 0x00000000);
}
/**
 * @brief Test ADC's zero flag behaviour.
 */
TEST_F(TestCPU_THUMB_DataInstructions_ADC, ADC_ZERO_FLAG) {
    // Z == 1.
    arm7.setFlag(Z_FLAG, 0);
    arm7.writeReg(6, 0x00000000);
    arm7.writeReg(7, 0x00000000);
    arm7.setFlag(C_FLAG, 0);
    writeProgramToMemory(
        ".thumb\n"
        "ADC R6, R7\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_EQ(arm7.readReg(6), 0);
    EXPECT_TRUE(arm7.readFlag(Z_FLAG));

    // Z == 0.
    arm7.setFlag(Z_FLAG, 1);
    arm7.writeReg(6, 0x00000003);
    arm7.writeReg(7, 0x00000001);
    arm7.setFlag(C_FLAG, 1);
    writeProgramToMemory(
        ".thumb\n"
        "ADC R6, R7\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_EQ(arm7.readReg(6), 0x00000005);
    EXPECT_FALSE(arm7.readFlag(Z_FLAG));
}
/**
 * @brief Test ADC's negative flag behaviour.
 */
TEST_F(TestCPU_THUMB_DataInstructions_ADC, ADC_NEGATIVE_FLAG) {
    // N == 0.
    arm7.writeReg(3, 0x00000010);
    arm7.writeReg(4, 0x00000005);
    arm7.setFlag(C_FLAG, 1);
    writeProgramToMemory(
        ".thumb\n"
        "ADC R3, R4\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_EQ(arm7.readReg(3), 0x00000016);
    EXPECT_FALSE(arm7.readFlag(N_FLAG));

    // N == 1.
    arm7.writeReg(3, 0x80000000);
    arm7.writeReg(4, 0x80000000);
    arm7.setFlag(C_FLAG, 1);
    writeProgramToMemory(
        ".thumb\n"
        "ADC R3, R4\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_EQ(arm7.readReg(3), 0x00000001);
    EXPECT_FALSE(arm7.readFlag(N_FLAG));
}
/**
 * @brief Test ADC's carry flag behaviour.
 */
TEST_F(TestCPU_THUMB_DataInstructions_ADC, ADC_CARRY_FLAG) {
    // C == 1.
    arm7.writeReg(5, 0xFFFFFFFF);
    arm7.writeReg(6, 0x00000001);
    arm7.setFlag(C_FLAG, 1);
    writeProgramToMemory(
        ".thumb\n"
        "ADC R5, R6\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_EQ(arm7.readReg(5), 0x00000001);
    EXPECT_TRUE(arm7.readFlag(C_FLAG));  // Carry occurred.

    // C == 0.
    arm7.writeReg(5, 0x00000001);
    arm7.writeReg(6, 0x00000000);
    arm7.setFlag(C_FLAG, 0);
    writeProgramToMemory(
        ".thumb\n"
        "ADC R5, R6\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_EQ(arm7.readReg(5), 0x00000001);
    EXPECT_FALSE(arm7.readFlag(C_FLAG));  // No carry.
}
/**
 * @brief Test ADC's overflow flag behaviour.
 */
TEST_F(TestCPU_THUMB_DataInstructions_ADC, ADC_OVERFLOW_FLAG) {
    // V == 1.
    arm7.writeReg(5, 0x7FFFFFFF);
    arm7.writeReg(6, 0x00000000);
    arm7.setFlag(C_FLAG, 1);

    writeProgramToMemory(
        ".thumb\n"
        "ADC R5, R6\n",
        MAIN_RAM_START, &bus, arm7.isARM7());

    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);

    EXPECT_EQ(arm7.readReg(5), 0x80000000);
    EXPECT_TRUE(arm7.readFlag(V_FLAG));

    // V == 0.
    arm7.writeReg(5, 0x00000002);
    arm7.writeReg(6, 0x00000001);
    arm7.setFlag(C_FLAG, 1);

    writeProgramToMemory(
        ".thumb\n"
        "ADC R5, R6\n",
        MAIN_RAM_START, &bus, arm7.isARM7());

    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);

    EXPECT_EQ(arm7.readReg(5), 0x00000004);
    EXPECT_FALSE(arm7.readFlag(V_FLAG));
}
// ==================================================================================================
// SUB
// ==================================================================================================
class TestCPU_THUMB_DataInstructions_SUB : public TestCPU_THUMB_DataInstructions {
protected:
    TestCPU_THUMB_DataInstructions_SUB() {}
    ~TestCPU_THUMB_DataInstructions_SUB() {}

    void SetUp() override { TestCPU_THUMB_DataInstructions::SetUp(); }
    void TearDown() override { TestCPU_THUMB_DataInstructions::TearDown(); }
};
/**
 * @brief Test SUB with small num regs.
 */
TEST_F(TestCPU_THUMB_DataInstructions_SUB, SUB_REG_SMALL) {
    arm7.writeReg(1, 0x00000003);
    arm7.writeReg(2, 0x00000001);

    writeProgramToMemory(
        ".thumb\n"
        "SUB R1, R2\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);

    EXPECT_EQ(arm7.readReg(1), 0x00000002);
}
/**
 * @brief Test SUB where result is zero.
 */
TEST_F(TestCPU_THUMB_DataInstructions_SUB, SUB_REG_ZERO_RESULT) {
    arm7.writeReg(3, 0x00000001);
    arm7.writeReg(4, 0x00000001);

    writeProgramToMemory(
        ".thumb\n"
        "SUB R3, R4\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);

    EXPECT_EQ(arm7.readReg(3), 0x00000000);
}
/**
 * @brief Test SUB's zero flag behaviour.
 */
TEST_F(TestCPU_THUMB_DataInstructions_SUB, SUB_ZERO_FLAG) {
    // Z == 1.
    arm7.setFlag(Z_FLAG, 0);
    arm7.writeReg(6, 0x00000000);
    arm7.writeReg(7, 0x00000000);
    writeProgramToMemory(
        ".thumb\n"
        "SUB R6, R7\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_EQ(arm7.readReg(6), 0);
    EXPECT_TRUE(arm7.readFlag(Z_FLAG));

    // Z == 0.
    arm7.setFlag(Z_FLAG, 1);
    arm7.writeReg(6, 0x00000003);
    arm7.writeReg(7, 0x00000001);
    writeProgramToMemory(
        ".thumb\n"
        "SUB R6, R7\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_EQ(arm7.readReg(6), 0x00000002);
    EXPECT_FALSE(arm7.readFlag(Z_FLAG));
}
/**
 * @brief Test SUB's negative flag behaviour.
 */
TEST_F(TestCPU_THUMB_DataInstructions_SUB, SUB_NEGATIVE_FLAG) {
    // N == 1.
    arm7.writeReg(3, 0x00000000);
    arm7.writeReg(4, 0x00000001);
    writeProgramToMemory(
        ".thumb\n"
        "SUB R3, R4\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_EQ(arm7.readReg(3), 0xFFFFFFFF);
    EXPECT_TRUE(arm7.readFlag(N_FLAG));

    // N == 0.
    arm7.writeReg(3, 0x00000010);
    arm7.writeReg(4, 0x00000005);
    writeProgramToMemory(
        ".thumb\n"
        "SUB R3, R4\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_EQ(arm7.readReg(3), 0x0000000B);
    EXPECT_FALSE(arm7.readFlag(N_FLAG));
}
/**
 * @brief Test SUB's carry flag behaviour.
 */
TEST_F(TestCPU_THUMB_DataInstructions_SUB, SUB_CARRY_FLAG) {
    // C == 1.
    arm7.writeReg(5, 0x00000002);
    arm7.writeReg(6, 0x00000001);
    writeProgramToMemory(
        ".thumb\n"
        "SUB R5, R6\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_EQ(arm7.readReg(5), 0x00000001);
    EXPECT_TRUE(arm7.readFlag(C_FLAG));  // No borrow.

    // C == 0.
    arm7.writeReg(5, 0x00000000);
    arm7.writeReg(6, 0x00000001);
    writeProgramToMemory(
        ".thumb\n"
        "SUB R5, R6\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_EQ(arm7.readReg(5), 0xFFFFFFFF);
    EXPECT_FALSE(arm7.readFlag(C_FLAG));  // Borrow occurred.
}
/**
 * @brief Test SUB's overflow flag behaviour.
 */
TEST_F(TestCPU_THUMB_DataInstructions_SUB, SUB_OVERFLOW_FLAG) {
    // V == 1.
    arm7.writeReg(5, 0x80000000);  // negative
    arm7.writeReg(6, 0x7FFFFFFE);
    writeProgramToMemory(
        ".thumb\n"
        "SUB R5, R6\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_EQ(arm7.readReg(5), 0x00000002);
    EXPECT_TRUE(arm7.readFlag(V_FLAG));

    // V == 0.
    arm7.writeReg(5, 0x00000002);
    arm7.writeReg(6, 0x00000001);
    writeProgramToMemory(
        ".thumb\n"
        "SUB R5, R6\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_EQ(arm7.readReg(5), 0x00000001);
    EXPECT_FALSE(arm7.readFlag(V_FLAG));
}
/**
 * @brief Test SUB Rdn, #imm3 register+immediate form.
 */
TEST_F(TestCPU_THUMB_DataInstructions_SUB, SUB_REG_IMM3) {
    arm7.writeReg(1, 0x00000010);

    writeProgramToMemory(
        ".thumb\n"
        "SUB R1, #5\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);

    EXPECT_EQ(arm7.readReg(1), 0x0000000B);
}
/**
 * @brief Test SUB Rd, Rn, #imm3 register+immediate form.
 */
TEST_F(TestCPU_THUMB_DataInstructions_SUB, SUB_REG_REG_IMM3) {
    arm7.writeReg(1, 0x00000010);

    writeProgramToMemory(
        ".thumb\n"
        "SUB R0, R1, #5\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);

    EXPECT_EQ(arm7.readReg(0), 0x0000000B);
}
/**
 * @brief Test SUB Rd, Rn, #imm8 register+immediate form.
 */
TEST_F(TestCPU_THUMB_DataInstructions_SUB, SUB_REG_IMM8) {
    arm7.writeReg(1, 0x000000FF);

    writeProgramToMemory(
        ".thumb\n"
        "SUB R1, #0xFF\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);

    EXPECT_EQ(arm7.readReg(1), 0x00000000);
}
/**
 * @brief Test SUB SP, #imm7 SP+immediate form.
 */
TEST_F(TestCPU_THUMB_DataInstructions_SUB, SUB_SP_IMM7) {
    arm7.writeReg(SP_REGISTER_NUM, 0x00010000);  // SP

    writeProgramToMemory(
        ".thumb\n"
        "SUB SP, #64\n",
        MAIN_RAM_START, &bus, arm7.isARM7());

    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);

    EXPECT_EQ(arm7.readReg(13), 0x0000FFC0);
}
// ==================================================================================================
// ROR
// ==================================================================================================
class TestCPU_THUMB_DataInstructions_ROR : public TestCPU_THUMB_DataInstructions {
protected:
    TestCPU_THUMB_DataInstructions_ROR() {}
    ~TestCPU_THUMB_DataInstructions_ROR() {}

    void SetUp() override { TestCPU_THUMB_DataInstructions::SetUp(); }
    void TearDown() override { TestCPU_THUMB_DataInstructions::TearDown(); }
};
/**
 * @brief Test ROR with small num regs.
 */
TEST_F(TestCPU_THUMB_DataInstructions_ROR, ROR_REG_SMALL) {
    arm7.writeReg(1, 0x00000080);
    arm7.writeReg(2, 3);

    writeProgramToMemory(
        ".thumb\n"
        "ROR R1, R2\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);

    EXPECT_EQ(arm7.readReg(1), 0x00000010);
}
/**
 * @brief Test ROR where result is zero.
 */
TEST_F(TestCPU_THUMB_DataInstructions_ROR, ROR_REG_FULL_WRAP) {
    arm7.writeReg(3, 0x00000001);
    arm7.writeReg(4, 32);

    writeProgramToMemory(
        ".thumb\n"
        "ROR R3, R4\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);

    EXPECT_EQ(arm7.readReg(3), 0x00000001);
}
/**
 * @brief Test ROR's zero flag behaviour.
 */
TEST_F(TestCPU_THUMB_DataInstructions_ROR, ROR_ZERO_FLAG) {
    // Z == 1.
    arm7.setFlag(Z_FLAG, 0);
    arm7.writeReg(6, 0x00000000);
    arm7.writeReg(7, 32);
    writeProgramToMemory(
        ".thumb\n"
        "ROR R6, R7\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_EQ(arm7.readReg(6), 0);
    EXPECT_TRUE(arm7.readFlag(Z_FLAG));

    // Z == 0.
    arm7.setFlag(Z_FLAG, 1);
    arm7.writeReg(6, 0x00000001);
    arm7.writeReg(7, 1);
    writeProgramToMemory(
        ".thumb\n"
        "ROR R6, R7\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_EQ(arm7.readReg(6), 0x80000000);
    EXPECT_FALSE(arm7.readFlag(Z_FLAG));
}
/**
 * @brief Test ROR's negative flag behaviour.
 */
TEST_F(TestCPU_THUMB_DataInstructions_ROR, ROR_NEGATIVE_FLAG) {
    // N == 0.
    arm7.writeReg(3, 0x00000001);
    arm7.writeReg(4, 2);
    writeProgramToMemory(
        ".thumb\n"
        "ROR R3, R4\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_EQ(arm7.readReg(3), 0x40000000);
    EXPECT_FALSE(arm7.readFlag(N_FLAG));

    // N == 1.
    arm7.writeReg(3, 0x00000001);
    arm7.writeReg(4, 1);
    writeProgramToMemory(
        ".thumb\n"
        "ROR R3, R4\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_EQ(arm7.readReg(3), 0x80000000);
    EXPECT_TRUE(arm7.readFlag(N_FLAG));
}
/**
 * @brief Test ROR's carry flag behaviour.
 */
TEST_F(TestCPU_THUMB_DataInstructions_ROR, ROR_CARRY_FLAG) {
    // C == 1.
    arm7.writeReg(5, 0x00000001);
    arm7.writeReg(6, 1);
    writeProgramToMemory(
        ".thumb\n"
        "ROR R5, R6\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_EQ(arm7.readReg(5), 0x80000000);
    EXPECT_TRUE(arm7.readFlag(C_FLAG));  // Bit shifted out was 1.

    // C == 0.
    arm7.writeReg(5, 0x00000001);
    arm7.writeReg(6, 2);
    writeProgramToMemory(
        ".thumb\n"
        "ROR R5, R6\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_EQ(arm7.readReg(5), 0x40000000);
    EXPECT_FALSE(arm7.readFlag(C_FLAG));  // Bit shifted out was 0.
}
/**
 * @brief Test ROR's overflow flag behaviour.
 */
TEST_F(TestCPU_THUMB_DataInstructions_ROR, ROR_OVERFLOW_FLAG) {
    // V == 1.
    arm7.writeReg(5, 0x00000001);
    arm7.writeReg(6, 1);
    arm7.setFlag(V_FLAG, 1);
    writeProgramToMemory(
        ".thumb\n"
        "ROR R5, R6\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_EQ(arm7.readReg(5), 0x80000000);
    EXPECT_TRUE(arm7.readFlag(V_FLAG));  // Flag is unchanged.

    // V == 0.
    arm7.setFlag(V_FLAG, 0);
    arm7.writeReg(5, 0x80000001);
    arm7.writeReg(6, 1);
    writeProgramToMemory(
        ".thumb\n"
        "ROR R5, R6\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_EQ(arm7.readReg(5), 0xC0000000);
    EXPECT_FALSE(arm7.readFlag(V_FLAG));  // Flag is unchanged.
}
// ==================================================================================================
// TST
// ==================================================================================================
class TestCPU_THUMB_DataInstructions_TST : public TestCPU_THUMB_DataInstructions {
protected:
    TestCPU_THUMB_DataInstructions_TST() {}
    ~TestCPU_THUMB_DataInstructions_TST() {}

    void SetUp() override { TestCPU_THUMB_DataInstructions::SetUp(); }
    void TearDown() override { TestCPU_THUMB_DataInstructions::TearDown(); }
};
/**
 * @brief Test TST with small num regs.
 */
TEST_F(TestCPU_THUMB_DataInstructions_TST, TST_REG_SMALL) {
    arm7.writeReg(1, 0x00000003);
    arm7.writeReg(2, 0x00000001);

    writeProgramToMemory(
        ".thumb\n"
        "TST R1, R2\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);

    EXPECT_EQ(arm7.readReg(1), 0x00000003);  // Operand reg is left unchanged.
    EXPECT_FALSE(arm7.readFlag(Z_FLAG));
}
/**
 * @brief Test TST where result is zero.
 */
TEST_F(TestCPU_THUMB_DataInstructions_TST, TST_REG_ZERO_RESULT) {
    arm7.writeReg(3, 0x00000001);
    arm7.writeReg(4, 0x00000002);

    writeProgramToMemory(
        ".thumb\n"
        "TST R3, R4\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);

    EXPECT_EQ(arm7.readReg(3), 0x00000001);  // Operand reg is left unchanged.
    EXPECT_TRUE(arm7.readFlag(Z_FLAG));
}
/**
 * @brief Test TST's zero flag behaviour.
 */
TEST_F(TestCPU_THUMB_DataInstructions_TST, TST_ZERO_FLAG) {
    // Z == 1.
    arm7.setFlag(Z_FLAG, 0);
    arm7.writeReg(6, 0x00000000);
    arm7.writeReg(7, 0x00000000);
    writeProgramToMemory(
        ".thumb\n"
        "TST R6, R7\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_TRUE(arm7.readFlag(Z_FLAG));

    // Z == 0.
    arm7.setFlag(Z_FLAG, 1);
    arm7.writeReg(6, 0x00000003);
    arm7.writeReg(7, 0x00000001);
    writeProgramToMemory(
        ".thumb\n"
        "TST R6, R7\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_FALSE(arm7.readFlag(Z_FLAG));
}
/**
 * @brief Test TST's negative flag behaviour.
 */
TEST_F(TestCPU_THUMB_DataInstructions_TST, TST_NEGATIVE_FLAG) {
    // N == 0.
    arm7.writeReg(3, 0x7FFFFFFF);
    arm7.writeReg(4, 0x00000001);
    writeProgramToMemory(
        ".thumb\n"
        "TST R3, R4\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_FALSE(arm7.readFlag(N_FLAG));

    // N == 1.
    arm7.writeReg(3, 0x80000000);
    arm7.writeReg(4, 0x80000000);
    writeProgramToMemory(
        ".thumb\n"
        "TST R3, R4\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_TRUE(arm7.readFlag(N_FLAG));
}
/**
 * @brief Test TST's carry flag behaviour.
 */
TEST_F(TestCPU_THUMB_DataInstructions_TST, TST_CARRY_FLAG) {
    // C unchanged.
    arm7.setFlag(C_FLAG, 1);
    arm7.writeReg(5, 0x00000001);
    arm7.writeReg(6, 0x00000001);
    writeProgramToMemory(
        ".thumb\n"
        "TST R5, R6\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_TRUE(arm7.readFlag(C_FLAG));  // C unchanged.

    // C unchanged.
    arm7.setFlag(C_FLAG, 0);
    arm7.writeReg(5, 0x00000002);
    arm7.writeReg(6, 0x00000001);
    writeProgramToMemory(
        ".thumb\n"
        "TST R5, R6\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_FALSE(arm7.readFlag(C_FLAG));  // C unchanged.
}
/**
 * @brief Test TST's overflow flag behaviour.
 */
TEST_F(TestCPU_THUMB_DataInstructions_TST, TST_OVERFLOW_FLAG) {
    // V unchanged.
    arm7.setFlag(V_FLAG, 1);
    arm7.writeReg(5, 0x80000000);
    arm7.writeReg(6, 0x80000000);
    writeProgramToMemory(
        ".thumb\n"
        "TST R5, R6\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_TRUE(arm7.readFlag(V_FLAG));  // V unchanged.

    // V unchanged.
    arm7.setFlag(V_FLAG, 0);
    arm7.writeReg(5, 0x00000001);
    arm7.writeReg(6, 0x00000002);
    writeProgramToMemory(
        ".thumb\n"
        "TST R5, R6\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_FALSE(arm7.readFlag(V_FLAG));  // V unchanged.
}
// ==================================================================================================
// RSB
// ==================================================================================================
class TestCPU_THUMB_DataInstructions_RSB : public TestCPU_THUMB_DataInstructions {
protected:
    TestCPU_THUMB_DataInstructions_RSB() {}
    ~TestCPU_THUMB_DataInstructions_RSB() {}

    void SetUp() override { TestCPU_THUMB_DataInstructions::SetUp(); }
    void TearDown() override { TestCPU_THUMB_DataInstructions::TearDown(); }
};
/**
 * @brief Test RSB with small num regs.
 */
TEST_F(TestCPU_THUMB_DataInstructions_RSB, RSB_REG_SMALL) {
    arm7.writeReg(2, 0x00000003);

    writeProgramToMemory(
        ".thumb\n"
        "NEG R1, R2\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);

    EXPECT_EQ(arm7.readReg(1), 0xFFFFFFFD);
}
/**
 * @brief Test RSB where result is zero.
 */
TEST_F(TestCPU_THUMB_DataInstructions_RSB, RSB_REG_ZERO_RESULT) {
    arm7.writeReg(4, 0x00000000);

    writeProgramToMemory(
        ".thumb\n"
        "NEG R3, R4\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);

    EXPECT_EQ(arm7.readReg(3), 0x00000000);
}
/**
 * @brief Test RSB's zero flag behaviour.
 */
TEST_F(TestCPU_THUMB_DataInstructions_RSB, RSB_ZERO_FLAG) {
    // Z == 1.
    arm7.setFlag(Z_FLAG, 0);
    arm7.writeReg(7, 0x00000000);
    writeProgramToMemory(
        ".thumb\n"
        "NEG R6, R7\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_EQ(arm7.readReg(6), 0);
    EXPECT_TRUE(arm7.readFlag(Z_FLAG));

    // Z == 0.
    arm7.setFlag(Z_FLAG, 1);
    arm7.writeReg(7, 0x00000003);
    writeProgramToMemory(
        ".thumb\n"
        "NEG R6, R7\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_EQ(arm7.readReg(6), 0xFFFFFFFD);
    EXPECT_FALSE(arm7.readFlag(Z_FLAG));
}
/**
 * @brief Test RSB's negative flag behaviour.
 */
TEST_F(TestCPU_THUMB_DataInstructions_RSB, RSB_NEGATIVE_FLAG) {
    // N == 0.
    arm7.writeReg(4, 0x00000010);
    writeProgramToMemory(
        ".thumb\n"
        "NEG R3, R4\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_EQ(arm7.readReg(3), 0xFFFFFFF0);
    EXPECT_TRUE(arm7.readFlag(N_FLAG));

    // N == 1.
    arm7.writeReg(4, 0x00000000);
    writeProgramToMemory(
        ".thumb\n"
        "NEG R3, R4\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_EQ(arm7.readReg(3), 0x00000000);
    EXPECT_FALSE(arm7.readFlag(N_FLAG));
}
/**
 * @brief Test RSB's carry flag behaviour.
 */
TEST_F(TestCPU_THUMB_DataInstructions_RSB, RSB_CARRY_FLAG) {
    // C == 1.
    arm7.writeReg(6, 0x00000000);
    writeProgramToMemory(
        ".thumb\n"
        "NEG R5, R6\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_EQ(arm7.readReg(5), 0x00000000);
    EXPECT_TRUE(arm7.readFlag(C_FLAG));  // No borrow.

    // C == 0.
    arm7.writeReg(6, 0x00000001);
    writeProgramToMemory(
        ".thumb\n"
        "NEG R5, R6\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_EQ(arm7.readReg(5), 0xFFFFFFFF);
    EXPECT_FALSE(arm7.readFlag(C_FLAG));  // Borrow occurred.
}
/**
 * @brief Test RSB's overflow flag behaviour.
 */
TEST_F(TestCPU_THUMB_DataInstructions_RSB, RSB_OVERFLOW_FLAG) {
    // V == 1.
    arm7.writeReg(6, 0x80000000);  // negative
    writeProgramToMemory(
        ".thumb\n"
        "NEG R5, R6\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_EQ(arm7.readReg(5), 0x80000000);
    EXPECT_TRUE(arm7.readFlag(V_FLAG));

    // V == 0.
    arm7.writeReg(6, 0x00000002);
    writeProgramToMemory(
        ".thumb\n"
        "NEG R5, R6\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_EQ(arm7.readReg(5), 0xFFFFFFFE);
    EXPECT_FALSE(arm7.readFlag(V_FLAG));
}
// ==================================================================================================
// CMP
// ==================================================================================================
class TestCPU_THUMB_DataInstructions_CMP : public TestCPU_THUMB_DataInstructions {
protected:
    TestCPU_THUMB_DataInstructions_CMP() {}
    ~TestCPU_THUMB_DataInstructions_CMP() {}

    void SetUp() override { TestCPU_THUMB_DataInstructions::SetUp(); }
    void TearDown() override { TestCPU_THUMB_DataInstructions::TearDown(); }
};
/**
 * @brief Test CMP with small num regs.
 */
TEST_F(TestCPU_THUMB_DataInstructions_CMP, CMP_REG_SMALL) {
    arm7.writeReg(1, 0x00000003);
    arm7.writeReg(2, 0x00000001);

    writeProgramToMemory(
        ".thumb\n"
        "CMP R1, R2\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);

    EXPECT_EQ(arm7.readReg(1), 0x00000003);  // Operand reg is left unchanged.
    EXPECT_FALSE(arm7.readFlag(Z_FLAG));
}
/**
 * @brief Test CMP where result is zero.
 */
TEST_F(TestCPU_THUMB_DataInstructions_CMP, CMP_REG_ZERO_RESULT) {
    arm7.writeReg(3, 0x00000001);
    arm7.writeReg(4, 0x00000001);

    writeProgramToMemory(
        ".thumb\n"
        "CMP R3, R4\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);

    EXPECT_EQ(arm7.readReg(3), 0x00000001);  // Operand reg is left unchanged.
    EXPECT_TRUE(arm7.readFlag(Z_FLAG));
}
/**
 * @brief Test CMP's zero flag behaviour.
 */
TEST_F(TestCPU_THUMB_DataInstructions_CMP, CMP_ZERO_FLAG) {
    // Z == 1.
    arm7.setFlag(Z_FLAG, 0);
    arm7.writeReg(6, 0x00000000);
    arm7.writeReg(7, 0x00000000);
    writeProgramToMemory(
        ".thumb\n"
        "CMP R6, R7\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_TRUE(arm7.readFlag(Z_FLAG));

    // Z == 0.
    arm7.setFlag(Z_FLAG, 1);
    arm7.writeReg(6, 0x00000003);
    arm7.writeReg(7, 0x00000001);
    writeProgramToMemory(
        ".thumb\n"
        "CMP R6, R7\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_FALSE(arm7.readFlag(Z_FLAG));
}
/**
 * @brief Test CMP's negative flag behaviour.
 */
TEST_F(TestCPU_THUMB_DataInstructions_CMP, CMP_NEGATIVE_FLAG) {
    // N == 0.
    arm7.writeReg(3, 0x80000000);
    arm7.writeReg(4, 0x7FFFFFFF);
    writeProgramToMemory(
        ".thumb\n"
        "CMP R3, R4\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_FALSE(arm7.readFlag(N_FLAG));

    // N == 1.
    arm7.writeReg(3, 0x00000001);
    arm7.writeReg(4, 0x00000002);
    writeProgramToMemory(
        ".thumb\n"
        "CMP R3, R4\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_TRUE(arm7.readFlag(N_FLAG));
}
/**
 * @brief Test CMP's carry flag behaviour.
 */
TEST_F(TestCPU_THUMB_DataInstructions_CMP, CMP_CARRY_FLAG) {
    // C == 1.
    arm7.writeReg(5, 0x00000002);
    arm7.writeReg(6, 0x00000001);
    writeProgramToMemory(
        ".thumb\n"
        "CMP R5, R6\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_TRUE(arm7.readFlag(C_FLAG));  // Rn >= Rm (no borrow).

    // C == 0.
    arm7.writeReg(5, 0x00000000);
    arm7.writeReg(6, 0x00000001);
    writeProgramToMemory(
        ".thumb\n"
        "CMP R5, R6\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_FALSE(arm7.readFlag(C_FLAG));  // Rn < Rm (borrow occurred).
}
/**
 * @brief Test CMP's overflow flag behaviour.
 */
TEST_F(TestCPU_THUMB_DataInstructions_CMP, CMP_OVERFLOW_FLAG) {
    // V == 1.
    arm7.writeReg(5, 0x80000000);
    arm7.writeReg(6, 0x7FFFFFFE);
    writeProgramToMemory(
        ".thumb\n"
        "CMP R5, R6\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_TRUE(arm7.readFlag(V_FLAG));

    // V == 0.
    arm7.writeReg(5, 0x00000002);
    arm7.writeReg(6, 0x00000001);
    writeProgramToMemory(
        ".thumb\n"
        "CMP R5, R6\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_FALSE(arm7.readFlag(V_FLAG));
}
/**
 * @brief Test CMP with immediate (imm3 form).
 */
TEST_F(TestCPU_THUMB_DataInstructions_CMP, CMP_IMM3) {
    arm7.writeReg(1, 0x00000005);

    writeProgramToMemory(
        ".thumb\n"
        "CMP R1, #3\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);

    EXPECT_EQ(arm7.readReg(1), 0x00000005);  // Operand reg unchanged.
    EXPECT_FALSE(arm7.readFlag(Z_FLAG));
}
/**
 * @brief Test CMP high register variant (R8-R15).
 */
TEST_F(TestCPU_THUMB_DataInstructions_CMP, CMP_REG_HIGH) {
    arm7.writeReg(9, 0x00000003);   // R9
    arm7.writeReg(10, 0x00000001);  // R10

    writeProgramToMemory(
        ".thumb\n"
        "CMP R9, R10\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);

    EXPECT_EQ(arm7.readReg(9), 0x00000003);  // Operand reg unchanged.
    EXPECT_FALSE(arm7.readFlag(Z_FLAG));
}
/**
 * @brief Test CMP Rd, #imm3 with equality case.
 */
TEST_F(TestCPU_THUMB_DataInstructions_CMP, CMP_IMM3_EQUAL) {
    arm7.writeReg(3, 0x00000004);

    writeProgramToMemory(
        ".thumb\n"
        "CMP R3, #4\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);

    EXPECT_EQ(arm7.readReg(3), 0x00000004);  // Operand reg unchanged.
    EXPECT_TRUE(arm7.readFlag(Z_FLAG));
}
// ==================================================================================================
// CMN
// ==================================================================================================
class TestCPU_THUMB_DataInstructions_CMN : public TestCPU_THUMB_DataInstructions {
protected:
    TestCPU_THUMB_DataInstructions_CMN() {}
    ~TestCPU_THUMB_DataInstructions_CMN() {}

    void SetUp() override { TestCPU_THUMB_DataInstructions::SetUp(); }
    void TearDown() override { TestCPU_THUMB_DataInstructions::TearDown(); }
};
/**
 * @brief Test CMN with small-number regs.
 */
TEST_F(TestCPU_THUMB_DataInstructions_CMN, CMN_REG_SMALL) {
    arm7.writeReg(1, 0x00000001);
    arm7.writeReg(2, 0x00000002);

    writeProgramToMemory(
        ".thumb\n"
        "CMN R1, R2\n",  // Compare R1 + R2 (sets flags only)
        MAIN_RAM_START, &bus, arm7.isARM7());

    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);

    // Result = 0x3, flags only affected.
    EXPECT_EQ(arm7.readFlag(Z_FLAG), false);
    EXPECT_EQ(arm7.readFlag(N_FLAG), false);
}
/**
 * @brief Test CMN's zero flag behaviour.
 */
TEST_F(TestCPU_THUMB_DataInstructions_CMN, CMN_ZERO_FLAG) {
    // Z == 1.
    arm7.setFlag(Z_FLAG, 0);
    arm7.writeReg(1, 0xFFFFFFFF);
    arm7.writeReg(2, 0x00000001);
    writeProgramToMemory(
        ".thumb\n"
        "CMN R1, R2\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_TRUE(arm7.readFlag(Z_FLAG));

    // Z == 0.
    arm7.setFlag(Z_FLAG, 1);
    arm7.writeReg(1, 0x00000001);
    arm7.writeReg(2, 0x00000001);
    writeProgramToMemory(
        ".thumb\n"
        "CMN R1, R2\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_FALSE(arm7.readFlag(Z_FLAG));
}
/**
 * @brief Test CMN's negative flag behaviour.
 */
TEST_F(TestCPU_THUMB_DataInstructions_CMN, CMN_NEGATIVE_FLAG) {
    // N == 0.
    arm7.setFlag(N_FLAG, 1);
    arm7.writeReg(3, 0x00000001);
    arm7.writeReg(4, 0x00000001);
    writeProgramToMemory(
        ".thumb\n"
        "CMN R3, R4\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_FALSE(arm7.readFlag(N_FLAG));

    // N == 1.
    arm7.setFlag(N_FLAG, 0);
    arm7.writeReg(3, 0x7FFFFFFF);
    arm7.writeReg(4, 0x00000001);
    writeProgramToMemory(
        ".thumb\n"
        "CMN R3, R4\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_TRUE(arm7.readFlag(N_FLAG));
}
/**
 * @brief Test CMN's carry flag behaviour.
 */
TEST_F(TestCPU_THUMB_DataInstructions_CMN, CMN_CARRY_FLAG) {
    // C == 1.
    arm7.setFlag(C_FLAG, 0);
    arm7.writeReg(5, 0xFFFFFFFF);
    arm7.writeReg(6, 0x00000001);
    writeProgramToMemory(
        ".thumb\n"
        "CMN R5, R6\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_TRUE(arm7.readFlag(C_FLAG));

    // C == 0.
    arm7.setFlag(C_FLAG, 1);
    arm7.writeReg(5, 0x7FFFFFFF);
    arm7.writeReg(6, 0x00000001);
    writeProgramToMemory(
        ".thumb\n"
        "CMN R5, R6\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_FALSE(arm7.readFlag(C_FLAG));
}
/**
 * @brief Test CMN's overflow flag behaviour.
 */
TEST_F(TestCPU_THUMB_DataInstructions_CMN, CMN_OVERFLOW_FLAG) {
    // V == 1.
    arm7.setFlag(V_FLAG, 0);
    arm7.writeReg(5, 0x7FFFFFFF);
    arm7.writeReg(6, 0x00000001);
    writeProgramToMemory(
        ".thumb\n"
        "CMN R5, R6\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_TRUE(arm7.readFlag(V_FLAG));

    // V == 0.
    arm7.setFlag(V_FLAG, 1);
    arm7.writeReg(5, 0x00000001);
    arm7.writeReg(6, 0x00000001);
    writeProgramToMemory(
        ".thumb\n"
        "CMN R5, R6\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_FALSE(arm7.readFlag(V_FLAG));
}
/**
 * @brief Test CMN without affecting registers.
 */
TEST_F(TestCPU_THUMB_DataInstructions_CMN, CMN_REG_UNCHANGED) {
    arm7.writeReg(1, 0x10000000);
    arm7.writeReg(2, 0x10000000);

    writeProgramToMemory(
        ".thumb\n"
        "CMN R1, R2\n",  // Result affects flags only, not registers
        MAIN_RAM_START, &bus, arm7.isARM7());

    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);

    EXPECT_EQ(arm7.readReg(1), 0x10000000);
    EXPECT_EQ(arm7.readReg(2), 0x10000000);
}
// ==================================================================================================
// ORR
// ==================================================================================================
class TestCPU_THUMB_DataInstructions_ORR : public TestCPU_THUMB_DataInstructions {
protected:
    TestCPU_THUMB_DataInstructions_ORR() {}
    ~TestCPU_THUMB_DataInstructions_ORR() {}

    void SetUp() override { TestCPU_THUMB_DataInstructions::SetUp(); }
    void TearDown() override { TestCPU_THUMB_DataInstructions::TearDown(); }
};
/**
 * @brief Test OR with small-number regs.
 */
TEST_F(TestCPU_THUMB_DataInstructions_ORR, ORR_REG_SMALL) {
    arm7.writeReg(1, 0xAAAAAAAA);
    arm7.writeReg(2, 0x55555555);

    writeProgramToMemory(
        ".thumb\n"
        "ORR R1, R2\n",  // R1 := R1 | R2
        MAIN_RAM_START, &bus, arm7.isARM7());

    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);

    EXPECT_EQ(arm7.readReg(1), 0xFFFFFFFF);
}
/**
 * @brief Test ORR where result is zero.
 */
TEST_F(TestCPU_THUMB_DataInstructions_ORR, ORR_REG_ZERO_RESULT) {
    arm7.writeReg(3, 0x00000000);
    arm7.writeReg(4, 0x00000000);

    writeProgramToMemory(
        ".thumb\n"
        "ORR R3, R4\n",
        MAIN_RAM_START, &bus, arm7.isARM7());

    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);

    EXPECT_EQ(arm7.readReg(3), 0x00000000);
}
/**
 * @brief Test ORR's zero flag behaviour.
 */
TEST_F(TestCPU_THUMB_DataInstructions_ORR, ORR_ZERO_FLAG) {
    // Z == 1.
    arm7.setFlag(Z_FLAG, 0);
    arm7.writeReg(1, 0x00000000);
    arm7.writeReg(2, 0x00000000);
    writeProgramToMemory(
        ".thumb\n"
        "ORR R1, R2\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_EQ(arm7.readReg(1), 0);
    EXPECT_TRUE(arm7.readFlag(Z_FLAG));

    // Z == 0.
    arm7.setFlag(Z_FLAG, 1);
    arm7.writeReg(1, 0x00000000);
    arm7.writeReg(2, 0x00000001);
    writeProgramToMemory(
        ".thumb\n"
        "ORR R1, R2\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_EQ(arm7.readReg(1), 1);
    EXPECT_FALSE(arm7.readFlag(Z_FLAG));
}
/**
 * @brief Test ORR's negative flag behaviour.
 */
TEST_F(TestCPU_THUMB_DataInstructions_ORR, ORR_NEGATIVE_FLAG) {
    // N == 0.
    arm7.setFlag(N_FLAG, 1);
    arm7.writeReg(3, 0x7FFFFFFF);  // MSB=0
    arm7.writeReg(4, 0x7FFFFFFF);
    writeProgramToMemory(
        ".thumb\n"
        "ORR R3, R4\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_EQ(arm7.readReg(3), 0x7FFFFFFF);
    EXPECT_FALSE(arm7.readFlag(N_FLAG));

    // N == 1.
    arm7.setFlag(N_FLAG, 0);
    arm7.writeReg(3, 0x80000000);  // MSB=1
    arm7.writeReg(4, 0x00000000);
    writeProgramToMemory(
        ".thumb\n"
        "ORR R3, R4\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_EQ(arm7.readReg(3), 0x80000000);
    EXPECT_TRUE(arm7.readFlag(N_FLAG));
}
/**
 * @brief Test ORR's carry flag behaviour.
 */
TEST_F(TestCPU_THUMB_DataInstructions_ORR, ORR_CARRY_FLAG) {
    // C == 1.
    arm7.setFlag(C_FLAG, 1);
    arm7.writeReg(5, 0xFFFFFFFF);
    arm7.writeReg(6, 0x00000001);
    writeProgramToMemory(
        ".thumb\n"
        "ORR R5, R6\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_EQ(arm7.readReg(5), 0xFFFFFFFF);
    EXPECT_TRUE(arm7.readFlag(C_FLAG));  // Flag is unchanged.

    // C == 0.
    arm7.setFlag(C_FLAG, 0);
    arm7.writeReg(5, 0xFFFFFFFF);
    arm7.writeReg(6, 0x00000001);
    writeProgramToMemory(
        ".thumb\n"
        "ORR R5, R6\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_EQ(arm7.readReg(5), 0xFFFFFFFF);
    EXPECT_FALSE(arm7.readFlag(C_FLAG));  // Flag is unchanged.
}
/**
 * @brief Test ORR's overflow flag behaviour.
 */
TEST_F(TestCPU_THUMB_DataInstructions_ORR, ORR_OVERFLOW_FLAG) {
    // V == 1.
    arm7.setFlag(V_FLAG, 1);
    arm7.writeReg(5, 0xFFFFFFFF);
    arm7.writeReg(6, 0x00000001);
    writeProgramToMemory(
        ".thumb\n"
        "ORR R5, R6\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_EQ(arm7.readReg(5), 0xFFFFFFFF);
    EXPECT_TRUE(arm7.readFlag(V_FLAG));  // Flag is unchanged.

    // V == 0.
    arm7.setFlag(V_FLAG, 0);
    arm7.writeReg(5, 0xFFFFFFFF);
    arm7.writeReg(6, 0x00000001);
    writeProgramToMemory(
        ".thumb\n"
        "ORR R5, R6\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_EQ(arm7.readReg(5), 0xFFFFFFFF);
    EXPECT_FALSE(arm7.readFlag(V_FLAG));  // Flag is unchanged.
}
// ==================================================================================================
// MUL
// ==================================================================================================
class TestCPU_THUMB_DataInstructions_MUL : public TestCPU_THUMB_DataInstructions {
protected:
    TestCPU_THUMB_DataInstructions_MUL() {}
    ~TestCPU_THUMB_DataInstructions_MUL() {}

    void SetUp() override { TestCPU_THUMB_DataInstructions::SetUp(); }
    void TearDown() override { TestCPU_THUMB_DataInstructions::TearDown(); }
};
/**
 * @brief Test MUL with small-number regs.
 */
TEST_F(TestCPU_THUMB_DataInstructions_MUL, MUL_REG_SMALL) {
    arm7.writeReg(1, 0x00000004);
    arm7.writeReg(2, 0x00000003);

    writeProgramToMemory(
        ".thumb\n"
        "MUL R1, R2\n",  // R1 := R1 * R2
        MAIN_RAM_START, &bus, arm7.isARM7());

    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);

    EXPECT_EQ(arm7.readReg(1), 0x0000000C);
}
/**
 * @brief Test MUL where result is zero.
 */
TEST_F(TestCPU_THUMB_DataInstructions_MUL, MUL_REG_ZERO_RESULT) {
    arm7.writeReg(3, 0x0000FFFF);
    arm7.writeReg(4, 0x00000000);

    writeProgramToMemory(
        ".thumb\n"
        "MUL R3, R4\n",
        MAIN_RAM_START, &bus, arm7.isARM7());

    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);

    EXPECT_EQ(arm7.readReg(3), 0x00000000);
}
/**
 * @brief Test MUL's zero flag behaviour.
 */
TEST_F(TestCPU_THUMB_DataInstructions_MUL, MUL_ZERO_FLAG) {
    // Z == 1.
    arm7.setFlag(Z_FLAG, 0);
    arm7.writeReg(1, 0x00000001);
    arm7.writeReg(2, 0x00000000);
    writeProgramToMemory(
        ".thumb\n"
        "MUL R1, R2\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_EQ(arm7.readReg(1), 0);
    EXPECT_TRUE(arm7.readFlag(Z_FLAG));

    // Z == 0.
    arm7.setFlag(Z_FLAG, 1);
    arm7.writeReg(1, 0x00000002);
    arm7.writeReg(2, 0x00000003);
    writeProgramToMemory(
        ".thumb\n"
        "MUL R1, R2\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_EQ(arm7.readReg(1), 0x00000006);
    EXPECT_FALSE(arm7.readFlag(Z_FLAG));
}
/**
 * @brief Test MUL's negative flag behaviour.
 */
TEST_F(TestCPU_THUMB_DataInstructions_MUL, MUL_NEGATIVE_FLAG) {
    // N == 0.
    arm7.setFlag(N_FLAG, 1);
    arm7.writeReg(3, 0x7FFFFFFF);  // MSB=0
    arm7.writeReg(4, 0x00000001);
    writeProgramToMemory(
        ".thumb\n"
        "MUL R3, R4\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_EQ(arm7.readReg(3), 0x7FFFFFFF);
    EXPECT_FALSE(arm7.readFlag(N_FLAG));

    // N == 1.
    arm7.setFlag(N_FLAG, 0);
    arm7.writeReg(3, 0x80000000);  // MSB=1
    arm7.writeReg(4, 0x00000001);
    writeProgramToMemory(
        ".thumb\n"
        "MUL R3, R4\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_EQ(arm7.readReg(3), 0x80000000);
    EXPECT_TRUE(arm7.readFlag(N_FLAG));
}
/**
 * @brief Test MUL's carry flag behaviour.
 */
TEST_F(TestCPU_THUMB_DataInstructions_MUL, MUL_CARRY_FLAG) {
    // C == 1.
    arm7.setFlag(C_FLAG, 1);
    arm7.writeReg(5, 0x00001000);
    arm7.writeReg(6, 0x00001000);
    writeProgramToMemory(
        ".thumb\n"
        "MUL R5, R6\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_EQ(arm7.readReg(5), 0x01000000);
    EXPECT_TRUE(arm7.readFlag(C_FLAG));  // Flag is unchanged.

    // C == 0.
    arm7.setFlag(C_FLAG, 0);
    arm7.writeReg(5, 0x00001000);
    arm7.writeReg(6, 0x00001000);
    writeProgramToMemory(
        ".thumb\n"
        "MUL R5, R6\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_EQ(arm7.readReg(5), 0x01000000);
    EXPECT_FALSE(arm7.readFlag(C_FLAG));  // Flag is unchanged.
}
/**
 * @brief Test MUL's overflow flag behaviour.
 */
TEST_F(TestCPU_THUMB_DataInstructions_MUL, MUL_OVERFLOW_FLAG) {
    // V == 1.
    arm7.setFlag(V_FLAG, 1);
    arm7.writeReg(5, 0x40000000);
    arm7.writeReg(6, 0x00000002);
    writeProgramToMemory(
        ".thumb\n"
        "MUL R5, R6\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_EQ(arm7.readReg(5), 0x80000000);
    EXPECT_TRUE(arm7.readFlag(V_FLAG));  // Flag is unchanged.

    // V == 0.
    arm7.setFlag(V_FLAG, 0);
    arm7.writeReg(5, 0x40000000);
    arm7.writeReg(6, 0x00000002);
    writeProgramToMemory(
        ".thumb\n"
        "MUL R5, R6\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_EQ(arm7.readReg(5), 0x80000000);
    EXPECT_FALSE(arm7.readFlag(V_FLAG));  // Flag is unchanged.
}
// ==================================================================================================
// BIC
// ==================================================================================================
class TestCPU_THUMB_DataInstructions_BIC : public TestCPU_THUMB_DataInstructions {
protected:
    TestCPU_THUMB_DataInstructions_BIC() {}
    ~TestCPU_THUMB_DataInstructions_BIC() {}

    void SetUp() override { TestCPU_THUMB_DataInstructions::SetUp(); }
    void TearDown() override { TestCPU_THUMB_DataInstructions::TearDown(); }
};
/**
 * @brief Test BIC with small-number regs.
 */
TEST_F(TestCPU_THUMB_DataInstructions_BIC, BIC_REG_SMALL) {
    arm7.writeReg(1, 0xFFFFFFFF);
    arm7.writeReg(2, 0x0F0F0F0F);

    writeProgramToMemory(
        ".thumb\n"
        "BIC R1, R2\n",  // R1 := R1 & ~R2
        MAIN_RAM_START, &bus, arm7.isARM7());

    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);

    EXPECT_EQ(arm7.readReg(1), 0xF0F0F0F0);
}
/**
 * @brief Test BIC where result is zero.
 */
TEST_F(TestCPU_THUMB_DataInstructions_BIC, BIC_REG_ZERO_RESULT) {
    arm7.writeReg(3, 0x00FF00FF);
    arm7.writeReg(4, 0x00FF00FF);

    writeProgramToMemory(
        ".thumb\n"
        "BIC R3, R4\n",
        MAIN_RAM_START, &bus, arm7.isARM7());

    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);

    EXPECT_EQ(arm7.readReg(3), 0x00000000);
}
/**
 * @brief Test BIC's zero flag behaviour.
 */
TEST_F(TestCPU_THUMB_DataInstructions_BIC, BIC_ZERO_FLAG) {
    // Z == 1.
    arm7.setFlag(Z_FLAG, 0);
    arm7.writeReg(1, 0x00000000);
    arm7.writeReg(2, 0xFFFFFFFF);
    writeProgramToMemory(
        ".thumb\n"
        "BIC R1, R2\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_EQ(arm7.readReg(1), 0);
    EXPECT_TRUE(arm7.readFlag(Z_FLAG));

    // Z == 0.
    arm7.setFlag(Z_FLAG, 1);
    arm7.writeReg(1, 0xFFFFFFFF);
    arm7.writeReg(2, 0xFFFFFFFE);
    writeProgramToMemory(
        ".thumb\n"
        "BIC R1, R2\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_EQ(arm7.readReg(1), 1);
    EXPECT_FALSE(arm7.readFlag(Z_FLAG));
}
/**
 * @brief Test BIC's negative flag behaviour.
 */
TEST_F(TestCPU_THUMB_DataInstructions_BIC, BIC_NEGATIVE_FLAG) {
    // N == 0.
    arm7.setFlag(N_FLAG, 1);
    arm7.writeReg(3, 0x7FFFFFFF);  // MSB=0
    arm7.writeReg(4, 0x00000000);
    writeProgramToMemory(
        ".thumb\n"
        "BIC R3, R4\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_EQ(arm7.readReg(3), 0x7FFFFFFF);
    EXPECT_FALSE(arm7.readFlag(N_FLAG));

    // N == 1.
    arm7.setFlag(N_FLAG, 0);
    arm7.writeReg(3, 0x80000000);  // MSB=1
    arm7.writeReg(4, 0x00000000);
    writeProgramToMemory(
        ".thumb\n"
        "BIC R3, R4\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_EQ(arm7.readReg(3), 0x80000000);
    EXPECT_TRUE(arm7.readFlag(N_FLAG));
}
/**
 * @brief Test BIC's carry flag behaviour.
 */
TEST_F(TestCPU_THUMB_DataInstructions_BIC, BIC_CARRY_FLAG) {
    // C == 1.
    arm7.setFlag(C_FLAG, 1);
    arm7.writeReg(5, 0xFFFFFFFF);
    arm7.writeReg(6, 0x00000001);
    writeProgramToMemory(
        ".thumb\n"
        "BIC R5, R6\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_EQ(arm7.readReg(5), 0xFFFFFFFE);
    EXPECT_TRUE(arm7.readFlag(C_FLAG));  // Flag is unchanged.

    // C == 0.
    arm7.setFlag(C_FLAG, 0);
    arm7.writeReg(5, 0xFFFFFFFF);
    arm7.writeReg(6, 0x00000001);
    writeProgramToMemory(
        ".thumb\n"
        "BIC R5, R6\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_EQ(arm7.readReg(5), 0xFFFFFFFE);
    EXPECT_FALSE(arm7.readFlag(C_FLAG));  // Flag is unchanged.
}
/**
 * @brief Test BIC's overflow flag behaviour.
 */
TEST_F(TestCPU_THUMB_DataInstructions_BIC, BIC_OVERFLOW_FLAG) {
    // V == 1.
    arm7.setFlag(V_FLAG, 1);
    arm7.writeReg(5, 0xFFFFFFFF);
    arm7.writeReg(6, 0x00000001);
    writeProgramToMemory(
        ".thumb\n"
        "BIC R5, R6\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_EQ(arm7.readReg(5), 0xFFFFFFFE);
    EXPECT_TRUE(arm7.readFlag(V_FLAG));  // Flag is unchanged.

    // V == 0.
    arm7.setFlag(V_FLAG, 0);
    arm7.writeReg(5, 0xFFFFFFFF);
    arm7.writeReg(6, 0x00000001);
    writeProgramToMemory(
        ".thumb\n"
        "BIC R5, R6\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_EQ(arm7.readReg(5), 0xFFFFFFFE);
    EXPECT_FALSE(arm7.readFlag(V_FLAG));  // Flag is unchanged.
}
// ==================================================================================================
// MVN
// ==================================================================================================
class TestCPU_THUMB_DataInstructions_MVN : public TestCPU_THUMB_DataInstructions {
protected:
    TestCPU_THUMB_DataInstructions_MVN() {}
    ~TestCPU_THUMB_DataInstructions_MVN() {}

    void SetUp() override { TestCPU_THUMB_DataInstructions::SetUp(); }
    void TearDown() override { TestCPU_THUMB_DataInstructions::TearDown(); }
};
/**
 * @brief Test MVN with small-number regs.
 */
TEST_F(TestCPU_THUMB_DataInstructions_MVN, MVN_REG_SMALL) {
    arm7.writeReg(2, 0x0F0F0F0F);

    writeProgramToMemory(
        ".thumb\n"
        "MVN R1, R2\n",  // R1 := ~R2
        MAIN_RAM_START, &bus, arm7.isARM7());

    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);

    EXPECT_EQ(arm7.readReg(1), 0xF0F0F0F0);
}
/**
 * @brief Test MVN where result is zero.
 */
TEST_F(TestCPU_THUMB_DataInstructions_MVN, MVN_REG_ZERO_RESULT) {
    arm7.writeReg(4, 0xFFFFFFFF);

    writeProgramToMemory(
        ".thumb\n"
        "MVN R3, R4\n",
        MAIN_RAM_START, &bus, arm7.isARM7());

    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);

    EXPECT_EQ(arm7.readReg(3), 0x00000000);
}
/**
 * @brief Test MVN's zero flag behaviour.
 */
TEST_F(TestCPU_THUMB_DataInstructions_MVN, MVN_ZERO_FLAG) {
    // Z == 1.
    arm7.setFlag(Z_FLAG, 0);
    arm7.writeReg(2, 0xFFFFFFFF);
    writeProgramToMemory(
        ".thumb\n"
        "MVN R1, R2\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_EQ(arm7.readReg(1), 0);
    EXPECT_TRUE(arm7.readFlag(Z_FLAG));

    // Z == 0.
    arm7.setFlag(Z_FLAG, 1);
    arm7.writeReg(2, 0xFFFFFFFE);
    writeProgramToMemory(
        ".thumb\n"
        "MVN R1, R2\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_EQ(arm7.readReg(1), 1);
    EXPECT_FALSE(arm7.readFlag(Z_FLAG));
}
/**
 * @brief Test MVN's negative flag behaviour.
 */
TEST_F(TestCPU_THUMB_DataInstructions_MVN, MVN_NEGATIVE_FLAG) {
    // N == 0.
    arm7.setFlag(N_FLAG, 1);
    arm7.writeReg(4, 0x80000000);  // ~input has MSB=0
    writeProgramToMemory(
        ".thumb\n"
        "MVN R3, R4\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_EQ(arm7.readReg(3), 0x7FFFFFFF);
    EXPECT_FALSE(arm7.readFlag(N_FLAG));

    // N == 1.
    arm7.setFlag(N_FLAG, 0);
    arm7.writeReg(4, 0x7FFFFFFF);  // ~input has MSB=1
    writeProgramToMemory(
        ".thumb\n"
        "MVN R3, R4\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_EQ(arm7.readReg(3), 0x80000000);
    EXPECT_TRUE(arm7.readFlag(N_FLAG));
}
/**
 * @brief Test MVN's carry flag behaviour.
 */
TEST_F(TestCPU_THUMB_DataInstructions_MVN, MVN_CARRY_FLAG) {
    // C == 1.
    arm7.setFlag(C_FLAG, 1);
    arm7.writeReg(6, 0x00000001);
    writeProgramToMemory(
        ".thumb\n"
        "MVN R5, R6\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_EQ(arm7.readReg(5), 0xFFFFFFFE);
    EXPECT_TRUE(arm7.readFlag(C_FLAG));  // Flag is unchanged.

    // C == 0.
    arm7.setFlag(C_FLAG, 0);
    arm7.writeReg(6, 0x00000001);
    writeProgramToMemory(
        ".thumb\n"
        "MVN R5, R6\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_EQ(arm7.readReg(5), 0xFFFFFFFE);
    EXPECT_FALSE(arm7.readFlag(C_FLAG));  // Flag is unchanged.
}
/**
 * @brief Test MVN's overflow flag behaviour.
 */
TEST_F(TestCPU_THUMB_DataInstructions_MVN, MVN_OVERFLOW_FLAG) {
    // V == 1.
    arm7.setFlag(V_FLAG, 1);
    arm7.writeReg(6, 0x00000001);
    writeProgramToMemory(
        ".thumb\n"
        "MVN R5, R6\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_EQ(arm7.readReg(5), 0xFFFFFFFE);
    EXPECT_TRUE(arm7.readFlag(V_FLAG));  // Flag is unchanged.

    // V == 0.
    arm7.setFlag(V_FLAG, 0);
    arm7.writeReg(6, 0x00000001);
    writeProgramToMemory(
        ".thumb\n"
        "MVN R5, R6\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    EXPECT_EQ(arm7.readReg(5), 0xFFFFFFFE);
    EXPECT_FALSE(arm7.readFlag(V_FLAG));  // Flag is unchanged.
}
// ==================================================================================================
// ADR
// ==================================================================================================
class TestCPU_THUMB_DataInstructions_ADR : public TestCPU_THUMB_DataInstructions {
protected:
    TestCPU_THUMB_DataInstructions_ADR() {}
    ~TestCPU_THUMB_DataInstructions_ADR() {}

    void SetUp() override { TestCPU_THUMB_DataInstructions::SetUp(); }
    void TearDown() override { TestCPU_THUMB_DataInstructions::TearDown(); }
};
/**
 * @brief Test ADR with small positive offset.
 */
TEST_F(TestCPU_THUMB_DataInstructions_ADR, ADR_REG_SMALL) {
    writeProgramToMemory(
        ".thumb\n"
        "start:\n"
        "ADR R1, label\n"
        "NOP\n"
        "NOP\n"
        "NOP\n"
        "label:\n",
        MAIN_RAM_START, &bus, arm7.isARM7());

    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute();

    EXPECT_EQ(arm7.readReg(1), MAIN_RAM_START + 0x4 + 0x04);
}
/**
 * @brief Test ADR with larger offset.
 */
TEST_F(TestCPU_THUMB_DataInstructions_ADR, ADR_REG_LARGE_OFFSET) {
    writeProgramToMemory(
        ".thumb\n"
        "start:\n"
        "ADR R1, label\n"
        "NOP\n"
        ".skip 0x3FC\n"
        "label:\n",
        MAIN_RAM_START, &bus, arm7.isARM7());

    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute();

    EXPECT_EQ(arm7.readReg(1), MAIN_RAM_START + 0x3FC + 0x4);
}
/**
 * @brief Test ADR's zero flag behaviour.
 */
TEST_F(TestCPU_THUMB_DataInstructions_ADR, ADR_ZERO_FLAG) {
    // Z == 1.
    arm7.setFlag(Z_FLAG, 1);
    writeProgramToMemory(
        ".thumb\n"
        "start:\n"
        "ADR R1, label\n"
        "NOP\n"
        "label:\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute();
    EXPECT_TRUE(arm7.readFlag(Z_FLAG));  // Flag is unchanged.

    // Z == 0.
    arm7.setFlag(Z_FLAG, 0);
    writeProgramToMemory(
        ".thumb\n"
        "start:\n"
        "ADR R1, label\n"
        "NOP\n"
        "label:\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute();
    EXPECT_FALSE(arm7.readFlag(Z_FLAG));  // Flag is unchanged.
}
/**
 * @brief Test ADR's negative flag behaviour.
 */
TEST_F(TestCPU_THUMB_DataInstructions_ADR, ADR_NEGATIVE_FLAG) {
    // N == 1.
    arm7.setFlag(N_FLAG, 1);
    writeProgramToMemory(
        ".thumb\n"
        "start:\n"
        "ADR R1, label\n"
        "NOP\n"
        "label:\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute();
    EXPECT_TRUE(arm7.readFlag(N_FLAG));  // Flag is unchanged.

    // N == 0.
    arm7.setFlag(N_FLAG, 0);
    writeProgramToMemory(
        ".thumb\n"
        "start:\n"
        "ADR R1, label\n"
        "NOP\n"
        "label:\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute();
    EXPECT_FALSE(arm7.readFlag(N_FLAG));  // Flag is unchanged.
}
/**
 * @brief Test ADR's carry flag behaviour.
 */
TEST_F(TestCPU_THUMB_DataInstructions_ADR, ADR_CARRY_FLAG) {
    // C == 1.
    arm7.setFlag(C_FLAG, 1);
    writeProgramToMemory(
        ".thumb\n"
        "start:\n"
        "ADR R1, label\n"
        "NOP\n"
        "label:\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute();
    EXPECT_TRUE(arm7.readFlag(C_FLAG));  // Flag is unchanged.

    // C == 0.
    arm7.setFlag(C_FLAG, 0);
    writeProgramToMemory(
        ".thumb\n"
        "start:\n"
        "ADR R1, label\n"
        "NOP\n"
        "label:\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute();
    EXPECT_FALSE(arm7.readFlag(C_FLAG));  // Flag is unchanged.
}
/**
 * @brief Test ADR's overflow flag behaviour.
 */
TEST_F(TestCPU_THUMB_DataInstructions_ADR, ADR_OVERFLOW_FLAG) {
    // V == 1.
    arm7.setFlag(V_FLAG, 1);
    writeProgramToMemory(
        ".thumb\n"
        "start:\n"
        "ADR R1, label\n"
        "NOP\n"
        "label:\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute();
    EXPECT_TRUE(arm7.readFlag(V_FLAG));  // Flag is unchanged.

    // V == 0.
    arm7.setFlag(V_FLAG, 0);
    writeProgramToMemory(
        ".thumb\n"
        "start:\n"
        "ADR R1, label\n"
        "NOP\n"
        "label:\n",
        MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute();
    EXPECT_FALSE(arm7.readFlag(V_FLAG));  // Flag is unchanged.
}
