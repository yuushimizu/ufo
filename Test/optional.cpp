#include <gtest/gtest.h>
#include "ufo/optional.hpp"

using namespace ufo;

namespace {
    TEST(OptionalTest, NonReference) {
        int x = 10;
        optional<int> o(x);
        ASSERT_TRUE(o);
        ASSERT_EQ(10, *o);
        ASSERT_NE(&x, &*o);
    }
    
    TEST(OptionalTest, Reference) {
        int x = 10;
        optional<int &> o(x);
        ASSERT_TRUE(o);
        ASSERT_EQ(&x, &*o);
        *o = 3;
        ASSERT_EQ(3, x);
    }
    
    TEST(OptionalTest, ConstReference) {
        int x = 10;
        optional<const int &> o (x);
        ASSERT_TRUE(o);
        ASSERT_EQ(&x, &*o);
        static_assert(std::is_same_v<const int &, decltype(*o)>);
    }
}
