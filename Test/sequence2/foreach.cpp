#include <gtest/gtest.h>
#include "ufo/sequence2/foreach.hpp"
#include "ufo/sequence2/range.hpp"
#include "test_utility.hpp"
#include <type_traits>

namespace {
    TEST(ForeachTest, Foreach) {
        auto v = std::vector<int> {};
        ufo::s2::range(1, 5) | ufo::s2::foreach([&v](int n) {
            v.emplace_back(n);
        });
        ASSERT_EQ((std::vector {1, 2, 3, 4}), v);
    }
    
    TEST(ForeachTest, IterateTwice) {
        auto v = std::vector<int> {};
        auto s = ufo::s2::range(1, 5);
        s | ufo::s2::foreach([&v](int n) {
            v.emplace_back(n);
        });
        s | ufo::s2::foreach([&v](int n) {
            v.emplace_back(n);
        });
        ASSERT_EQ((std::vector {1, 2, 3, 4, 1, 2, 3, 4}), v);
    }
    
    TEST(ForeachTest, PreventSequenceCopy) {
        auto v = std::vector<int> {};
        auto s = ufo::test::nocopyseq(ufo::s2::range(1, 4));
        s | ufo::s2::foreach([&v](int n) {
            v.emplace_back(n);
        });
        ASSERT_EQ((std::vector {1, 2, 3}), v);
    }
    
    TEST(ForeachTest, WithContainer) {
        auto v = std::vector<int> {};
        std::vector {10, 20, 30} | ufo::s2::foreach([&v](int n) {
            v.emplace_back(n);
        });
        ASSERT_EQ((std::vector {10, 20, 30}), v);
    }
    
    TEST(ForeachTest, WithOption) {
        auto v = std::vector<int> {};
        ufo::option(123) | ufo::s2::foreach([&v](int n) {
            v.emplace_back(n);
        });
        ASSERT_EQ((std::vector {123}), v);
    }
}
