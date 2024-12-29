#include "../src/interconnect.h"

#include <gtest/gtest.h>

/**
 * @brief Test fixture for the interconnect.
 */
class InterconnectTest : public testing::Test {
protected:
    InterconnectTest() {}
    ~InterconnectTest() {}

    void SetUp() override {}

    void TearDown() override {}
};

TEST_F(InterconnectTest, temp) {
    Interconnect bus;

    EXPECT_EQ(1, 1); 
}

TEST_F(InterconnectTest, temp2) {
    Interconnect bus;


    EXPECT_EQ(1, 1); 
}