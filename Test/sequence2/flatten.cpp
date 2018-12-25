#include <gtest/gtest.h>
#include "ufo/sequence2/flatten.hpp"
#include "ufo/sequence2/range.hpp"
#include "ufo/sequence2/map.hpp"
#include "ufo/placeholder.hpp"
#include "../test_utility.hpp"
#include "test_utility.hpp"
#include <type_traits>
#include <memory>
#include <array>

namespace {
    using ufo::_;
    
    TEST(FlattenTest, Flatten) {
        auto s = std::vector {0, 1, 2} | ufo::s2::map([](auto n) {return ufo::s2::range(n, 3);}) | ufo::s2::flatten();
        ufo::test::sequence_assert<int>(s);
        ASSERT_EQ((std::vector {0, 1, 2, 1, 2, 2}), ufo::test::collect_values(s));
    }
    
    TEST(FlattenTest, IterateTwice) {
        auto s = std::vector {1, 2, 3} | ufo::s2::map([](auto n) {return ufo::s2::range(0, n);}) | ufo::s2::flatten();
        ASSERT_EQ((std::vector {0, 0, 1, 0, 1, 2}), ufo::test::collect_values(s));
        ASSERT_EQ((std::vector {0, 0, 1, 0, 1, 2}), ufo::test::collect_values(s));
    }
    
    TEST(FlattenTest, FromContainers) {
        auto s = std::vector {1, 2, 3} | ufo::s2::map([](auto n) {return std::vector {n, n};}) | ufo::s2::flatten();
        ASSERT_EQ((std::vector {1, 1, 2, 2, 3, 3}), ufo::test::collect_values(s));
    }
    
    TEST(FlattenTest, NestedContainer) {
        auto s = std::vector {std::vector {1, 2}, std::vector {3, 4}} | ufo::s2::flatten();
        ASSERT_EQ((std::vector {1, 2, 3, 4}), ufo::test::collect_values(s));
    }
    
    TEST(FlattenTest, Empty) {
        auto s = std::vector<int> {} | ufo::s2::map([](auto n) {return ufo::s2::range(0, 10);}) | ufo::s2::flatten();
        ASSERT_EQ((std::vector<int> {}), ufo::test::collect_values(s));
    }
    
    TEST(FlattenTest, ContainsEmpty) {
        auto s = std::vector {1, 2, 3, 4} | ufo::s2::map([](auto n) {return ufo::s2::range(0, n % 2 == 0 ? 0 : n);}) | ufo::s2::flatten();
        ASSERT_EQ((std::vector {0, 0, 1, 2}), ufo::test::collect_values(s));
    }

    TEST(FlattenTest, MoveSequence) {
        auto s = ufo::test::nocopyseq(ufo::s2::range(3) | ufo::s2::map([](auto n) {return ufo::s2::range(0, n);})) | ufo::s2::flatten();
        static_assert(!std::is_copy_constructible_v<decltype(s)>);
        ASSERT_EQ((std::vector {0, 0, 1}), ufo::test::collect_values(s));
    }
    
    TEST(FlattenTest, MoveInnerSequence) {
        auto s = ufo::s2::range(4) | ufo::s2::map([](auto n) {return ufo::test::nocopyseq(ufo::s2::range(0, n));}) | ufo::s2::flatten();
        static_assert(std::is_copy_constructible_v<decltype(s)>);
        ASSERT_EQ((std::vector {0, 0, 1, 0, 1, 2}), ufo::test::collect_values(s));
    }
}
