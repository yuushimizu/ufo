#include <gtest/gtest.h>
#include "ufo/sequence2/range.hpp"
#include "test_utility.hpp"
#include <type_traits>

namespace {
    TEST(RangeTest, Range) {
        auto s = ufo::s2::range(4, 16, 2);
        ufo::test::sequence_assert<int>(s);
        ASSERT_EQ((std::vector {4, 6, 8, 10, 12, 14}), ufo::test::collect_values(s));
    }
    
    TEST(RangeTest, DefaultStep) {
        auto s = ufo::s2::range(10, 15);
        ASSERT_EQ((std::vector {10, 11, 12, 13, 14}), ufo::test::collect_values(s));
    }
    
    TEST(RangeTest, DefaultBegin) {
        auto s = ufo::s2::range(10);
        ASSERT_EQ((std::vector {0, 1, 2, 3, 4, 5, 6, 7, 8, 9}), ufo::test::collect_values(s));
    }
    
    TEST(RangeTest, NegativeStep) {
        auto s = ufo::s2::range(10, 0, -1);
        ASSERT_EQ((std::vector {10, 9, 8, 7, 6, 5, 4, 3, 2, 1}), ufo::test::collect_values(s));
    }
    
    TEST(RangeTest, Empty) {
        auto s = ufo::s2::range(10, 0);
        ASSERT_EQ(ufo::nullopt, s.begin().next());
    }
}
