#include <gtest/gtest.h>
#include "ufo/sequence2/first.hpp"
#include "ufo/sequence2/range.hpp"
#include "test_utility.hpp"
#include <type_traits>

namespace {
    TEST(FirstTest, First) {
        ASSERT_EQ(3, *(ufo::s2::range(3, 8) | ufo::s2::first()));
    }
    
    TEST(FirstTest, CallTwice) {
        auto s = ufo::s2::range(4, 10);
        ASSERT_EQ(4, *(s | ufo::s2::first()));
        ASSERT_EQ(4, *(s | ufo::s2::first()));
    }
    
    TEST(FirstTest, PreventSequenceCopy) {
        auto s = ufo::test::nocopyseq(ufo::s2::range(10, 20));
        ASSERT_EQ(10, *(s | ufo::s2::first()));
    }
    
    TEST(FirstTest, Empty) {
        ASSERT_EQ(ufo::nullopt, ufo::s2::range(10, 0) | ufo::s2::first());
    }
    
    TEST(FirstTest, WithContainer) {
        ASSERT_EQ(8, *(std::vector {8, 9, 10} | ufo::s2::first()));
    }
}
