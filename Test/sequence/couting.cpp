#include <gtest/gtest.h>
#include "ufo/sequence/counting.hpp"
#include <type_traits>

using namespace ufo;

namespace {
    TEST(CoutingTest, BeginAndStep) {
        auto r = couting(10, 3);
        static_assert(std::is_same_v<option<int>, decltype(r.next())>);
        ASSERT_EQ(10, *r.next());
        ASSERT_EQ(13, *r.next());
        ASSERT_EQ(16, *r.next());
        ASSERT_EQ(19, *r.next());
        ASSERT_EQ(22, *r.next());
    }
    
    TEST(CoutingTest, Begin) {
        auto r = couting(5);
        static_assert(std::is_same_v<option<int>, decltype(r.next())>);
        ASSERT_EQ(5, *r.next());
        ASSERT_EQ(6, *r.next());
        ASSERT_EQ(7, *r.next());
        ASSERT_EQ(8, *r.next());
        ASSERT_EQ(9, *r.next());
    }
    
    TEST(CoutingTest, Default) {
        auto r = couting<int>();
        static_assert(std::is_same_v<option<int>, decltype(r.next())>);
        ASSERT_EQ(0, *r.next());
        ASSERT_EQ(1, *r.next());
        ASSERT_EQ(2, *r.next());
        ASSERT_EQ(3, *r.next());
        ASSERT_EQ(4, *r.next());
    }
    
    TEST(CoutingTest, Double) {
        auto r = couting<double>(1.3, 0.4);
        static_assert(std::is_same_v<option<double>, decltype(r.next())>);
        ASSERT_DOUBLE_EQ(1.3, *r.next());
        ASSERT_DOUBLE_EQ(1.7, *r.next());
        ASSERT_DOUBLE_EQ(2.1, *r.next());
        ASSERT_DOUBLE_EQ(2.5, *r.next());
        ASSERT_DOUBLE_EQ(2.9, *r.next());
    }
}

