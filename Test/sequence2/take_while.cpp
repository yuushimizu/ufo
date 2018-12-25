#include <gtest/gtest.h>
#include "ufo/sequence2/take_while.hpp"
#include "ufo/sequence2/counting.hpp"
#include "ufo/placeholder.hpp"
#include "../test_utility.hpp"
#include "test_utility.hpp"
#include <type_traits>
#include <memory>

namespace {
    using ufo::_;
    
    TEST(TakeWhileTest, TakeWhile) {
        auto s = ufo::s2::counting(1) | ufo::s2::take_while(_ <= 5);
        ufo::test::sequence_assert<int>(s);
        ASSERT_EQ((std::vector {1, 2, 3, 4, 5}), ufo::test::collect_values(s));
    }
    
    TEST(TakeWhileTest, IterateTwice) {
        auto s = ufo::s2::counting(3) | ufo::s2::take_while(_ < 6);
        ASSERT_EQ((std::vector {3, 4, 5}), ufo::test::collect_values(s));
        ASSERT_EQ((std::vector {3, 4, 5}), ufo::test::collect_values(s));
    }
    
    TEST(TakeWhileTest, Empty) {
        auto s = ufo::s2::counting(1) | ufo::s2::take_while(_ > 10);
        ASSERT_EQ(ufo::nullopt, s.begin().next());
    }
    
    TEST(TakeWhileTest, FunctionIsCopiedIntoSequence) {
        auto s = []() {
            auto f = [x = std::make_shared<int>(4)](int n) {
                return n < *x;
            };
            auto op = ufo::s2::take_while(f);
            return ufo::s2::counting(0) | op;
        }();
        ASSERT_EQ((std::vector {0, 1, 2, 3}), ufo::test::collect_values(s));
    }
    
    TEST(TakeWhileTest, FunctionIsMovedIntoSequence) {
        auto s = ufo::s2::counting(0) | ufo::s2::take_while([x = std::make_unique<int>(3)](int n) {return n < *x;});
        static_assert(!std::is_copy_constructible_v<decltype(s)>);
        ASSERT_EQ((std::vector {0, 1, 2}), ufo::test::collect_values(s));
    }
    
    TEST(TakeWhileTest, ReferenceWrapperIsCopied) {
        auto f = ufo::test::nocopy(_ > 5);
        auto s = ufo::s2::counting(10, -1) | ufo::s2::take_while(std::ref(f));
        ASSERT_EQ((std::vector {10, 9, 8, 7, 6}), ufo::test::collect_values(s));
        auto s2 = s;
        ASSERT_EQ((std::vector {10, 9, 8, 7, 6}), ufo::test::collect_values(s2));
    }
    
    TEST(TakeWhileTest, MoveSequence) {
        auto s = ufo::test::nocopyseq(ufo::s2::counting(0)) | ufo::s2::take_while(_ < 3);
        static_assert(!std::is_copy_constructible_v<decltype(s)>);
        ASSERT_EQ((std::vector {0, 1, 2}), ufo::test::collect_values(s));
    }
    
    TEST(TakeWhileTest, WithContainer) {
        auto s = std::vector {4, 2, 1, 6, 5, 2, 9, 7, 5} | ufo::s2::take_while(_ < 9);
        ASSERT_EQ((std::vector {4, 2, 1, 6, 5, 2}), ufo::test::collect_values(s));
    }
}
