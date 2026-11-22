#include "common.h"

/**
 * @brief Test fixture for the DMA.
 */
class TestE2E_DMA : public TestE2E {
protected:
    TestE2E_DMA() {}
    ~TestE2E_DMA() {}

    void SetUp() override { TestE2E::SetUp(); }

    void TearDown() override { TestE2E::TearDown(); }
};
// =================================================================================================
TEST_F(TestE2E_DMA, DMA_0_SimpleTransfer) {
    core.loadROM(getTestROMFile("dma_0"));

    core.startExecution();
    core.runApplicationIteration();

    // TODO Make core work in e2e.
    // ASSERT_EQ(bus->read32ARM9(0x02001010), 0x12345678);
    // ASSERT_EQ(bus->read32ARM9(0x02001014), 0xAABBCCDD);
    // ASSERT_EQ(bus->read32ARM9(0x02001018), 0x11);
    // ASSERT_EQ(bus->read32ARM9(0x0200101C), 0x22);
}
// =================================================================================================
