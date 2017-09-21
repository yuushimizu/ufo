#include <gtest/gtest.h>
#include <functional>
#include <iterator>
#include "ufo/range/repeatedly.hpp"

using namespace ufo;

namespace {
    TEST(RepeatedlyTest, ReturnValue) {
        auto r = repeatedly([]() {return 100;});
        auto i = std::begin(r);
        ASSERT_EQ(100, *i);
        ++i;
        ASSERT_EQ(100, *i);
        ++i;
        ASSERT_EQ(100, *i);
    }
}
