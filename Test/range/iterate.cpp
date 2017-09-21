#include <gtest/gtest.h>
#include <iterator>
#include "ufo/range/iterate.hpp"

using namespace ufo;

namespace {
    TEST(IterateTest, Iteration) {
        auto r = iterate(5, [](auto &&i) {return i * 2;});
        auto i = std::begin(r);
        ASSERT_EQ(5, *i);
        ++i;
        ASSERT_EQ(10, *i);
        ++i;
        ASSERT_EQ(20, *i);
    }
}
