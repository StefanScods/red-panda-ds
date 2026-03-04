#include <gtest/gtest.h>

#include "../src/cpu.h"
#include "../src/interconnect.h"
#include "../src/memoryDefines.h"
#include "../src/utils/armEncode.h"
#include "commonTest.h"

/**
 * @brief Test fixture for the CPU's instructions in the load and store category.
 */
class TestCPU_ARM_LoadAndStoreInstructions : public testing::Test {
protected:
    Interconnect bus;
    ARM7TDMI arm7;
    ARM946ES arm9;
    TestCPU_ARM_LoadAndStoreInstructions() {}
    ~TestCPU_ARM_LoadAndStoreInstructions() {}

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
class TestCPU_ARM_LoadAndStoreInstructions_STR : public TestCPU_ARM_LoadAndStoreInstructions {
protected:
    TestCPU_ARM_LoadAndStoreInstructions_STR() {}
    ~TestCPU_ARM_LoadAndStoreInstructions_STR() {}

    void SetUp() override { TestCPU_ARM_LoadAndStoreInstructions::SetUp(); }

    void TearDown() override { TestCPU_ARM_LoadAndStoreInstructions::TearDown(); }
};
/**
 * @brief Tests the basic STR instruction targeting a register.
 */
TEST_F(TestCPU_ARM_LoadAndStoreInstructions_STR, STR_REGISTER) {
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
TEST_F(TestCPU_ARM_LoadAndStoreInstructions_STR, STR_REGISTER_PLUS_IMM_OFFSET) {
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
TEST_F(TestCPU_ARM_LoadAndStoreInstructions_STR, STR_REGISTER_PLUS_IMM_OFFSET_PREINDEXED) {
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
TEST_F(TestCPU_ARM_LoadAndStoreInstructions_STR, STR_REGISTER_PLUS_IMM_OFFSET_POSTINDEXED) {
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
TEST_F(TestCPU_ARM_LoadAndStoreInstructions_STR, STR_REGISTER_PLUS_REGISTER_OFFSET) {
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
TEST_F(TestCPU_ARM_LoadAndStoreInstructions_STR, STR_REGISTER_PLUS_REGISTER_OFFSET_PREINDEXED) {
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
TEST_F(TestCPU_ARM_LoadAndStoreInstructions_STR, STR_REGISTER_PLUS_REGISTER_OFFSET_POSTINDEXED) {
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
class TestCPU_ARM_LoadAndStoreInstructions_LDR : public TestCPU_ARM_LoadAndStoreInstructions {
protected:
    TestCPU_ARM_LoadAndStoreInstructions_LDR() {}
    ~TestCPU_ARM_LoadAndStoreInstructions_LDR() {}

    void SetUp() override { TestCPU_ARM_LoadAndStoreInstructions::SetUp(); }

    void TearDown() override { TestCPU_ARM_LoadAndStoreInstructions::TearDown(); }
};
/**
 * @brief Tests the basic LDR instruction targeting a register.
 */
TEST_F(TestCPU_ARM_LoadAndStoreInstructions_LDR, LDR_REGISTER) {
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
class TestCPU_ARM_LoadAndStoreInstructions_PUSH : public TestCPU_ARM_LoadAndStoreInstructions {
protected:
    TestCPU_ARM_LoadAndStoreInstructions_PUSH() {}
    ~TestCPU_ARM_LoadAndStoreInstructions_PUSH() {}

    void SetUp() override { TestCPU_ARM_LoadAndStoreInstructions::SetUp(); }

    void TearDown() override { TestCPU_ARM_LoadAndStoreInstructions::TearDown(); }
};
/**
 * @brief Tests PUSH on one register.
 */
TEST_F(TestCPU_ARM_LoadAndStoreInstructions_PUSH, PUSH_ONE) {
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
TEST_F(TestCPU_ARM_LoadAndStoreInstructions_PUSH, PUSH_TWO) {
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
TEST_F(TestCPU_ARM_LoadAndStoreInstructions_PUSH, PUSH_ALL) {
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
class TestCPU_ARM_LoadAndStoreInstructions_POP : public TestCPU_ARM_LoadAndStoreInstructions {
protected:
    TestCPU_ARM_LoadAndStoreInstructions_POP() {}
    ~TestCPU_ARM_LoadAndStoreInstructions_POP() {}

    void SetUp() override { TestCPU_ARM_LoadAndStoreInstructions::SetUp(); }

    void TearDown() override { TestCPU_ARM_LoadAndStoreInstructions::TearDown(); }
};
/**
 * @brief Tests POP on one register.
 */
TEST_F(TestCPU_ARM_LoadAndStoreInstructions_POP, POP_ONE) {
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
TEST_F(TestCPU_ARM_LoadAndStoreInstructions_POP, POP_TWO) {
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
TEST_F(TestCPU_ARM_LoadAndStoreInstructions_POP, PUSH_POP_TWO) {
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
TEST_F(TestCPU_ARM_LoadAndStoreInstructions_POP, POP_ALL) {
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
// ==================================================================================================
// STM Tests
// ==================================================================================================
class TestCPU_ARM_LoadAndStoreInstructions_STM : public TestCPU_ARM_LoadAndStoreInstructions {
protected:
    TestCPU_ARM_LoadAndStoreInstructions_STM() {}
    ~TestCPU_ARM_LoadAndStoreInstructions_STM() {}

    void SetUp() override { TestCPU_ARM_LoadAndStoreInstructions::SetUp(); }
    void TearDown() override { TestCPU_ARM_LoadAndStoreInstructions::TearDown(); }
};
/**
 * @brief Tests STMDB on multiple registers.
 */
TEST_F(TestCPU_ARM_LoadAndStoreInstructions_STM, STMDB_MULTI) {
    uint32_t base = MAIN_RAM_START + 0x200;
    std::vector<uint32_t> values = {0x11111111, 0x22222222, 0x33333333, 0x44444444};

    // Set a default value to the regs.
    arm7.reset();
    for (size_t i = 0; i < values.size(); i++) arm7.writeReg(i, values[i]);
    arm7.writeReg(8, base);  // R8 as base
    arm7.setPC(MAIN_RAM_START);

    // Execute the test code.
    writeProgramToMemory("STMDB R8!, {R0-R3}\n", MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.fetchAndExecute();

    // Assert memory contents in descending order
    for (size_t i = 0; i < values.size(); i++)
        ASSERT_EQ(bus.read32ARM7(base - (i + 1) * 4), values[values.size() - i - 1]);

    // Assert base register updated
    ASSERT_EQ(arm7.readReg(8), base - 16);
}
/**
 * @brief Tests STMDA on multiple registers.
 */
TEST_F(TestCPU_ARM_LoadAndStoreInstructions_STM, STMDA_MULTI) {
    uint32_t base = MAIN_RAM_START + 0x300;
    std::vector<uint32_t> values = {0x55555555, 0x66666666, 0x77777777, 0x88888888};

    // Set a default value to the regs.
    arm7.reset();
    for (size_t i = 0; i < values.size(); i++) arm7.writeReg(i, values[i]);
    arm7.writeReg(8, base);  // R8 as base
    arm7.setPC(MAIN_RAM_START);

    // Execute the test code.
    writeProgramToMemory("STMDA R8!, {R0-R3}\n", MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.fetchAndExecute();

    // Assert memory contents in descending order
    for (size_t i = 0; i < values.size(); i++)
        ASSERT_EQ(bus.read32ARM7(base - (i * 4)), values[values.size() - i - 1]);

    // Assert base register updated
    ASSERT_EQ(arm7.readReg(8), base - 16);
}
/**
 * @brief Tests STMIB on multiple registers.
 */
TEST_F(TestCPU_ARM_LoadAndStoreInstructions_STM, STMIB_MULTI) {
    uint32_t base = MAIN_RAM_START + 0x400;
    std::vector<uint32_t> values = {0x99999999, 0xAAAAAAAA, 0xBBBBBBBB, 0xCCCCCCCC};

    // Set a default value to the regs.
    arm7.reset();
    for (size_t i = 0; i < values.size(); i++) arm7.writeReg(i, values[i]);
    arm7.writeReg(8, base);  // R8 as base
    arm7.setPC(MAIN_RAM_START);

    // Execute the test code.
    writeProgramToMemory("STMIB R8!, {R0-R3}\n", MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.fetchAndExecute();

    // Assert memory contents in ascending order
    for (size_t i = 0; i < values.size(); i++)
        ASSERT_EQ(bus.read32ARM7(base + (i + 1) * 4), values[i]);

    // Assert base register updated
    ASSERT_EQ(arm7.readReg(8), base + 16);
}
/**
 * @brief Tests STMIA on multiple registers.
 */
TEST_F(TestCPU_ARM_LoadAndStoreInstructions_STM, STMIA_MULTI) {
    uint32_t base = MAIN_RAM_START + 0x500;
    std::vector<uint32_t> values = {0xDDDDDDDD, 0xEEEEEEEE, 0xFFFFFFFF, 0x00000000};

    // Set a default value to the regs.
    arm7.reset();
    for (size_t i = 0; i < values.size(); i++) arm7.writeReg(i, values[i]);
    arm7.writeReg(8, base);  // R8 as base
    arm7.setPC(MAIN_RAM_START);

    // Execute the test code.
    writeProgramToMemory("STMIA R8!, {R0-R3}\n", MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.fetchAndExecute();

    // Assert memory contents in ascending order
    for (size_t i = 0; i < values.size(); i++) ASSERT_EQ(bus.read32ARM7(base + i * 4), values[i]);

    // Assert base register updated
    ASSERT_EQ(arm7.readReg(8), base + 16);
}
/**
 * @brief Tests STMDB on multiple registers with no write back.
 */
TEST_F(TestCPU_ARM_LoadAndStoreInstructions_STM, STMDB_MULTI_NO_WRITE_BACK) {
    uint32_t base = MAIN_RAM_START + 0x200;
    std::vector<uint32_t> values = {0x11111111, 0x22222222, 0x33333333, 0x44444444};

    // Set a default value to the regs.
    arm7.reset();
    for (size_t i = 0; i < values.size(); i++) arm7.writeReg(i, values[i]);
    arm7.writeReg(8, base);  // R8 as base
    arm7.setPC(MAIN_RAM_START);

    // Execute the test code.
    writeProgramToMemory("STMDB R8, {R0-R3}\n", MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.fetchAndExecute();

    // Assert memory contents in descending order
    for (size_t i = 0; i < values.size(); i++)
        ASSERT_EQ(bus.read32ARM7(base - (i + 1) * 4), values[values.size() - i - 1]);

    // Assert base register updated
    ASSERT_EQ(arm7.readReg(8), base);
}
/**
 * @brief Tests STMDA on multiple registers with no write back.
 */
TEST_F(TestCPU_ARM_LoadAndStoreInstructions_STM, STMDA_MULTI_NO_WRITE_BACK) {
    uint32_t base = MAIN_RAM_START + 0x300;
    std::vector<uint32_t> values = {0x55555555, 0x66666666, 0x77777777, 0x88888888};

    // Set a default value to the regs.
    arm7.reset();
    for (size_t i = 0; i < values.size(); i++) arm7.writeReg(i, values[i]);
    arm7.writeReg(8, base);  // R8 as base
    arm7.setPC(MAIN_RAM_START);

    // Execute the test code.
    writeProgramToMemory("STMDA R8, {R0-R3}\n", MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.fetchAndExecute();

    // Assert memory contents in descending order
    for (size_t i = 0; i < values.size(); i++)
        ASSERT_EQ(bus.read32ARM7(base - (i * 4)), values[values.size() - i - 1]);

    // Assert base register updated
    ASSERT_EQ(arm7.readReg(8), base);
}
/**
 * @brief Tests STMIB on multiple registers with no write back.
 */
TEST_F(TestCPU_ARM_LoadAndStoreInstructions_STM, STMIB_MULTI_NO_WRITE_BACK) {
    uint32_t base = MAIN_RAM_START + 0x400;
    std::vector<uint32_t> values = {0x99999999, 0xAAAAAAAA, 0xBBBBBBBB, 0xCCCCCCCC};

    // Set a default value to the regs.
    arm7.reset();
    for (size_t i = 0; i < values.size(); i++) arm7.writeReg(i, values[i]);
    arm7.writeReg(8, base);  // R8 as base
    arm7.setPC(MAIN_RAM_START);

    // Execute the test code.
    writeProgramToMemory("STMIB R8, {R0-R3}\n", MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.fetchAndExecute();

    // Assert memory contents in ascending order
    for (size_t i = 0; i < values.size(); i++)
        ASSERT_EQ(bus.read32ARM7(base + (i + 1) * 4), values[i]);

    // Assert base register updated
    ASSERT_EQ(arm7.readReg(8), base);
}
/**
 * @brief Tests STMIA on multiple registers with no write back.
 */
TEST_F(TestCPU_ARM_LoadAndStoreInstructions_STM, STMIA_MULTI_NO_WRITE_BACK) {
    uint32_t base = MAIN_RAM_START + 0x500;
    std::vector<uint32_t> values = {0xDDDDDDDD, 0xEEEEEEEE, 0xFFFFFFFF, 0x00000000};

    // Set a default value to the regs.
    arm7.reset();
    for (size_t i = 0; i < values.size(); i++) arm7.writeReg(i, values[i]);
    arm7.writeReg(8, base);  // R8 as base
    arm7.setPC(MAIN_RAM_START);

    // Execute the test code.
    writeProgramToMemory("STMIA R8, {R0-R3}\n", MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.fetchAndExecute();

    // Assert memory contents in ascending order
    for (size_t i = 0; i < values.size(); i++) ASSERT_EQ(bus.read32ARM7(base + i * 4), values[i]);

    // Assert base register updated
    ASSERT_EQ(arm7.readReg(8), base);
}
// ==================================================================================================
// LDM
// ==================================================================================================
class TestCPU_ARM_LoadAndStoreInstructions_LDM : public TestCPU_ARM_LoadAndStoreInstructions {
protected:
    TestCPU_ARM_LoadAndStoreInstructions_LDM() {}
    ~TestCPU_ARM_LoadAndStoreInstructions_LDM() {}

    void SetUp() override { TestCPU_ARM_LoadAndStoreInstructions::SetUp(); }
    void TearDown() override { TestCPU_ARM_LoadAndStoreInstructions::TearDown(); }
};
/**
 * @brief Tests LDMDB on multiple registers.
 */
TEST_F(TestCPU_ARM_LoadAndStoreInstructions_LDM, LDMDB_MULTI) {
    uint32_t base = MAIN_RAM_START + 0x600;
    std::vector<uint32_t> values = {0xAAAA0001, 0xBBBB0002, 0xCCCC0003, 0xDDDD0004};

    // Set a default value to the regs.
    arm7.reset();
    arm7.writeReg(8, base);  // R8 as base
    arm7.setPC(MAIN_RAM_START);

    // Write values to memory in descending order
    for (size_t i = 0; i < values.size(); i++) bus.write32ARM7(base - (i + 1) * 4, values[i]);

    // Execute the test code.
    writeProgramToMemory("LDMDB R8!, {R0-R3}\n", MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.fetchAndExecute();

    // Assert registers loaded correctly
    for (size_t i = 0; i < values.size(); i++)
        ASSERT_EQ(arm7.readReg(i), values[values.size() - i - 1]);

    // Assert base register updated
    ASSERT_EQ(arm7.readReg(8), base - 16);
}
/**
 * @brief Tests LDMDA on multiple registers.
 */
TEST_F(TestCPU_ARM_LoadAndStoreInstructions_LDM, LDMDA_MULTI) {
    uint32_t base = MAIN_RAM_START + 0x700;
    std::vector<uint32_t> values = {0x11110001, 0x22220002, 0x33330003, 0x44440004};

    // Set a default value to the regs.
    arm7.reset();
    arm7.writeReg(8, base);  // R8 as base
    arm7.setPC(MAIN_RAM_START);

    // Write values to memory in descending order
    for (size_t i = 0; i < values.size(); i++) bus.write32ARM7(base - (i * 4), values[i]);

    // Execute the test code.
    writeProgramToMemory("LDMDA R8!, {R0-R3}\n", MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.fetchAndExecute();

    // Assert registers loaded correctly
    for (size_t i = 0; i < values.size(); i++)
        ASSERT_EQ(arm7.readReg(i), values[values.size() - i - 1]);

    // Assert base register updated
    ASSERT_EQ(arm7.readReg(8), base - 16);
}
/**
 * @brief Tests LDMIB on multiple registers.
 */
TEST_F(TestCPU_ARM_LoadAndStoreInstructions_LDM, LDMIB_MULTI) {
    uint32_t base = MAIN_RAM_START + 0x800;
    std::vector<uint32_t> values = {0x99990001, 0xAAAA0002, 0xBBBB0003, 0xCCCC0004};

    // Set a default value to the regs.
    arm7.reset();
    arm7.writeReg(8, base);  // R8 as base
    arm7.setPC(MAIN_RAM_START);

    // Write values to memory in ascending order
    for (size_t i = 0; i < values.size(); i++) bus.write32ARM7(base + ((i + 1) * 4), values[i]);

    // Execute the test code.
    writeProgramToMemory("LDMIB R8!, {R0-R3}\n", MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.fetchAndExecute();

    // Assert registers loaded correctly
    for (size_t i = 0; i < values.size(); i++) ASSERT_EQ(arm7.readReg(i), values[i]);

    // Assert base register updated
    ASSERT_EQ(arm7.readReg(8), base + 16);
}
/**
 * @brief Tests LDMIA on multiple registers.
 */
TEST_F(TestCPU_ARM_LoadAndStoreInstructions_LDM, LDMIA_MULTI) {
    uint32_t base = MAIN_RAM_START + 0x900;
    std::vector<uint32_t> values = {0xDDDD0001, 0xEEEE0002, 0xFFFF0003, 0x00010004};

    // Set a default value to the regs.
    arm7.reset();
    arm7.writeReg(8, base);  // R8 as base
    arm7.setPC(MAIN_RAM_START);

    // Write values to memory in ascending order
    for (size_t i = 0; i < values.size(); i++) bus.write32ARM7(base + i * 4, values[i]);

    // Execute the test code.
    writeProgramToMemory("LDMIA R8!, {R0-R3}\n", MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.fetchAndExecute();

    // Assert registers loaded correctly
    for (size_t i = 0; i < values.size(); i++) ASSERT_EQ(arm7.readReg(i), values[i]);

    // Assert base register updated
    ASSERT_EQ(arm7.readReg(8), base + 16);
}
/**
 * @brief Tests LDMDB on multiple registers with no write back.
 */
TEST_F(TestCPU_ARM_LoadAndStoreInstructions_LDM, LDMDB_MULTI_NO_WRITE_BACK) {
    uint32_t base = MAIN_RAM_START + 0x600;
    std::vector<uint32_t> values = {0xAAAA0001, 0xBBBB0002, 0xCCCC0003, 0xDDDD0004};

    // Set a default value to the regs.
    arm7.reset();
    arm7.writeReg(8, base);  // R8 as base
    arm7.setPC(MAIN_RAM_START);

    // Write values to memory in descending order
    for (size_t i = 0; i < values.size(); i++) bus.write32ARM7(base - (i + 1) * 4, values[i]);

    // Execute the test code.
    writeProgramToMemory("LDMDB R8, {R0-R3}\n", MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.fetchAndExecute();

    // Assert registers loaded correctly
    for (size_t i = 0; i < values.size(); i++)
        ASSERT_EQ(arm7.readReg(i), values[values.size() - i - 1]);

    // Assert base register updated
    ASSERT_EQ(arm7.readReg(8), base);
}
/**
 * @brief Tests LDMDA on multiple registers with no write back.
 */
TEST_F(TestCPU_ARM_LoadAndStoreInstructions_LDM, LDMDA_MULTI_NO_WRITE_BACK) {
    uint32_t base = MAIN_RAM_START + 0x700;
    std::vector<uint32_t> values = {0x11110001, 0x22220002, 0x33330003, 0x44440004};

    // Set a default value to the regs.
    arm7.reset();
    arm7.writeReg(8, base);  // R8 as base
    arm7.setPC(MAIN_RAM_START);

    // Write values to memory in descending order
    for (size_t i = 0; i < values.size(); i++) bus.write32ARM7(base - (i * 4), values[i]);

    // Execute the test code.
    writeProgramToMemory("LDMDA R8, {R0-R3}\n", MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.fetchAndExecute();

    // Assert registers loaded correctly
    for (size_t i = 0; i < values.size(); i++)
        ASSERT_EQ(arm7.readReg(i), values[values.size() - i - 1]);

    // Assert base register updated
    ASSERT_EQ(arm7.readReg(8), base);
}
/**
 * @brief Tests LDMIB on multiple registers with no write back.
 */
TEST_F(TestCPU_ARM_LoadAndStoreInstructions_LDM, LDMIB_MULTI_NO_WRITE_BACK) {
    uint32_t base = MAIN_RAM_START + 0x800;
    std::vector<uint32_t> values = {0x99990001, 0xAAAA0002, 0xBBBB0003, 0xCCCC0004};

    // Set a default value to the regs.
    arm7.reset();
    arm7.writeReg(8, base);  // R8 as base
    arm7.setPC(MAIN_RAM_START);

    // Write values to memory in ascending order
    for (size_t i = 0; i < values.size(); i++) bus.write32ARM7(base + ((i + 1) * 4), values[i]);

    // Execute the test code.
    writeProgramToMemory("LDMIB R8, {R0-R3}\n", MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.fetchAndExecute();

    // Assert registers loaded correctly
    for (size_t i = 0; i < values.size(); i++) ASSERT_EQ(arm7.readReg(i), values[i]);

    // Assert base register updated
    ASSERT_EQ(arm7.readReg(8), base);
}
/**
 * @brief Tests LDMIA on multiple registers with no write back.
 */
TEST_F(TestCPU_ARM_LoadAndStoreInstructions_LDM, LDMIA_MULTI_NO_WRITE_BACK) {
    uint32_t base = MAIN_RAM_START + 0x900;
    std::vector<uint32_t> values = {0xDDDD0001, 0xEEEE0002, 0xFFFF0003, 0x00010004};

    // Set a default value to the regs.
    arm7.reset();
    arm7.writeReg(8, base);  // R8 as base
    arm7.setPC(MAIN_RAM_START);

    // Write values to memory in ascending order
    for (size_t i = 0; i < values.size(); i++) bus.write32ARM7(base + i * 4, values[i]);

    // Execute the test code.
    writeProgramToMemory("LDMIA R8, {R0-R3}\n", MAIN_RAM_START, &bus, arm7.isARM7());
    arm7.fetchAndExecute();

    // Assert registers loaded correctly
    for (size_t i = 0; i < values.size(); i++) ASSERT_EQ(arm7.readReg(i), values[i]);

    // Assert base register updated
    ASSERT_EQ(arm7.readReg(8), base);
}