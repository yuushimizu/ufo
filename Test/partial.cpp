#include <gtest/gtest.h>
#include "ufo/partial.hpp"

using namespace ufo;

namespace {
    TEST(PartialTest, LeftEqual) {
        auto f = (_ == 10);
        ASSERT_TRUE(f(10));
        ASSERT_FALSE(f(11));
    }
    
    TEST(PartialTest, RightEqual) {
        auto f = (24 == _);
        ASSERT_TRUE(f(24));
        ASSERT_FALSE(f(20));
    }
    
    TEST(PartialTest, LeftPlus) {
        auto f = (_ + 1);
        ASSERT_EQ(15, f(14));
    }

    TEST(PartialTest, RightPlus) {
        auto f = (10 + _);
        ASSERT_EQ(23, f(13));
    }
    
    TEST(PartialTest, LeftModulo) {
        auto f = (_ % 2);
        ASSERT_EQ(1, f(7));
    }
    
    TEST(PartialTest, RightModulo) {
        auto f = (5 % _);
        ASSERT_EQ(2, f(3));
    }
    
    TEST(PartialTest, ModuloAndEqual) {
        auto f = (_ % 2 == 0);
        ASSERT_TRUE(f(8));
        ASSERT_FALSE(f(11));
    }
    
    TEST(PartialTest, NestedEqual) {
        auto f = (_ == 10 == false == true);
        ASSERT_TRUE(f(12));
        ASSERT_FALSE(f(10));
    }
}

