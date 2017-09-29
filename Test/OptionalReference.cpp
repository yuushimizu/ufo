#include <gtest/gtest.h>
#include "ufo/OptionalReference.hpp"

using namespace ufo;

namespace {
    TEST(OptionalReferenceTest, Constructor) {
        int x = 10;
        OptionalReference<int> o(x);
        ASSERT_TRUE(o);
        ASSERT_EQ(10, *o);
        *o = 13;
        ASSERT_EQ(13, *o);
        ASSERT_EQ(13, x);
    }
    
    TEST(OptionalReferenceTest, Const) {
        const int x = 10;
        OptionalReference<const int> o(x);
        ASSERT_EQ(&x, &*o);
    }
    
    TEST(OptionalReferenceTest, DefaultConstructor) {
        OptionalReference<int> o {};
        ASSERT_FALSE(o);
    }
    
    TEST(OptionalReferenceTest, NulloptConstructor) {
        OptionalReference<int> o(nullopt);
        ASSERT_FALSE(o);
    }
}

