#include <gtest/gtest.h>
#include "ufo/math.hpp"

using namespace ufo;

namespace {
    TEST(DigitsTest, Zero) {
        ASSERT_EQ(1, digits(0));
    }
    
    TEST(DigitsTest, One) {
        ASSERT_EQ(1, digits(1));
    }
    
    TEST(DigitsTest, Nine) {
        ASSERT_EQ(1, digits(9));
    }
    
    TEST(DigitsTest, Ten) {
        ASSERT_EQ(2, digits(10));
    }
    
    TEST(DigitsTest, NinetyNine) {
        ASSERT_EQ(2, digits(99));
    }
    
    TEST(DigitsTest, Hundred) {
        ASSERT_EQ(3, digits(100));
    }
}
