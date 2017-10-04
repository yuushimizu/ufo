#include <gtest/gtest.h>
#include "ufo/range2/filter.hpp"
#include <vector>
#include <type_traits>
#include "ufo/range2/core.hpp"
#include "ufo/placeholder.hpp"
#include "delete_copy.hpp"

using namespace ufo;

namespace {
    TEST(FilterTest, LValue) {
        std::vector<int> v {1, 2, 3, 4, 5};
        auto r = v | filter(_ % 2 == 0);
        static_assert(std::is_same_v<int &, decltype(r | first)>);
        ASSERT_FALSE(r | is_empty);
        ASSERT_EQ(&v[1], &(r | first));
        r.pop();
        ASSERT_FALSE(r | is_empty);
        ASSERT_EQ(&v[3], &(r | first));
        r.pop();
        ASSERT_TRUE(r | is_empty);
    }
    
    TEST(FilterTest, RValue) {
        auto r = std::vector<int> {1, 2, 3, 4, 5} | filter(_ % 2 != 0);
        static_assert(std::is_same_v<int, decltype(r | first)>);
        ASSERT_FALSE(r | is_empty);
        ASSERT_EQ(1, r | first);
        r.pop();
        ASSERT_FALSE(r | is_empty);
        ASSERT_EQ(3, r | first);
        r.pop();
        ASSERT_FALSE(r | is_empty);
        ASSERT_EQ(5, r | first);
        r.pop();
        ASSERT_TRUE(r | is_empty);
    }
    
    TEST(FilterTest, Empty) {
        auto r = std::vector<int> {} | filter(_ == 0);
        ASSERT_TRUE(r | is_empty);
    }
    
    TEST(FilterTset, NotMatched) {
        auto r = std::vector<int> {1, 2, 3} | filter(_ > 5);
        ASSERT_TRUE(r | is_empty);
    }
    
    TEST(FilterTest, FunctionNotCopied) {
        auto r = std::vector<int> {1, 2, 3} | filter(test::delete_function_copy([](auto n) {return n % 2 == 0;}));
        ASSERT_FALSE(r.empty());
        ASSERT_EQ(2, r.first());
        r.pop();
        ASSERT_TRUE(r.empty());
    }
}
