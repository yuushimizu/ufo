#include <gtest/gtest.h>
#include "ufo/sequence2/filter.hpp"
#include "ufo/sequence2/range.hpp"
#include "ufo/placeholder.hpp"
#include "../test_utility.hpp"
#include "test_utility.hpp"
#include <type_traits>
#include <memory>

namespace {
    using ufo::_;
    
    TEST(FilterTest, Filter) {
        auto s = ufo::s2::range(10) | ufo::s2::filter(_ % 2);
        ufo::test::sequence_assert<int>(s);
        ASSERT_EQ((std::vector {1, 3, 5, 7, 9}), ufo::test::collect_values(s));
    }
    
    TEST(FilterTest, IterateTwice) {
        auto s = ufo::s2::range(10) | ufo::s2::filter(!(_ % 2));
        ASSERT_EQ((std::vector {0, 2, 4, 6, 8}), ufo::test::collect_values(s));
        ASSERT_EQ((std::vector {0, 2, 4, 6, 8}), ufo::test::collect_values(s));
    }
    
    TEST(FilterTest, Empty) {
        auto s = ufo::s2::range(10, 0) | ufo::s2::filter(_ > 0);
        ASSERT_EQ((std::vector<int> {}), ufo::test::collect_values(s));
    }
    
    TEST(FilterTest, FilterToEmpty) {
        auto s = ufo::s2::range(10) | ufo::s2::filter(_ > 100);
        ASSERT_EQ((std::vector<int> {}), ufo::test::collect_values(s));
    }
    
    TEST(FilterTest, FunctionIsCopiedIntoSequence) {
        auto s = []() {
            auto f = [x = std::make_shared<int>(4)](int n) {
                return n < *x;
            };
            auto op = ufo::s2::filter(f);
            return ufo::s2::range(10) | op;
        }();
        ASSERT_EQ((std::vector {0, 1, 2, 3}), ufo::test::collect_values(s));
    }
    
    TEST(FilterTest, FunctionIsMovedIntoSequence) {
        auto s = ufo::s2::range(5) | ufo::s2::filter([x = std::make_unique<int>(3)](int n) {return n != *x;});
        static_assert(!std::is_copy_constructible_v<decltype(s)>);
        ASSERT_EQ((std::vector {0, 1, 2, 4}), ufo::test::collect_values(s));
    }
    
    TEST(FilterTest, ReferenceWrapperIsCopied) {
        auto f = ufo::test::nocopy(_ % 2);
        auto s = ufo::s2::range(10) | ufo::s2::filter(std::ref(f));
        ASSERT_EQ((std::vector {1, 3, 5, 7, 9}), ufo::test::collect_values(s));
        auto s2 = s;
        ASSERT_EQ((std::vector {1, 3, 5, 7, 9}), ufo::test::collect_values(s2));
    }
    
    TEST(FilterTest, MoveSequence) {
        auto s = ufo::test::nocopyseq(ufo::s2::range(5)) | ufo::s2::filter(_ > 1);
        static_assert(!std::is_copy_constructible_v<decltype(s)>);
        ASSERT_EQ((std::vector {2, 3, 4}), ufo::test::collect_values(s));
    }

    TEST(FilterTest, WithContainer) {
        auto s = std::vector {5, 7, 9, 3, 2, 1, 8} | ufo::s2::filter(_ > 4);
        ASSERT_EQ((std::vector {5, 7, 9, 8}), ufo::test::collect_values(s));
    }
}
