#include <gtest/gtest.h>
#include "ufo/sequence/filter.hpp"
#include <vector>
#include <type_traits>
#include "ufo/sequence/core.hpp"
#include "ufo/placeholder.hpp"
#include "delete_copy.hpp"

using namespace ufo;

namespace {
    TEST(FilterTest, LValue) {
        std::vector<int> v {1, 2, 3, 4, 5};
        auto r = v | filter(_ % 2 == 0);
        static_assert(std::is_same_v<int &, decltype(r.front())>);
        ASSERT_FALSE(r.empty());
        ASSERT_EQ(&v[1], &(r.front()));
        r.pop();
        ASSERT_FALSE(r.empty());
        ASSERT_EQ(&v[3], &(r.front()));
        r.pop();
        ASSERT_TRUE(r.empty());
    }
    
    TEST(FilterTest, RValue) {
        auto r = std::vector<int> {1, 2, 3, 4, 5} | filter(_ % 2 != 0);
        static_assert(std::is_same_v<int, decltype(r.front())>);
        ASSERT_FALSE(r.empty());
        ASSERT_EQ(1, r.front());
        r.pop();
        ASSERT_FALSE(r.empty());
        ASSERT_EQ(3, r.front());
        r.pop();
        ASSERT_FALSE(r.empty());
        ASSERT_EQ(5, r.front());
        r.pop();
        ASSERT_TRUE(r.empty());
    }
    
    TEST(FilterTest, Empty) {
        auto r = std::vector<int> {} | filter(_ == 0);
        ASSERT_TRUE(r.empty());
    }
    
    TEST(FilterTset, NotMatched) {
        auto r = std::vector<int> {1, 2, 3} | filter(_ > 5);
        ASSERT_TRUE(r.empty());
    }
    
    TEST(FilterTest, FunctionNotCopied) {
        auto r = std::vector<int> {1, 2, 3} | filter(test::delete_function_copy([](auto n) {return n % 2 == 0;}));
        ASSERT_FALSE(r.empty());
        ASSERT_EQ(2, r.front());
        r.pop();
        ASSERT_TRUE(r.empty());
    }
}
