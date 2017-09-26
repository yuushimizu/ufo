#include <gtest/gtest.h>
#include "ufo/it.hpp"

using namespace ufo;

namespace {
    TEST(ItTest, LeftEqual) {
        auto f = (it == 10);
        ASSERT_TRUE(f(10));
        ASSERT_FALSE(f(11));
    }
    
    TEST(ItTest, RightEqual) {
        auto f = (24 == it);
        ASSERT_TRUE(f(24));
        ASSERT_FALSE(f(20));
    }
}

