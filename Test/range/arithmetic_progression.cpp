#include <gtest/gtest.h>
#include <iterator>
#include "ufo/range/arithmetic_progression.hpp"

using namespace ufo;

namespace {
    struct P {
        int x;
        int y;
    };
    
    P operator+(const P &lhs, const P &rhs) {
        return P {lhs.x + rhs.x, lhs.y + rhs.y};
    }
    
    bool operator==(const P &lhs, const P &rhs) {
        return lhs.x == rhs.x && lhs.y == rhs.y;
    }
    
    TEST(ArithmeticProgressionTest, Default) {
        auto a = arithmetic_progression();
        auto i = std::begin(a);
        ASSERT_EQ(0, *i);
        ++i;
        ASSERT_EQ(1, *i);
        ++i;
        ASSERT_EQ(2, *i);
    }
    
    TEST(ArithmeticProgressionTest, Begin) {
        auto a = arithmetic_progression(10);
        auto i = std::begin(a);
        ASSERT_EQ(10, *i);
        ++i;
        ASSERT_EQ(11, *i);
        ++i;
        ASSERT_EQ(12, *i);
    }
    
    TEST(ArithmeticProgressionTest, Step) {
        auto a = arithmetic_progression(100, 10);
        auto i = std::begin(a);
        ASSERT_EQ(100, *i);
        ++i;
        ASSERT_EQ(110, *i);
        ++i;
        ASSERT_EQ(120, *i);
    }
    
    TEST(ArithmeticProgressionTest, Double) {
        auto a = arithmetic_progression(1.2, 0.1);
        auto i = std::begin(a);
        ASSERT_DOUBLE_EQ(1.2, *i);
        ++i;
        ASSERT_DOUBLE_EQ(1.3, *i);
        ++i;
        ASSERT_DOUBLE_EQ(1.4, *i);
    }
    
    TEST(ArithmeticProgressionTest, NonNumeric) {
        auto a = arithmetic_progression(P {10, 0}, P {0, 2});
        auto i = std::begin(a);
        ASSERT_EQ((P {10, 0}), *i);
        ++i;
        ASSERT_EQ((P {10, 2}), *i);
        ++i;
        ASSERT_EQ((P {10, 4}), *i);
    }
}
