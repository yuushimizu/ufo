#include <gtest/gtest.h>
#include "ufo/sequence2/concat.hpp"
#include "ufo/sequence2/range.hpp"
#include "ufo/sequence2/map.hpp"
#include "ufo/placeholder.hpp"
#include "test_utility.hpp"
#include "../test_utility.hpp"
#include <deque>

namespace {
    using ufo::_;
    
    TEST(ConcatTest, ConcatTwo) {
        auto s = ufo::s2::concat(ufo::s2::range(5), ufo::s2::range(3) | ufo::s2::map(_ * 3));
        ufo::test::sequence_assert<int>(s);
        ASSERT_EQ((std::vector {0, 1, 2, 3, 4, 0, 3, 6}), ufo::test::collect_values(s));
    }
    
    TEST(ConcatTest, ConcatThree) {
        auto s = ufo::s2::concat(ufo::s2::range(3), ufo::s2::range(3) | ufo::s2::map(_ * 2), ufo::s2::range(3) | ufo::s2::map(_ + 10));
        ASSERT_EQ((std::vector {0, 1, 2, 0, 2, 4, 10, 11, 12}), ufo::test::collect_values(s));
    }
    
    TEST(ConcatTest, IterateTwice) {
        auto s = ufo::s2::concat(ufo::s2::range(4), ufo::s2::range(4) | ufo::s2::map(_ + 10));
        ASSERT_EQ((std::vector {0, 1, 2, 3, 10, 11, 12, 13}), ufo::test::collect_values(s));
        ASSERT_EQ((std::vector {0, 1, 2, 3, 10, 11, 12, 13}), ufo::test::collect_values(s));
    }
    
    TEST(ConcatTest, FromContainers) {
        auto s = ufo::s2::concat(std::vector {10, 20, 30}, std::deque {50, 60, 70});
        ASSERT_EQ((std::vector {10, 20, 30, 50, 60, 70}), ufo::test::collect_values(s));
    }
    
    TEST(ConcatTest, EmptyFront) {
        auto s = ufo::s2::concat(ufo::s2::range(0), ufo::s2::range(4));
        ASSERT_EQ((std::vector {0, 1, 2, 3}), ufo::test::collect_values(s));
    }
    
    TEST(ConcatTest, EmptyBack) {
        auto s = ufo::s2::concat(ufo::s2::range(5), ufo::s2::range(0));
        ASSERT_EQ((std::vector {0, 1, 2, 3, 4}), ufo::test::collect_values(s));
    }
    
    TEST(ConcatTest, Empty) {
        auto s = ufo::s2::concat(std::vector<int> {}, std::deque<int> {});
        ASSERT_EQ((std::vector<int> {}), ufo::test::collect_values(s));
    }
    
    TEST(ConcatTest, MoveSequence) {
        auto s = ufo::s2::concat(ufo::test::nocopyseq(ufo::s2::range(3)), ufo::test::nocopyseq(ufo::s2::range(5)));
        static_assert(!std::is_copy_constructible_v<decltype(s)>);
        ASSERT_EQ((std::vector {0, 1, 2, 0, 1, 2, 3, 4}), ufo::test::collect_values(s));
    }
}
