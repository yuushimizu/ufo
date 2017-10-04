#include <gtest/gtest.h>
#include "ufo/sequence/iterate.hpp"
#include <type_traits>

using namespace ufo;

namespace {
    TEST(IterateTest, Iteration) {
        auto r = iterate([](int n) {return n * 2;}, 1);
        static_assert(std::is_same_v<int, decltype(r.first())>);
        ASSERT_FALSE(r.empty());
        ASSERT_EQ(1, r.first());
        r.pop();
        ASSERT_FALSE(r.empty());
        ASSERT_EQ(2, r.first());
        r.pop();
        ASSERT_FALSE(r.empty());
        ASSERT_EQ(4, r.first());
        r.pop();
        ASSERT_FALSE(r.empty());
        ASSERT_EQ(8, r.first());
        r.pop();
        ASSERT_FALSE(r.empty());
        ASSERT_EQ(16, r.first());
        r.pop();
        ASSERT_FALSE(r.empty());
        ASSERT_EQ(32, r.first());
    }
}
