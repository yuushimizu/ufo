#include <gtest/gtest.h>
#include "ufo/sequence/counting.hpp"
#include <type_traits>

using namespace ufo;

namespace {
    TEST(CoutingTest, BeginAndStep) {
        auto r = couting(10, 3);
        static_assert(std::is_same_v<int, decltype(r.front())>);
        ASSERT_FALSE(r.empty());
        ASSERT_EQ(10, r.front());
        r.pop();
        ASSERT_FALSE(r.empty());
        ASSERT_EQ(13, r.front());
        r.pop();
        ASSERT_FALSE(r.empty());
        ASSERT_EQ(16, r.front());
        r.pop();
        ASSERT_FALSE(r.empty());
        ASSERT_EQ(19, r.front());
        r.pop();
        ASSERT_FALSE(r.empty());
        ASSERT_EQ(22, r.front());
    }
    
    TEST(CoutingTest, Begin) {
        auto r = couting(5);
        static_assert(std::is_same_v<int, decltype(r.front())>);
        ASSERT_FALSE(r.empty());
        ASSERT_EQ(5, r.front());
        r.pop();
        ASSERT_FALSE(r.empty());
        ASSERT_EQ(6, r.front());
        r.pop();
        ASSERT_FALSE(r.empty());
        ASSERT_EQ(7, r.front());
        r.pop();
        ASSERT_FALSE(r.empty());
        ASSERT_EQ(8, r.front());
        r.pop();
        ASSERT_FALSE(r.empty());
        ASSERT_EQ(9, r.front());
    }
    
    TEST(CoutingTest, Default) {
        auto r = couting<int>();
        static_assert(std::is_same_v<int, decltype(r.front())>);
        ASSERT_FALSE(r.empty());
        ASSERT_EQ(0, r.front());
        r.pop();
        ASSERT_FALSE(r.empty());
        ASSERT_EQ(1, r.front());
        r.pop();
        ASSERT_FALSE(r.empty());
        ASSERT_EQ(2, r.front());
        r.pop();
        ASSERT_FALSE(r.empty());
        ASSERT_EQ(3, r.front());
        r.pop();
        ASSERT_FALSE(r.empty());
        ASSERT_EQ(4, r.front());
    }
    
    TEST(CoutingTest, Double) {
        auto r = couting<double>(1.3, 0.4);
        static_assert(std::is_same_v<double, decltype(r.front())>);
        ASSERT_FALSE(r.empty());
        ASSERT_DOUBLE_EQ(1.3, r.front());
        r.pop();
        ASSERT_FALSE(r.empty());
        ASSERT_DOUBLE_EQ(1.7, r.front());
        r.pop();
        ASSERT_FALSE(r.empty());
        ASSERT_DOUBLE_EQ(2.1, r.front());
        r.pop();
        ASSERT_FALSE(r.empty());
        ASSERT_DOUBLE_EQ(2.5, r.front());
        r.pop();
        ASSERT_FALSE(r.empty());
        ASSERT_DOUBLE_EQ(2.9, r.front());
    }
}

