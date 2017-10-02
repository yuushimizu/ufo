#include <gtest/gtest.h>
#include "ufo/range2/take.hpp"
#include <vector>
#include <type_traits>
#include "ufo/range2/core.hpp"

using namespace ufo;

namespace {
    TEST(TakeTest, LValue) {
        std::vector<int> v {10, 20, 30};
        auto r = v | take(2);
        static_assert(std::is_same_v<int &, decltype(r | first)>);
        ASSERT_FALSE(r | is_empty);
        ASSERT_EQ(&v[0], &(r | first));
        r = std::move(r) | rest;
        ASSERT_FALSE(r | is_empty);
        ASSERT_EQ(&v[1], &(r | first));
        r = std::move(r) | rest;
        ASSERT_TRUE(r | is_empty);
    }
    
    TEST(TakeTest, RValue) {
        auto r = std::vector<int> {10, 20, 30} | take(2);
        static_assert(std::is_same_v<int, decltype(r | first)>);
        ASSERT_FALSE(r | is_empty);
        ASSERT_EQ(10, r | first);
        r = std::move(r) | rest;
        ASSERT_FALSE(r | is_empty);
        ASSERT_EQ(20, r | first);
        r = std::move(r) | rest;
        ASSERT_TRUE(r | is_empty);
    }
    
    TEST(TakeTest, Empty) {
        auto r = std::vector<int> {} | take(5);
        ASSERT_TRUE(r | is_empty);
    }
    
    TEST(TakeTest, Over) {
        auto r = std::vector<int> {10} | take(5);
        ASSERT_FALSE(r | is_empty);
        r = std::move(r) | rest;
        ASSERT_TRUE(r | is_empty);
    }
}
