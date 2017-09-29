#include <gtest/gtest.h>
#include "ufo/optref.hpp"
#include <experimental/optional>

using namespace ufo;

namespace {
    TEST(OptrefTest, Constructor) {
        int x = 10;
        optref<int> o(x);
        ASSERT_TRUE(o);
        ASSERT_EQ(10, *o);
        *o = 13;
        ASSERT_EQ(13, *o);
        ASSERT_EQ(13, x);
    }
    
    TEST(OptrefTest, Const) {
        const int x = 10;
        optref<const int> o(x);
        ASSERT_EQ(&x, &*o);
    }
    
    TEST(OptrefTest, DefaultConstructor) {
        optref<int> o {};
        ASSERT_FALSE(o);
    }
    
    TEST(OptrefTest, NulloptConstructor) {
        optref<int> o(std::experimental::nullopt);
        ASSERT_FALSE(o);
    }
}

