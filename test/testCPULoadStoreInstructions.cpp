#include <gtest/gtest.h>

#include "../src/cpu.h"
#include "../src/interconnect.h"
#include "../src/memoryDefines.h"
#include "../src/utils/armEncode.h"
#include "commonTest.h"

/**
 * @brief Test fixture for the CPU's instructions in the load and store category.
 */
class TestCPULoadAndStoreInstructions : public testing::Test {
protected:
    Interconnect bus;
    ARM7TDMI arm7;
    TestCPULoadAndStoreInstructions() {}
    ~TestCPULoadAndStoreInstructions() {}

    void SetUp() override {
        bus.init();
        bus.bindARM7(&arm7);
    }

    void TearDown() override {}
};

class TestCPULoadAndStoreInstructions_STR : public TestCPULoadAndStoreInstructions {
protected:
    TestCPULoadAndStoreInstructions_STR() {}
    ~TestCPULoadAndStoreInstructions_STR() {}

    void SetUp() override { TestCPULoadAndStoreInstructions::SetUp(); }

    void TearDown() override { TestCPULoadAndStoreInstructions::TearDown(); }
};

TEST_F(TestCPULoadAndStoreInstructions_STR, STR_REGISTER) {
    writeProgramToMemory(
        "MOV R1, #0x02200000\n"  // Store the target address in R1
        "MOV R0, #0xFF\n"        // Store a random value in R0
        "STR R0, [R1]",          // Write the contents of R2 to the address pointed by R0
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(3);
    ASSERT_EQ(bus.read32ARM7(0x02200000), 0xFF);
}

TEST_F(TestCPULoadAndStoreInstructions_STR, STR_REGISTER_PLUS_IMM_OFFSET) {
    writeProgramToMemory(
        "MOV R1, #0x02200000\n"  // Store the target address in R1
        "MOV R0, #0xFF\n"        // Store a random value in R0
        "STR R0, [R1, #0x10]",   // Write the contents of R0 to the address pointed by R1 + 0x10
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(3);
    ASSERT_EQ(bus.read32ARM7(0x02200010), 0xFF);
    ASSERT_EQ(arm7.readReg(1), 0x02200000);
}

TEST_F(TestCPULoadAndStoreInstructions_STR, STR_REGISTER_PLUS_IMM_OFFSET_PREINDEXED) {
    writeProgramToMemory(
        "MOV R1, #0x02200000\n"  // Store the target address in R1
        "MOV R0, #0xFF\n"        // Store a random value in R0
        "STR R0, [R1, #0x10]!",  // Write the contents of R0 to the address pointed by R1 + 0x10
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(3);
    ASSERT_EQ(bus.read32ARM7(0x02200010), 0xFF);
    // R1 should now equal R1 + R2
    ASSERT_EQ(arm7.readReg(1), 0x02200010);
}

TEST_F(TestCPULoadAndStoreInstructions_STR, STR_REGISTER_PLUS_IMM_OFFSET_POSTINDEXED) {
    writeProgramToMemory(
        "MOV R1, #0x02200000\n"  // Store the target address in R1
        "MOV R0, #0xFF\n"        // Store a random value in R0
        "STR R0, [R1], #0x10",   // Write the contents of R0 to the address pointed by R1 + 0x10
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(3);
    ASSERT_EQ(bus.read32ARM7(0x02200000), 0xFF);
    // R1 should now equal R1 + R2
    ASSERT_EQ(arm7.readReg(1), 0x02200010);
}

TEST_F(TestCPULoadAndStoreInstructions_STR, STR_REGISTER_PLUS_REGISTER_OFFSET) {
    writeProgramToMemory(
        "MOV R1, #0x02200000\n"  // Store the target address in R1
        "MOV R0, #0xFF\n"        // Store a random value in R0
        "MOV R2, #0x10\n"        // Store a random value in R2
        "STR R0, [R1, R2]",      // Write the contents of R0 to the address pointed by R1 + R2
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(4);
    ASSERT_EQ(bus.read32ARM7(0x02200010), 0xFF);
    ASSERT_EQ(arm7.readReg(1), 0x02200000);
}

TEST_F(TestCPULoadAndStoreInstructions_STR, STR_REGISTER_PLUS_REGISTER_OFFSET_PREINDEXED) {
    writeProgramToMemory(
        "MOV R1, #0x02200000\n"  // Store the target address in R1
        "MOV R0, #0xFF\n"        // Store a random value in R0
        "MOV R2, #0x10\n"        // Store a random value in R2
        "STR R0, [R1, R2]!",     // Write the contents of R0 to the address pointed by R1 + R2
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(4);
    ASSERT_EQ(bus.read32ARM7(0x02200010), 0xFF);
    // R1 should now equal R1 + R2
    ASSERT_EQ(arm7.readReg(1), 0x02200010);
}

TEST_F(TestCPULoadAndStoreInstructions_STR, STR_REGISTER_PLUS_REGISTER_OFFSET_POSTINDEXED) {
    writeProgramToMemory(
        "MOV R1, #0x02200000\n"  // Store the target address in R1
        "MOV R0, #0xFF\n"        // Store a random value in R0
        "MOV R2, #0x10\n"        // Store a random value in R2
        "STR R0, [R1], R2",      // Write the contents of R0 to the address pointed by R1
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(4);
    ASSERT_EQ(bus.read32ARM7(0x02200000), 0xFF);
    // R1 should now equal R1 + R2
    ASSERT_EQ(arm7.readReg(1), 0x02200010);
}

class TestCPULoadAndStoreInstructions_LDR : public TestCPULoadAndStoreInstructions {
protected:
    TestCPULoadAndStoreInstructions_LDR() {}
    ~TestCPULoadAndStoreInstructions_LDR() {}

    void SetUp() override { TestCPULoadAndStoreInstructions::SetUp(); }

    void TearDown() override { TestCPULoadAndStoreInstructions::TearDown(); }
};

TEST_F(TestCPULoadAndStoreInstructions_LDR, LDR_REGISTER) {
    writeProgramToMemory(
        "MOV R1, #0x02200000\n"  // Store the target address in R1
        "MOV R2, #0xFF\n"        // Store a random value in R2
        "STR R2, [R1]\n"         // Write the contents of R2 to the address pointed by R1
        "LDR R0, [R1]\n",        // Read the data back into R0
        MAIN_RAM_START, &bus, true);
    arm7.setPC(MAIN_RAM_START);
    arm7.fetchAndExecute(4);
    ASSERT_EQ(arm7.readReg(0), 0xFF);
}
