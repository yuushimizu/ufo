#include <gtest/gtest.h>
#include "ufo/range2/counting.hpp"
#include <type_traits>

using namespace ufo;

namespace {
    TEST(CoutingTest, BeginAndStep) {
        auto r = couting(10, 3);
        static_assert(std::is_same_v<int, decltype(r.first())>);
        ASSERT_FALSE(r.empty());
        ASSERT_EQ(10, r.first());
        r.pop();
        ASSERT_FALSE(r.empty());
        ASSERT_EQ(13, r.first());
        r.pop();
        ASSERT_FALSE(r.empty());
        ASSERT_EQ(16, r.first());
        r.pop();
        ASSERT_FALSE(r.empty());
        ASSERT_EQ(19, r.first());
        r.pop();
        ASSERT_FALSE(r.empty());
        ASSERT_EQ(22, r.first());
    }
    
    TEST(CoutingTest, Begin) {
        auto r = couting(5);
        static_assert(std::is_same_v<int, decltype(r.first())>);
        ASSERT_FALSE(r.empty());
        ASSERT_EQ(5, r.first());
        r.pop();
        ASSERT_FALSE(r.empty());
        ASSERT_EQ(6, r.first());
        r.pop();
        ASSERT_FALSE(r.empty());
        ASSERT_EQ(7, r.first());
        r.pop();
        ASSERT_FALSE(r.empty());
        ASSERT_EQ(8, r.first());
        r.pop();
        ASSERT_FALSE(r.empty());
        ASSERT_EQ(9, r.first());
    }
    
    TEST(CoutingTest, Default) {
        auto r = couting<int>();
        static_assert(std::is_same_v<int, decltype(r.first())>);
        ASSERT_FALSE(r.empty());
        ASSERT_EQ(0, r.first());
        r.pop();
        ASSERT_FALSE(r.empty());
        ASSERT_EQ(1, r.first());
        r.pop();
        ASSERT_FALSE(r.empty());
        ASSERT_EQ(2, r.first());
        r.pop();
        ASSERT_FALSE(r.empty());
        ASSERT_EQ(3, r.first());
        r.pop();
        ASSERT_FALSE(r.empty());
        ASSERT_EQ(4, r.first());
    }
    
    TEST(CoutingTest, Double) {
        auto r = couting<double>(1.3, 0.4);
        static_assert(std::is_same_v<double, decltype(r.first())>);
        ASSERT_FALSE(r.empty());
        ASSERT_DOUBLE_EQ(1.3, r.first());
        r.pop();
        ASSERT_FALSE(r.empty());
        ASSERT_DOUBLE_EQ(1.7, r.first());
        r.pop();
        ASSERT_FALSE(r.empty());
        ASSERT_DOUBLE_EQ(2.1, r.first());
        r.pop();
        ASSERT_FALSE(r.empty());
        ASSERT_DOUBLE_EQ(2.5, r.first());
        r.pop();
        ASSERT_FALSE(r.empty());
        ASSERT_DOUBLE_EQ(2.9, r.first());
    }
}

