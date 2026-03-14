#include <gtest/gtest.h>

#include "core/utils/utils.h"

/**
 * @brief Test fixture for the application's utility functions.
 */
class TestUtils : public testing::Test {
protected:
    TestUtils() {}
    ~TestUtils() {}

    void SetUp() override {}

    void TearDown() override {}
};

TEST_F(TestUtils, ReadBit) {
    // Read different portions of the data.
    uint32_t data = 0b10110000111100001111000011110000;
    ASSERT_EQ(readBit(data, 0), 0b0);
    ASSERT_EQ(readBit(data, 1), 0b0);
    ASSERT_EQ(readBit(data, 6), 0b1);
    ASSERT_EQ(readBit(data, 7), 0b1);
    ASSERT_EQ(readBit(data, 30), 0b0);
    ASSERT_EQ(readBit(data, 31), 0b1);
}

TEST_F(TestUtils, ReadBits) {
    // Read different portions of the data.
    uint32_t data = 0b11110000111100001111000011110000;
    ASSERT_EQ(readBits(data, 0, 31), data);
    ASSERT_EQ(readBits(data, 0, 15), 0b1111000011110000);
    ASSERT_EQ(readBits(data, 16, 31), 0b1111000011110000);
    ASSERT_EQ(readBits(data, 0, 7), 0b11110000);
    ASSERT_EQ(readBits(data, 4, 11), 0b00001111);
    ASSERT_EQ(readBits(data, 6, 12), 0b1000011);
    ASSERT_EQ(readBits(data, 31, 31), 0b1);
    ASSERT_EQ(readBits(data, 0, 0), 0b0);
}

TEST_F(TestUtils, WriteBit) {
    // Write different portions of the data.
    uint32_t data = 0b10110000111100001111000011110000;
    writeBit(data, 1, 0);
    ASSERT_EQ(data, 0b10110000111100001111000011110001);

    data = 0b10110000111100001111000011110000;
    writeBit(data, 0, 31);
    ASSERT_EQ(data, 0b00110000111100001111000011110000);

    data = 0b10110000111100001111000011110000;
    writeBit(data, 0, 30);
    ASSERT_EQ(data, 0b10110000111100001111000011110000);
}

TEST_F(TestUtils, WriteBits) {
    // Write different portions of the data.
    uint32_t data = 0b11110000111100001111000011110000;
    writeBits<uint32_t>(data, 0b00001111000011110000111100001011, 0, 31);
    ASSERT_EQ(data, 0b00001111000011110000111100001011);

    data = 0b11110000111100001111000011110000;
    writeBits<uint32_t>(data, 0b1111, 0, 3);
    ASSERT_EQ(data, 0b11110000111100001111000011111111);

    data = 0b11110000111100001111000011110000;
    writeBits<uint32_t>(data, 0b0000, 28, 31);
    ASSERT_EQ(data, 0b00000000111100001111000011110000);
}

TEST_F(TestUtils, ROR) {
    uint32_t data = 0b00000000000000000000000000000011;

    u32AndBool returnValue = ROR(data, 0);
    ASSERT_EQ(returnValue.data_u32, 0b0000000000000000000000000000011);
    ASSERT_EQ(returnValue.data_bool, 0);

    returnValue = ROR(data, 1);
    ASSERT_EQ(returnValue.data_u32, 0b10000000000000000000000000000001);
    ASSERT_EQ(returnValue.data_bool, 1);

    returnValue = ROR(data, 2);
    ASSERT_EQ(returnValue.data_u32, 0b11000000000000000000000000000000);
    ASSERT_EQ(returnValue.data_bool, 1);

    returnValue = ROR(data, 3);
    ASSERT_EQ(returnValue.data_u32, 0b01100000000000000000000000000000);
    ASSERT_EQ(returnValue.data_bool, 0);

    returnValue = ROR(data, 32);
    ASSERT_EQ(returnValue.data_u32, 0b00000000000000000000000000000011);
    ASSERT_EQ(returnValue.data_bool, 0);
}

TEST_F(TestUtils, ROL) {
    uint32_t data = 0b00000000000000000000000000000011;

    u32AndBool returnValue = ROL(data, 0);
    ASSERT_EQ(returnValue.data_u32, 0b0000000000000000000000000000011);
    ASSERT_EQ(returnValue.data_bool, 0);

    returnValue = ROL(data, 1);
    ASSERT_EQ(returnValue.data_u32, 0b00000000000000000000000000000110);
    ASSERT_EQ(returnValue.data_bool, 0);

    returnValue = ROL(data, 2);
    ASSERT_EQ(returnValue.data_u32, 0b00000000000000000000000000001100);
    ASSERT_EQ(returnValue.data_bool, 0);

    returnValue = ROL(data, 31);
    ASSERT_EQ(returnValue.data_u32, 0b10000000000000000000000000000001);
    ASSERT_EQ(returnValue.data_bool, 1);

    returnValue = ROL(data, 32);
    ASSERT_EQ(returnValue.data_u32, 0b00000000000000000000000000000011);
    ASSERT_EQ(returnValue.data_bool, 1);
}