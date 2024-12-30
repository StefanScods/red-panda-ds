#include <gtest/gtest.h>

#include "../src/interconnect.h"
#include "../src/memoryDefines.h"

/**
 * @brief Test fixture for the interconnect.
 */
class TestInterconnect : public testing::Test {
protected:
    Interconnect bus;
    TestInterconnect() {}
    ~TestInterconnect() {}

    void SetUp() override { bus.init(); }

    void TearDown() override {}
};

class TestInterconnect_MainRAM : public TestInterconnect {
protected:
    TestInterconnect_MainRAM() {}
    ~TestInterconnect_MainRAM() {}

    void SetUp() override { TestInterconnect::SetUp(); }

    void TearDown() override { TestInterconnect::TearDown(); }
};

class TestInterconnect_MainRAM_ARM7 : public TestInterconnect_MainRAM {
protected:
    TestInterconnect_MainRAM_ARM7() {}
    ~TestInterconnect_MainRAM_ARM7() {}

    void SetUp() override { TestInterconnect_MainRAM::SetUp(); }

    void TearDown() override { TestInterconnect_MainRAM::TearDown(); }
};

TEST_F(TestInterconnect_MainRAM_ARM7, ReadAndWrite32Bit) {
    // Loop over the entire memory space.
    for (uint32_t i = MAIN_RAM_START; i < MAIN_RAM_START + MAIN_RAM_SIZE; i += 4) {
        // Write the value and read it back.
        bus.write32ARM7(i, i);
        uint32_t val = bus.read32ARM7(i);
        // Ensure the value survived.
        ASSERT_EQ(val, i);
    }
}

TEST_F(TestInterconnect_MainRAM_ARM7, ReadAndWrite16Bit) {
    // Loop over the entire memory space.
    for (uint32_t i = MAIN_RAM_START; i < MAIN_RAM_START + MAIN_RAM_SIZE; i += 2) {
        // Write the value and read it back.
        uint16_t valWrite = i >> 16;
        bus.write16ARM7(i, valWrite);
        uint16_t valRead = bus.read16ARM7(i);
        // Ensure the value survived.
        ASSERT_EQ(valRead, valWrite);
    }
}
