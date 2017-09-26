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
}

