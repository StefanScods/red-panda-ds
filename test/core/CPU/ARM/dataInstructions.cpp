#include <gtest/gtest.h>

#include "commonTest.h"
#include "core/cpu.h"
#include "core/interconnect.h"
#include "core/memoryDefines.h"
#include "core/utils/armEncode.h"

/**
 * @brief Test fixture for the CPU's instructions in the data category.
 */
class TestCPU_ARM_DataInstructions : public testing::Test {
protected:
    Interconnect bus;
    ARM7TDMI arm7{new NDS_ARM7_BIOS()};
    ARM946ES arm9{new NDS_ARM9_BIOS()};
    TestCPU_ARM_DataInstructions() {}
    ~TestCPU_ARM_DataInstructions() {}

    void SetUp() override {
        bus.init();
        bus.bindARM7(&arm7);
        bus.bindARM9(&arm9);
    }

    void TearDown() override {}
};

// ==================================================================================================
// AND
// ==================================================================================================
class TestCPU_ARM_DataInstructions_AND : public TestCPU_ARM_DataInstructions {
protected:
    TestCPU_ARM_DataInstructions_AND() {}
    ~TestCPU_ARM_DataInstructions_AND() {}

    void SetUp() override { TestCPU_ARM_DataInstructions::SetUp(); }
    void TearDown() override { TestCPU_ARM_DataInstructions::TearDown(); }
};
/**
 * @brief Tests an AND operation using an immediate as the second operand.
 */
TEST_F(TestCPU_ARM_DataInstructions_AND, AND_IMMEDIATE) {
    writeProgramToMemory(
        "MOV R1, #0b110011\n"
        "AND R0, R1, #0b110000\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(2);
    ASSERT_EQ(arm7.readReg(0), 0b110000);
}
/**
 * @brief Tests an AND operation's negative flag behaviour using an immediate as the second operand.
 */
TEST_F(TestCPU_ARM_DataInstructions_AND, AND_IMMEDIATE_NEGATIVE_FLAG) {
    // N = true
    writeProgramToMemory(
        "MOV R1, #0xFF000000\n"
        "ANDs R0, R1, #0xF0000000\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(2);
    ASSERT_EQ(arm7.readReg(0), 0xF0000000);
    ASSERT_EQ(arm7.readFlag(N_FLAG), 1);

    // N = false
    arm7.reset();
    writeProgramToMemory(
        "MOV R1, #0x0F000000\n"
        "ANDs R0, R1, #0xF0000000\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(2);
    ASSERT_EQ(arm7.readReg(0), 0x00000000);
    ASSERT_EQ(arm7.readFlag(N_FLAG), 0);

    // Don't update flag.
    arm7.reset();
    writeProgramToMemory(
        "MOV R1, #0xFF000000\n"
        "AND R0, R1, #0xF0000000\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(2);
    ASSERT_EQ(arm7.readReg(0), 0xF0000000);
    ASSERT_EQ(arm7.readFlag(N_FLAG), 0);
}
/**
 * @brief Tests an AND operation's zero flag behaviour using an immediate as the second operand.
 */
TEST_F(TestCPU_ARM_DataInstructions_AND, AND_IMMEDIATE_ZERO_FLAG) {
    // Z = true
    writeProgramToMemory(
        "MOV R1, #0x0F000000\n"
        "ANDs R0, R1, #0xF0000000\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(2);
    ASSERT_EQ(arm7.readReg(0), 0x00000000);
    ASSERT_EQ(arm7.readFlag(Z_FLAG), 1);

    // Z = false
    arm7.reset();
    writeProgramToMemory(
        "MOV R1, #0xFF000000\n"
        "ANDs R0, R1, #0xF0000000\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(2);
    ASSERT_EQ(arm7.readReg(0), 0xF0000000);
    ASSERT_EQ(arm7.readFlag(Z_FLAG), 0);
}
/**
 * @brief Tests an AND operation's carry flag behaviour using an immediate as the second operand.
 */
TEST_F(TestCPU_ARM_DataInstructions_AND, AND_IMMEDIATE_CARRY_FLAG) {
    // C = true
    writeProgramToMemory("ANDs r0, r0, #0x80000000\n", MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readFlag(C_FLAG), 1);

    // C = false
    arm7.reset();
    writeProgramToMemory("ANDs r0, r0, #0x40000000\n", MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readFlag(C_FLAG), 0);
}
/**
 * @brief Tests an AND operation's overflow flag behaviour using an immediate as the second operand.
 */
TEST_F(TestCPU_ARM_DataInstructions_AND, AND_IMMEDIATE_OVERFLOW_FLAG) {
    // V = true
    writeProgramToMemory("ANDs r1, r0, #0x0\n", MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.setFlag(V_FLAG, 1);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readFlag(V_FLAG), 1);
}

// ==================================================================================================
// EOR
// ==================================================================================================
class TestCPU_ARM_DataInstructions_EOR : public TestCPU_ARM_DataInstructions {
protected:
    TestCPU_ARM_DataInstructions_EOR() {}
    ~TestCPU_ARM_DataInstructions_EOR() {}

    void SetUp() override { TestCPU_ARM_DataInstructions::SetUp(); }
    void TearDown() override { TestCPU_ARM_DataInstructions::TearDown(); }
};
/**
 * @brief Tests an EOR operation using an immediate as the second operand.
 */
TEST_F(TestCPU_ARM_DataInstructions_EOR, EOR_IMMEDIATE) {
    writeProgramToMemory(
        "MOV R1, #0b110011\n"
        "EOR R0, R1, #0b101010\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(2);
    ASSERT_EQ(arm7.readReg(0), 0b011001);
}
/**
 * @brief Tests an EOR operation's negative flag behaviour using an immediate as the second operand.
 */
TEST_F(TestCPU_ARM_DataInstructions_EOR, EOR_IMMEDIATE_NEGATIVE_FLAG) {
    // N = true
    writeProgramToMemory(
        "MOV R1, #0xF0000000\n"
        "EORs R0, R1, #0x00000000\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(2);
    ASSERT_EQ(arm7.readReg(0), 0xF0000000);
    ASSERT_EQ(arm7.readFlag(N_FLAG), 1);

    // N = false
    arm7.reset();
    writeProgramToMemory(
        "MOV R1, #0x0F000000\n"
        "EORs R0, R1, #0x00000000\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(2);
    ASSERT_EQ(arm7.readReg(0), 0x0F000000);
    ASSERT_EQ(arm7.readFlag(N_FLAG), 0);

    // Don't update flag.
    arm7.reset();
    writeProgramToMemory(
        "MOV R1, #0xF0000000\n"
        "EOR R0, R1, #0x00000000\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(2);
    ASSERT_EQ(arm7.readReg(0), 0xF0000000);
    ASSERT_EQ(arm7.readFlag(N_FLAG), 0);
}
/**
 * @brief Tests an EOR operation's zero flag behaviour using an immediate as the second operand.
 */
TEST_F(TestCPU_ARM_DataInstructions_EOR, EOR_IMMEDIATE_ZERO_FLAG) {
    // Z = true
    writeProgramToMemory(
        "MOV R1, #0xF0000000\n"
        "EORs R0, R1, #0xF0000000\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(2);
    ASSERT_EQ(arm7.readReg(0), 0x00000000);
    ASSERT_EQ(arm7.readFlag(Z_FLAG), 1);

    // Z = false
    arm7.reset();
    writeProgramToMemory(
        "MOV R1, #0xF0000000\n"
        "EORs R0, R1, #0x00000000\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(2);
    ASSERT_EQ(arm7.readReg(0), 0xF0000000);
    ASSERT_EQ(arm7.readFlag(Z_FLAG), 0);
}
/**
 * @brief Tests an EOR operation's carry flag behaviour using an immediate as the second operand.
 */
TEST_F(TestCPU_ARM_DataInstructions_EOR, EOR_IMMEDIATE_CARRY_FLAG) {
    // C = true
    writeProgramToMemory("EORs r0, r0, #0x80000000\n", MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readFlag(C_FLAG), 1);

    // C = false
    arm7.reset();
    writeProgramToMemory("EORs r0, r0, #0x40000000\n", MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readFlag(C_FLAG), 0);
}
/**
 * @brief Tests an EOR operation's overflow flag behaviour using an immediate as the second operand.
 */
TEST_F(TestCPU_ARM_DataInstructions_EOR, EOR_IMMEDIATE_OVERFLOW_FLAG) {
    // V = true
    writeProgramToMemory("EORs r1, r0, #0x0\n", MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.setFlag(V_FLAG, 1);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readFlag(V_FLAG), 1);
}

// ==================================================================================================
// SUB
// ==================================================================================================
class TestCPU_ARM_DataInstructions_SUB : public TestCPU_ARM_DataInstructions {
protected:
    TestCPU_ARM_DataInstructions_SUB() {}
    ~TestCPU_ARM_DataInstructions_SUB() {}

    void SetUp() override { TestCPU_ARM_DataInstructions::SetUp(); }
    void TearDown() override { TestCPU_ARM_DataInstructions::TearDown(); }
};
/**
 * @brief Tests a SUB operation using an immediate as the second operand.
 */
TEST_F(TestCPU_ARM_DataInstructions_SUB, SUB_IMMEDIATE) {
    writeProgramToMemory(
        "MOV R1, #10\n"
        "SUB R0, R1, #3\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(2);
    ASSERT_EQ(arm7.readReg(0), 7);
}
/**
 * @brief Tests a SUB operation's negative flag behaviour using an immediate as the second operand.
 */
TEST_F(TestCPU_ARM_DataInstructions_SUB, SUB_IMMEDIATE_NEGATIVE_FLAG) {
    // N = true
    writeProgramToMemory(
        "MOV R1, #0\n"
        "SUBs R0, R1, #1\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(2);
    ASSERT_EQ(arm7.readReg(0), 0xFFFFFFFF);
    ASSERT_EQ(arm7.readFlag(N_FLAG), 1);

    // N = false
    arm7.reset();
    writeProgramToMemory(
        "MOV R1, #5\n"
        "SUBs R0, R1, #1\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(2);
    ASSERT_EQ(arm7.readReg(0), 4);
    ASSERT_EQ(arm7.readFlag(N_FLAG), 0);
}
/**
 * @brief Tests a SUB operation's zero flag behaviour using an immediate as the second operand.
 */
TEST_F(TestCPU_ARM_DataInstructions_SUB, SUB_IMMEDIATE_ZERO_FLAG) {
    // Z = true
    writeProgramToMemory(
        "MOV R1, #5\n"
        "SUBs R0, R1, #5\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(2);
    ASSERT_EQ(arm7.readReg(0), 0);
    ASSERT_EQ(arm7.readFlag(Z_FLAG), 1);

    // Z = false
    arm7.reset();
    writeProgramToMemory(
        "MOV R1, #5\n"
        "SUBs R0, R1, #3\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(2);
    ASSERT_EQ(arm7.readReg(0), 2);
    ASSERT_EQ(arm7.readFlag(Z_FLAG), 0);
}
/**
 * @brief Tests a SUB operation's carry flag behaviour using an immediate as the second operand.
 */
TEST_F(TestCPU_ARM_DataInstructions_SUB, SUB_IMMEDIATE_CARRY_FLAG) {
    // C = true
    writeProgramToMemory(
        "MOV r0, #1\n"
        "SUBs r0, r0, #1\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(2);
    ASSERT_EQ(arm7.readFlag(C_FLAG), 1);

    // C = false
    arm7.reset();
    writeProgramToMemory(
        "MOV r0, #0\n"
        "SUBs r0, r0, #1\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(2);
    ASSERT_EQ(arm7.readFlag(C_FLAG), 0);
}
/**
 * @brief Tests a SUB operation's overflow flag behaviour using an immediate as the second operand.
 */
TEST_F(TestCPU_ARM_DataInstructions_SUB, SUB_IMMEDIATE_OVERFLOW_FLAG) {
    // V = true
    writeProgramToMemory(
        "MOV r1, #0x80000000\n"
        "SUBs r0, r1, #1\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(2);
    ASSERT_EQ(arm7.readFlag(V_FLAG), 1);

    // V = false
    arm7.reset();
    writeProgramToMemory(
        "MOV r1, #100\n"
        "SUBs r0, r1, #50\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(2);
    ASSERT_EQ(arm7.readFlag(V_FLAG), 0);
}

// ==================================================================================================
// RSB
// ==================================================================================================
class TestCPU_ARM_DataInstructions_RSB : public TestCPU_ARM_DataInstructions {
protected:
    TestCPU_ARM_DataInstructions_RSB() {}
    ~TestCPU_ARM_DataInstructions_RSB() {}

    void SetUp() override { TestCPU_ARM_DataInstructions::SetUp(); }
    void TearDown() override { TestCPU_ARM_DataInstructions::TearDown(); }
};
/**
 * @brief Tests an RSB operation using an immediate as the second operand.
 */
TEST_F(TestCPU_ARM_DataInstructions_RSB, RSB_IMMEDIATE) {
    writeProgramToMemory(
        "MOV R1, #3\n"
        "RSB R0, R1, #10\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(2);
    ASSERT_EQ(arm7.readReg(0), 7);
}
/**
 * @brief Tests an RSB operation's negative flag behaviour using an immediate as the second operand.
 */
TEST_F(TestCPU_ARM_DataInstructions_RSB, RSB_IMMEDIATE_NEGATIVE_FLAG) {
    // N = true
    writeProgramToMemory(
        "MOV R1, #10\n"
        "RSBs R0, R1, #5\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(2);
    ASSERT_EQ(arm7.readReg(0), -5);
    ASSERT_EQ(arm7.readFlag(N_FLAG), 1);

    // N = false
    arm7.reset();
    writeProgramToMemory(
        "MOV R1, #3\n"
        "RSBs R0, R1, #5\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(2);
    ASSERT_EQ(arm7.readReg(0), 2);
    ASSERT_EQ(arm7.readFlag(N_FLAG), 0);
}
/**
 * @brief Tests an RSB operation's zero flag behaviour using an immediate as the second operand.
 */
TEST_F(TestCPU_ARM_DataInstructions_RSB, RSB_IMMEDIATE_ZERO_FLAG) {
    // Z = true
    writeProgramToMemory(
        "MOV R1, #5\n"
        "RSBs R0, R1, #5\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(2);
    ASSERT_EQ(arm7.readReg(0), 0);
    ASSERT_EQ(arm7.readFlag(Z_FLAG), 1);

    // Z = false
    arm7.reset();
    writeProgramToMemory(
        "MOV R1, #3\n"
        "RSBs R0, R1, #5\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(2);
    ASSERT_EQ(arm7.readReg(0), 2);
    ASSERT_EQ(arm7.readFlag(Z_FLAG), 0);
}
/**
 * @brief Tests an RSB operation's carry flag behaviour using an immediate as the second operand.
 */
TEST_F(TestCPU_ARM_DataInstructions_RSB, RSB_IMMEDIATE_CARRY_FLAG) {
    // C = true
    writeProgramToMemory(
        "MOV r0, #1\n"
        "RSBs r0, r0, #5\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(2);
    ASSERT_EQ(arm7.readFlag(C_FLAG), 1);

    // C = false
    arm7.reset();
    writeProgramToMemory(
        "MOV r0, #10\n"
        "RSBs r0, r0, #5\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(2);
    ASSERT_EQ(arm7.readFlag(C_FLAG), 0);
}
/**
 * @brief Tests an RSB operation's overflow flag behaviour using an immediate as the second operand.
 */
TEST_F(TestCPU_ARM_DataInstructions_RSB, RSB_IMMEDIATE_OVERFLOW_FLAG) {
    // V = true
    writeProgramToMemory(
        "MOV r1, #0x7FFFFFFF\n"
        "RSBs r0, r1, #0x80000000\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(2);
    ASSERT_EQ(arm7.readFlag(V_FLAG), 1);

    // V = false
    arm7.reset();
    writeProgramToMemory(
        "MOV r1, #50\n"
        "RSBs r0, r1, #100\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(2);
    ASSERT_EQ(arm7.readFlag(V_FLAG), 0);
}

// ==================================================================================================
// ADD
// ==================================================================================================
class TestCPU_ARM_DataInstructions_ADD : public TestCPU_ARM_DataInstructions {
protected:
    TestCPU_ARM_DataInstructions_ADD() {}
    ~TestCPU_ARM_DataInstructions_ADD() {}

    void SetUp() override { TestCPU_ARM_DataInstructions::SetUp(); }
    void TearDown() override { TestCPU_ARM_DataInstructions::TearDown(); }
};
/**
 * @brief Tests an ADD operation using an immediate as the second operand.
 */
TEST_F(TestCPU_ARM_DataInstructions_ADD, ADD_IMMEDIATE) {
    writeProgramToMemory(
        "MOV R1, #100\n"
        "ADD R0, R1, #10\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(2);
    ASSERT_EQ(arm7.readReg(0), 110);
}
/**
 * @brief Tests an ADD operation's negative flag behaviour using an immediate as the second operand.
 */
TEST_F(TestCPU_ARM_DataInstructions_ADD, ADD_IMMEDIATE_NEGATIVE_FLAG) {
    // N = true
    writeProgramToMemory(
        "MOV R1, #0xFF000000\n"
        "ADDs R0, R1, #0xFF\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(2);
    ASSERT_EQ(arm7.readReg(0), 0xFF0000FF);
    ASSERT_EQ(arm7.readFlag(N_FLAG), 1);

    // N = false
    arm7.reset();
    writeProgramToMemory(
        "MOV R1, #0x0FF00000\n"
        "ADDs R0, R1, #0xFF\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(2);
    ASSERT_EQ(arm7.readReg(0), 0x0FF000FF);
    ASSERT_EQ(arm7.readFlag(N_FLAG), 0);

    // Don't update flag.
    arm7.reset();
    writeProgramToMemory(
        "MOV R1, #0xFF000000\n"
        "ADD R0, R1, #0xFF\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(2);
    ASSERT_EQ(arm7.readReg(0), 0xFF0000FF);
    ASSERT_EQ(arm7.readFlag(N_FLAG), 0);
}
/**
 * @brief Tests an ADD operation's zero flag behaviour using an immediate as the second operand.
 */
TEST_F(TestCPU_ARM_DataInstructions_ADD, ADD_IMMEDIATE_ZERO_FLAG) {
    // Z = true
    writeProgramToMemory(
        "MOV R9, #0\n"
        "ADDs R10, R9, #0\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(2);
    ASSERT_EQ(arm7.readReg(10), 0);
    ASSERT_EQ(arm7.readFlag(Z_FLAG), 1);

    // Z = false
    arm7.reset();
    writeProgramToMemory(
        "MOV R5, #1\n"
        "ADDs R4, R5, #7\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(2);
    ASSERT_EQ(arm7.readReg(4), 8);
    ASSERT_EQ(arm7.readFlag(Z_FLAG), 0);
}
/**
 * @brief Tests an ADD operation's carry flag behaviour using an immediate as the second operand.
 */
TEST_F(TestCPU_ARM_DataInstructions_ADD, ADD_IMMEDIATE_CARRY_FLAG) {
    // C = true
    writeProgramToMemory(
        "MOV R1, #0xFF\n"
        "ADD R1, R1, #0xFF00\n"
        "ADD R1, R1, #0xFF0000\n"
        "ADD R1, R1, #0xFF000000\n"
        "ADDs R1, R1, #1\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(5);
    ASSERT_EQ(arm7.readReg(1), 0);
    ASSERT_EQ(arm7.readFlag(C_FLAG), 1);

    // C = false
    arm7.reset();
    writeProgramToMemory(
        "MOV R1, #0xFF\n"
        "ADDs R1, R1, #0xFF00\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(2);
    ASSERT_EQ(arm7.readReg(1), 0xFFFF);
    ASSERT_EQ(arm7.readFlag(C_FLAG), 0);
}
/**
 * @brief Tests an ADD operation's overflow flag behaviour using an immediate as the second operand.
 */
TEST_F(TestCPU_ARM_DataInstructions_ADD, ADD_IMMEDIATE_OVERFLOW_FLAG) {
    // V = true
    writeProgramToMemory(
        "MOV R1, #0xFF\n"
        "ADD R1, R1, #0xFF00\n"
        "ADD R1, R1, #0xFF0000\n"
        "ADD R1, R1, #0x7F000000\n"
        "ADDs R1, R1, #1\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(5);
    ASSERT_EQ(arm7.readReg(1), 0x80000000);
    ASSERT_EQ(arm7.readFlag(V_FLAG), 1);

    // V = false
    arm7.reset();
    writeProgramToMemory(
        "MOV R9, #1\n"
        "ADDs R7, R9, #7\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(2);
    ASSERT_EQ(arm7.readReg(7), 8);
    ASSERT_EQ(arm7.readFlag(V_FLAG), 0);
}
// ==================================================================================================
// ADC
// ==================================================================================================
class TestCPU_ARM_DataInstructions_ADC : public TestCPU_ARM_DataInstructions {
protected:
    TestCPU_ARM_DataInstructions_ADC() {}
    ~TestCPU_ARM_DataInstructions_ADC() {}

    void SetUp() override { TestCPU_ARM_DataInstructions::SetUp(); }
    void TearDown() override { TestCPU_ARM_DataInstructions::TearDown(); }
};
/**
 * @brief Tests an ADC operation using an immediate as the second operand.
 */
TEST_F(TestCPU_ARM_DataInstructions_ADC, ADC_IMMEDIATE) {
    arm7.setFlag(C_FLAG, 1);
    writeProgramToMemory(
        "MOV R1, #5\n"
        "ADC R0, R1, #3\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(2);
    ASSERT_EQ(arm7.readReg(0), 9);
}
/**
 * @brief Tests an ADC operation's negative flag behaviour using an immediate as the second operand.
 */
TEST_F(TestCPU_ARM_DataInstructions_ADC, ADC_IMMEDIATE_NEGATIVE_FLAG) {
    // N = true
    arm7.setFlag(C_FLAG, 1);
    writeProgramToMemory(
        "MOV R1, #0x7FFFFFFE\n"
        "ADCs R0, R1, #1\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(2);
    ASSERT_EQ(arm7.readFlag(N_FLAG), 1);

    // N = false
    arm7.reset();
    arm7.setFlag(C_FLAG, 1);
    writeProgramToMemory(
        "MOV R1, #1\n"
        "ADCs R0, R1, #1\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(2);
    ASSERT_EQ(arm7.readFlag(N_FLAG), 0);
}
/**
 * @brief Tests an ADC operation's zero flag behaviour using an immediate as the second operand.
 */
TEST_F(TestCPU_ARM_DataInstructions_ADC, ADC_IMMEDIATE_ZERO_FLAG) {
    // Z = true
    arm7.setFlag(C_FLAG, 1);
    writeProgramToMemory(
        "MOV R1, #0\n"
        "ADCs R0, R1, #0\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(2);
    ASSERT_EQ(arm7.readReg(0), 1);
    ASSERT_EQ(arm7.readFlag(Z_FLAG), 0);

    // Z = false
    arm7.reset();
    arm7.setFlag(C_FLAG, 1);
    writeProgramToMemory(
        "MOV R1, #1\n"
        "ADCs R0, R1, #1\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(2);
    ASSERT_EQ(arm7.readReg(0), 3);
    ASSERT_EQ(arm7.readFlag(Z_FLAG), 0);
}
/**
 * @brief Tests an ADC operation's carry flag behaviour using an immediate as the second operand.
 */
TEST_F(TestCPU_ARM_DataInstructions_ADC, ADC_IMMEDIATE_CARRY_FLAG) {
    // C = true
    arm7.setFlag(C_FLAG, 1);
    writeProgramToMemory(
        "MOV R1, #0xFFFFFFFF\n"
        "ADCs R0, R1, #1\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(2);
    ASSERT_EQ(arm7.readFlag(C_FLAG), 1);

    // C = false
    arm7.reset();
    arm7.setFlag(C_FLAG, 1);
    writeProgramToMemory(
        "MOV R1, #1\n"
        "ADCs R0, R1, #1\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(2);
    ASSERT_EQ(arm7.readFlag(C_FLAG), 0);
}
/**
 * @brief Tests an ADC operation's overflow flag behaviour using an immediate as the second operand.
 */
TEST_F(TestCPU_ARM_DataInstructions_ADC, ADC_IMMEDIATE_OVERFLOW_FLAG) {
    // V = true
    arm7.setFlag(C_FLAG, 1);
    writeProgramToMemory(
        "MOV R1, #0x7FFFFFFF\n"
        "ADCs R0, R1, #1\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(2);
    ASSERT_EQ(arm7.readFlag(V_FLAG), 1);

    // V = false
    arm7.reset();
    arm7.setFlag(C_FLAG, 1);
    writeProgramToMemory(
        "MOV R1, #10\n"
        "ADCs R0, R1, #20\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(2);
    ASSERT_EQ(arm7.readFlag(V_FLAG), 0);
}

// ==================================================================================================
// SBC
// ==================================================================================================
class TestCPU_ARM_DataInstructions_SBC : public TestCPU_ARM_DataInstructions {
protected:
    TestCPU_ARM_DataInstructions_SBC() {}
    ~TestCPU_ARM_DataInstructions_SBC() {}

    void SetUp() override { TestCPU_ARM_DataInstructions::SetUp(); }
    void TearDown() override { TestCPU_ARM_DataInstructions::TearDown(); }
};
/**
 * @brief Tests an SBC operation using an immediate as the second operand.
 */
TEST_F(TestCPU_ARM_DataInstructions_SBC, SBC_IMMEDIATE) {
    arm7.setFlag(C_FLAG, 1);
    writeProgramToMemory(
        "MOV R1, #10\n"
        "SBC R0, R1, #3\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(2);
    ASSERT_EQ(arm7.readReg(0), 7);
}
/**
 * @brief Tests an SBC operation's negative flag behaviour using an immediate as the second operand.
 */
TEST_F(TestCPU_ARM_DataInstructions_SBC, SBC_IMMEDIATE_NEGATIVE_FLAG) {
    // N = true
    arm7.setFlag(C_FLAG, 1);
    writeProgramToMemory(
        "MOV R1, #5\n"
        "SBCs R0, R1, #10\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(2);
    ASSERT_EQ(arm7.readFlag(N_FLAG), 1);

    // N = false
    arm7.reset();
    arm7.setFlag(C_FLAG, 1);
    writeProgramToMemory(
        "MOV R1, #5\n"
        "SBCs R0, R1, #3\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(2);
    ASSERT_EQ(arm7.readFlag(N_FLAG), 0);
}
/**
 * @brief Tests an SBC operation's zero flag behaviour using an immediate as the second operand.
 */
TEST_F(TestCPU_ARM_DataInstructions_SBC, SBC_IMMEDIATE_ZERO_FLAG) {
    // Z = true
    arm7.setFlag(C_FLAG, 1);
    writeProgramToMemory(
        "MOV R1, #5\n"
        "SBCs R0, R1, #5\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(2);
    ASSERT_EQ(arm7.readReg(0), 0);
    ASSERT_EQ(arm7.readFlag(Z_FLAG), 1);

    // Z = false
    arm7.reset();
    arm7.setFlag(C_FLAG, 1);
    writeProgramToMemory(
        "MOV R1, #5\n"
        "SBCs R0, R1, #3\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(2);
    ASSERT_EQ(arm7.readFlag(Z_FLAG), 0);
}
/**
 * @brief Tests an SBC operation's carry flag behaviour using an immediate as the second operand.
 */
TEST_F(TestCPU_ARM_DataInstructions_SBC, SBC_IMMEDIATE_CARRY_FLAG) {
    // C = true
    arm7.setFlag(C_FLAG, 1);
    writeProgramToMemory(
        "MOV R1, #5\n"
        "SBCs R0, R1, #3\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(2);
    ASSERT_EQ(arm7.readFlag(C_FLAG), 1);

    // C = false
    arm7.reset();
    arm7.setFlag(C_FLAG, 1);
    writeProgramToMemory(
        "MOV R1, #3\n"
        "SBCs R0, R1, #5\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(2);
    ASSERT_EQ(arm7.readFlag(C_FLAG), 0);
}
/**
 * @brief Tests an SBC operation's overflow flag behaviour using an immediate as the second operand.
 */
TEST_F(TestCPU_ARM_DataInstructions_SBC, SBC_IMMEDIATE_OVERFLOW_FLAG) {
    // V = true
    arm7.setFlag(C_FLAG, 1);
    writeProgramToMemory(
        "MOV R1, #0x7FFFFFFF\n"
        "SBCs R0, R1, #0x80000000\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(2);
    ASSERT_EQ(arm7.readFlag(V_FLAG), 1);

    // V = false
    arm7.reset();
    arm7.setFlag(C_FLAG, 1);
    writeProgramToMemory(
        "MOV R1, #50\n"
        "SBCs R0, R1, #20\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(2);
    ASSERT_EQ(arm7.readFlag(V_FLAG), 0);
}

// ==================================================================================================
// RSC
// ==================================================================================================
class TestCPU_ARM_DataInstructions_RSC : public TestCPU_ARM_DataInstructions {
protected:
    TestCPU_ARM_DataInstructions_RSC() {}
    ~TestCPU_ARM_DataInstructions_RSC() {}

    void SetUp() override { TestCPU_ARM_DataInstructions::SetUp(); }
    void TearDown() override { TestCPU_ARM_DataInstructions::TearDown(); }
};
/**
 * @brief Tests an RSC operation using an immediate as the second operand.
 */
TEST_F(TestCPU_ARM_DataInstructions_RSC, RSC_IMMEDIATE) {
    arm7.setFlag(C_FLAG, 1);
    writeProgramToMemory(
        "MOV R1, #3\n"
        "RSC R0, R1, #10\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(2);
    ASSERT_EQ(arm7.readReg(0), 7);  // (-3 - 1) + 10 + 1
}
/**
 * @brief Tests an RSC operation's negative flag behaviour using an immediate as the second operand.
 */
TEST_F(TestCPU_ARM_DataInstructions_RSC, RSC_IMMEDIATE_NEGATIVE_FLAG) {
    // N = true
    arm7.setFlag(C_FLAG, 1);
    writeProgramToMemory(
        "MOV R1, #10\n"
        "RSCs R0, R1, #5\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(2);
    ASSERT_EQ(arm7.readFlag(N_FLAG), 1);

    // N = false
    arm7.reset();
    arm7.setFlag(C_FLAG, 1);
    writeProgramToMemory(
        "MOV R1, #3\n"
        "RSCs R0, R1, #5\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(2);
    ASSERT_EQ(arm7.readFlag(N_FLAG), 0);
}
/**
 * @brief Tests an RSC operation's zero flag behaviour using an immediate as the second operand.
 */
TEST_F(TestCPU_ARM_DataInstructions_RSC, RSC_IMMEDIATE_ZERO_FLAG) {
    arm7.setFlag(C_FLAG, 1);
    writeProgramToMemory(
        "MOV R1, #3\n"
        "RSCs R0, R1, #3\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(2);
    ASSERT_EQ(arm7.readReg(0), 0);
    ASSERT_EQ(arm7.readFlag(Z_FLAG), 1);

    arm7.reset();
    arm7.setFlag(C_FLAG, 1);
    writeProgramToMemory(
        "MOV R1, #3\n"
        "RSCs R0, R1, #2\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(2);
    ASSERT_EQ(arm7.readFlag(Z_FLAG), 0);
}
/**
 * @brief Tests an RSC operation's carry flag behaviour using an immediate as the second operand.
 */
TEST_F(TestCPU_ARM_DataInstructions_RSC, RSC_IMMEDIATE_CARRY_FLAG) {
    // C = true
    arm7.setFlag(C_FLAG, 1);
    writeProgramToMemory(
        "MOV R1, #3\n"
        "RSCs R0, R1, #5\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(2);
    ASSERT_EQ(arm7.readFlag(C_FLAG), 1);

    // C = false
    arm7.reset();
    arm7.setFlag(C_FLAG, 1);
    writeProgramToMemory(
        "MOV R1, #5\n"
        "RSCs R0, R1, #3\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(2);
    ASSERT_EQ(arm7.readFlag(C_FLAG), 0);
}
/**
 * @brief Tests an RSC operation's overflow flag behaviour using an immediate as the second operand.
 */
TEST_F(TestCPU_ARM_DataInstructions_RSC, RSC_IMMEDIATE_OVERFLOW_FLAG) {
    arm7.setFlag(C_FLAG, 1);
    writeProgramToMemory(
        "MOV R1, #0x7FFFFFFF\n"
        "RSCs R0, R1, #0x80000000\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(2);
    ASSERT_EQ(arm7.readFlag(V_FLAG), 1);

    arm7.reset();
    arm7.setFlag(C_FLAG, 1);
    writeProgramToMemory(
        "MOV R1, #20\n"
        "RSCs R0, R1, #50\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(2);
    ASSERT_EQ(arm7.readFlag(V_FLAG), 0);
}

// ==================================================================================================
// TST
// ==================================================================================================
class TestCPU_ARM_DataInstructions_TST : public TestCPU_ARM_DataInstructions {
protected:
    TestCPU_ARM_DataInstructions_TST() {}
    ~TestCPU_ARM_DataInstructions_TST() {}

    void SetUp() override { TestCPU_ARM_DataInstructions::SetUp(); }
    void TearDown() override { TestCPU_ARM_DataInstructions::TearDown(); }
};
/**
 * @brief Tests a TST operation's negative flag behaviour using an immediate as the second operand.
 */
TEST_F(TestCPU_ARM_DataInstructions_TST, TST_IMMEDIATE_NEGATIVE_FLAG) {
    // N = true
    writeProgramToMemory(
        "MOV R1, #0x80000000\n"
        "TST R1, #0xF0000000\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(2);
    ASSERT_EQ(arm7.readFlag(N_FLAG), 1);

    // N = false
    arm7.reset();
    writeProgramToMemory(
        "MOV R1, #0x7FFFFFFF\n"
        "TST R1, #0xF0000000\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(2);
    ASSERT_EQ(arm7.readFlag(N_FLAG), 0);
}
/**
 * @brief Tests a TST operation's zero flag behaviour using an immediate as the second operand.
 */
TEST_F(TestCPU_ARM_DataInstructions_TST, TST_IMMEDIATE_ZERO_FLAG) {
    // Z = true
    writeProgramToMemory(
        "MOV R1, #0x5\n"
        "TST R1, #0x2\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(2);
    ASSERT_EQ(arm7.readFlag(Z_FLAG), 1);

    // Z = false
    arm7.reset();
    writeProgramToMemory(
        "MOV R1, #0x5\n"
        "TST R1, #0x1\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(2);
    ASSERT_EQ(arm7.readFlag(Z_FLAG), 0);
}
/**
 * @brief Tests a TST operation's carry flag behaviour using a rotated immediate.
 */
TEST_F(TestCPU_ARM_DataInstructions_TST, TST_IMMEDIATE_CARRY_FLAG) {
    // C = true
    writeProgramToMemory(
        "MOV R1, #1\n"
        "TST R1, #0x80000000\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(2);
    ASSERT_EQ(arm7.readFlag(C_FLAG), 1);

    // C = false
    arm7.reset();
    writeProgramToMemory(
        "MOV R1, #1\n"
        "TST R1, #0x1\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(2);
    ASSERT_EQ(arm7.readFlag(C_FLAG), 0);
}

// ==================================================================================================
// TEQ
// ==================================================================================================
class TestCPU_ARM_DataInstructions_TEQ : public TestCPU_ARM_DataInstructions {
protected:
    TestCPU_ARM_DataInstructions_TEQ() {}
    ~TestCPU_ARM_DataInstructions_TEQ() {}

    void SetUp() override { TestCPU_ARM_DataInstructions::SetUp(); }
    void TearDown() override { TestCPU_ARM_DataInstructions::TearDown(); }
};
/**
 * @brief Tests a TEQ operation's negative flag behaviour using an immediate as the second operand.
 */
TEST_F(TestCPU_ARM_DataInstructions_TEQ, TEQ_IMMEDIATE_NEGATIVE_FLAG) {
    // N = true
    writeProgramToMemory(
        "MOV R1, #0x80000000\n"
        "TEQ R1, #0\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(2);
    ASSERT_EQ(arm7.readFlag(N_FLAG), 1);

    // N = false
    arm7.reset();
    writeProgramToMemory(
        "MOV R1, #0x7FFFFFFF\n"
        "TEQ R1, #0\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(2);
    ASSERT_EQ(arm7.readFlag(N_FLAG), 0);
}
/**
 * @brief Tests a TEQ operation's zero flag behaviour using an immediate as the second operand.
 */
TEST_F(TestCPU_ARM_DataInstructions_TEQ, TEQ_IMMEDIATE_ZERO_FLAG) {
    // Z = true
    writeProgramToMemory(
        "MOV R1, #0x5\n"
        "TEQ R1, #0x5\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(2);
    ASSERT_EQ(arm7.readFlag(Z_FLAG), 1);

    // Z = false
    arm7.reset();
    writeProgramToMemory(
        "MOV R1, #0x5\n"
        "TEQ R1, #0x3\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(2);
    ASSERT_EQ(arm7.readFlag(Z_FLAG), 0);
}
/**
 * @brief Tests a TEQ operation's carry flag behaviour using a rotated immediate.
 */
TEST_F(TestCPU_ARM_DataInstructions_TEQ, TEQ_IMMEDIATE_CARRY_FLAG) {
    // C = true
    writeProgramToMemory(
        "MOV R1, #1\n"
        "TEQ R1, #0x80000000\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(2);
    ASSERT_EQ(arm7.readFlag(C_FLAG), 1);

    // C = false
    arm7.reset();
    writeProgramToMemory(
        "MOV R1, #1\n"
        "TEQ R1, #0x1\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(2);
    ASSERT_EQ(arm7.readFlag(C_FLAG), 0);
}

// ==================================================================================================
// CMP
// ==================================================================================================
class TestCPU_ARM_DataInstructions_CMP : public TestCPU_ARM_DataInstructions {
protected:
    TestCPU_ARM_DataInstructions_CMP() {}
    ~TestCPU_ARM_DataInstructions_CMP() {}

    void SetUp() override { TestCPU_ARM_DataInstructions::SetUp(); }
    void TearDown() override { TestCPU_ARM_DataInstructions::TearDown(); }
};
/**
 * @brief Tests a CMP operation's negative flag behaviour using an immediate as the second operand.
 */
TEST_F(TestCPU_ARM_DataInstructions_CMP, CMP_IMMEDIATE_NEGATIVE_FLAG) {
    // N = true
    writeProgramToMemory(
        "MOV R1, #3\n"
        "CMP R1, #5\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(2);
    ASSERT_EQ(arm7.readFlag(N_FLAG), 1);

    // N = false
    arm7.reset();
    writeProgramToMemory(
        "MOV R1, #5\n"
        "CMP R1, #3\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(2);
    ASSERT_EQ(arm7.readFlag(N_FLAG), 0);
}
/**
 * @brief Tests a CMP operation's zero flag behaviour using an immediate as the second operand.
 */
TEST_F(TestCPU_ARM_DataInstructions_CMP, CMP_IMMEDIATE_ZERO_FLAG) {
    // Z = true
    writeProgramToMemory(
        "MOV R1, #5\n"
        "CMP R1, #5\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(2);
    ASSERT_EQ(arm7.readFlag(Z_FLAG), 1);

    // Z = false
    arm7.reset();
    writeProgramToMemory(
        "MOV R1, #5\n"
        "CMP R1, #3\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(2);
    ASSERT_EQ(arm7.readFlag(Z_FLAG), 0);
}
/**
 * @brief Tests a CMP operation's carry flag behaviour using an immediate as the second operand.
 */
TEST_F(TestCPU_ARM_DataInstructions_CMP, CMP_IMMEDIATE_CARRY_FLAG) {
    // C = true
    writeProgramToMemory(
        "MOV R1, #5\n"
        "CMP R1, #3\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(2);
    ASSERT_EQ(arm7.readFlag(C_FLAG), 1);

    // C = false
    arm7.reset();
    writeProgramToMemory(
        "MOV R1, #3\n"
        "CMP R1, #5\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(2);
    ASSERT_EQ(arm7.readFlag(C_FLAG), 0);
}
/**
 * @brief Tests a CMP operation's overflow flag behaviour using an immediate as the second operand.
 */
TEST_F(TestCPU_ARM_DataInstructions_CMP, CMP_IMMEDIATE_OVERFLOW_FLAG) {
    // V = true
    writeProgramToMemory(
        "MOV R1, #0x80000000\n"
        "CMP R1, #1\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(2);
    ASSERT_EQ(arm7.readFlag(V_FLAG), 1);

    // V = false
    arm7.reset();
    writeProgramToMemory(
        "MOV R1, #100\n"
        "CMP R1, #50\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(2);
    ASSERT_EQ(arm7.readFlag(V_FLAG), 0);
}
// ==================================================================================================
// CMN
// ==================================================================================================
class TestCPU_ARM_DataInstructions_CMN : public TestCPU_ARM_DataInstructions {
protected:
    TestCPU_ARM_DataInstructions_CMN() {}
    ~TestCPU_ARM_DataInstructions_CMN() {}

    void SetUp() override { TestCPU_ARM_DataInstructions::SetUp(); }
    void TearDown() override { TestCPU_ARM_DataInstructions::TearDown(); }
};
/**
 * @brief Tests a CMN operation's negative flag behaviour using an immediate as the second operand.
 */
TEST_F(TestCPU_ARM_DataInstructions_CMN, CMN_IMMEDIATE_NEGATIVE_FLAG) {
    // N = true
    writeProgramToMemory(
        "MOV R1, #0x80000000\n"
        "CMN R1, #1\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(2);
    ASSERT_EQ(arm7.readFlag(N_FLAG), 1);

    // N = false
    arm7.reset();
    writeProgramToMemory(
        "MOV R1, #5\n"
        "CMN R1, #3\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(2);
    ASSERT_EQ(arm7.readFlag(N_FLAG), 0);
}
/**
 * @brief Tests a CMN operation's zero flag behaviour using an immediate as the second operand.
 */
TEST_F(TestCPU_ARM_DataInstructions_CMN, CMN_IMMEDIATE_ZERO_FLAG) {
    // Z = true
    writeProgramToMemory(
        "MOV R1, #0\n"
        "CMN R1, #0\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(2);
    ASSERT_EQ(arm7.readFlag(Z_FLAG), 1);

    // Z = false
    arm7.reset();
    writeProgramToMemory(
        "MOV R1, #5\n"
        "CMN R1, #3\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(2);
    ASSERT_EQ(arm7.readFlag(Z_FLAG), 0);
}
/**
 * @brief Tests a CMN operation's carry flag behaviour using an immediate as the second operand.
 */
TEST_F(TestCPU_ARM_DataInstructions_CMN, CMN_IMMEDIATE_CARRY_FLAG) {
    // C = true
    writeProgramToMemory(
        "MOV R1, #0xFFFFFFFF\n"
        "CMN R1, #1\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(2);
    ASSERT_EQ(arm7.readFlag(C_FLAG), 1);

    // C = false
    arm7.reset();
    writeProgramToMemory(
        "MOV R1, #5\n"
        "CMN R1, #3\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(2);
    ASSERT_EQ(arm7.readFlag(C_FLAG), 0);
}
/**
 * @brief Tests a CMN operation's overflow flag behaviour using an immediate as the second operand.
 */
TEST_F(TestCPU_ARM_DataInstructions_CMN, CMN_IMMEDIATE_OVERFLOW_FLAG) {
    // V = true
    writeProgramToMemory(
        "MOV R1, #0x7FFFFFFF\n"
        "CMN R1, #1\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(2);
    ASSERT_EQ(arm7.readFlag(V_FLAG), 1);

    // V = false
    arm7.reset();
    writeProgramToMemory(
        "MOV R1, #100\n"
        "CMN R1, #50\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(2);
    ASSERT_EQ(arm7.readFlag(V_FLAG), 0);
}

// ==================================================================================================
// ORR
// ==================================================================================================
class TestCPU_ARM_DataInstructions_ORR : public TestCPU_ARM_DataInstructions {
protected:
    TestCPU_ARM_DataInstructions_ORR() {}
    ~TestCPU_ARM_DataInstructions_ORR() {}

    void SetUp() override { TestCPU_ARM_DataInstructions::SetUp(); }
    void TearDown() override { TestCPU_ARM_DataInstructions::TearDown(); }
};
/**
 * @brief Tests an ORR operation using an immediate as the second operand.
 */
TEST_F(TestCPU_ARM_DataInstructions_ORR, ORR_IMMEDIATE) {
    writeProgramToMemory(
        "MOV R1, #0b110001\n"
        "ORR R0, R1, #0b101100\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(2);
    ASSERT_EQ(arm7.readReg(0), 0b111101);
}
/**
 * @brief Tests an ORR operation using an immediate as the second operand.
 */
TEST_F(TestCPU_ARM_DataInstructions_ORR, ORR_IMMEDIATE_NEGATIVE_FLAG) {
    // N = true
    writeProgramToMemory(
        "MOV R1, #0x80000000\n"
        "ORRs R0, R1, #0x1\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(2);
    ASSERT_EQ(arm7.readFlag(N_FLAG), 1);

    // N = false
    arm7.reset();
    writeProgramToMemory(
        "MOV R1, #0x7FFFFFFF\n"
        "ORRs R0, R1, #0x1\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(2);
    ASSERT_EQ(arm7.readFlag(N_FLAG), 0);
}
/**
 * @brief Tests an ORR operation's zero flag behaviour using an immediate as the second operand.
 */
TEST_F(TestCPU_ARM_DataInstructions_ORR, ORR_IMMEDIATE_ZERO_FLAG) {
    // Z = true
    writeProgramToMemory(
        "MOV R1, #0\n"
        "ORRs R0, R1, #0\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(2);
    ASSERT_EQ(arm7.readFlag(Z_FLAG), 1);

    // Z = false
    arm7.reset();
    writeProgramToMemory(
        "MOV R1, #0\n"
        "ORRs R0, R1, #1\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(2);
    ASSERT_EQ(arm7.readFlag(Z_FLAG), 0);
}
/**
 * @brief Tests an ORR operation's carry flag behaviour using an immediate as the second operand.
 */
TEST_F(TestCPU_ARM_DataInstructions_ORR, ORR_IMMEDIATE_CARRY_FLAG) {
    // C = true
    writeProgramToMemory(
        "MOV R1, #1\n"
        "ORRs R0, R1, #0x80000000\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(2);
    ASSERT_EQ(arm7.readFlag(C_FLAG), 1);

    // C = false
    arm7.reset();
    writeProgramToMemory(
        "MOV R1, #1\n"
        "ORRs R0, R1, #1\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(2);
    ASSERT_EQ(arm7.readFlag(C_FLAG), 0);
}

// ==================================================================================================
// MOV
// ==================================================================================================
class TestCPU_ARM_DataInstructions_MOV : public TestCPU_ARM_DataInstructions {
protected:
    TestCPU_ARM_DataInstructions_MOV() {}
    ~TestCPU_ARM_DataInstructions_MOV() {}

    void SetUp() override { TestCPU_ARM_DataInstructions::SetUp(); }
    void TearDown() override { TestCPU_ARM_DataInstructions::TearDown(); }
};

/**
 * @brief Test moving value between registers.
 */
TEST_F(TestCPU_ARM_DataInstructions_MOV, MOV_REGISTER) {
    writeProgramToMemory(
        "MOV R1, #0xFF000000\n"
        "MOV R0, R1\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(2);
    ASSERT_EQ(arm7.readReg(0), 0xFF000000);
}
/**
 * @brief Test moving value between registers and applying a LSL.
 */
TEST_F(TestCPU_ARM_DataInstructions_MOV, MOV_REGISTER_LSL) {
    writeProgramToMemory(
        "MOV R1, #0x00FF0000\n"
        "MOV R0, R1, LSL#8\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(2);
    ASSERT_EQ(arm7.readReg(0), 0xFF000000);
}
/**
 * @brief Test moving value between registers and applying a LSR.
 */
TEST_F(TestCPU_ARM_DataInstructions_MOV, MOV_REGISTER_LSR) {
    writeProgramToMemory(
        "MOV R1, #0x00FF0000\n"
        "MOV R0, R1, LSR#8\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(2);
    ASSERT_EQ(arm7.readReg(0), 0x0000FF00);
}
/**
 * @brief Test moving value between registers and applying a ASR.
 */
TEST_F(TestCPU_ARM_DataInstructions_MOV, MOV_REGISTER_ASR) {
    writeProgramToMemory(
        "MOV R1, #0xFF000000\n"
        "MOV R0, R1, ASR#8\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(2);
    ASSERT_EQ(arm7.readReg(0), 0xFFFF0000);
}
/**
 * @brief Test moving value between registers and applying a ROR.
 */
TEST_F(TestCPU_ARM_DataInstructions_MOV, MOV_REGISTER_ROR) {
    writeProgramToMemory(
        "MOV R1, #0xF000000F\n"
        "MOV R0, R1, ROR#8\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(2);
    ASSERT_EQ(arm7.readReg(0), 0x0FF00000);
}
/**
 * @brief Test moving value between registers and applying a RRX.
 */
TEST_F(TestCPU_ARM_DataInstructions_MOV, MOV_REGISTER_RRX) {
    arm7.setFlag(C_FLAG, 1);
    writeProgramToMemory(
        "MOV R1, #0x00000001\n"
        "MOVs R0, R1, RRX\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(2);
    ASSERT_EQ(arm7.readReg(0), 0x80000000);
    ASSERT_EQ(arm7.readFlag(C_FLAG), 1);
}
/**
 * @brief Test moving value between registers and applying a LSL with a register value amount.
 */
TEST_F(TestCPU_ARM_DataInstructions_MOV, MOV_REGISTER_LSL_REG) {
    writeProgramToMemory(
        "MOV R2, #0x00000008\n"
        "MOV R1, #0x00FF0000\n"
        "MOV R0, R1, LSL R2\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(3);
    ASSERT_EQ(arm7.readReg(0), 0xFF000000);
}
/**
 * @brief Test moving value between registers and applying a LSR with a register value amount.
 */
TEST_F(TestCPU_ARM_DataInstructions_MOV, MOV_REGISTER_LSR_REG) {
    writeProgramToMemory(
        "MOV R2, #0x00000008\n"
        "MOV R1, #0x00FF0000\n"
        "MOV R0, R1, LSR R2\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(3);
    ASSERT_EQ(arm7.readReg(0), 0x0000FF00);
}
/**
 * @brief Test moving value between registers and applying a ASR with a register value amount.
 */
TEST_F(TestCPU_ARM_DataInstructions_MOV, MOV_REGISTER_ASR_REG) {
    writeProgramToMemory(
        "MOV R2, #0x00000008\n"
        "MOV R1, #0xFF000000\n"
        "MOV R0, R1, ASR R2\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(3);
    ASSERT_EQ(arm7.readReg(0), 0xFFFF0000);
}
/**
 * @brief Test moving value between registers and applying a ROR with a register value amount.
 */
TEST_F(TestCPU_ARM_DataInstructions_MOV, MOV_REGISTER_ROR_REG) {
    writeProgramToMemory(
        "MOV R2, #0x00000008\n"
        "MOV R1, #0xF000000F\n"
        "MOV R0, R1, ROR R2\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(3);
    ASSERT_EQ(arm7.readReg(0), 0x0FF00000);
}
/**
 * @brief Test moving value between registers and applying a ROR with a register value amount (0).
 */
TEST_F(TestCPU_ARM_DataInstructions_MOV, MOV_REGISTER_ROR_ZERO_REG) {
    writeProgramToMemory(
        "MOV R2, #0x00000000\n"
        "MOV R1, #0xF000000F\n"
        "MOV R0, R1, ROR R2\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(3);
    ASSERT_EQ(arm7.readReg(0), 0xF000000F);
}
/**
 * @brief Test moving imm values into all possible regs. Small imm values
 * are defined as able to be encoded in 8 bits.
 */
TEST_F(TestCPU_ARM_DataInstructions_MOV, MOV_SMALL_IMMEDIATE) {
    // Generate test cases.
    std::vector<uint32_t> immValuesToTest = {0, 1, 2, 8, 16, 42, 113, 173, 255};
    std::vector<std::string> instructions = {
        "MOV " + BASE_REG_TOKEN + ", #0",   "MOV " + BASE_REG_TOKEN + ", #1",
        "MOV " + BASE_REG_TOKEN + ", #2",   "MOV " + BASE_REG_TOKEN + ", #8",
        "MOV " + BASE_REG_TOKEN + ", #16",  "MOV " + BASE_REG_TOKEN + ", #42",
        "MOV " + BASE_REG_TOKEN + ", #113", "MOV " + BASE_REG_TOKEN + ", #173",
        "MOV " + BASE_REG_TOKEN + ", #255"};
    std::vector<InstructionTestCase> testCases =
        genInstuctionTestCase(instructions, immValuesToTest, true);

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
/**`
 * @brief Test moving imm values into all possible regs. Large imm values
 * are defined as able to be encoded in 8 bits + a 4 bit shift.
 */
TEST_F(TestCPU_ARM_DataInstructions_MOV, MOV_LARGE_IMMEDIATE) {
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
        genInstuctionTestCase(instructions, immValuesToTest, true);

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
/**
 * @brief Tests a MOV operation's negative flag behaviour using an immediate as the second operand.
 */
TEST_F(TestCPU_ARM_DataInstructions_MOV, MOV_IMMEDIATE_NEGATIVE_FLAG) {
    // N = true
    writeProgramToMemory("MOVs R0, #0xFF000000\n", MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readReg(0), 0xFF000000);
    ASSERT_EQ(arm7.readFlag(N_FLAG), 1);

    // N = false
    arm7.reset();
    writeProgramToMemory("MOVs R0, #1\n", MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readReg(0), 1);
    ASSERT_EQ(arm7.readFlag(N_FLAG), 0);

    // Don't update flag.
    arm7.reset();
    writeProgramToMemory("MOV R0, #0xFF000000\n", MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readReg(0), 0xFF000000);
    ASSERT_EQ(arm7.readFlag(N_FLAG), 0);
}
/**
 * @brief Tests a MOV operation's zero flag behaviour using an immediate as the second operand.
 */
TEST_F(TestCPU_ARM_DataInstructions_MOV, MOV_IMMEDIATE_ZERO_FLAG) {
    // Z = true
    writeProgramToMemory("MOVs R0, #0\n", MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readReg(0), 0);
    ASSERT_EQ(arm7.readFlag(Z_FLAG), 1);

    // Z = false
    arm7.reset();
    writeProgramToMemory("MOVs R0, #1\n", MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readReg(0), 1);
    ASSERT_EQ(arm7.readFlag(Z_FLAG), 0);
}
/**
 * @brief Tests a MOV operation's carry flag behaviour using an immediate as the second operand.
 */
TEST_F(TestCPU_ARM_DataInstructions_MOV, MOV_IMMEDIATE_CARRY_FLAG) {
    // C = true
    writeProgramToMemory("MOVs R0, #0xF0000001\n", MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readReg(0), 0xF0000001);
    ASSERT_EQ(arm7.readFlag(C_FLAG), 1);

    // C = false
    arm7.reset();
    writeProgramToMemory("MOVs R0, #1\n", MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readReg(0), 1);
    ASSERT_EQ(arm7.readFlag(C_FLAG), 0);
}

// ==================================================================================================
// BIC
// ==================================================================================================
class TestCPU_ARM_DataInstructions_BIC : public TestCPU_ARM_DataInstructions {
protected:
    TestCPU_ARM_DataInstructions_BIC() {}
    ~TestCPU_ARM_DataInstructions_BIC() {}

    void SetUp() override { TestCPU_ARM_DataInstructions::SetUp(); }
    void TearDown() override { TestCPU_ARM_DataInstructions::TearDown(); }
};
/**
 * @brief Tests a BIC operation using an immediate as the second operand.
 */
TEST_F(TestCPU_ARM_DataInstructions_BIC, BIC_IMMEDIATE) {
    // N = true
    writeProgramToMemory(
        "MOV R1, #0x000000F1\n"
        "BIC R0, R1, #0x1\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(2);
    ASSERT_EQ(arm7.readReg(0), 0x000000F0);
}
/**
 * @brief Tests a BIC operation's negative flag behaviour using an immediate as the second operand.
 */
TEST_F(TestCPU_ARM_DataInstructions_BIC, BIC_IMMEDIATE_NEGATIVE_FLAG) {
    // N = true
    writeProgramToMemory(
        "MOV R1, #0x80000001\n"
        "BICs R0, R1, #0x1\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(2);
    ASSERT_EQ(arm7.readFlag(N_FLAG), 1);

    // N = false
    arm7.reset();
    writeProgramToMemory(
        "MOV R1, #0x7FFFFFFF\n"
        "BICs R0, R1, #0x1\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(2);
    ASSERT_EQ(arm7.readFlag(N_FLAG), 0);
}
/**
 * @brief Tests a BIC operation's zero flag behaviour using an immediate as the second operand.
 */
TEST_F(TestCPU_ARM_DataInstructions_BIC, BIC_IMMEDIATE_ZERO_FLAG) {
    // Z = true
    writeProgramToMemory(
        "MOV R1, #0x1\n"
        "BICs R0, R1, #0x1\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(2);
    ASSERT_EQ(arm7.readFlag(Z_FLAG), 1);

    // Z = false
    arm7.reset();
    writeProgramToMemory(
        "MOV R1, #0x3\n"
        "BICs R0, R1, #0x1\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(2);
    ASSERT_EQ(arm7.readFlag(Z_FLAG), 0);
}
/**
 * @brief Tests a BIC operation's carry flag behaviour using a rotated immediate.
 */
TEST_F(TestCPU_ARM_DataInstructions_BIC, BIC_IMMEDIATE_CARRY_FLAG) {
    // C = true
    writeProgramToMemory(
        "MOV R1, #1\n"
        "BICs R0, R1, #0x80000000\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(2);
    ASSERT_EQ(arm7.readFlag(C_FLAG), 1);

    // C = false
    arm7.reset();
    writeProgramToMemory(
        "MOV R1, #1\n"
        "BICs R0, R1, #1\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(2);
    ASSERT_EQ(arm7.readFlag(C_FLAG), 0);
}

// ==================================================================================================
// MVN
// ==================================================================================================
class TestCPU_ARM_DataInstructions_MVN : public TestCPU_ARM_DataInstructions {
protected:
    TestCPU_ARM_DataInstructions_MVN() {}
    ~TestCPU_ARM_DataInstructions_MVN() {}

    void SetUp() override { TestCPU_ARM_DataInstructions::SetUp(); }
    void TearDown() override { TestCPU_ARM_DataInstructions::TearDown(); }
};
/**
 * @brief Tests an MVN operation using an immediate as the second operand.
 */
TEST_F(TestCPU_ARM_DataInstructions_MVN, MVN_IMMEDIATE) {
    // N = true
    writeProgramToMemory("MVN R0, #0x000000F0\n", MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readReg(0), 0xFFFFFF0F);
}
/**
 * @brief Tests an MVN operation's negative flag behaviour using an immediate as the second operand.
 */
TEST_F(TestCPU_ARM_DataInstructions_MVN, MVN_IMMEDIATE_NEGATIVE_FLAG) {
    // N = true
    writeProgramToMemory("MVNs R0,#0x7F000000\n", MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readFlag(N_FLAG), 1);

    // N = false
    arm7.reset();
    writeProgramToMemory("MVNs R0,#0x80000000\n", MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readFlag(N_FLAG), 0);
}
/**
 * @brief Tests an MVN operation's zero flag behaviour using an immediate as the second operand.
 */
TEST_F(TestCPU_ARM_DataInstructions_MVN, MVN_IMMEDIATE_ZERO_FLAG) {
    // Z = false
    arm7.reset();
    writeProgramToMemory("MVNs R0, #0x0\n", MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readFlag(Z_FLAG), 0);
}
/**
 * @brief Tests an MVN operation's carry flag behaviour using a rotated immediate.
 */
TEST_F(TestCPU_ARM_DataInstructions_MVN, MVN_IMMEDIATE_CARRY_FLAG) {
    // C = true
    writeProgramToMemory("MVNs R0, #0x80000000\n", MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readFlag(C_FLAG), 1);

    // C = false
    arm7.reset();
    writeProgramToMemory("MVNs R0, #1\n", MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readFlag(C_FLAG), 0);
}

// ==================================================================================================
// MUL
// ==================================================================================================
class TestCPU_ARM_DataInstructions_MUL : public TestCPU_ARM_DataInstructions {
protected:
    TestCPU_ARM_DataInstructions_MUL() {}
    ~TestCPU_ARM_DataInstructions_MUL() {}

    void SetUp() override { TestCPU_ARM_DataInstructions::SetUp(); }
    void TearDown() override { TestCPU_ARM_DataInstructions::TearDown(); }
};
/**
 * @brief Tests an MUL operation using small numbers.
 */
TEST_F(TestCPU_ARM_DataInstructions_MUL, MUL_0) {
    writeProgramToMemory(
        "MOV R1, #7\n"
        "MOV R2, #6\n"
        "MUL R0, R1, R2\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(3);
    ASSERT_EQ(arm7.readReg(0), 42);
}
/**
 * @brief Tests an MUL operation using large positive numbers.
 */
TEST_F(TestCPU_ARM_DataInstructions_MUL, MUL_1) {
    writeProgramToMemory(
        "MOV R1, #0x7FFFFFFF\n"
        "MOV R2, #2\n"
        "MUL R0, R1, R2\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(3);
    ASSERT_EQ(arm7.readReg(0), 0xFFFFFFFE);
}
/**
 * @brief Tests an MUL operation using large negative numbers.
 */
TEST_F(TestCPU_ARM_DataInstructions_MUL, MUL_2) {
    writeProgramToMemory(
        "MOV R1, #0x80000000\n"
        "MOV R2, #2\n"
        "MUL R0, R1, R2\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(3);
    ASSERT_EQ(arm7.readReg(0), 0);
}
/**
 * @brief Tests an MUL operation zero flag behaviour
 */
TEST_F(TestCPU_ARM_DataInstructions_MUL, MUL_ZERO_FLAG) {
    // Z = true
    writeProgramToMemory(
        "MOV R1, #7\n"
        "MOV R2, #0\n"
        "MULs R0, R1, R2\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(3);
    ASSERT_EQ(arm7.readFlag(Z_FLAG), 1);

    // Z = false
    arm7.reset();
    arm7.setFlag(Z_FLAG, 1);
    writeProgramToMemory(
        "MOV R1, #7\n"
        "MOV R2, #1\n"
        "MULs R0, R1, R2\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(3);
    ASSERT_EQ(arm7.readFlag(Z_FLAG), 0);
}
/**
 * @brief Tests an MUL operation negative flag behaviour
 */
TEST_F(TestCPU_ARM_DataInstructions_MUL, MUL_NEGATIVE_FLAG) {
    // N = true
    writeProgramToMemory(
        "MOV R1, #0xFFFFFFFF\n"
        "MOV R2, #1\n"
        "MULs R0, R1, R2\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(3);
    ASSERT_EQ(arm7.readFlag(N_FLAG), 1);

    // N = false
    arm7.reset();
    arm7.setFlag(N_FLAG, 1);
    writeProgramToMemory(
        "MOV R1, #7\n"
        "MOV R2, #1\n"
        "MULs R0, R1, R2\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(3);
    ASSERT_EQ(arm7.readFlag(Z_FLAG), 0);
}

// ==================================================================================================
// MLA
// ==================================================================================================
class TestCPU_ARM_DataInstructions_MLA : public TestCPU_ARM_DataInstructions {
protected:
    TestCPU_ARM_DataInstructions_MLA() {}
    ~TestCPU_ARM_DataInstructions_MLA() {}

    void SetUp() override { TestCPU_ARM_DataInstructions::SetUp(); }
    void TearDown() override { TestCPU_ARM_DataInstructions::TearDown(); }
};
/**
 * @brief Tests an MLA operation using small numbers.
 */
TEST_F(TestCPU_ARM_DataInstructions_MLA, MLA_0) {
    writeProgramToMemory(
        "MOV R1, #7\n"
        "MOV R2, #6\n"
        "MOV R3, #5\n"
        "MLA R0, R1, R2, R3\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(4);
    ASSERT_EQ(arm7.readReg(0), 47);
}
/**
 * @brief Tests an MLA operation using large positive numbers.
 */
TEST_F(TestCPU_ARM_DataInstructions_MLA, MLA_1) {
    writeProgramToMemory(
        "MOV R1, #0x7FFFFFFF\n"
        "MOV R2, #2\n"
        "MOV R3, #1\n"
        "MLA R0, R1, R2, R3\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(4);
    ASSERT_EQ(arm7.readReg(0), 0xFFFFFFFF);
}
/**
 * @brief Tests an MLA operation using large negative numbers.
 */
TEST_F(TestCPU_ARM_DataInstructions_MLA, MLA_2) {
    writeProgramToMemory(
        "MOV R1, #0x80000000\n"
        "MOV R2, #2\n"
        "MOV R3, #1\n"
        "MLA R0, R1, R2, R3\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(4);
    ASSERT_EQ(arm7.readReg(0), 1);
}
/**
 * @brief Tests an MLA operation zero flag behaviour
 */
TEST_F(TestCPU_ARM_DataInstructions_MLA, MLA_ZERO_FLAG) {
    // Z = true
    writeProgramToMemory(
        "MOV R1, #7\n"
        "MOV R2, #0\n"
        "MOV R3, #0\n"
        "MLAs R0, R1, R2, R3\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(4);
    ASSERT_EQ(arm7.readFlag(Z_FLAG), 1);

    // Z = false
    arm7.reset();
    arm7.setFlag(Z_FLAG, 1);
    writeProgramToMemory(
        "MOV R1, #7\n"
        "MOV R2, #1\n"
        "MOV R3, #1\n"
        "MLAs R0, R1, R2, R3\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(4);
    ASSERT_EQ(arm7.readFlag(Z_FLAG), 0);
}
/**
 * @brief Tests an MLA operation negative flag behaviour
 */
TEST_F(TestCPU_ARM_DataInstructions_MLA, MLA_NEGATIVE_FLAG) {
    // N = true
    writeProgramToMemory(
        "MOV R1, #0xFFFFFFFF\n"
        "MOV R2, #1\n"
        "MOV R3, #0\n"
        "MLAs R0, R1, R2, R3\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(4);
    ASSERT_EQ(arm7.readFlag(N_FLAG), 1);

    // N = false
    arm7.reset();
    arm7.setFlag(N_FLAG, 1);
    writeProgramToMemory(
        "MOV R1, #7\n"
        "MOV R2, #1\n"
        "MOV R3, #0\n"
        "MLAs R0, R1, R2, R3\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(4);
    ASSERT_EQ(arm7.readFlag(Z_FLAG), 0);
}

// ==================================================================================================
// UMULL
// ==================================================================================================
class TestCPU_ARM_DataInstructions_UMULL : public TestCPU_ARM_DataInstructions {
protected:
    TestCPU_ARM_DataInstructions_UMULL() {}
    ~TestCPU_ARM_DataInstructions_UMULL() {}

    void SetUp() override { TestCPU_ARM_DataInstructions::SetUp(); }
    void TearDown() override { TestCPU_ARM_DataInstructions::TearDown(); }
};
/**
 * @brief Tests an UMULL operation using small numbers.
 */
TEST_F(TestCPU_ARM_DataInstructions_UMULL, UMULL_0) {
    writeProgramToMemory(
        "MOV R1, #7\n"
        "MOV R2, #6\n"
        "UMULL R0, R3, R1, R2\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(3);
    ASSERT_EQ(arm7.readReg(0), 42);
    ASSERT_EQ(arm7.readReg(3), 0);
}
/**
 * @brief Tests an UMULL operation using large positive numbers.
 */
TEST_F(TestCPU_ARM_DataInstructions_UMULL, UMULL_1) {
    writeProgramToMemory(
        "MOV R1, #0xFFFFFFFF\n"
        "MOV R2, #2\n"
        "UMULL R0, R3, R1, R2\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(3);
    ASSERT_EQ(arm7.readReg(0), 0xFFFFFFFE);
    ASSERT_EQ(arm7.readReg(3), 1);
}
/**
 * @brief Tests an UMULL operation using large negative numbers.
 */
TEST_F(TestCPU_ARM_DataInstructions_UMULL, UMULL_2) {
    writeProgramToMemory(
        "MOV R1, #0xFFFFFFFF\n"
        "MOV R2, #0x80000000\n"
        "UMULL R0, R3, R1, R2\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(3);
    ASSERT_EQ(arm7.readReg(0), 0x80000000);
    ASSERT_EQ(arm7.readReg(3), 0x7FFFFFFF);
}
/**
 * @brief Tests an UMULL operation with zero result
 */
TEST_F(TestCPU_ARM_DataInstructions_UMULL, UMULL_ZERO_FLAG) {
    // Z = true
    writeProgramToMemory(
        "MOV R1, #0\n"
        "MOV R2, #7\n"
        "UMULLs R0, R3, R1, R2\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(3);
    ASSERT_EQ(arm7.readFlag(Z_FLAG), 1);

    // Z = false
    arm7.reset();
    arm7.setFlag(Z_FLAG, 1);
    writeProgramToMemory(
        "MOV R1, #7\n"
        "MOV R2, #1\n"
        "UMULLs R0, R3, R1, R2\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(3);
    ASSERT_EQ(arm7.readFlag(Z_FLAG), 0);
}
/**
 * @brief Tests an UMULL operation negative flag behaviour
 */
TEST_F(TestCPU_ARM_DataInstructions_UMULL, UMULL_NEGATIVE_FLAG) {
    // N = true
    writeProgramToMemory(
        "MOV R1, #0xFFFFFFFF\n"
        "MOV R2, #0xFFFFFFFF\n"
        "UMULLs R0, R3, R1, R2\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(3);
    ASSERT_EQ(arm7.readFlag(N_FLAG), 1);

    // N = false
    arm7.reset();
    arm7.setFlag(N_FLAG, 1);
    writeProgramToMemory(
        "MOV R1, #7\n"
        "MOV R2, #1\n"
        "UMULLs R0, R3, R1, R2\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(3);
    ASSERT_EQ(arm7.readFlag(N_FLAG), 0);
}

// UMLAL
// ==================================================================================================
class TestCPU_ARM_DataInstructions_UMLAL : public TestCPU_ARM_DataInstructions {
protected:
    TestCPU_ARM_DataInstructions_UMLAL() {}
    ~TestCPU_ARM_DataInstructions_UMLAL() {}

    void SetUp() override { TestCPU_ARM_DataInstructions::SetUp(); }
    void TearDown() override { TestCPU_ARM_DataInstructions::TearDown(); }
};
/**
 * @brief Tests a UMLAL operation using small numbers.
 */
TEST_F(TestCPU_ARM_DataInstructions_UMLAL, UMLAL_0) {
    writeProgramToMemory(
        "MOV R0, #0\n"
        "MOV R3, #0\n"
        "MOV R1, #7\n"
        "MOV R2, #6\n"
        "UMLAL R0, R3, R1, R2\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(5);
    ASSERT_EQ(arm7.readReg(0), 42);
    ASSERT_EQ(arm7.readReg(3), 0);
}
/**
 * @brief Tests a UMLAL operation using large positive numbers.
 */
TEST_F(TestCPU_ARM_DataInstructions_UMLAL, UMLAL_1) {
    writeProgramToMemory(
        "MOV R0, #1\n"
        "MOV R3, #1\n"
        "MOV R1, #0xFFFFFFFF\n"
        "MOV R2, #2\n"
        "UMLAL R0, R3, R1, R2\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(5);
    ASSERT_EQ(arm7.readReg(0), 0xFFFFFFFF);
    ASSERT_EQ(arm7.readReg(3), 2);
}
/**
 * @brief Tests an UMLAL operation using large negative numbers.
 */
TEST_F(TestCPU_ARM_DataInstructions_UMLAL, UMLAL_2) {
    writeProgramToMemory(
        "MOV R0, #0xFFFFFFFF\n"
        "MOV R3, #0xFFFFFFFF\n"
        "MOV R1, #-6\n"
        "MOV R2, #10\n"
        "UMLAL R0, R3, R1, R2\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(5);
    ASSERT_EQ(arm7.readReg(0), 0xFFFFFFC3);
    ASSERT_EQ(arm7.readReg(3), 0x00000009);
}
/**
 * @brief Tests a UMLAL operation with zero result
 */
TEST_F(TestCPU_ARM_DataInstructions_UMLAL, UMLAL_ZERO_FLAG) {
    // Z = true
    writeProgramToMemory(
        "MOV R0, #0\n"
        "MOV R3, #0\n"
        "MOV R1, #0\n"
        "MOV R2, #0\n"
        "UMLALs R0, R3, R1, R2\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(5);
    ASSERT_EQ(arm7.readFlag(Z_FLAG), 1);

    // Z = false
    arm7.reset();
    arm7.setFlag(Z_FLAG, 1);
    writeProgramToMemory(
        "MOV R0, #0\n"
        "MOV R3, #0\n"
        "MOV R1, #7\n"
        "MOV R2, #1\n"
        "UMLALs R0, R3, R1, R2\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(5);
    ASSERT_EQ(arm7.readFlag(Z_FLAG), 0);
}
/**
 * @brief Tests a UMLAL operation negative flag behaviour
 */
TEST_F(TestCPU_ARM_DataInstructions_UMLAL, UMLAL_NEGATIVE_FLAG) {
    // N = true
    writeProgramToMemory(
        "MOV R0, #0\n"
        "MOV R3, #0x80000000\n"
        "MOV R1, #2\n"
        "MOV R2, #1\n"
        "UMLALs R0, R3, R1, R2\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(5);
    ASSERT_EQ(arm7.readFlag(N_FLAG), 1);

    // N = false
    arm7.reset();
    arm7.setFlag(N_FLAG, 1);
    writeProgramToMemory(
        "MOV R0, #0\n"
        "MOV R3, #0\n"
        "MOV R1, #7\n"
        "MOV R2, #1\n"
        "UMLALs R0, R3, R1, R2\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(5);
    ASSERT_EQ(arm7.readFlag(N_FLAG), 0);
}

// ==================================================================================================
// SMULL
// ==================================================================================================
class TestCPU_ARM_DataInstructions_SMULL : public TestCPU_ARM_DataInstructions {
protected:
    TestCPU_ARM_DataInstructions_SMULL() {}
    ~TestCPU_ARM_DataInstructions_SMULL() {}

    void SetUp() override { TestCPU_ARM_DataInstructions::SetUp(); }
    void TearDown() override { TestCPU_ARM_DataInstructions::TearDown(); }
};
/**
 * @brief Tests an SMULL operation using small numbers.
 */
TEST_F(TestCPU_ARM_DataInstructions_SMULL, SMULL_0) {
    writeProgramToMemory(
        "MOV R1, #7\n"
        "MOV R2, #6\n"
        "SMULL R0, R3, R1, R2\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(3);
    ASSERT_EQ(arm7.readReg(0), 42);
    ASSERT_EQ(arm7.readReg(3), 0);
}
/**
 * @brief Tests an SMULL operation using large positive numbers.
 */
TEST_F(TestCPU_ARM_DataInstructions_SMULL, SMULL_1) {
    writeProgramToMemory(
        "MOV R1, #0x7FFFFFFF\n"
        "MOV R2, #2\n"
        "SMULL R0, R3, R1, R2\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(3);
    ASSERT_EQ(arm7.readReg(0), 0xFFFFFFFE);
    ASSERT_EQ(arm7.readReg(3), 0);
}
/**
 * @brief Tests an SMULL operation using large negative numbers.
 */
TEST_F(TestCPU_ARM_DataInstructions_SMULL, SMULL_2) {
    writeProgramToMemory(
        "MOV R1, #0xFFFFFFFF\n"
        "MOV R2, #0x80000000\n"
        "SMULL R0, R3, R1, R2\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(3);
    ASSERT_EQ(arm7.readReg(0), 0x80000000);
    ASSERT_EQ(arm7.readReg(3), 0);
}
/**
 * @brief Tests an SMULL operation using large negative numbers.
 */
TEST_F(TestCPU_ARM_DataInstructions_SMULL, SMULL_3) {
    writeProgramToMemory(
        "MOV R1, #0xFFFFFFFF\n"
        "MOV R2, #2\n"
        "SMULL R0, R3, R1, R2\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(3);
    ASSERT_EQ(arm7.readReg(0), 0xFFFFFFFE);
    ASSERT_EQ(arm7.readReg(3), 0xFFFFFFFF);
}
/**
 * @brief Tests an SMULL operation zero flag behaviour
 */
TEST_F(TestCPU_ARM_DataInstructions_SMULL, SMULL_ZERO_FLAG) {
    // Z = true
    writeProgramToMemory(
        "MOV R1, #0\n"
        "MOV R2, #0\n"
        "SMULLs R0, R3, R1, R2\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(3);
    ASSERT_EQ(arm7.readFlag(Z_FLAG), 1);

    // Z = false
    arm7.reset();
    arm7.setFlag(Z_FLAG, 1);
    writeProgramToMemory(
        "MOV R1, #7\n"
        "MOV R2, #1\n"
        "SMULLs R0, R3, R1, R2\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(3);
    ASSERT_EQ(arm7.readFlag(Z_FLAG), 0);
}
/**
 * @brief Tests an SMULL operation negative flag behaviour
 */
TEST_F(TestCPU_ARM_DataInstructions_SMULL, SMULL_NEGATIVE_FLAG) {
    // N = true
    writeProgramToMemory(
        "MOV R1, #2\n"
        "MOV R2, #0x80000000\n"
        "SMULLs R0, R3, R1, R2\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(3);
    ASSERT_EQ(arm7.readFlag(N_FLAG), 1);

    // N = false
    arm7.reset();
    arm7.setFlag(N_FLAG, 1);
    writeProgramToMemory(
        "MOV R1, #7\n"
        "MOV R2, #1\n"
        "SMULLs R0, R3, R1, R2\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(3);
    ASSERT_EQ(arm7.readFlag(N_FLAG), 0);
}

// ==================================================================================================
// SMLAL
// ==================================================================================================
class TestCPU_ARM_DataInstructions_SMLAL : public TestCPU_ARM_DataInstructions {
protected:
    TestCPU_ARM_DataInstructions_SMLAL() {}
    ~TestCPU_ARM_DataInstructions_SMLAL() {}

    void SetUp() override { TestCPU_ARM_DataInstructions::SetUp(); }
    void TearDown() override { TestCPU_ARM_DataInstructions::TearDown(); }
};
/**
 * @brief Tests an SMLAL operation using small numbers.
 */
TEST_F(TestCPU_ARM_DataInstructions_SMLAL, SMLAL_0) {
    writeProgramToMemory(
        "MOV R0, #0\n"
        "MOV R3, #0\n"
        "MOV R1, #7\n"
        "MOV R2, #6\n"
        "SMLAL R0, R3, R1, R2\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(5);
    ASSERT_EQ(arm7.readReg(0), 42);
    ASSERT_EQ(arm7.readReg(3), 0);
}
/**
 * @brief Tests an SMLAL operation using large positive numbers.
 */
TEST_F(TestCPU_ARM_DataInstructions_SMLAL, SMLAL_1) {
    writeProgramToMemory(
        "MOV R0, #1\n"
        "MOV R3, #1\n"
        "MOV R1, #0x7FFFFFFF\n"
        "MOV R2, #2\n"
        "SMLAL R0, R3, R1, R2\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(5);
    ASSERT_EQ(arm7.readReg(0), 0xFFFFFFFF);
    ASSERT_EQ(arm7.readReg(3), 1);
}
/**
 * @brief Tests an SMLAL operation using large negative numbers.
 */
TEST_F(TestCPU_ARM_DataInstructions_SMLAL, SMLAL_2) {
    writeProgramToMemory(
        "MOV R0, #0xFFFFFFFF\n"
        "MOV R3, #0xFFFFFFFF\n"
        "MOV R1, #-6\n"
        "MOV R2, #10\n"
        "SMLAL R0, R3, R1, R2\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(5);
    ASSERT_EQ(arm7.readReg(0), 0xFFFFFFC3);
    ASSERT_EQ(arm7.readReg(3), 0xFFFFFFFF);
}
/**
 * @brief Tests an SMLAL operation zero flag behaviour
 */
TEST_F(TestCPU_ARM_DataInstructions_SMLAL, SMLAL_ZERO_FLAG) {
    // Z = true
    writeProgramToMemory(
        "MOV R0, #0\n"
        "MOV R3, #0\n"
        "MOV R1, #0\n"
        "MOV R2, #0\n"
        "SMLALs R0, R3, R1, R2\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(5);
    ASSERT_EQ(arm7.readFlag(Z_FLAG), 1);

    // Z = false
    arm7.reset();
    arm7.setFlag(Z_FLAG, 1);
    writeProgramToMemory(
        "MOV R0, #1\n"
        "MOV R3, #1\n"
        "MOV R1, #7\n"
        "MOV R2, #1\n"
        "SMLALs R0, R3, R1, R2\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(5);
    ASSERT_EQ(arm7.readFlag(Z_FLAG), 0);
}
/**
 * @brief Tests an SMLAL operation negative flag behaviour
 */
TEST_F(TestCPU_ARM_DataInstructions_SMLAL, SMLAL_NEGATIVE_FLAG) {
    // N = true
    writeProgramToMemory(
        "MOV R0, #0\n"
        "MOV R3, #0x80000000\n"
        "MOV R1, #2\n"
        "MOV R2, #1\n"
        "SMLALs R0, R3, R1, R2\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(5);
    ASSERT_EQ(arm7.readFlag(N_FLAG), 1);

    // N = false
    arm7.reset();
    arm7.setFlag(N_FLAG, 1);
    writeProgramToMemory(
        "MOV R0, #0\n"
        "MOV R3, #0\n"
        "MOV R1, #7\n"
        "MOV R2, #1\n"
        "SMLALs R0, R3, R1, R2\n",
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(5);
    ASSERT_EQ(arm7.readFlag(N_FLAG), 0);
}

// ==================================================================================================
// SWP
// ==================================================================================================
class TestCPU_ARM_DataInstructions_SWP : public TestCPU_ARM_DataInstructions {
protected:
    TestCPU_ARM_DataInstructions_SWP() {}
    ~TestCPU_ARM_DataInstructions_SWP() {}

    void SetUp() override { TestCPU_ARM_DataInstructions::SetUp(); }
    void TearDown() override { TestCPU_ARM_DataInstructions::TearDown(); }
};
/**
 * @brief Tests SWP (Swap Word) instruction.
 *
 * SWP Rd, Rm, [Rn]
 *
 * Rd = Mem[Rn]
 * Mem[Rn] = Rm
 */
TEST_F(TestCPU_ARM_DataInstructions_SWP, SWP_WORD) {
    uint32_t baseAddress = MAIN_RAM_START + 0x400;
    uint32_t memoryValue = 0x11223344;
    uint32_t registerValue = 0xAABBCCDD;

    arm7.reset();

    // Initialize memory
    bus.write32ARM7(baseAddress, memoryValue);

    // Setup registers
    arm7.writeReg(0, 0);              // Rd (R0)
    arm7.writeReg(1, registerValue);  // Rm (R1)
    arm7.writeReg(2, baseAddress);    // Rn (R2)

    writeProgramToMemory("SWP R0, R1, [R2]\n", MAIN_RAM_START, &bus,
                         /*arm7=*/true);

    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);

    // Rd should now contain original memory value
    ASSERT_EQ(arm7.readReg(0), memoryValue);

    // Memory should now contain original Rm value
    ASSERT_EQ(bus.read32ARM7(baseAddress), registerValue);
}
/**
 * @brief Tests SWPB (Swap Byte) instruction.
 *
 * SWPB Rd, Rm, [Rn]
 *
 * Rd = Mem8[Rn]
 * Mem8[Rn] = Rm[7:0]
 */
TEST_F(TestCPU_ARM_DataInstructions_SWP, SWPB_BYTE) {
    uint32_t baseAddress = MAIN_RAM_START + 0x500;
    uint8_t memoryValue = 0x44;
    uint32_t registerValue = 0xAABBCCDD;  // only lowest byte (0xDD) used

    arm7.reset();

    // Initialize memory (write single byte)
    bus.write8ARM7(baseAddress, memoryValue);

    // Setup registers
    arm7.writeReg(0, 0);              // Rd (R0)
    arm7.writeReg(1, registerValue);  // Rm (R1)
    arm7.writeReg(2, baseAddress);    // Rn (R2)

    writeProgramToMemory("SWPB R0, R1, [R2]\n", MAIN_RAM_START, &bus,
                         /*arm7=*/true);

    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);

    // Rd should contain zero-extended original memory byte
    ASSERT_EQ(arm7.readReg(0), memoryValue);

    // Memory should contain low byte of Rm
    ASSERT_EQ(bus.read8ARM7(baseAddress), 0xDD);
}

// ==================================================================================================
// QADD
// ==================================================================================================
class TestCPU_ARM_DataInstructions_QADD : public TestCPU_ARM_DataInstructions {
protected:
    TestCPU_ARM_DataInstructions_QADD() {}
    ~TestCPU_ARM_DataInstructions_QADD() {}

    void SetUp() override { TestCPU_ARM_DataInstructions::SetUp(); }
    void TearDown() override { TestCPU_ARM_DataInstructions::TearDown(); }
};
/**
 * @brief Tests QADD causing no saturation.
 */
TEST_F(TestCPU_ARM_DataInstructions_QADD, QADD_NO_SATURATION) {
    arm9.writeReg(1, 10);
    arm9.writeReg(2, 20);

    writeProgramToMemory("QADD R0, R1, R2", MAIN_RAM_START, &bus,
                         /*arm7=*/arm9.isARM7());
    arm9.setPC(MAIN_RAM_START);
    arm9.fetchAndExecute(1);

    EXPECT_EQ(arm9.readReg(0), 30);
    EXPECT_EQ(arm9.readFlag(Q_BIT), 0);
}
/**
 * @brief Tests QADD causing max saturation.
 */
TEST_F(TestCPU_ARM_DataInstructions_QADD, QADD_MAX_SATURATION) {
    arm9.writeReg(1, INT32_MAX);
    arm9.writeReg(2, 1);

    writeProgramToMemory("QADD R0, R1, R2", MAIN_RAM_START, &bus,
                         /*arm7=*/arm9.isARM7());
    arm9.setPC(MAIN_RAM_START);
    arm9.fetchAndExecute(1);

    EXPECT_EQ(arm9.readReg(0), INT32_MAX);
    EXPECT_EQ(arm9.readFlag(Q_BIT), 1);
}
/**
 * @brief Tests QADD causing min saturation.
 */
TEST_F(TestCPU_ARM_DataInstructions_QADD, QADD_MIN_SATURATION) {
    arm9.writeReg(1, INT32_MIN);
    arm9.writeReg(2, -1);

    writeProgramToMemory("QADD R0, R1, R2", MAIN_RAM_START, &bus,
                         /*arm7=*/arm9.isARM7());
    arm9.setPC(MAIN_RAM_START);
    arm9.fetchAndExecute(1);

    EXPECT_EQ(arm9.readReg(0), INT32_MIN);
    EXPECT_EQ(arm9.readFlag(Q_BIT), 1);
}
/**
 * @brief Tests QADD causing no saturation (max bound).
 */
TEST_F(TestCPU_ARM_DataInstructions_QADD, QADD_MAX_NO_SATURATION) {
    arm9.writeReg(1, INT32_MAX);
    arm9.writeReg(2, 0);

    writeProgramToMemory("QADD R0, R1, R2", MAIN_RAM_START, &bus,
                         /*arm7=*/arm9.isARM7());
    arm9.setPC(MAIN_RAM_START);
    arm9.fetchAndExecute(1);

    EXPECT_EQ(arm9.readReg(0), INT32_MAX);
    EXPECT_EQ(arm9.readFlag(Q_BIT), 0);
}
/**
 * @brief Tests QADD causing no saturation (min bound).
 */
TEST_F(TestCPU_ARM_DataInstructions_QADD, QADD_MIN_NO_SATURATION) {
    arm9.writeReg(1, INT32_MIN);
    arm9.writeReg(2, 0);

    writeProgramToMemory("QADD R0, R1, R2", MAIN_RAM_START, &bus,
                         /*arm7=*/arm9.isARM7());
    arm9.setPC(MAIN_RAM_START);
    arm9.fetchAndExecute(1);

    EXPECT_EQ(arm9.readReg(0), INT32_MIN);
    EXPECT_EQ(arm9.readFlag(Q_BIT), 0);
}
// ==================================================================================================
// QSUB
// ==================================================================================================
class TestCPU_ARM_DataInstructions_QSUB : public TestCPU_ARM_DataInstructions {
protected:
    TestCPU_ARM_DataInstructions_QSUB() {}
    ~TestCPU_ARM_DataInstructions_QSUB() {}

    void SetUp() override { TestCPU_ARM_DataInstructions::SetUp(); }
    void TearDown() override { TestCPU_ARM_DataInstructions::TearDown(); }
};
/**
 * @brief Tests QSUB causing no saturation.
 */
TEST_F(TestCPU_ARM_DataInstructions_QSUB, QSUB_NO_SATURATION) {
    arm9.writeReg(1, 50);
    arm9.writeReg(2, 20);

    writeProgramToMemory("QSUB R0, R1, R2", MAIN_RAM_START, &bus,
                         /*arm7=*/arm9.isARM7());
    arm9.setPC(MAIN_RAM_START);
    arm9.fetchAndExecute(1);

    EXPECT_EQ(arm9.readReg(0), 30);
    EXPECT_EQ(arm9.readFlag(Q_BIT), 0);
}
/**
 * @brief Tests QSUB causing max saturation.
 */
TEST_F(TestCPU_ARM_DataInstructions_QSUB, QSUB_MAX_SATURATION) {
    // INT32_MAX - (-1) -> overflow -> saturate to INT32_MAX
    arm9.writeReg(1, INT32_MAX);
    arm9.writeReg(2, -1);

    writeProgramToMemory("QSUB R0, R1, R2", MAIN_RAM_START, &bus,
                         /*arm7=*/arm9.isARM7());
    arm9.setPC(MAIN_RAM_START);
    arm9.fetchAndExecute(1);

    EXPECT_EQ(arm9.readReg(0), INT32_MAX);
    EXPECT_EQ(arm9.readFlag(Q_BIT), 1);
}
/**
 * @brief Tests QSUB causing min saturation.
 */
TEST_F(TestCPU_ARM_DataInstructions_QSUB, QSUB_MIN_SATURATION) {
    // INT32_MIN - 1 -> overflow -> saturate to INT32_MIN
    arm9.writeReg(1, INT32_MIN);
    arm9.writeReg(2, 1);

    writeProgramToMemory("QSUB R0, R1, R2", MAIN_RAM_START, &bus,
                         /*arm7=*/arm9.isARM7());
    arm9.setPC(MAIN_RAM_START);
    arm9.fetchAndExecute(1);

    EXPECT_EQ(arm9.readReg(0), INT32_MIN);
    EXPECT_EQ(arm9.readFlag(Q_BIT), 1);
}
/**
 * @brief Tests QSUB causing no saturation (max bound).
 */
TEST_F(TestCPU_ARM_DataInstructions_QSUB, QSUB_MAX_NO_SATURATION) {
    // INT32_MAX - 0 -> no overflow
    arm9.writeReg(1, INT32_MAX);
    arm9.writeReg(2, 0);

    writeProgramToMemory("QSUB R0, R1, R2", MAIN_RAM_START, &bus,
                         /*arm7=*/arm9.isARM7());
    arm9.setPC(MAIN_RAM_START);
    arm9.fetchAndExecute(1);

    EXPECT_EQ(arm9.readReg(0), INT32_MAX);
    EXPECT_EQ(arm9.readFlag(Q_BIT), 0);
}
/**
 * @brief Tests QSUB causing no saturation (min bound).
 */
TEST_F(TestCPU_ARM_DataInstructions_QSUB, QSUB_MIN_NO_SATURATION) {
    // INT32_MIN - 0 -> no overflow
    arm9.writeReg(1, INT32_MIN);
    arm9.writeReg(2, 0);

    writeProgramToMemory("QSUB R0, R1, R2", MAIN_RAM_START, &bus,
                         /*arm7=*/arm9.isARM7());
    arm9.setPC(MAIN_RAM_START);
    arm9.fetchAndExecute(1);

    EXPECT_EQ(arm9.readReg(0), INT32_MIN);
    EXPECT_EQ(arm9.readFlag(Q_BIT), 0);
}
// ==================================================================================================
// QDADD
// ==================================================================================================
class TestCPU_ARM_DataInstructions_QDADD : public TestCPU_ARM_DataInstructions {
protected:
    TestCPU_ARM_DataInstructions_QDADD() {}
    ~TestCPU_ARM_DataInstructions_QDADD() {}

    void SetUp() override { TestCPU_ARM_DataInstructions::SetUp(); }
    void TearDown() override { TestCPU_ARM_DataInstructions::TearDown(); }
};
/**
 * @brief Normal operation: doubling does not overflow, sum does not overflow
 */
TEST_F(TestCPU_ARM_DataInstructions_QDADD, QDADD_NO_SATURATION) {
    arm9.writeReg(1, 10);
    arm9.writeReg(2, 20);  // 20*2=40, +10=50

    writeProgramToMemory("QDADD R0, R1, R2", MAIN_RAM_START, &bus, arm9.isARM7());
    arm9.setPC(MAIN_RAM_START);
    arm9.fetchAndExecute(1);

    EXPECT_EQ(arm9.readReg(0), 50);
    EXPECT_EQ(arm9.readFlag(Q_BIT), 0);
}
/**
 * @brief Doubling alone causes saturation.
 */
TEST_F(TestCPU_ARM_DataInstructions_QDADD, QDADD_DOUBLE_SATURATION) {
    arm9.writeReg(1, 0);
    arm9.writeReg(2, INT32_MAX / 2 + 1);  // 2*(INT32_MAX/2+1) = overflow

    writeProgramToMemory("QDADD R0, R1, R2", MAIN_RAM_START, &bus, arm9.isARM7());
    arm9.setPC(MAIN_RAM_START);
    arm9.fetchAndExecute(1);

    EXPECT_EQ(arm9.readReg(0), INT32_MAX);
    EXPECT_EQ(arm9.readFlag(Q_BIT), 1);
}
/**
 * @brief  Addition after doubling causes saturation
 */
TEST_F(TestCPU_ARM_DataInstructions_QDADD, QDADD_ADD_SATURATION) {
    arm9.writeReg(1, INT32_MAX);
    arm9.writeReg(2, 1);  // 2*1 + INT32_MAX = overflow

    writeProgramToMemory("QDADD R0, R1, R2", MAIN_RAM_START, &bus, arm9.isARM7());
    arm9.setPC(MAIN_RAM_START);
    arm9.fetchAndExecute(1);

    EXPECT_EQ(arm9.readReg(0), INT32_MAX);
    EXPECT_EQ(arm9.readFlag(Q_BIT), 1);
}
/**
 * @brief Both operands zero – edge case, no saturation
 */
TEST_F(TestCPU_ARM_DataInstructions_QDADD, QDADD_ZERO_NO_SATURATION) {
    arm9.writeReg(1, 0);
    arm9.writeReg(2, 0);

    writeProgramToMemory("QDADD R0, R1, R2", MAIN_RAM_START, &bus, arm9.isARM7());
    arm9.setPC(MAIN_RAM_START);
    arm9.fetchAndExecute(1);

    EXPECT_EQ(arm9.readReg(0), 0);
    EXPECT_EQ(arm9.readFlag(Q_BIT), 0);
}
/**
 * @brief Max values, doubling + addition triggers saturation
 */
TEST_F(TestCPU_ARM_DataInstructions_QDADD, QDADD_MAX_VALUES) {
    arm9.writeReg(1, INT32_MAX - 1);
    arm9.writeReg(2, INT32_MAX / 2);  // 2*(INT32_MAX/2) + INT32_MAX-1 > MAX

    writeProgramToMemory("QDADD R0, R1, R2", MAIN_RAM_START, &bus, arm9.isARM7());
    arm9.setPC(MAIN_RAM_START);
    arm9.fetchAndExecute(1);

    EXPECT_EQ(arm9.readReg(0), INT32_MAX);
    EXPECT_EQ(arm9.readFlag(Q_BIT), 1);
}
/**
 * @brief Min values, doubling negative + addition triggers negative saturation
 */
TEST_F(TestCPU_ARM_DataInstructions_QDADD, QDADD_MIN_VALUES) {
    arm9.writeReg(1, INT32_MIN + 1);
    arm9.writeReg(2, INT32_MIN / 2);  // 2*(INT32_MIN/2) + INT32_MIN+1 < MIN

    writeProgramToMemory("QDADD R0, R1, R2", MAIN_RAM_START, &bus, arm9.isARM7());
    arm9.setPC(MAIN_RAM_START);
    arm9.fetchAndExecute(1);

    EXPECT_EQ(arm9.readReg(0), INT32_MIN);
    EXPECT_EQ(arm9.readFlag(Q_BIT), 1);
}
/**
 * @brief Negative doubling without saturation
 */
TEST_F(TestCPU_ARM_DataInstructions_QDADD, QDADD_NEGATIVE_NO_SATURATION) {
    arm9.writeReg(1, -10);
    arm9.writeReg(2, -20);  // -20*2 = -40, -40 + -10 = -50

    writeProgramToMemory("QDADD R0, R1, R2", MAIN_RAM_START, &bus, arm9.isARM7());
    arm9.setPC(MAIN_RAM_START);
    arm9.fetchAndExecute(1);

    EXPECT_EQ(arm9.readReg(0), -50);
    EXPECT_EQ(arm9.readFlag(Q_BIT), 0);
}
/**
 * @brief Check Q flag sticky: first saturate, then normal operation
 */
TEST_F(TestCPU_ARM_DataInstructions_QDADD, QDADD_Q_BIT_STICKY) {
    arm9.writeReg(1, INT32_MAX);
    arm9.writeReg(2, 1);

    writeProgramToMemory(
        "QDADD R0, R1, R2\n"
        "QDADD R0, R1, R2",
        MAIN_RAM_START, &bus, arm9.isARM7());
    arm9.setPC(MAIN_RAM_START);
    arm9.fetchAndExecute(1);  // saturate
    EXPECT_EQ(arm9.readFlag(Q_BIT), 1);

    arm9.fetchAndExecute(1);  // normal operation, Q remains set
    EXPECT_EQ(arm9.readFlag(Q_BIT), 1);
}
// ==================================================================================================
// QDSUB
// ==================================================================================================
class TestCPU_ARM_DataInstructions_QDSUB : public TestCPU_ARM_DataInstructions {
protected:
    TestCPU_ARM_DataInstructions_QDSUB() {}
    ~TestCPU_ARM_DataInstructions_QDSUB() {}

    void SetUp() override { TestCPU_ARM_DataInstructions::SetUp(); }
    void TearDown() override { TestCPU_ARM_DataInstructions::TearDown(); }
};
/**
 * @brief Normal operation: no saturation
 */
TEST_F(TestCPU_ARM_DataInstructions_QDSUB, QDSUB_NO_SATURATION) {
    arm9.writeReg(1, 50);
    arm9.writeReg(2, 10);  // 10*2=20, 50-20=30

    writeProgramToMemory("QDSUB R0, R1, R2", MAIN_RAM_START, &bus, arm9.isARM7());
    arm9.setPC(MAIN_RAM_START);
    arm9.fetchAndExecute(1);

    EXPECT_EQ(arm9.readReg(0), 30);
    EXPECT_EQ(arm9.readFlag(Q_BIT), 0);
}
/**
 * @brief Doubling alone triggers saturation
 */
TEST_F(TestCPU_ARM_DataInstructions_QDSUB, QDSUB_DOUBLE_SATURATION) {
    arm9.writeReg(1, 0);
    arm9.writeReg(2, INT32_MIN / 2 - 1);  // 2*INT32_MIN/2-1 < MIN

    writeProgramToMemory("QDSUB R0, R1, R2", MAIN_RAM_START, &bus, arm9.isARM7());
    arm9.setPC(MAIN_RAM_START);
    arm9.fetchAndExecute(1);

    EXPECT_EQ(arm9.readReg(0), INT32_MAX);
    EXPECT_EQ(arm9.readFlag(Q_BIT), 1);
}
/**
 * @brief Subtraction after doubling triggers saturation
 */
TEST_F(TestCPU_ARM_DataInstructions_QDSUB, QDSUB_SUB_SATURATION) {
    arm9.writeReg(1, INT32_MIN + 1);
    arm9.writeReg(2, 2);  // 2*2 =4, INT32_MIN+1 - 4 < MIN

    writeProgramToMemory("QDSUB R0, R1, R2", MAIN_RAM_START, &bus, arm9.isARM7());
    arm9.setPC(MAIN_RAM_START);
    arm9.fetchAndExecute(1);

    EXPECT_EQ(arm9.readReg(0), INT32_MIN);
    EXPECT_EQ(arm9.readFlag(Q_BIT), 1);
}
/**
 * @brief Negative doubling without saturation
 */
TEST_F(TestCPU_ARM_DataInstructions_QDSUB, QDSUB_NEGATIVE_NO_SATURATION) {
    arm9.writeReg(1, -50);
    arm9.writeReg(2, -10);  // -10*2=-20, -50-(-20)=-30

    writeProgramToMemory("QDSUB R0, R1, R2", MAIN_RAM_START, &bus, arm9.isARM7());
    arm9.setPC(MAIN_RAM_START);
    arm9.fetchAndExecute(1);

    EXPECT_EQ(arm9.readReg(0), -30);
    EXPECT_EQ(arm9.readFlag(Q_BIT), 0);
}
/**
 * @brief Max operands, no saturation
 */
TEST_F(TestCPU_ARM_DataInstructions_QDSUB, QDSUB_MAX_NO_SATURATION) {
    arm9.writeReg(1, INT32_MAX);
    arm9.writeReg(2, 0);

    writeProgramToMemory("QDSUB R0, R1, R2", MAIN_RAM_START, &bus, arm9.isARM7());
    arm9.setPC(MAIN_RAM_START);
    arm9.fetchAndExecute(1);

    EXPECT_EQ(arm9.readReg(0), INT32_MAX);
    EXPECT_EQ(arm9.readFlag(Q_BIT), 0);
}
/**
 * @brief Min operands, no saturation
 */
TEST_F(TestCPU_ARM_DataInstructions_QDSUB, QDSUB_MIN_NO_SATURATION) {
    arm9.writeReg(1, INT32_MIN);
    arm9.writeReg(2, 0);

    writeProgramToMemory("QDSUB R0, R1, R2", MAIN_RAM_START, &bus, arm9.isARM7());
    arm9.setPC(MAIN_RAM_START);
    arm9.fetchAndExecute(1);

    EXPECT_EQ(arm9.readReg(0), INT32_MIN);
    EXPECT_EQ(arm9.readFlag(Q_BIT), 0);
}
/**
 * @brief Q flag sticky behavior
 */
TEST_F(TestCPU_ARM_DataInstructions_QDSUB, QDSUB_Q_BIT_STICKY) {
    arm9.writeReg(1, INT32_MIN);
    arm9.writeReg(2, 2);  // first instruction saturates

    writeProgramToMemory(
        "QDSUB R0, R1, R2\n"
        "QDSUB R0, R1, R2",
        MAIN_RAM_START, &bus, arm9.isARM7());
    arm9.setPC(MAIN_RAM_START);
    arm9.fetchAndExecute(1);  // saturate
    EXPECT_EQ(arm9.readFlag(Q_BIT), 1);

    arm9.fetchAndExecute(1);  // no saturation, Q still set
    EXPECT_EQ(arm9.readFlag(Q_BIT), 1);
}
// ==================================================================================================
// MRS
// ==================================================================================================
class TestCPU_ARM_DataInstructions_MRS : public TestCPU_ARM_DataInstructions {
protected:
    TestCPU_ARM_DataInstructions_MRS() {}
    ~TestCPU_ARM_DataInstructions_MRS() {}

    void SetUp() override { TestCPU_ARM_DataInstructions::SetUp(); }
    void TearDown() override { TestCPU_ARM_DataInstructions::TearDown(); }
};
// ==================================================================================================
/**
 * @brief Tests MRS reading CPSR's flag bits.
 */
TEST_F(TestCPU_ARM_DataInstructions_MRS, MRS_CPSR_FLAG_BITS) {
    uint32_t cpsr = 0xF8000000;
    arm9.setCPSR(cpsr);

    writeProgramToMemory("MRS R0, CPSR", MAIN_RAM_START, &bus, arm9.isARM7());
    arm9.setPC(MAIN_RAM_START);
    arm9.fetchAndExecute(1);
    EXPECT_EQ(arm9.readReg(0), cpsr);
}
// ==================================================================================================
/**
 * @brief Tests MRS reading CPSR's mode bits.
 */
TEST_F(TestCPU_ARM_DataInstructions_MRS, MRS_CPSR_MODE_BITS) {
    uint32_t cpsr = 0x00000013;  // Supervisor mode
    arm9.setCPSR(cpsr);

    writeProgramToMemory("MRS R0, CPSR", MAIN_RAM_START, &bus, arm9.isARM7());
    arm9.setPC(MAIN_RAM_START);
    arm9.fetchAndExecute(1);
    EXPECT_EQ(arm9.readReg(0), cpsr);
}
// ==================================================================================================
/**
 * @brief Tests MRS reading SPSR.
 */
TEST_F(TestCPU_ARM_DataInstructions_MRS, MRS_SPSR) {
    uint32_t spsr = 0xF80000FF;
    arm9.setProcessorMode(ProcessorModes::IRQ);
    arm9.setSPSR(spsr);

    writeProgramToMemory("MRS R1, SPSR", MAIN_RAM_START, &bus, arm9.isARM7());
    arm9.setPC(MAIN_RAM_START);
    arm9.fetchAndExecute(1);
    EXPECT_EQ(arm9.readReg(1), spsr);
}
// ==================================================================================================
// MSR
// ==================================================================================================
class TestCPU_ARM_DataInstructions_MSR : public TestCPU_ARM_DataInstructions {
protected:
    TestCPU_ARM_DataInstructions_MSR() {}
    ~TestCPU_ARM_DataInstructions_MSR() {}

    void SetUp() override { TestCPU_ARM_DataInstructions::SetUp(); }
    void TearDown() override { TestCPU_ARM_DataInstructions::TearDown(); }
};
/**
 * @brief Tests MSR writing CPSR's flag bits.
 */
TEST_F(TestCPU_ARM_DataInstructions_MSR, MSR_CPSR_FLAG_BITS) {
    arm9.setProcessorMode(ProcessorModes::Supervisor);
    uint32_t flagBits = 0xF8000000;
    arm9.writeReg(0, 0xFFFFFFFF);
    writeProgramToMemory("MSR CPSR_f, R0", MAIN_RAM_START, &bus, arm9.isARM7());
    arm9.setPC(MAIN_RAM_START);

    arm9.fetchAndExecute(1);
    EXPECT_EQ(arm9.readCPSR() & flagBits, flagBits);
}
/**
 * @brief Tests MSR writing CPSR's mode bits.
 */
TEST_F(TestCPU_ARM_DataInstructions_MSR, MSR_CPSR_MODE_BITS) {
    arm9.setProcessorMode(ProcessorModes::IRQ);
    arm9.writeReg(0, ProcessorModes::Supervisor);
    writeProgramToMemory("MSR CPSR_c, R0", MAIN_RAM_START, &bus, arm9.isARM7());
    arm9.setPC(MAIN_RAM_START);

    arm9.fetchAndExecute(1);
    EXPECT_EQ(arm9.getProcessorMode(), ProcessorModes::Supervisor);
}
/**
 * @brief Tests MSR writing all of CPSR's bits.
 */
TEST_F(TestCPU_ARM_DataInstructions_MSR, MSR_CPSR_FULL_BITS) {
    arm9.setProcessorMode(ProcessorModes::IRQ);
    uint32_t test = 0xF8000000 | ProcessorModes::Supervisor;
    arm9.writeReg(0, test);
    writeProgramToMemory("MSR CPSR_fsxc, R0", MAIN_RAM_START, &bus, arm9.isARM7());
    arm9.setPC(MAIN_RAM_START);
    arm9.fetchAndExecute(1);
    EXPECT_EQ(arm9.readCPSR(), test);
}
/**
 * @brief Tests MSR writing CPSR's flag bits via an immediate.
 */
TEST_F(TestCPU_ARM_DataInstructions_MSR, MSR_CPSR_FLAG_BITS_IMM) {
    arm9.setProcessorMode(ProcessorModes::IRQ);
    writeProgramToMemory("MSR CPSR_fsxc, #0xFF000000", MAIN_RAM_START, &bus, arm9.isARM7());
    arm9.setPC(MAIN_RAM_START);
    arm9.fetchAndExecute(1);
    EXPECT_EQ(arm9.readCPSR(), 0xF8000000);
}
/**
 * @brief Tests MSR only writes to the flag bits in user mode..
 */
TEST_F(TestCPU_ARM_DataInstructions_MSR, MSR_USER_MODE_PROTECTION) {
    arm9.setProcessorMode(ProcessorModes::User);
    uint32_t test = 0xF8000000 | ProcessorModes::Supervisor;
    arm9.writeReg(0, test);
    writeProgramToMemory("MSR CPSR_fsxc, R0", MAIN_RAM_START, &bus, arm9.isARM7());
    arm9.setPC(MAIN_RAM_START);
    arm9.fetchAndExecute(1);
    EXPECT_EQ(arm9.readCPSR(), 0xF8000000);
}
/**
 * @brief Tests MSR writing all of SPSR's bits.
 */
TEST_F(TestCPU_ARM_DataInstructions_MSR, MSR_SPSR) {
    arm9.setProcessorMode(ProcessorModes::IRQ);
    arm9.writeReg(1, 0xAAAAAAAA);
    writeProgramToMemory("MSR SPSR_fsxc, R1", MAIN_RAM_START, &bus, arm9.isARM7());
    arm9.setPC(MAIN_RAM_START);
    arm9.fetchAndExecute(1);
    EXPECT_EQ(arm9.readSPSR(), 0xAAAAAAAA);
}
/**
 * @brief Tests MSR writing all of SPSR's bits using an immediate.
 */
TEST_F(TestCPU_ARM_DataInstructions_MSR, MSR_SPSR_IMM) {
    arm9.setProcessorMode(ProcessorModes::IRQ);
    writeProgramToMemory("MSR SPSR_fsxc, #0xFF000000", MAIN_RAM_START, &bus, arm9.isARM7());
    arm9.setPC(MAIN_RAM_START);
    arm9.fetchAndExecute(1);
    EXPECT_EQ(arm9.readSPSR(), 0xFF000000);
}
// ==================================================================================================
// CLZ
// ==================================================================================================
class TestCPU_ARM_DataInstructions_CLZ : public TestCPU_ARM_DataInstructions {
protected:
    TestCPU_ARM_DataInstructions_CLZ() {}
    ~TestCPU_ARM_DataInstructions_CLZ() {}

    void SetUp() override { TestCPU_ARM_DataInstructions::SetUp(); }
    void TearDown() override { TestCPU_ARM_DataInstructions::TearDown(); }
};
/**
 * @brief Tests CLZ on data with 32 leading zeros.
 */
TEST_F(TestCPU_ARM_DataInstructions_CLZ, CLZ_ALL_ZEROS) {
    arm9.writeReg(1, 0x00000000);
    writeProgramToMemory("CLZ R0, R1", MAIN_RAM_START, &bus, arm9.isARM7());
    arm9.setPC(MAIN_RAM_START);
    arm9.fetchAndExecute(1);
    EXPECT_EQ(arm9.readReg(0), 32);
}
/**
 * @brief Tests CLZ on data with no leading zeros.
 */
TEST_F(TestCPU_ARM_DataInstructions_CLZ, CLZ_ALL_ONES) {
    arm9.writeReg(1, 0xFFFFFFFF);
    writeProgramToMemory("CLZ R0, R1", MAIN_RAM_START, &bus, arm9.isARM7());
    arm9.setPC(MAIN_RAM_START);
    arm9.fetchAndExecute(1);
    EXPECT_EQ(arm9.readReg(0), 0);
}
/**
 * @brief Tests CLZ on data with 8 leading zeros.
 */
TEST_F(TestCPU_ARM_DataInstructions_CLZ, CLZ_8_LEADING) {
    arm9.writeReg(1, 0x00FFFFFF);
    writeProgramToMemory("CLZ R0, R1", MAIN_RAM_START, &bus, arm9.isARM7());
    arm9.setPC(MAIN_RAM_START);
    arm9.fetchAndExecute(1);
    EXPECT_EQ(arm9.readReg(0), 8);

    arm9.writeReg(1, 0x00800000);
    writeProgramToMemory("CLZ R0, R1", MAIN_RAM_START, &bus, arm9.isARM7());
    arm9.setPC(MAIN_RAM_START);
    arm9.fetchAndExecute(1);
    EXPECT_EQ(arm9.readReg(0), 8);
}
// ==================================================================================================
// STRH
// ==================================================================================================
class TestCPU_ARM_DataInstructions_STRH : public TestCPU_ARM_DataInstructions {
protected:
    TestCPU_ARM_DataInstructions_STRH() {}
    ~TestCPU_ARM_DataInstructions_STRH() {}

    void SetUp() override { TestCPU_ARM_DataInstructions::SetUp(); }
    void TearDown() override { TestCPU_ARM_DataInstructions::TearDown(); }
};
/**
 * @brief Tests STRH with register offset.
 */
TEST_F(TestCPU_ARM_DataInstructions_STRH, STRH_REG_BASIC) {
    uint32_t base = MAIN_RAM_START + 0x400;

    arm9.writeReg(1, base);
    arm9.writeReg(2, 4);
    arm9.writeReg(0, 0x12345678);

    writeProgramToMemory("STRH R0, [R1, R2]", MAIN_RAM_START, &bus, arm9.isARM7());
    arm9.setPC(MAIN_RAM_START);

    arm9.fetchAndExecute();

    EXPECT_EQ(bus.read16ARM9(base + 4), 0x5678);
    EXPECT_EQ(arm9.readReg(1), base);
}
/**
 * @brief Tests STRH register offset with subtraction.
 */
TEST_F(TestCPU_ARM_DataInstructions_STRH, STRH_REG_NEG_OFFSET) {
    uint32_t base = MAIN_RAM_START + 0x500;

    arm9.writeReg(1, base);
    arm9.writeReg(2, 4);
    arm9.writeReg(0, 0xABCD1234);

    writeProgramToMemory("STRH R0, [R1, -R2]", MAIN_RAM_START, &bus, arm9.isARM7());
    arm9.setPC(MAIN_RAM_START);

    arm9.fetchAndExecute();

    EXPECT_EQ(bus.read16ARM9(base - 4), 0x1234);
}
/**
 * @brief Tests STRH register offset with writeback.
 */
TEST_F(TestCPU_ARM_DataInstructions_STRH, STRH_REG_WRITEBACK) {
    uint32_t base = MAIN_RAM_START + 0x600;

    arm9.writeReg(1, base);
    arm9.writeReg(2, 8);
    arm9.writeReg(0, 0xCAFEBEEF);

    writeProgramToMemory("STRH R0, [R1, R2]!", MAIN_RAM_START, &bus, arm9.isARM7());
    arm9.setPC(MAIN_RAM_START);

    arm9.fetchAndExecute();

    EXPECT_EQ(bus.read16ARM9(base + 8), 0xBEEF);
    EXPECT_EQ(arm9.readReg(1), base + 8);
}
/**
 * @brief Tests STRH with immediate offset.
 */
TEST_F(TestCPU_ARM_DataInstructions_STRH, STRH_IMM_BASIC) {
    uint32_t base = MAIN_RAM_START + 0x700;

    arm9.writeReg(1, base);
    arm9.writeReg(0, 0xDEADBEEF);

    writeProgramToMemory("STRH R0, [R1, #6]", MAIN_RAM_START, &bus, arm9.isARM7());
    arm9.setPC(MAIN_RAM_START);

    arm9.fetchAndExecute();

    EXPECT_EQ(bus.read16ARM9(base + 6), 0xBEEF);
    EXPECT_EQ(arm9.readReg(1), base);
}
/**
 * @brief Tests STRH immediate negative offset.
 */
TEST_F(TestCPU_ARM_DataInstructions_STRH, STRH_IMM_NEG_OFFSET) {
    uint32_t base = MAIN_RAM_START + 0x800;

    arm9.writeReg(1, base);
    arm9.writeReg(0, 0xFACEB00C);

    writeProgramToMemory("STRH R0, [R1, #-2]", MAIN_RAM_START, &bus, arm9.isARM7());
    arm9.setPC(MAIN_RAM_START);

    arm9.fetchAndExecute();

    EXPECT_EQ(bus.read16ARM9(base - 2), 0xB00C);
}
/**
 * @brief Tests STRH immediate with writeback.
 */
TEST_F(TestCPU_ARM_DataInstructions_STRH, STRH_IMM_WRITEBACK) {
    uint32_t base = MAIN_RAM_START + 0x900;

    arm9.writeReg(1, base);
    arm9.writeReg(0, 0x11223344);

    writeProgramToMemory("STRH R0, [R1, #10]!", MAIN_RAM_START, &bus, arm9.isARM7());
    arm9.setPC(MAIN_RAM_START);

    arm9.fetchAndExecute();

    EXPECT_EQ(bus.read16ARM9(base + 10), 0x3344);
    EXPECT_EQ(arm9.readReg(1), base + 10);
}
// ==================================================================================================
// STRD
// ==================================================================================================
class TestCPU_ARM_DataInstructions_STRD : public TestCPU_ARM_DataInstructions {
protected:
    TestCPU_ARM_DataInstructions_STRD() {}
    ~TestCPU_ARM_DataInstructions_STRD() {}

    void SetUp() override { TestCPU_ARM_DataInstructions::SetUp(); }
    void TearDown() override { TestCPU_ARM_DataInstructions::TearDown(); }
};
/**
 * @brief Tests STRD with register offset.
 */
TEST_F(TestCPU_ARM_DataInstructions_STRD, STRD_REG_BASIC) {
    uint32_t base = MAIN_RAM_START + 0x1000;

    arm9.writeReg(0, 0xAAAABBBB);
    arm9.writeReg(1, 0xCCCCDDDD);
    arm9.writeReg(2, base);
    arm9.writeReg(3, 8);

    writeProgramToMemory("STRD R0, R1, [R2, R3]", MAIN_RAM_START, &bus, arm9.isARM7());
    arm9.setPC(MAIN_RAM_START);

    arm9.fetchAndExecute();

    EXPECT_EQ(bus.read32ARM9(base + 8), 0xAAAABBBB);
    EXPECT_EQ(bus.read32ARM9(base + 12), 0xCCCCDDDD);
}
/**
 * @brief Tests STRD with immediate offset.
 */
TEST_F(TestCPU_ARM_DataInstructions_STRD, STRD_IMM_BASIC) {
    uint32_t base = MAIN_RAM_START + 0x1100;

    arm9.writeReg(4, base);

    arm9.writeReg(0, 0x11112222);
    arm9.writeReg(1, 0x33334444);

    writeProgramToMemory("STRD R0, R1, [R4, #8]", MAIN_RAM_START, &bus, arm9.isARM7());
    arm9.setPC(MAIN_RAM_START);

    arm9.fetchAndExecute();

    EXPECT_EQ(bus.read32ARM9(base + 8), 0x11112222);
    EXPECT_EQ(bus.read32ARM9(base + 12), 0x33334444);

    EXPECT_EQ(arm9.readReg(4), base);
}
/**
 * @brief Tests STRD with negative immediate offset.
 */
TEST_F(TestCPU_ARM_DataInstructions_STRD, STRD_IMM_NEG_OFFSET) {
    uint32_t base = MAIN_RAM_START + 0x1200;

    arm9.writeReg(4, base);

    arm9.writeReg(0, 0xAAAABBBB);
    arm9.writeReg(1, 0xCCCCDDDD);

    writeProgramToMemory("STRD R0, R1, [R4, #-8]", MAIN_RAM_START, &bus, arm9.isARM7());
    arm9.setPC(MAIN_RAM_START);

    arm9.fetchAndExecute();

    EXPECT_EQ(bus.read32ARM9(base - 8), 0xAAAABBBB);
    EXPECT_EQ(bus.read32ARM9(base - 4), 0xCCCCDDDD);
}
/**
 * @brief Tests STRD immediate offset with writeback.
 */
TEST_F(TestCPU_ARM_DataInstructions_STRD, STRD_IMM_WRITEBACK) {
    uint32_t base = MAIN_RAM_START + 0x1300;

    arm9.writeReg(5, base);

    arm9.writeReg(0, 0xDEADBEEF);
    arm9.writeReg(1, 0xCAFEBABE);

    writeProgramToMemory("STRD R0, R1, [R5, #16]!", MAIN_RAM_START, &bus, arm9.isARM7());
    arm9.setPC(MAIN_RAM_START);

    arm9.fetchAndExecute();

    EXPECT_EQ(bus.read32ARM9(base + 16), 0xDEADBEEF);
    EXPECT_EQ(bus.read32ARM9(base + 20), 0xCAFEBABE);

    EXPECT_EQ(arm9.readReg(5), base + 16);
}
// ==================================================================================================
// LDRH
// ==================================================================================================
class TestCPU_ARM_DataInstructions_LDRH : public TestCPU_ARM_DataInstructions {
protected:
    TestCPU_ARM_DataInstructions_LDRH() {}
    ~TestCPU_ARM_DataInstructions_LDRH() {}

    void SetUp() override { TestCPU_ARM_DataInstructions::SetUp(); }
    void TearDown() override { TestCPU_ARM_DataInstructions::TearDown(); }
};
/**
 * @brief Tests LDRH with register offset.
 */
TEST_F(TestCPU_ARM_DataInstructions_LDRH, LDRH_REG_BASIC) {
    uint32_t base = MAIN_RAM_START + 0x400;

    arm9.writeReg(1, base);
    arm9.writeReg(2, 4);

    bus.write16ARM9(base + 4, 0x5678);

    writeProgramToMemory("LDRH R0, [R1, R2]", MAIN_RAM_START, &bus, arm9.isARM7());
    arm9.setPC(MAIN_RAM_START);

    arm9.fetchAndExecute();

    EXPECT_EQ(arm9.readReg(0), 0x00005678);
    EXPECT_EQ(arm9.readReg(1), base);
}
/**
 * @brief Tests LDRH register offset with subtraction.
 */
TEST_F(TestCPU_ARM_DataInstructions_LDRH, LDRH_REG_NEG_OFFSET) {
    uint32_t base = MAIN_RAM_START + 0x500;

    arm9.writeReg(1, base);
    arm9.writeReg(2, 4);

    bus.write16ARM9(base - 4, 0x1234);

    writeProgramToMemory("LDRH R0, [R1, -R2]", MAIN_RAM_START, &bus, arm9.isARM7());
    arm9.setPC(MAIN_RAM_START);

    arm9.fetchAndExecute();

    EXPECT_EQ(arm9.readReg(0), 0x00001234);
}
/**
 * @brief Tests LDRH register offset with writeback.
 */
TEST_F(TestCPU_ARM_DataInstructions_LDRH, LDRH_REG_WRITEBACK) {
    uint32_t base = MAIN_RAM_START + 0x600;

    arm9.writeReg(1, base);
    arm9.writeReg(2, 8);

    bus.write16ARM9(base + 8, 0xBEEF);

    writeProgramToMemory("LDRH R0, [R1, R2]!", MAIN_RAM_START, &bus, arm9.isARM7());
    arm9.setPC(MAIN_RAM_START);

    arm9.fetchAndExecute();

    EXPECT_EQ(arm9.readReg(0), 0x0000BEEF);
    EXPECT_EQ(arm9.readReg(1), base + 8);
}
/**
 * @brief Tests LDRH with immediate offset.
 */
TEST_F(TestCPU_ARM_DataInstructions_LDRH, LDRH_IMM_BASIC) {
    uint32_t base = MAIN_RAM_START + 0x700;

    arm9.writeReg(1, base);

    bus.write16ARM9(base + 6, 0xCAFE);

    writeProgramToMemory("LDRH R0, [R1, #6]", MAIN_RAM_START, &bus, arm9.isARM7());
    arm9.setPC(MAIN_RAM_START);

    arm9.fetchAndExecute();

    EXPECT_EQ(arm9.readReg(0), 0x0000CAFE);
    EXPECT_EQ(arm9.readReg(1), base);
}
/**
 * @brief Tests LDRH immediate negative offset.
 */
TEST_F(TestCPU_ARM_DataInstructions_LDRH, LDRH_IMM_NEG_OFFSET) {
    uint32_t base = MAIN_RAM_START + 0x800;

    arm9.writeReg(1, base);

    bus.write16ARM9(base - 2, 0xB00C);

    writeProgramToMemory("LDRH R0, [R1, #-2]", MAIN_RAM_START, &bus, arm9.isARM7());
    arm9.setPC(MAIN_RAM_START);

    arm9.fetchAndExecute();

    EXPECT_EQ(arm9.readReg(0), 0x0000B00C);
}
/**
 * @brief Tests LDRH immediate with writeback.
 */
TEST_F(TestCPU_ARM_DataInstructions_LDRH, LDRH_IMM_WRITEBACK) {
    uint32_t base = MAIN_RAM_START + 0x900;

    arm9.writeReg(1, base);

    bus.write16ARM9(base + 10, 0x3344);

    writeProgramToMemory("LDRH R0, [R1, #10]!", MAIN_RAM_START, &bus, arm9.isARM7());
    arm9.setPC(MAIN_RAM_START);

    arm9.fetchAndExecute();

    EXPECT_EQ(arm9.readReg(0), 0x00003344);
    EXPECT_EQ(arm9.readReg(1), base + 10);
}
// ==================================================================================================
// LDRD
// ==================================================================================================
class TestCPU_ARM_DataInstructions_LDRD : public TestCPU_ARM_DataInstructions {
protected:
    TestCPU_ARM_DataInstructions_LDRD() {}
    ~TestCPU_ARM_DataInstructions_LDRD() {}

    void SetUp() override { TestCPU_ARM_DataInstructions::SetUp(); }
    void TearDown() override { TestCPU_ARM_DataInstructions::TearDown(); }
};
/**
 * @brief Tests LDRD with register offset.
 */
TEST_F(TestCPU_ARM_DataInstructions_LDRD, LDRD_REG_BASIC) {
    uint32_t base = MAIN_RAM_START + 0x400;

    arm9.writeReg(1, base);
    arm9.writeReg(2, 8);

    bus.write32ARM9(base + 8, 0x11112222);
    bus.write32ARM9(base + 12, 0x33334444);

    writeProgramToMemory("LDRD R0, R1, [R1, R2]", MAIN_RAM_START, &bus, arm9.isARM7());
    arm9.setPC(MAIN_RAM_START);

    arm9.fetchAndExecute();

    EXPECT_EQ(arm9.readReg(0), 0x11112222);
    EXPECT_EQ(arm9.readReg(1), 0x33334444);
}
/**
 * @brief Tests LDRD register offset with subtraction.
 */
TEST_F(TestCPU_ARM_DataInstructions_LDRD, LDRD_REG_NEG_OFFSET) {
    uint32_t base = MAIN_RAM_START + 0x500;

    arm9.writeReg(1, base);
    arm9.writeReg(2, 8);

    bus.write32ARM9(base - 8, 0xAAAABBBB);
    bus.write32ARM9(base - 4, 0xCCCCDDDD);

    writeProgramToMemory("LDRD R0, R1, [R1, -R2]", MAIN_RAM_START, &bus, arm9.isARM7());
    arm9.setPC(MAIN_RAM_START);

    arm9.fetchAndExecute();

    EXPECT_EQ(arm9.readReg(0), 0xAAAABBBB);
    EXPECT_EQ(arm9.readReg(1), 0xCCCCDDDD);
}
/**
 * @brief Tests LDRD register offset with writeback.
 */
TEST_F(TestCPU_ARM_DataInstructions_LDRD, LDRD_REG_WRITEBACK) {
    uint32_t base = MAIN_RAM_START + 0x600;

    arm9.writeReg(2, base);
    arm9.writeReg(3, 8);

    bus.write32ARM9(base + 8, 0xCAFEBABE);
    bus.write32ARM9(base + 12, 0xDEADBEEF);

    writeProgramToMemory("LDRD R0, R1, [R2, R3]!", MAIN_RAM_START, &bus, arm9.isARM7());
    arm9.setPC(MAIN_RAM_START);

    arm9.fetchAndExecute();

    EXPECT_EQ(arm9.readReg(0), 0xCAFEBABE);
    EXPECT_EQ(arm9.readReg(1), 0xDEADBEEF);
    EXPECT_EQ(arm9.readReg(2), base + 8);
}
/**
 * @brief Tests LDRD with immediate offset.
 */
TEST_F(TestCPU_ARM_DataInstructions_LDRD, LDRD_IMM_BASIC) {
    uint32_t base = MAIN_RAM_START + 0x700;

    arm9.writeReg(2, base);

    bus.write32ARM9(base + 8, 0x11111111);
    bus.write32ARM9(base + 12, 0x22222222);

    writeProgramToMemory("LDRD R0, R1, [R2, #8]", MAIN_RAM_START, &bus, arm9.isARM7());
    arm9.setPC(MAIN_RAM_START);

    arm9.fetchAndExecute();

    EXPECT_EQ(arm9.readReg(0), 0x11111111);
    EXPECT_EQ(arm9.readReg(1), 0x22222222);
    EXPECT_EQ(arm9.readReg(2), base);
}
/**
 * @brief Tests LDRD immediate negative offset.
 */
TEST_F(TestCPU_ARM_DataInstructions_LDRD, LDRD_IMM_NEG_OFFSET) {
    uint32_t base = MAIN_RAM_START + 0x800;

    arm9.writeReg(2, base);

    bus.write32ARM9(base - 8, 0xABABABAB);
    bus.write32ARM9(base - 4, 0xCDCDCDCD);

    writeProgramToMemory("LDRD R0, R1, [R2, #-8]", MAIN_RAM_START, &bus, arm9.isARM7());
    arm9.setPC(MAIN_RAM_START);

    arm9.fetchAndExecute();

    EXPECT_EQ(arm9.readReg(0), 0xABABABAB);
    EXPECT_EQ(arm9.readReg(1), 0xCDCDCDCD);
}
/**
 * @brief Tests LDRD immediate with writeback.
 */
TEST_F(TestCPU_ARM_DataInstructions_LDRD, LDRD_IMM_WRITEBACK) {
    uint32_t base = MAIN_RAM_START + 0x900;

    arm9.writeReg(2, base);

    bus.write32ARM9(base + 16, 0x01020304);
    bus.write32ARM9(base + 20, 0xAABBCCDD);

    writeProgramToMemory("LDRD R0, R1, [R2, #16]!", MAIN_RAM_START, &bus, arm9.isARM7());
    arm9.setPC(MAIN_RAM_START);

    arm9.fetchAndExecute();

    EXPECT_EQ(arm9.readReg(0), 0x01020304);
    EXPECT_EQ(arm9.readReg(1), 0xAABBCCDD);
    EXPECT_EQ(arm9.readReg(2), base + 16);
}
// ==================================================================================================
// LDRSB
// ==================================================================================================
class TestCPU_ARM_DataInstructions_LDRSB : public TestCPU_ARM_DataInstructions {
protected:
    TestCPU_ARM_DataInstructions_LDRSB() {}
    ~TestCPU_ARM_DataInstructions_LDRSB() {}

    void SetUp() override { TestCPU_ARM_DataInstructions::SetUp(); }
    void TearDown() override { TestCPU_ARM_DataInstructions::TearDown(); }
};
/**
 * @brief Tests LDRSB with register offset.
 */
TEST_F(TestCPU_ARM_DataInstructions_LDRSB, LDRSB_REG_BASIC) {
    uint32_t base = MAIN_RAM_START + 0x400;

    arm9.writeReg(1, base);
    arm9.writeReg(2, 4);

    bus.write8ARM9(base + 4, 0x7F);

    writeProgramToMemory("LDRSB R0, [R1, R2]", MAIN_RAM_START, &bus, arm9.isARM7());
    arm9.setPC(MAIN_RAM_START);

    arm9.fetchAndExecute();

    EXPECT_EQ(arm9.readReg(0), 0x0000007F);
    EXPECT_EQ(arm9.readReg(1), base);
}
/**
 * @brief Tests LDRSB register offset with subtraction.
 */
TEST_F(TestCPU_ARM_DataInstructions_LDRSB, LDRSB_REG_NEG_OFFSET) {
    uint32_t base = MAIN_RAM_START + 0x500;

    arm9.writeReg(1, base);
    arm9.writeReg(2, 4);

    bus.write8ARM9(base - 4, 0x20);

    writeProgramToMemory("LDRSB R0, [R1, -R2]", MAIN_RAM_START, &bus, arm9.isARM7());
    arm9.setPC(MAIN_RAM_START);

    arm9.fetchAndExecute();

    EXPECT_EQ(arm9.readReg(0), 0x00000020);
}
/**
 * @brief Tests LDRSB register offset with writeback.
 */
TEST_F(TestCPU_ARM_DataInstructions_LDRSB, LDRSB_REG_WRITEBACK) {
    uint32_t base = MAIN_RAM_START + 0x600;

    arm9.writeReg(1, base);
    arm9.writeReg(2, 8);

    bus.write8ARM9(base + 8, 0x55);

    writeProgramToMemory("LDRSB R0, [R1, R2]!", MAIN_RAM_START, &bus, arm9.isARM7());
    arm9.setPC(MAIN_RAM_START);

    arm9.fetchAndExecute();

    EXPECT_EQ(arm9.readReg(0), 0x00000055);
    EXPECT_EQ(arm9.readReg(1), base + 8);
}
/**
 * @brief Tests LDRSB with immediate offset.
 */
TEST_F(TestCPU_ARM_DataInstructions_LDRSB, LDRSB_IMM_BASIC) {
    uint32_t base = MAIN_RAM_START + 0x700;

    arm9.writeReg(1, base);

    bus.write8ARM9(base + 6, 0x33);

    writeProgramToMemory("LDRSB R0, [R1, #6]", MAIN_RAM_START, &bus, arm9.isARM7());
    arm9.setPC(MAIN_RAM_START);

    arm9.fetchAndExecute();

    EXPECT_EQ(arm9.readReg(0), 0x00000033);
    EXPECT_EQ(arm9.readReg(1), base);
}
/**
 * @brief Tests LDRSB immediate negative offset.
 */
TEST_F(TestCPU_ARM_DataInstructions_LDRSB, LDRSB_IMM_NEG_OFFSET) {
    uint32_t base = MAIN_RAM_START + 0x800;

    arm9.writeReg(1, base);

    bus.write8ARM9(base - 2, 0x11);

    writeProgramToMemory("LDRSB R0, [R1, #-2]", MAIN_RAM_START, &bus, arm9.isARM7());
    arm9.setPC(MAIN_RAM_START);

    arm9.fetchAndExecute();

    EXPECT_EQ(arm9.readReg(0), 0x00000011);
}
/**
 * @brief Tests LDRSB immediate with writeback.
 */
TEST_F(TestCPU_ARM_DataInstructions_LDRSB, LDRSB_IMM_WRITEBACK) {
    uint32_t base = MAIN_RAM_START + 0x900;

    arm9.writeReg(1, base);

    bus.write8ARM9(base + 10, 0x44);

    writeProgramToMemory("LDRSB R0, [R1, #10]!", MAIN_RAM_START, &bus, arm9.isARM7());
    arm9.setPC(MAIN_RAM_START);

    arm9.fetchAndExecute();

    EXPECT_EQ(arm9.readReg(0), 0x00000044);
    EXPECT_EQ(arm9.readReg(1), base + 10);
}
/**
 * @brief Tests LDRSB sign extension (negative value).
 */
TEST_F(TestCPU_ARM_DataInstructions_LDRSB, LDRSB_SIGN_EXTEND) {
    uint32_t base = MAIN_RAM_START + 0xA00;

    arm9.writeReg(1, base);

    bus.write8ARM9(base + 2, 0x80);  // -128

    writeProgramToMemory("LDRSB R0, [R1, #2]", MAIN_RAM_START, &bus, arm9.isARM7());
    arm9.setPC(MAIN_RAM_START);

    arm9.fetchAndExecute();

    EXPECT_EQ(arm9.readReg(0), 0xFFFFFF80);
}
// ==================================================================================================
// LDRSH
// ==================================================================================================
class TestCPU_ARM_DataInstructions_LDRSH : public TestCPU_ARM_DataInstructions {
protected:
    TestCPU_ARM_DataInstructions_LDRSH() {}
    ~TestCPU_ARM_DataInstructions_LDRSH() {}

    void SetUp() override { TestCPU_ARM_DataInstructions::SetUp(); }
    void TearDown() override { TestCPU_ARM_DataInstructions::TearDown(); }
};
/**
 * @brief Tests LDRSH with register offset.
 */
TEST_F(TestCPU_ARM_DataInstructions_LDRSH, LDRSH_REG_BASIC) {
    uint32_t base = MAIN_RAM_START + 0x400;

    arm9.writeReg(1, base);
    arm9.writeReg(2, 4);

    bus.write16ARM9(base + 4, 0x7FFF);

    writeProgramToMemory("LDRSH R0, [R1, R2]", MAIN_RAM_START, &bus, arm9.isARM7());
    arm9.setPC(MAIN_RAM_START);

    arm9.fetchAndExecute();

    EXPECT_EQ(arm9.readReg(0), 0x00007FFF);
    EXPECT_EQ(arm9.readReg(1), base);
}
/**
 * @brief Tests LDRSH register offset with subtraction.
 */
TEST_F(TestCPU_ARM_DataInstructions_LDRSH, LDRSH_REG_NEG_OFFSET) {
    uint32_t base = MAIN_RAM_START + 0x500;

    arm9.writeReg(1, base);
    arm9.writeReg(2, 4);

    bus.write16ARM9(base - 4, 0x1234);

    writeProgramToMemory("LDRSH R0, [R1, -R2]", MAIN_RAM_START, &bus, arm9.isARM7());
    arm9.setPC(MAIN_RAM_START);

    arm9.fetchAndExecute();

    EXPECT_EQ(arm9.readReg(0), 0x00001234);
}
/**
 * @brief Tests LDRSH register offset with writeback.
 */
TEST_F(TestCPU_ARM_DataInstructions_LDRSH, LDRSH_REG_WRITEBACK) {
    uint32_t base = MAIN_RAM_START + 0x600;

    arm9.writeReg(1, base);
    arm9.writeReg(2, 8);

    bus.write16ARM9(base + 8, 0x1111);

    writeProgramToMemory("LDRSH R0, [R1, R2]!", MAIN_RAM_START, &bus, arm9.isARM7());
    arm9.setPC(MAIN_RAM_START);

    arm9.fetchAndExecute();

    EXPECT_EQ(arm9.readReg(0), 0x00001111);
    EXPECT_EQ(arm9.readReg(1), base + 8);
}
/**
 * @brief Tests LDRSH with immediate offset.
 */
TEST_F(TestCPU_ARM_DataInstructions_LDRSH, LDRSH_IMM_BASIC) {
    uint32_t base = MAIN_RAM_START + 0x700;

    arm9.writeReg(1, base);

    bus.write16ARM9(base + 6, 0x2222);

    writeProgramToMemory("LDRSH R0, [R1, #6]", MAIN_RAM_START, &bus, arm9.isARM7());
    arm9.setPC(MAIN_RAM_START);

    arm9.fetchAndExecute();

    EXPECT_EQ(arm9.readReg(0), 0x00002222);
    EXPECT_EQ(arm9.readReg(1), base);
}
/**
 * @brief Tests LDRSH immediate negative offset.
 */
TEST_F(TestCPU_ARM_DataInstructions_LDRSH, LDRSH_IMM_NEG_OFFSET) {
    uint32_t base = MAIN_RAM_START + 0x800;

    arm9.writeReg(1, base);

    bus.write16ARM9(base - 2, 0x3333);

    writeProgramToMemory("LDRSH R0, [R1, #-2]", MAIN_RAM_START, &bus, arm9.isARM7());
    arm9.setPC(MAIN_RAM_START);

    arm9.fetchAndExecute();

    EXPECT_EQ(arm9.readReg(0), 0x00003333);
}
/**
 * @brief Tests LDRSH immediate with writeback.
 */
TEST_F(TestCPU_ARM_DataInstructions_LDRSH, LDRSH_IMM_WRITEBACK) {
    uint32_t base = MAIN_RAM_START + 0x900;

    arm9.writeReg(1, base);

    bus.write16ARM9(base + 10, 0x4444);

    writeProgramToMemory("LDRSH R0, [R1, #10]!", MAIN_RAM_START, &bus, arm9.isARM7());
    arm9.setPC(MAIN_RAM_START);

    arm9.fetchAndExecute();

    EXPECT_EQ(arm9.readReg(0), 0x00004444);
    EXPECT_EQ(arm9.readReg(1), base + 10);
}
/**
 * @brief Tests LDRSH sign extension.
 */
TEST_F(TestCPU_ARM_DataInstructions_LDRSH, LDRSH_SIGN_EXTEND) {
    uint32_t base = MAIN_RAM_START + 0xA00;

    arm9.writeReg(1, base);

    bus.write16ARM9(base + 2, 0x8000);  // -32768

    writeProgramToMemory("LDRSH R0, [R1, #2]", MAIN_RAM_START, &bus, arm9.isARM7());
    arm9.setPC(MAIN_RAM_START);

    arm9.fetchAndExecute();

    EXPECT_EQ(arm9.readReg(0), 0xFFFF8000);
}
