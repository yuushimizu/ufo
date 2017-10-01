#include <gtest/gtest.h>
#include "ufo/range2/take.hpp"
#include <vector>
#include <type_traits>
#include "ufo/range2/core.hpp"

using namespace ufo;

namespace {
    TEST(TakeTest, LValue) {
        std::vector<int> v {10, 20, 30};
        auto cw = v | take(2);
        static_assert(std::is_same_v<int &, decltype(cw | first)>);
        ASSERT_FALSE(cw | is_empty);
        ASSERT_EQ(&v[0], &(cw | first));
        cw = std::move(cw) | rest;
        ASSERT_FALSE(cw | is_empty);
        ASSERT_EQ(&v[1], &(cw | first));
        cw = std::move(cw) | rest;
        ASSERT_TRUE(cw | is_empty);
    }
}
