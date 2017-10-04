#include <gtest/gtest.h>
#include "ufo/optref.hpp"
#include <experimental/optional>
#include <type_traits>

using namespace ufo;

namespace {
    TEST(OptrefTest, Constructor) {
        int x = 10;
        optref<int> o(x);
        static_assert(std::is_same_v<int &, decltype(*o)>);
        ASSERT_TRUE(o);
        ASSERT_EQ(10, *o);
        *o = 13;
        ASSERT_EQ(13, *o);
        ASSERT_EQ(13, x);
    }
    
    TEST(OptrefTest, Const) {
        const int x = 10;
        optref<const int> o(x);
        static_assert(std::is_same_v<const int &, decltype(*o)>);
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
    
    TEST(OptrefTest, ConstNonConst) {
        int x = 10;
        const optref<int> o(x);
        static_assert(std::is_same_v<int &, decltype(*o)>);
        ASSERT_EQ(&x, &*o);
    }
}

