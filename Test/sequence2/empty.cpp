#include <gtest/gtest.h>
#include "ufo/sequence2/empty.hpp"
#include "test_utility.hpp"
#include <type_traits>

namespace {
    TEST(EmptyTest, Empty) {
        auto s = ufo::s2::empty<int>();
        ufo::test::sequence_assert<int>(s);
        ASSERT_EQ((std::vector<int> {}), ufo::test::collect_values(s));
    }
    
    TEST(EmptyTest, IterateTwice) {
        auto s = ufo::s2::empty<double>();
        ASSERT_EQ((std::vector<double> {}), ufo::test::collect_values(s));
        ASSERT_EQ((std::vector<double> {}), ufo::test::collect_values(s));
    }
}
