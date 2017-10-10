#include <gtest/gtest.h>
#include "ufo/sequence/range.hpp"
#include <type_traits>

using namespace ufo;

namespace {
    TEST(RangeTest, BeginEndStep) {
        auto r = range(5, 11, 2);
        static_assert(std::is_same_v<option<int>, decltype(r.next())>);
        ASSERT_EQ(5, *r.next());
        ASSERT_EQ(7, *r.next());
        ASSERT_EQ(9, *r.next());
        ASSERT_FALSE(r.next());
    }
    
    TEST(RangeTest, BeginEnd) {
        auto r = range(3, 10);
        static_assert(std::is_same_v<option<int>, decltype(r.next())>);
        ASSERT_EQ(3, *r.next());
        ASSERT_EQ(4, *r.next());
        ASSERT_EQ(5, *r.next());
        ASSERT_EQ(6, *r.next());
        ASSERT_EQ(7, *r.next());
        ASSERT_EQ(8, *r.next());
        ASSERT_EQ(9, *r.next());
        ASSERT_FALSE(r.next());
    }
    
    TEST(RangeTest, End) {
        auto r = range(5);
        static_assert(std::is_same_v<option<int>, decltype(r.next())>);
        ASSERT_EQ(0, *r.next());
        ASSERT_EQ(1, *r.next());
        ASSERT_EQ(2, *r.next());
        ASSERT_EQ(3, *r.next());
        ASSERT_EQ(4, *r.next());
        ASSERT_FALSE(r.next());
    }
    
    TEST(RangeTest, Empty) {
        auto r = range(5, 5, 1);
        ASSERT_FALSE(r.next());
    }
    
    TEST(RangeTest, Double) {
        auto r = range(0.2, 1.2, 0.3);
        ASSERT_DOUBLE_EQ(0.2, *r.next());
        ASSERT_DOUBLE_EQ(0.5, *r.next());
        ASSERT_DOUBLE_EQ(0.8, *r.next());
        ASSERT_DOUBLE_EQ(1.1, *r.next());
        ASSERT_FALSE(r.next());
    }
    
    TEST(RangeTest, MinusStep) {
        auto r = range(10, 4, -2);
        ASSERT_EQ(10, *r.next());
        ASSERT_EQ(8, *r.next());
        ASSERT_EQ(6, *r.next());
        ASSERT_FALSE(r.next());
    }
}
