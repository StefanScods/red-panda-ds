#include <gtest/gtest.h>

#include "../src/cpu.h"
#include "../src/interconnect.h"
#include "../src/memoryDefines.h"
#include "../src/utils/armEncode.h"
#include "commonTest.h"

/**
 * @brief Test fixture for the CPU's instructions in the load and store category.
 */
class TestCPU_THUMB_LoadAndStoreInstructions : public testing::Test {
protected:
    Interconnect bus;
    ARM7TDMI arm7;
    ARM946ES arm9;
    TestCPU_THUMB_LoadAndStoreInstructions() {}
    ~TestCPU_THUMB_LoadAndStoreInstructions() {}

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
// STR
// ==================================================================================================
class TestCPU_THUMB_LoadAndStoreInstructions_STR : public TestCPU_THUMB_LoadAndStoreInstructions {
protected:
    TestCPU_THUMB_LoadAndStoreInstructions_STR() {}
    ~TestCPU_THUMB_LoadAndStoreInstructions_STR() {}

    void SetUp() override { TestCPU_THUMB_LoadAndStoreInstructions::SetUp(); }
    void TearDown() override { TestCPU_THUMB_LoadAndStoreInstructions::TearDown(); }
};
/**
 * @brief Test STR with a immediate offset.
 */
TEST_F(TestCPU_THUMB_LoadAndStoreInstructions_STR, STR_IMMEDIATE_OFFSET) {
    uint32_t base = MAIN_RAM_START + 0x400;

    arm7.writeReg(0, 0xABCD1234);
    arm7.writeReg(1, base);
    writeProgramToMemory(
        ".thumb\n"
        "STR R0, [R1, #4]\n",
        MAIN_RAM_START, &bus, arm7.isARM7());

    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);

    EXPECT_EQ(bus.read32ARM7(base + 4), 0xABCD1234);
}
/**
 * @brief Test STR with a register offset.
 */
TEST_F(TestCPU_THUMB_LoadAndStoreInstructions_STR, STR_REGISTER_OFFSET) {
    uint32_t base = MAIN_RAM_START + 0x400;

    arm7.writeReg(0, 0xABCD1234);
    arm7.writeReg(1, base);
    arm7.writeReg(2, 0x4);
    writeProgramToMemory(
        ".thumb\n"
        "STR R0, [R1, R2]\n",
        MAIN_RAM_START, &bus, arm7.isARM7());

    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);

    EXPECT_EQ(bus.read32ARM7(base + 4), 0xABCD1234);
}
/**
 * @brief Test STR with a SP + offset as the target.
 */
TEST_F(TestCPU_THUMB_LoadAndStoreInstructions_STR, STR_SP_PLUS_OFFSET) {
    uint32_t base = MAIN_RAM_START + 0x400;

    arm7.writeReg(0, 0xABCD1234);
    arm7.writeReg(SP_REGISTER_NUM, base);
    writeProgramToMemory(
        ".thumb\n"
        "STR R0, [SP, #0x1FC]\n",
        MAIN_RAM_START, &bus, arm7.isARM7());

    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);

    EXPECT_EQ(bus.read32ARM7(base + 0x1FC), 0xABCD1234);
}
// ==================================================================================================
// STRH
// ==================================================================================================
class TestCPU_THUMB_LoadAndStoreInstructions_STRH : public TestCPU_THUMB_LoadAndStoreInstructions {
protected:
    TestCPU_THUMB_LoadAndStoreInstructions_STRH() {}
    ~TestCPU_THUMB_LoadAndStoreInstructions_STRH() {}

    void SetUp() override { TestCPU_THUMB_LoadAndStoreInstructions::SetUp(); }
    void TearDown() override { TestCPU_THUMB_LoadAndStoreInstructions::TearDown(); }
};
/**
 * @brief Test STRH with a immediate offset.
 */
TEST_F(TestCPU_THUMB_LoadAndStoreInstructions_STRH, STRH_IMMEDIATE_OFFSET) {
    uint32_t base = MAIN_RAM_START + 0x400;

    arm7.writeReg(0, 0xABCD1234);
    arm7.writeReg(1, base);
    writeProgramToMemory(
        ".thumb\n"
        "STRH R0, [R1, #4]\n",
        MAIN_RAM_START, &bus, arm7.isARM7());

    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);

    EXPECT_EQ(bus.read16ARM7(base + 4), 0x1234);
    EXPECT_EQ(bus.read32ARM7(base + 4), 0x00001234);
}
/**
 * @brief Test STRH with a register offset.
 */
TEST_F(TestCPU_THUMB_LoadAndStoreInstructions_STRH, STRH_REGISTER_OFFSET) {
    uint32_t base = MAIN_RAM_START + 0x400;

    arm7.writeReg(0, 0xABCD1234);
    arm7.writeReg(1, base);
    arm7.writeReg(2, 0x4);
    writeProgramToMemory(
        ".thumb\n"
        "STRH R0, [R1, R2]\n",
        MAIN_RAM_START, &bus, arm7.isARM7());

    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);

    EXPECT_EQ(bus.read16ARM7(base + 4), 0x1234);
    EXPECT_EQ(bus.read32ARM7(base + 4), 0x00001234);
}
// ==================================================================================================
// STRB
// ==================================================================================================
class TestCPU_THUMB_LoadAndStoreInstructions_STRB : public TestCPU_THUMB_LoadAndStoreInstructions {
protected:
    TestCPU_THUMB_LoadAndStoreInstructions_STRB() {}
    ~TestCPU_THUMB_LoadAndStoreInstructions_STRB() {}

    void SetUp() override { TestCPU_THUMB_LoadAndStoreInstructions::SetUp(); }
    void TearDown() override { TestCPU_THUMB_LoadAndStoreInstructions::TearDown(); }
};
/**
 * @brief Test STRB with a immediate offset.
 */
TEST_F(TestCPU_THUMB_LoadAndStoreInstructions_STRB, STRB_IMMEDIATE_OFFSET) {
    uint32_t base = MAIN_RAM_START + 0x400;

    arm7.writeReg(0, 0xABCD1234);
    arm7.writeReg(1, base);
    writeProgramToMemory(
        ".thumb\n"
        "STRB R0, [R1, #4]\n",
        MAIN_RAM_START, &bus, arm7.isARM7());

    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);

    EXPECT_EQ(bus.read16ARM7(base + 4), 0x34);
    EXPECT_EQ(bus.read32ARM7(base + 4), 0x00000034);
}
/**
 * @brief Test STRB with a register offset.
 */
TEST_F(TestCPU_THUMB_LoadAndStoreInstructions_STRB, STRB_REGISTER_OFFSET) {
    uint32_t base = MAIN_RAM_START + 0x400;

    arm7.writeReg(0, 0xABCD1234);
    arm7.writeReg(1, base);
    arm7.writeReg(2, 0x4);
    writeProgramToMemory(
        ".thumb\n"
        "STRB R0, [R1, R2]\n",
        MAIN_RAM_START, &bus, arm7.isARM7());

    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);

    EXPECT_EQ(bus.read16ARM7(base + 4), 0x0034);
    EXPECT_EQ(bus.read32ARM7(base + 4), 0x00000034);
}
// ==================================================================================================
// LDR
// ==================================================================================================
class TestCPU_THUMB_LoadAndStoreInstructions_LDR : public TestCPU_THUMB_LoadAndStoreInstructions {
protected:
    TestCPU_THUMB_LoadAndStoreInstructions_LDR() {}
    ~TestCPU_THUMB_LoadAndStoreInstructions_LDR() {}

    void SetUp() override { TestCPU_THUMB_LoadAndStoreInstructions::SetUp(); }
    void TearDown() override { TestCPU_THUMB_LoadAndStoreInstructions::TearDown(); }
};
/**
 * @brief Test LDR with a immediate offset.
 */
TEST_F(TestCPU_THUMB_LoadAndStoreInstructions_LDR, LDR_IMMEDIATE_OFFSET) {
    uint32_t base = MAIN_RAM_START + 0x400;

    bus.write32ARM7(base + 4, 0xABCD1234);

    arm7.writeReg(1, base);
    writeProgramToMemory(
        ".thumb\n"
        "LDR R0, [R1, #4]\n",
        MAIN_RAM_START, &bus, arm7.isARM7());

    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);

    EXPECT_EQ(arm7.readReg(0), 0xABCD1234);
}
/**
 * @brief Test LDR with a register offset.
 */
TEST_F(TestCPU_THUMB_LoadAndStoreInstructions_LDR, LDR_REGISTER_OFFSET) {
    uint32_t base = MAIN_RAM_START + 0x400;

    bus.write32ARM7(base + 4, 0xABCD1234);

    arm7.writeReg(1, base);
    arm7.writeReg(2, 0x4);
    writeProgramToMemory(
        ".thumb\n"
        "LDR R0, [R1, R2]\n",
        MAIN_RAM_START, &bus, arm7.isARM7());

    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);

    EXPECT_EQ(arm7.readReg(0), 0xABCD1234);
}
/**
 * @brief Test LDR with a SP + offset as the target.
 */
TEST_F(TestCPU_THUMB_LoadAndStoreInstructions_LDR, LDR_SP_PLUS_OFFSET) {
    uint32_t base = MAIN_RAM_START + 0x400;

    bus.write32ARM7(base + 0x1FC, 0xABCD1234);

    arm7.writeReg(SP_REGISTER_NUM, base);
    writeProgramToMemory(
        ".thumb\n"
        "LDR R0, [SP, #0x1FC]\n",
        MAIN_RAM_START, &bus, arm7.isARM7());

    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);

    EXPECT_EQ(arm7.readReg(0), 0xABCD1234);
}

// ==================================================================================================
// LDRH
// ==================================================================================================
class TestCPU_THUMB_LoadAndStoreInstructions_LDRH : public TestCPU_THUMB_LoadAndStoreInstructions {
protected:
    TestCPU_THUMB_LoadAndStoreInstructions_LDRH() {}
    ~TestCPU_THUMB_LoadAndStoreInstructions_LDRH() {}

    void SetUp() override { TestCPU_THUMB_LoadAndStoreInstructions::SetUp(); }
    void TearDown() override { TestCPU_THUMB_LoadAndStoreInstructions::TearDown(); }
};
/**
 * @brief Test LDRH with a immediate offset.
 */
TEST_F(TestCPU_THUMB_LoadAndStoreInstructions_LDRH, LDRH_IMMEDIATE_OFFSET) {
    uint32_t base = MAIN_RAM_START + 0x400;

    bus.write32ARM7(base + 4, 0xABCD1234);

    arm7.writeReg(1, base);
    writeProgramToMemory(
        ".thumb\n"
        "LDRH R0, [R1, #4]\n",
        MAIN_RAM_START, &bus, arm7.isARM7());

    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);

    EXPECT_EQ(arm7.readReg(0), 0x00001234);
}
/**
 * @brief Test LDRH with a register offset.
 */
TEST_F(TestCPU_THUMB_LoadAndStoreInstructions_LDRH, LDRH_REGISTER_OFFSET) {
    uint32_t base = MAIN_RAM_START + 0x400;

    bus.write32ARM7(base + 4, 0xABCD1234);

    arm7.writeReg(1, base);
    arm7.writeReg(2, 0x4);
    writeProgramToMemory(
        ".thumb\n"
        "LDRH R0, [R1, R2]\n",
        MAIN_RAM_START, &bus, arm7.isARM7());

    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);

    EXPECT_EQ(arm7.readReg(0), 0x00001234);
}

// ==================================================================================================
// LDRB
// ==================================================================================================
class TestCPU_THUMB_LoadAndStoreInstructions_LDRB : public TestCPU_THUMB_LoadAndStoreInstructions {
protected:
    TestCPU_THUMB_LoadAndStoreInstructions_LDRB() {}
    ~TestCPU_THUMB_LoadAndStoreInstructions_LDRB() {}

    void SetUp() override { TestCPU_THUMB_LoadAndStoreInstructions::SetUp(); }
    void TearDown() override { TestCPU_THUMB_LoadAndStoreInstructions::TearDown(); }
};
/**
 * @brief Test LDRB with a immediate offset.
 */
TEST_F(TestCPU_THUMB_LoadAndStoreInstructions_LDRB, LDRB_IMMEDIATE_OFFSET) {
    uint32_t base = MAIN_RAM_START + 0x400;

    bus.write32ARM7(base + 4, 0xABCD1234);

    arm7.writeReg(1, base);
    writeProgramToMemory(
        ".thumb\n"
        "LDRB R0, [R1, #4]\n",
        MAIN_RAM_START, &bus, arm7.isARM7());

    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);

    EXPECT_EQ(arm7.readReg(0), 0x00000034);
}
/**
 * @brief Test LDRB with a register offset.
 */
TEST_F(TestCPU_THUMB_LoadAndStoreInstructions_LDRB, LDRB_REGISTER_OFFSET) {
    uint32_t base = MAIN_RAM_START + 0x400;

    bus.write32ARM7(base + 4, 0xABCD1234);

    arm7.writeReg(1, base);
    arm7.writeReg(2, 0x4);
    writeProgramToMemory(
        ".thumb\n"
        "LDRB R0, [R1, R2]\n",
        MAIN_RAM_START, &bus, arm7.isARM7());

    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);

    EXPECT_EQ(arm7.readReg(0), 0x00000034);
}
// ==================================================================================================
// LDRSB
// ==================================================================================================
class TestCPU_THUMB_LoadAndStoreInstructions_LDRSB : public TestCPU_THUMB_LoadAndStoreInstructions {
protected:
    TestCPU_THUMB_LoadAndStoreInstructions_LDRSB() {}
    ~TestCPU_THUMB_LoadAndStoreInstructions_LDRSB() {}

    void SetUp() override { TestCPU_THUMB_LoadAndStoreInstructions::SetUp(); }
    void TearDown() override { TestCPU_THUMB_LoadAndStoreInstructions::TearDown(); }
};
/**
 * @brief Test LDRSB with a register offset.
 */
TEST_F(TestCPU_THUMB_LoadAndStoreInstructions_LDRSB, LDRSB_REGISTER_OFFSET_POSITIVE) {
    uint32_t base = MAIN_RAM_START + 0x400;

    bus.write8ARM7(base + 4, 0x7F);

    arm7.writeReg(1, base);
    arm7.writeReg(2, 0x4);
    writeProgramToMemory(
        ".thumb\n"
        "LDRSB R0, [R1, R2]\n",
        MAIN_RAM_START, &bus, arm7.isARM7());

    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);

    EXPECT_EQ(arm7.readReg(0), 0x0000007F);
}
/**
 * @brief Test LDRSB sign extension.
 */
TEST_F(TestCPU_THUMB_LoadAndStoreInstructions_LDRSB, LDRSB_REGISTER_OFFSET_NEGATIVE) {
    uint32_t base = MAIN_RAM_START + 0x400;

    bus.write8ARM7(base + 4, 0x80);

    arm7.writeReg(1, base);
    arm7.writeReg(2, 0x4);
    writeProgramToMemory(
        ".thumb\n"
        "LDRSB R0, [R1, R2]\n",
        MAIN_RAM_START, &bus, arm7.isARM7());

    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);

    EXPECT_EQ(arm7.readReg(0), 0xFFFFFF80);
}
// ==================================================================================================
// LDRSH
// ==================================================================================================
class TestCPU_THUMB_LoadAndStoreInstructions_LDRSH : public TestCPU_THUMB_LoadAndStoreInstructions {
protected:
    TestCPU_THUMB_LoadAndStoreInstructions_LDRSH() {}
    ~TestCPU_THUMB_LoadAndStoreInstructions_LDRSH() {}

    void SetUp() override { TestCPU_THUMB_LoadAndStoreInstructions::SetUp(); }
    void TearDown() override { TestCPU_THUMB_LoadAndStoreInstructions::TearDown(); }
};
/**
 * @brief Test LDRSH with a register offset.
 */
TEST_F(TestCPU_THUMB_LoadAndStoreInstructions_LDRSH, LDRSH_REGISTER_OFFSET_POSITIVE) {
    uint32_t base = MAIN_RAM_START + 0x400;

    bus.write16ARM7(base + 4, 0x7FFF);

    arm7.writeReg(1, base);
    arm7.writeReg(2, 0x4);
    writeProgramToMemory(
        ".thumb\n"
        "LDRSH R0, [R1, R2]\n",
        MAIN_RAM_START, &bus, arm7.isARM7());

    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);

    EXPECT_EQ(arm7.readReg(0), 0x00007FFF);
}
/**
 * @brief Test LDRSH sign extension.
 */
TEST_F(TestCPU_THUMB_LoadAndStoreInstructions_LDRSH, LDRSH_REGISTER_OFFSET_NEGATIVE) {
    uint32_t base = MAIN_RAM_START + 0x400;

    bus.write16ARM7(base + 4, 0x8000);

    arm7.writeReg(1, base);
    arm7.writeReg(2, 0x4);
    writeProgramToMemory(
        ".thumb\n"
        "LDRSH R0, [R1, R2]\n",
        MAIN_RAM_START, &bus, arm7.isARM7());

    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(1);

    EXPECT_EQ(arm7.readReg(0), 0xFFFF8000);
}