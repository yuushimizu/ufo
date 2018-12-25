#include <gtest/gtest.h>
#include "ufo/sequence2/take.hpp"
#include "ufo/sequence2/counting.hpp"
#include "ufo/placeholder.hpp"
#include "../test_utility.hpp"
#include "test_utility.hpp"
#include <type_traits>
#include <memory>

namespace {
    using ufo::_;
    
    TEST(TakeTest, Take) {
        auto s = ufo::s2::counting(1) | ufo::s2::take(4);
        ufo::test::sequence_assert<int>(s);
        ASSERT_EQ((std::vector {1, 2, 3, 4}), ufo::test::collect_values(s));
    }
    
    TEST(TakeTest, IterateTwice) {
        auto s = ufo::s2::counting(3) | ufo::s2::take(5);
        ASSERT_EQ((std::vector {3, 4, 5, 6, 7}), ufo::test::collect_values(s));
        ASSERT_EQ((std::vector {3, 4, 5, 6, 7}), ufo::test::collect_values(s));
    }
    
    TEST(TakeTest, Over) {
        auto s = std::vector {10, 20} | ufo::s2::take(5);
        ASSERT_EQ((std::vector {10, 20}), ufo::test::collect_values(s));
    }
    
    TEST(TakeTest, Zero) {
        auto s = ufo::s2::counting(1) | ufo::s2::take(0);
        ASSERT_EQ(ufo::nullopt, s.begin().next());
    }
    
    TEST(TakeTest, MoveSequence) {
        auto s = ufo::test::nocopyseq(ufo::s2::counting(0)) | ufo::s2::take(3);
        static_assert(!std::is_copy_constructible_v<decltype(s)>);
        ASSERT_EQ((std::vector {0, 1, 2}), ufo::test::collect_values(s));
    }
    
    TEST(TakeTest, WithContainer) {
        auto s = std::vector {4, 2, 1, 6, 5, 2, 9, 7, 5} | ufo::s2::take(5);
        ASSERT_EQ((std::vector {4, 2, 1, 6, 5}), ufo::test::collect_values(s));
    }
}
