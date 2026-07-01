#include <gtest/gtest.h>

#include "core/cpu.h"
#include "core/interconnect.h"
#include "core/memoryDefines.h"

using namespace RedPandaDS::Core;

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

// ==================================================================================================
// Main RAM
// ==================================================================================================
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
class TestInterconnect_MainRAM_ARM9 : public TestInterconnect_MainRAM {
protected:
    ARM946ES arm9;
    TestInterconnect_MainRAM_ARM9() {}
    ~TestInterconnect_MainRAM_ARM9() {}

    void SetUp() override {
        TestInterconnect_MainRAM::SetUp();
        bus.bindARM9(&arm9);
    }

    void TearDown() override { TestInterconnect_MainRAM::TearDown(); }
};
// ==================================================================================================
TEST_F(TestInterconnect_MainRAM_ARM7, ReadAndWrite32Bit) {
    // Loop over the entire memory space.
    for (uint32_t i = MAIN_RAM_START; i < MAIN_RAM_START + MAIN_RAM_SIZE; i += 4) {
        // Check the address is valid.
        ASSERT_TRUE(bus.isAddressValidARM7(i));
        // Write the value and read it back.
        bus.write32ARM7(i, i);
        uint32_t val = bus.read32ARM7(i);
        // Ensure the value survived.
        ASSERT_EQ(val, i);
    }
}
// ==================================================================================================
TEST_F(TestInterconnect_MainRAM_ARM7, ReadAndWrite16Bit) {
    // Loop over the entire memory space.
    for (uint32_t i = MAIN_RAM_START; i < MAIN_RAM_START + MAIN_RAM_SIZE; i += 2) {
        // Check the address is valid.
        ASSERT_TRUE(bus.isAddressValidARM7(i));
        // Write the value and read it back.
        uint16_t valWrite = i & 0x0000FFFF;
        bus.write16ARM7(i, valWrite);
        uint16_t valRead = bus.read16ARM7(i);
        // Ensure the value survived.
        ASSERT_EQ(valRead, valWrite);
    }
}
// ==================================================================================================
TEST_F(TestInterconnect_MainRAM_ARM9, ReadAndWrite32Bit) {
    // Loop over the entire memory space.
    for (uint32_t i = MAIN_RAM_START; i < MAIN_RAM_START + MAIN_RAM_SIZE; i += 4) {
        // Check the address is valid.
        ASSERT_TRUE(bus.isAddressValidARM9(i));
        // Write the value and read it back.
        bus.write32ARM9(i, i);
        uint32_t val = bus.read32ARM9(i);
        // Ensure the value survived.
        ASSERT_EQ(val, i);
    }
}
// ==================================================================================================
TEST_F(TestInterconnect_MainRAM_ARM9, ReadAndWrite16Bit) {
    // Loop over the entire memory space.
    for (uint32_t i = MAIN_RAM_START; i < MAIN_RAM_START + MAIN_RAM_SIZE; i += 2) {
        // Check the address is valid.
        ASSERT_TRUE(bus.isAddressValidARM9(i));
        // Write the value and read it back.
        uint16_t valWrite = i & 0x0000FFFF;
        bus.write16ARM9(i, valWrite);
        uint16_t valRead = bus.read16ARM9(i);
        // Ensure the value survived.
        ASSERT_EQ(valRead, valWrite);
    }
}
// ==================================================================================================

// ==================================================================================================
// Work RAM
// ==================================================================================================
class TestInterconnect_WRAM : public TestInterconnect {
protected:
    TestInterconnect_WRAM() {}
    ~TestInterconnect_WRAM() {}

    void SetUp() override { TestInterconnect::SetUp(); }

    void TearDown() override { TestInterconnect::TearDown(); }
};
class TestInterconnect_WRAM_ARM7WRAM : public TestInterconnect_WRAM {
protected:
    TestInterconnect_WRAM_ARM7WRAM() {}
    ~TestInterconnect_WRAM_ARM7WRAM() {}

    void SetUp() override { TestInterconnect_WRAM::SetUp(); }

    void TearDown() override { TestInterconnect_WRAM::TearDown(); }
};
// ==================================================================================================
TEST_F(TestInterconnect_WRAM_ARM7WRAM, ReadAndWrite32Bit) {
    // Loop over the entire memory space.
    for (uint32_t i = ARM7_WRAM_START; i < ARM7_WRAM_START + ARM7_WRAM_SIZE; i += 4) {
        // Check the address is valid.
        ASSERT_TRUE(bus.isAddressValidARM7(i));
        // Write the value and read it back.
        bus.write32ARM7(i, i);
        uint32_t val = bus.read32ARM7(i);
        // Ensure the value survived.
        ASSERT_EQ(val, i);
    }
}
// ==================================================================================================
TEST_F(TestInterconnect_WRAM_ARM7WRAM, ReadAndWrite16Bit) {
    // Loop over the entire memory space.
    for (uint32_t i = ARM7_WRAM_START; i < ARM7_WRAM_START + ARM7_WRAM_SIZE; i += 2) {
        // Check the address is valid.
        ASSERT_TRUE(bus.isAddressValidARM7(i));
        // Write the value and read it back.
        uint16_t valWrite = i & 0x0000FFFF;
        bus.write16ARM7(i, valWrite);
        uint16_t valRead = bus.read16ARM7(i);
        // Ensure the value survived.
        ASSERT_EQ(valRead, valWrite);
    }
}
// ==================================================================================================
