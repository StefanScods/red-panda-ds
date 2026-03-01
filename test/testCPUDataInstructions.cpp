#include <gtest/gtest.h>

#include "../src/cpu.h"
#include "../src/interconnect.h"
#include "../src/memoryDefines.h"
#include "../src/utils/armEncode.h"
#include "commonTest.h"

// TODO Handle ARM9 only commands.

/**
 * @brief Test fixture for the CPU's instructions in the data category.
 */
class TestCPUDataInstructions : public testing::Test {
protected:
    Interconnect bus;
    ARM7TDMI arm7;
    ARM946ES arm9;
    TestCPUDataInstructions() {}
    ~TestCPUDataInstructions() {}

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
class TestCPUDataInstructions_AND : public TestCPUDataInstructions {
protected:
    TestCPUDataInstructions_AND() {}
    ~TestCPUDataInstructions_AND() {}

    void SetUp() override { TestCPUDataInstructions::SetUp(); }
    void TearDown() override { TestCPUDataInstructions::TearDown(); }
};
/**
 * @brief Tests an AND operation using an immediate as the second operand.
 */
TEST_F(TestCPUDataInstructions_AND, AND_IMMEDIATE) {
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
TEST_F(TestCPUDataInstructions_AND, AND_IMMEDIATE_NEGATIVE_FLAG) {
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
TEST_F(TestCPUDataInstructions_AND, AND_IMMEDIATE_ZERO_FLAG) {
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
TEST_F(TestCPUDataInstructions_AND, AND_IMMEDIATE_CARRY_FLAG) {
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
TEST_F(TestCPUDataInstructions_AND, AND_IMMEDIATE_OVERFLOW_FLAG) {
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
class TestCPUDataInstructions_EOR : public TestCPUDataInstructions {
protected:
    TestCPUDataInstructions_EOR() {}
    ~TestCPUDataInstructions_EOR() {}

    void SetUp() override { TestCPUDataInstructions::SetUp(); }
    void TearDown() override { TestCPUDataInstructions::TearDown(); }
};
/**
 * @brief Tests an EOR operation using an immediate as the second operand.
 */
TEST_F(TestCPUDataInstructions_EOR, EOR_IMMEDIATE) {
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
TEST_F(TestCPUDataInstructions_EOR, EOR_IMMEDIATE_NEGATIVE_FLAG) {
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
TEST_F(TestCPUDataInstructions_EOR, EOR_IMMEDIATE_ZERO_FLAG) {
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
TEST_F(TestCPUDataInstructions_EOR, EOR_IMMEDIATE_CARRY_FLAG) {
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
TEST_F(TestCPUDataInstructions_EOR, EOR_IMMEDIATE_OVERFLOW_FLAG) {
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
class TestCPUDataInstructions_SUB : public TestCPUDataInstructions {
protected:
    TestCPUDataInstructions_SUB() {}
    ~TestCPUDataInstructions_SUB() {}

    void SetUp() override { TestCPUDataInstructions::SetUp(); }
    void TearDown() override { TestCPUDataInstructions::TearDown(); }
};
/**
 * @brief Tests a SUB operation using an immediate as the second operand.
 */
TEST_F(TestCPUDataInstructions_SUB, SUB_IMMEDIATE) {
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
TEST_F(TestCPUDataInstructions_SUB, SUB_IMMEDIATE_NEGATIVE_FLAG) {
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
TEST_F(TestCPUDataInstructions_SUB, SUB_IMMEDIATE_ZERO_FLAG) {
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
TEST_F(TestCPUDataInstructions_SUB, SUB_IMMEDIATE_CARRY_FLAG) {
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
TEST_F(TestCPUDataInstructions_SUB, SUB_IMMEDIATE_OVERFLOW_FLAG) {
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
class TestCPUDataInstructions_RSB : public TestCPUDataInstructions {
protected:
    TestCPUDataInstructions_RSB() {}
    ~TestCPUDataInstructions_RSB() {}

    void SetUp() override { TestCPUDataInstructions::SetUp(); }
    void TearDown() override { TestCPUDataInstructions::TearDown(); }
};
/**
 * @brief Tests an RSB operation using an immediate as the second operand.
 */
TEST_F(TestCPUDataInstructions_RSB, RSB_IMMEDIATE) {
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
TEST_F(TestCPUDataInstructions_RSB, RSB_IMMEDIATE_NEGATIVE_FLAG) {
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
TEST_F(TestCPUDataInstructions_RSB, RSB_IMMEDIATE_ZERO_FLAG) {
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
TEST_F(TestCPUDataInstructions_RSB, RSB_IMMEDIATE_CARRY_FLAG) {
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
TEST_F(TestCPUDataInstructions_RSB, RSB_IMMEDIATE_OVERFLOW_FLAG) {
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
class TestCPUDataInstructions_ADD : public TestCPUDataInstructions {
protected:
    TestCPUDataInstructions_ADD() {}
    ~TestCPUDataInstructions_ADD() {}

    void SetUp() override { TestCPUDataInstructions::SetUp(); }
    void TearDown() override { TestCPUDataInstructions::TearDown(); }
};
/**
 * @brief Tests an ADD operation using an immediate as the second operand.
 */
TEST_F(TestCPUDataInstructions_ADD, ADD_IMMEDIATE) {
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
TEST_F(TestCPUDataInstructions_ADD, ADD_IMMEDIATE_NEGATIVE_FLAG) {
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
TEST_F(TestCPUDataInstructions_ADD, ADD_IMMEDIATE_ZERO_FLAG) {
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
TEST_F(TestCPUDataInstructions_ADD, ADD_IMMEDIATE_CARRY_FLAG) {
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
TEST_F(TestCPUDataInstructions_ADD, ADD_IMMEDIATE_OVERFLOW_FLAG) {
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
class TestCPUDataInstructions_ADC : public TestCPUDataInstructions {
protected:
    TestCPUDataInstructions_ADC() {}
    ~TestCPUDataInstructions_ADC() {}

    void SetUp() override { TestCPUDataInstructions::SetUp(); }
    void TearDown() override { TestCPUDataInstructions::TearDown(); }
};
/**
 * @brief Tests an ADC operation using an immediate as the second operand.
 */
TEST_F(TestCPUDataInstructions_ADC, ADC_IMMEDIATE) {
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
TEST_F(TestCPUDataInstructions_ADC, ADC_IMMEDIATE_NEGATIVE_FLAG) {
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
TEST_F(TestCPUDataInstructions_ADC, ADC_IMMEDIATE_ZERO_FLAG) {
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
TEST_F(TestCPUDataInstructions_ADC, ADC_IMMEDIATE_CARRY_FLAG) {
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
TEST_F(TestCPUDataInstructions_ADC, ADC_IMMEDIATE_OVERFLOW_FLAG) {
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
class TestCPUDataInstructions_SBC : public TestCPUDataInstructions {
protected:
    TestCPUDataInstructions_SBC() {}
    ~TestCPUDataInstructions_SBC() {}

    void SetUp() override { TestCPUDataInstructions::SetUp(); }
    void TearDown() override { TestCPUDataInstructions::TearDown(); }
};
/**
 * @brief Tests an SBC operation using an immediate as the second operand.
 */
TEST_F(TestCPUDataInstructions_SBC, SBC_IMMEDIATE) {
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
TEST_F(TestCPUDataInstructions_SBC, SBC_IMMEDIATE_NEGATIVE_FLAG) {
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
TEST_F(TestCPUDataInstructions_SBC, SBC_IMMEDIATE_ZERO_FLAG) {
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
TEST_F(TestCPUDataInstructions_SBC, SBC_IMMEDIATE_CARRY_FLAG) {
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
TEST_F(TestCPUDataInstructions_SBC, SBC_IMMEDIATE_OVERFLOW_FLAG) {
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
class TestCPUDataInstructions_RSC : public TestCPUDataInstructions {
protected:
    TestCPUDataInstructions_RSC() {}
    ~TestCPUDataInstructions_RSC() {}

    void SetUp() override { TestCPUDataInstructions::SetUp(); }
    void TearDown() override { TestCPUDataInstructions::TearDown(); }
};
/**
 * @brief Tests an RSC operation using an immediate as the second operand.
 */
TEST_F(TestCPUDataInstructions_RSC, RSC_IMMEDIATE) {
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
TEST_F(TestCPUDataInstructions_RSC, RSC_IMMEDIATE_NEGATIVE_FLAG) {
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
TEST_F(TestCPUDataInstructions_RSC, RSC_IMMEDIATE_ZERO_FLAG) {
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
TEST_F(TestCPUDataInstructions_RSC, RSC_IMMEDIATE_CARRY_FLAG) {
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
TEST_F(TestCPUDataInstructions_RSC, RSC_IMMEDIATE_OVERFLOW_FLAG) {
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
class TestCPUDataInstructions_TST : public TestCPUDataInstructions {
protected:
    TestCPUDataInstructions_TST() {}
    ~TestCPUDataInstructions_TST() {}

    void SetUp() override { TestCPUDataInstructions::SetUp(); }
    void TearDown() override { TestCPUDataInstructions::TearDown(); }
};
/**
 * @brief Tests a TST operation's negative flag behaviour using an immediate as the second operand.
 */
TEST_F(TestCPUDataInstructions_TST, TST_IMMEDIATE_NEGATIVE_FLAG) {
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
TEST_F(TestCPUDataInstructions_TST, TST_IMMEDIATE_ZERO_FLAG) {
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
TEST_F(TestCPUDataInstructions_TST, TST_IMMEDIATE_CARRY_FLAG) {
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
class TestCPUDataInstructions_TEQ : public TestCPUDataInstructions {
protected:
    TestCPUDataInstructions_TEQ() {}
    ~TestCPUDataInstructions_TEQ() {}

    void SetUp() override { TestCPUDataInstructions::SetUp(); }
    void TearDown() override { TestCPUDataInstructions::TearDown(); }
};
/**
 * @brief Tests a TEQ operation's negative flag behaviour using an immediate as the second operand.
 */
TEST_F(TestCPUDataInstructions_TEQ, TEQ_IMMEDIATE_NEGATIVE_FLAG) {
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
TEST_F(TestCPUDataInstructions_TEQ, TEQ_IMMEDIATE_ZERO_FLAG) {
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
TEST_F(TestCPUDataInstructions_TEQ, TEQ_IMMEDIATE_CARRY_FLAG) {
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
class TestCPUDataInstructions_CMP : public TestCPUDataInstructions {
protected:
    TestCPUDataInstructions_CMP() {}
    ~TestCPUDataInstructions_CMP() {}

    void SetUp() override { TestCPUDataInstructions::SetUp(); }
    void TearDown() override { TestCPUDataInstructions::TearDown(); }
};
/**
 * @brief Tests a CMP operation's negative flag behaviour using an immediate as the second operand.
 */
TEST_F(TestCPUDataInstructions_CMP, CMP_IMMEDIATE_NEGATIVE_FLAG) {
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
TEST_F(TestCPUDataInstructions_CMP, CMP_IMMEDIATE_ZERO_FLAG) {
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
TEST_F(TestCPUDataInstructions_CMP, CMP_IMMEDIATE_CARRY_FLAG) {
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
TEST_F(TestCPUDataInstructions_CMP, CMP_IMMEDIATE_OVERFLOW_FLAG) {
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
class TestCPUDataInstructions_CMN : public TestCPUDataInstructions {
protected:
    TestCPUDataInstructions_CMN() {}
    ~TestCPUDataInstructions_CMN() {}

    void SetUp() override { TestCPUDataInstructions::SetUp(); }
    void TearDown() override { TestCPUDataInstructions::TearDown(); }
};
/**
 * @brief Tests a CMN operation's negative flag behaviour using an immediate as the second operand.
 */
TEST_F(TestCPUDataInstructions_CMN, CMN_IMMEDIATE_NEGATIVE_FLAG) {
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
TEST_F(TestCPUDataInstructions_CMN, CMN_IMMEDIATE_ZERO_FLAG) {
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
TEST_F(TestCPUDataInstructions_CMN, CMN_IMMEDIATE_CARRY_FLAG) {
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
TEST_F(TestCPUDataInstructions_CMN, CMN_IMMEDIATE_OVERFLOW_FLAG) {
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
class TestCPUDataInstructions_ORR : public TestCPUDataInstructions {
protected:
    TestCPUDataInstructions_ORR() {}
    ~TestCPUDataInstructions_ORR() {}

    void SetUp() override { TestCPUDataInstructions::SetUp(); }
    void TearDown() override { TestCPUDataInstructions::TearDown(); }
};
/**
 * @brief Tests an ORR operation using an immediate as the second operand.
 */
TEST_F(TestCPUDataInstructions_ORR, ORR_IMMEDIATE) {
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
TEST_F(TestCPUDataInstructions_ORR, ORR_IMMEDIATE_NEGATIVE_FLAG) {
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
TEST_F(TestCPUDataInstructions_ORR, ORR_IMMEDIATE_ZERO_FLAG) {
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
TEST_F(TestCPUDataInstructions_ORR, ORR_IMMEDIATE_CARRY_FLAG) {
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
class TestCPUDataInstructions_MOV : public TestCPUDataInstructions {
protected:
    TestCPUDataInstructions_MOV() {}
    ~TestCPUDataInstructions_MOV() {}

    void SetUp() override { TestCPUDataInstructions::SetUp(); }
    void TearDown() override { TestCPUDataInstructions::TearDown(); }
};

/**
 * @brief Test moving value between registers.
 */
TEST_F(TestCPUDataInstructions_MOV, MOV_REGISTER) {
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
TEST_F(TestCPUDataInstructions_MOV, MOV_REGISTER_LSL) {
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
TEST_F(TestCPUDataInstructions_MOV, MOV_REGISTER_LSR) {
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
TEST_F(TestCPUDataInstructions_MOV, MOV_REGISTER_ASR) {
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
TEST_F(TestCPUDataInstructions_MOV, MOV_REGISTER_ROR) {
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
TEST_F(TestCPUDataInstructions_MOV, MOV_REGISTER_RRX) {
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
TEST_F(TestCPUDataInstructions_MOV, MOV_REGISTER_LSL_REG) {
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
TEST_F(TestCPUDataInstructions_MOV, MOV_REGISTER_LSR_REG) {
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
TEST_F(TestCPUDataInstructions_MOV, MOV_REGISTER_ASR_REG) {
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
TEST_F(TestCPUDataInstructions_MOV, MOV_REGISTER_ROR_REG) {
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
TEST_F(TestCPUDataInstructions_MOV, MOV_REGISTER_ROR_ZERO_REG) {
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
TEST_F(TestCPUDataInstructions_MOV, MOV_IMMEDIATE_NEGATIVE_FLAG) {
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
TEST_F(TestCPUDataInstructions_MOV, MOV_IMMEDIATE_ZERO_FLAG) {
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
TEST_F(TestCPUDataInstructions_MOV, MOV_IMMEDIATE_CARRY_FLAG) {
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
class TestCPUDataInstructions_BIC : public TestCPUDataInstructions {
protected:
    TestCPUDataInstructions_BIC() {}
    ~TestCPUDataInstructions_BIC() {}

    void SetUp() override { TestCPUDataInstructions::SetUp(); }
    void TearDown() override { TestCPUDataInstructions::TearDown(); }
};
/**
 * @brief Tests a BIC operation using an immediate as the second operand.
 */
TEST_F(TestCPUDataInstructions_BIC, BIC_IMMEDIATE) {
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
TEST_F(TestCPUDataInstructions_BIC, BIC_IMMEDIATE_NEGATIVE_FLAG) {
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
TEST_F(TestCPUDataInstructions_BIC, BIC_IMMEDIATE_ZERO_FLAG) {
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
TEST_F(TestCPUDataInstructions_BIC, BIC_IMMEDIATE_CARRY_FLAG) {
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
class TestCPUDataInstructions_MVN : public TestCPUDataInstructions {
protected:
    TestCPUDataInstructions_MVN() {}
    ~TestCPUDataInstructions_MVN() {}

    void SetUp() override { TestCPUDataInstructions::SetUp(); }
    void TearDown() override { TestCPUDataInstructions::TearDown(); }
};
/**
 * @brief Tests an MVN operation using an immediate as the second operand.
 */
TEST_F(TestCPUDataInstructions_MVN, MVN_IMMEDIATE) {
    // N = true
    writeProgramToMemory("MVN R0, #0x000000F0\n", MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);
    ASSERT_EQ(arm7.readReg(0), 0xFFFFFF0F);
}
/**
 * @brief Tests an MVN operation's negative flag behaviour using an immediate as the second operand.
 */
TEST_F(TestCPUDataInstructions_MVN, MVN_IMMEDIATE_NEGATIVE_FLAG) {
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
TEST_F(TestCPUDataInstructions_MVN, MVN_IMMEDIATE_ZERO_FLAG) {
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
TEST_F(TestCPUDataInstructions_MVN, MVN_IMMEDIATE_CARRY_FLAG) {
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
class TestCPUDataInstructions_MUL : public TestCPUDataInstructions {
protected:
    TestCPUDataInstructions_MUL() {}
    ~TestCPUDataInstructions_MUL() {}

    void SetUp() override { TestCPUDataInstructions::SetUp(); }
    void TearDown() override { TestCPUDataInstructions::TearDown(); }
};
/**
 * @brief Tests an MUL operation using small numbers.
 */
TEST_F(TestCPUDataInstructions_MUL, MUL_0) {
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
TEST_F(TestCPUDataInstructions_MUL, MUL_1) {
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
TEST_F(TestCPUDataInstructions_MUL, MUL_2) {
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
TEST_F(TestCPUDataInstructions_MUL, MUL_ZERO_FLAG) {
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
TEST_F(TestCPUDataInstructions_MUL, MUL_NEGATIVE_FLAG) {
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
class TestCPUDataInstructions_MLA : public TestCPUDataInstructions {
protected:
    TestCPUDataInstructions_MLA() {}
    ~TestCPUDataInstructions_MLA() {}

    void SetUp() override { TestCPUDataInstructions::SetUp(); }
    void TearDown() override { TestCPUDataInstructions::TearDown(); }
};
/**
 * @brief Tests an MLA operation using small numbers.
 */
TEST_F(TestCPUDataInstructions_MLA, MLA_0) {
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
TEST_F(TestCPUDataInstructions_MLA, MLA_1) {
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
TEST_F(TestCPUDataInstructions_MLA, MLA_2) {
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
TEST_F(TestCPUDataInstructions_MLA, MLA_ZERO_FLAG) {
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
TEST_F(TestCPUDataInstructions_MLA, MLA_NEGATIVE_FLAG) {
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
class TestCPUDataInstructions_UMULL : public TestCPUDataInstructions {
protected:
    TestCPUDataInstructions_UMULL() {}
    ~TestCPUDataInstructions_UMULL() {}

    void SetUp() override { TestCPUDataInstructions::SetUp(); }
    void TearDown() override { TestCPUDataInstructions::TearDown(); }
};
/**
 * @brief Tests an UMULL operation using small numbers.
 */
TEST_F(TestCPUDataInstructions_UMULL, UMULL_0) {
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
TEST_F(TestCPUDataInstructions_UMULL, UMULL_1) {
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
TEST_F(TestCPUDataInstructions_UMULL, UMULL_2) {
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
TEST_F(TestCPUDataInstructions_UMULL, UMULL_ZERO_FLAG) {
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
TEST_F(TestCPUDataInstructions_UMULL, UMULL_NEGATIVE_FLAG) {
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
class TestCPUDataInstructions_UMLAL : public TestCPUDataInstructions {
protected:
    TestCPUDataInstructions_UMLAL() {}
    ~TestCPUDataInstructions_UMLAL() {}

    void SetUp() override { TestCPUDataInstructions::SetUp(); }
    void TearDown() override { TestCPUDataInstructions::TearDown(); }
};
/**
 * @brief Tests a UMLAL operation using small numbers.
 */
TEST_F(TestCPUDataInstructions_UMLAL, UMLAL_0) {
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
TEST_F(TestCPUDataInstructions_UMLAL, UMLAL_1) {
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
TEST_F(TestCPUDataInstructions_UMLAL, UMLAL_2) {
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
TEST_F(TestCPUDataInstructions_UMLAL, UMLAL_ZERO_FLAG) {
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
TEST_F(TestCPUDataInstructions_UMLAL, UMLAL_NEGATIVE_FLAG) {
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
class TestCPUDataInstructions_SMULL : public TestCPUDataInstructions {
protected:
    TestCPUDataInstructions_SMULL() {}
    ~TestCPUDataInstructions_SMULL() {}

    void SetUp() override { TestCPUDataInstructions::SetUp(); }
    void TearDown() override { TestCPUDataInstructions::TearDown(); }
};
/**
 * @brief Tests an SMULL operation using small numbers.
 */
TEST_F(TestCPUDataInstructions_SMULL, SMULL_0) {
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
TEST_F(TestCPUDataInstructions_SMULL, SMULL_1) {
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
TEST_F(TestCPUDataInstructions_SMULL, SMULL_2) {
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
TEST_F(TestCPUDataInstructions_SMULL, SMULL_3) {
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
TEST_F(TestCPUDataInstructions_SMULL, SMULL_ZERO_FLAG) {
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
TEST_F(TestCPUDataInstructions_SMULL, SMULL_NEGATIVE_FLAG) {
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
class TestCPUDataInstructions_SMLAL : public TestCPUDataInstructions {
protected:
    TestCPUDataInstructions_SMLAL() {}
    ~TestCPUDataInstructions_SMLAL() {}

    void SetUp() override { TestCPUDataInstructions::SetUp(); }
    void TearDown() override { TestCPUDataInstructions::TearDown(); }
};
/**
 * @brief Tests an SMLAL operation using small numbers.
 */
TEST_F(TestCPUDataInstructions_SMLAL, SMLAL_0) {
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
TEST_F(TestCPUDataInstructions_SMLAL, SMLAL_1) {
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
TEST_F(TestCPUDataInstructions_SMLAL, SMLAL_2) {
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
TEST_F(TestCPUDataInstructions_SMLAL, SMLAL_ZERO_FLAG) {
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
TEST_F(TestCPUDataInstructions_SMLAL, SMLAL_NEGATIVE_FLAG) {
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
class TestCPUDataInstructions_SWP : public TestCPUDataInstructions {
protected:
    TestCPUDataInstructions_SWP() {}
    ~TestCPUDataInstructions_SWP() {}

    void SetUp() override { TestCPUDataInstructions::SetUp(); }
    void TearDown() override { TestCPUDataInstructions::TearDown(); }
};
/**
 * @brief Tests SWP (Swap Word) instruction.
 *
 * SWP Rd, Rm, [Rn]
 *
 * Rd = Mem[Rn]
 * Mem[Rn] = Rm
 */
TEST_F(TestCPUDataInstructions_SWP, SWP_WORD) {
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
TEST_F(TestCPUDataInstructions_SWP, SWPB_BYTE) {
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