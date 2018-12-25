#include <gtest/gtest.h>
#include "ufo/sequence2/counting.hpp"
#include "test_utility.hpp"
#include <type_traits>

namespace {
    TEST(CountingTest, Counting) {
        auto s = ufo::s2::counting(10, 2);
        ufo::test::sequence_assert<int>(s);
        ASSERT_EQ((std::vector {10, 12, 14}), ufo::test::collect_values(s, 3));
    }
    
    TEST(CountingTest, DefaultStep) {
        auto s = ufo::s2::counting(5);
        ASSERT_EQ((std::vector {5, 6, 7}), ufo::test::collect_values(s, 3));
    }
}
