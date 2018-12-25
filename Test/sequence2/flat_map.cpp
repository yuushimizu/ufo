#include <gtest/gtest.h>
#include "ufo/sequence2/flat_map.hpp"
#include "ufo/sequence2/range.hpp"
#include "ufo/placeholder.hpp"
#include "../test_utility.hpp"
#include "test_utility.hpp"
#include <type_traits>
#include <memory>
#include <array>

namespace {
    using ufo::_;
    
    TEST(FlatMapTest, FlatMap) {
        auto s = std::vector {0, 1, 2} | ufo::s2::flat_map([](auto n) {return ufo::s2::range(n, 4);});
        ufo::test::sequence_assert<int>(s);
        ASSERT_EQ((std::vector {0, 1, 2, 3, 1, 2, 3, 2, 3}), ufo::test::collect_values(s));
    }
    
    TEST(FlatMapTest, IterateTwice) {
        auto s = std::vector {1, 2, 3} | ufo::s2::flat_map([](auto n) {return ufo::s2::range(0, n);});
        ASSERT_EQ((std::vector {0, 0, 1, 0, 1, 2}), ufo::test::collect_values(s));
        ASSERT_EQ((std::vector {0, 0, 1, 0, 1, 2}), ufo::test::collect_values(s));
    }
    
    TEST(FlatMapTest, FromContainers) {
        auto s = std::vector {1, 2, 3} | ufo::s2::flat_map([](auto n) {return std::vector {n ,n};});
        ASSERT_EQ((std::vector {1, 1, 2, 2, 3, 3}), ufo::test::collect_values(s));
    }
    
    TEST(FlatMapTest, Empty) {
        auto s = std::vector<int> {} | ufo::s2::flat_map([](auto n) {return ufo::s2::range(0, 10);});
        ASSERT_EQ((std::vector<int> {}), ufo::test::collect_values(s));
    }
    
    TEST(FlatMapTest, MapToEmpty) {
        auto s = std::vector {1, 2, 3, 4} | ufo::s2::flat_map([](auto n) {return ufo::s2::range(0, n % 2 == 0 ? 0 : n);});
        ASSERT_EQ((std::vector {0, 0, 1, 2}), ufo::test::collect_values(s));
    }
    
    TEST(FlatMapTest, MoveSequence) {
        auto s = ufo::test::nocopyseq(ufo::s2::range(4)) | ufo::s2::flat_map([](auto n) {return ufo::s2::range(0, n);});
        static_assert(!std::is_copy_constructible_v<decltype(s)>);
        ASSERT_EQ((std::vector {0, 0, 1, 0, 1, 2}), ufo::test::collect_values(s));
    }
    
    TEST(FlatMapTest, MoveInnerSequence) {
        auto s = ufo::s2::range(4) | ufo::s2::flat_map([](auto n) {return ufo::test::nocopyseq(ufo::s2::range(0, n));});
        static_assert(std::is_copy_constructible_v<decltype(s)>);
        ASSERT_EQ((std::vector {0, 0, 1, 0, 1, 2}), ufo::test::collect_values(s));
    }
}
