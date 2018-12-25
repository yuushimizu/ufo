#include <gtest/gtest.h>
#include "ufo/functional/identity.hpp"

namespace {
    TEST(IdentityTest, LValueReference) {
        int n = 100;
        static_assert(std::is_same_v<int &, decltype(ufo::identity(n))>);
        ASSERT_EQ(&n, &ufo::identity(n));
    }
    
    TEST(IdentityTest, RValue) {
        int n = 100;
        static_assert(std::is_same_v<int &&, decltype(ufo::identity(std::move(n)))>);
        int &&r = ufo::identity(std::move(n));
        ASSERT_EQ(&n, &r);
    }
}
