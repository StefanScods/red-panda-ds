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
    ARM946ES arm9;
    TestCPULoadAndStoreInstructions() {}
    ~TestCPULoadAndStoreInstructions() {}

    void SetUp() override {
        bus.init();
        bus.bindARM7(&arm7);
        bus.bindARM9(&arm9);
    }

    void TearDown() override {}
};

// ==================================================================================================
// STR
// ==================================================================================================
class TestCPULoadAndStoreInstructions_STR : public TestCPULoadAndStoreInstructions {
protected:
    TestCPULoadAndStoreInstructions_STR() {}
    ~TestCPULoadAndStoreInstructions_STR() {}

    void SetUp() override { TestCPULoadAndStoreInstructions::SetUp(); }

    void TearDown() override { TestCPULoadAndStoreInstructions::TearDown(); }
};
/**
 * @brief Tests the basic STR instruction targeting a register.
 */
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
/**
 * @brief Tests the STR instruction plus immediate offset targeting a register.
 */
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
/**
 * @brief Tests the STR instruction plus immediate offset preindexed targeting a register.
 */
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
/**
 * @brief Tests the STR instruction plus immediate offset postindexed targeting a register.
 */
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
/**
 * @brief Tests the STR instruction plus register offset targeting a register.
 */
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
/**
 * @brief Tests the STR instruction plus register offset preindexed targeting a register.
 */
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
/**
 * @brief Tests the STR instruction plus register offset postindexed targeting a register.
 */
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
// ==================================================================================================
// LDR
// ==================================================================================================
class TestCPULoadAndStoreInstructions_LDR : public TestCPULoadAndStoreInstructions {
protected:
    TestCPULoadAndStoreInstructions_LDR() {}
    ~TestCPULoadAndStoreInstructions_LDR() {}

    void SetUp() override { TestCPULoadAndStoreInstructions::SetUp(); }

    void TearDown() override { TestCPULoadAndStoreInstructions::TearDown(); }
};
/**
 * @brief Tests the basic LDR instruction targeting a register.
 */
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

// ==================================================================================================
// PUSH
// ==================================================================================================
class TestCPULoadAndStoreInstructions_PUSH : public TestCPULoadAndStoreInstructions {
protected:
    TestCPULoadAndStoreInstructions_PUSH() {}
    ~TestCPULoadAndStoreInstructions_PUSH() {}

    void SetUp() override { TestCPULoadAndStoreInstructions::SetUp(); }

    void TearDown() override { TestCPULoadAndStoreInstructions::TearDown(); }
};
/**
 * @brief Tests PUSH on one register.
 */
TEST_F(TestCPULoadAndStoreInstructions_PUSH, PUSH_ONE) {
    uint32_t SPLocation = MAIN_RAM_START + 0xF0;
    // Set a default value to the regs.
    arm7.reset();
    arm7.writeReg(0, 0xFFFFFFFF);
    arm7.writeReg(SP_REGISTER_NUM, SPLocation);
    arm7.setPC(MAIN_RAM_START);
    // Execute the test code.
    writeProgramToMemory("PUSH {R0}\n", MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.fetchAndExecute();
    // Assert the value has been pushed and SP has moved.
    ASSERT_EQ(bus.read32ARM7(SPLocation - 4), 0xFFFFFFFF);
    ASSERT_EQ(arm7.readReg(SP_REGISTER_NUM), SPLocation - 4);
}
/**
 * @brief Tests PUSH on two registers.
 */
TEST_F(TestCPULoadAndStoreInstructions_PUSH, PUSH_TWO) {
    uint32_t SPLocation = MAIN_RAM_START + 0xF0;
    // Set a default value to the regs.
    arm7.reset();
    arm7.writeReg(0, 0xFFFFFFFF);
    arm7.writeReg(1, 0xFFFFFF00);
    arm7.writeReg(SP_REGISTER_NUM, SPLocation);
    arm7.setPC(MAIN_RAM_START);
    // Execute the test code.
    writeProgramToMemory("PUSH {R0, R1}\n", MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.fetchAndExecute();
    // Assert the value has been pushed and SP has moved.
    ASSERT_EQ(bus.read32ARM7(SPLocation - 4), 0xFFFFFF00);
    ASSERT_EQ(bus.read32ARM7(SPLocation - 8), 0xFFFFFFFF);
    ASSERT_EQ(arm7.readReg(SP_REGISTER_NUM), SPLocation - 8);
}
/**
 * @brief Tests PUSH on all registers.
 */
TEST_F(TestCPULoadAndStoreInstructions_PUSH, PUSH_ALL) {
    uint32_t SPLocation = MAIN_RAM_START + 0xF0;
    std::vector<uint32_t> values = {1, 2,  3,  4,  5,  6,          7,  8,
                                    9, 10, 11, 12, 13, SPLocation, 15, 0x02000008};
    arm7.reset();
    arm7.writeReg(SP_REGISTER_NUM, SPLocation);
    arm7.setPC(MAIN_RAM_START);
    // Load values into registers
    for (uint32_t i = 0; i < PC_REGISTER_NUM; i++) {
        arm7.writeReg(i, values[i]);
    }
    // Execute the test code.
    writeProgramToMemory("PUSH {R0-R15}\n", MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.fetchAndExecute();
    // Assert the stack and SP are correct.
    for (size_t i = 0; i <= PC_REGISTER_NUM; i++) {
        uint32_t memValue = bus.read32ARM7(SPLocation - ((PC_REGISTER_NUM - i + 1) * 4));
        ASSERT_EQ(memValue, values[i]);
    }
    ASSERT_EQ(arm7.readReg(SP_REGISTER_NUM), SPLocation - 16 * 4);
}
// ==================================================================================================
// POP
// ==================================================================================================
class TestCPULoadAndStoreInstructions_POP : public TestCPULoadAndStoreInstructions {
protected:
    TestCPULoadAndStoreInstructions_POP() {}
    ~TestCPULoadAndStoreInstructions_POP() {}

    void SetUp() override { TestCPULoadAndStoreInstructions::SetUp(); }

    void TearDown() override { TestCPULoadAndStoreInstructions::TearDown(); }
};
/**
 * @brief Tests POP on one register.
 */
TEST_F(TestCPULoadAndStoreInstructions_POP, POP_ONE) {
    uint32_t SPLocation = MAIN_RAM_START + 0xF0;
    // Set a default value to the regs.
    arm7.reset();
    arm7.writeReg(0, 0);
    arm7.writeReg(SP_REGISTER_NUM, SPLocation);
    bus.write32ARM7(SPLocation, 0xFFFFFF00);
    arm7.setPC(MAIN_RAM_START);
    // Execute the test code.
    writeProgramToMemory("POP {R0}\n", MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.fetchAndExecute();
    // Assert the value has been popped and SP has moved.
    ASSERT_EQ(arm7.readReg(0), 0xFFFFFF00);
    ASSERT_EQ(arm7.readReg(SP_REGISTER_NUM), SPLocation + 4);
}
/**
 * @brief Tests POP on two registers.
 */
TEST_F(TestCPULoadAndStoreInstructions_POP, POP_TWO) {
    uint32_t SPLocation = MAIN_RAM_START + 0xF0;
    // Set a default value to the regs.
    arm7.reset();
    arm7.writeReg(0, 0);
    arm7.writeReg(1, 0);
    arm7.writeReg(SP_REGISTER_NUM, SPLocation);
    bus.write32ARM7(SPLocation, 0xFFFFFF00);
    bus.write32ARM7(SPLocation + 4, 0xFFFFFFFF);
    arm7.setPC(MAIN_RAM_START);
    // Execute the test code.
    writeProgramToMemory("POP {R0, R1}\n", MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.fetchAndExecute();
    // Assert the value has been popped and SP has moved.
    ASSERT_EQ(arm7.readReg(0), 0xFFFFFF00);
    ASSERT_EQ(arm7.readReg(1), 0xFFFFFFFF);
    ASSERT_EQ(arm7.readReg(SP_REGISTER_NUM), SPLocation + 8);
}
/**
 * @brief Tests POP on two registers.
 */
TEST_F(TestCPULoadAndStoreInstructions_POP, PUSH_POP_TWO) {
    uint32_t r0Value = 0xFF000000;
    uint32_t r1Value = 0x000000FF;
    uint32_t SPLocation = MAIN_RAM_START + 0xF0;
    // Set a default value to the regs.
    arm7.reset();
    arm7.writeReg(SP_REGISTER_NUM, SPLocation);
    arm7.setPC(MAIN_RAM_START);

    // Execute the test code.
    writeProgramToMemory("MOV R0, #" + std::to_string(r0Value) + "\nMOV R1, #" +
                             std::to_string(r1Value) +
                             "\nPUSH {R0, R1}\n"
                             "MOV R0, #0\n"
                             "MOV R1, #0\n"
                             "POP {R0, R1}\n",
                         MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.fetchAndExecute(6);
    // Assert the value has been pushed and popped correctly.
    ASSERT_EQ(arm7.readReg(0), r0Value);
    ASSERT_EQ(arm7.readReg(1), r1Value);
    ASSERT_EQ(arm7.readReg(SP_REGISTER_NUM), SPLocation);
}
/**
 * @brief Tests POP on all registers.
 */
TEST_F(TestCPULoadAndStoreInstructions_POP, POP_ALL) {
    uint32_t SPLocation = MAIN_RAM_START + 0xF0;
    std::vector<uint32_t> values = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 0x02000F00};
    arm7.reset();
    arm7.writeReg(SP_REGISTER_NUM, SPLocation);
    arm7.setPC(MAIN_RAM_START);
    // Write values to memory to simulate stack
    for (size_t i = 0; i < 16; i++) bus.write32ARM7(SPLocation + i * 4, values[i]);
    // Execute the test code.
    writeProgramToMemory("POP {R0-R15}\n", MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.fetchAndExecute();
    // Assert each register has the expected value
    for (uint32_t i = 0; i < PC_REGISTER_NUM; i++) {
        if (i == SP_REGISTER_NUM) continue;
        ASSERT_EQ(arm7.readReg(i), values[i]);
    }
    ASSERT_EQ(arm7.readReg(PC_REGISTER_NUM), 0x02000F04);
    ASSERT_EQ(arm7.readReg(SP_REGISTER_NUM), SPLocation + 16 * 4);
}