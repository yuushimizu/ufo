#include <gtest/gtest.h>
#include "ufo/sequence/iterate.hpp"
#include <type_traits>
#include "ufo/placeholder.hpp"

using namespace ufo;

namespace {
    TEST(IterateTest, Iteration) {
        auto r = iterate(_ * 2, 1);
        static_assert(std::is_same_v<int, decltype(r.front())>);
        ASSERT_FALSE(r.empty());
        ASSERT_EQ(1, r.front());
        r.pop();
        ASSERT_FALSE(r.empty());
        ASSERT_EQ(2, r.front());
        r.pop();
        ASSERT_FALSE(r.empty());
        ASSERT_EQ(4, r.front());
        r.pop();
        ASSERT_FALSE(r.empty());
        ASSERT_EQ(8, r.front());
        r.pop();
        ASSERT_FALSE(r.empty());
        ASSERT_EQ(16, r.front());
        r.pop();
        ASSERT_FALSE(r.empty());
        ASSERT_EQ(32, r.front());
    }
}
