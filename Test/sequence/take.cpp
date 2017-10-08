#include <gtest/gtest.h>
#include "ufo/sequence/take.hpp"
#include <vector>
#include <type_traits>
#include "ufo/sequence/core.hpp"

using namespace ufo;

namespace {
    TEST(TakeTest, LValue) {
        std::vector<int> v {10, 20, 30};
        auto r = v | take(2);
        static_assert(std::is_same_v<int &, decltype(r.front())>);
        ASSERT_FALSE(r.empty());
        ASSERT_EQ(&v[0], &(r.front()));
        r.pop();
        ASSERT_FALSE(r.empty());
        ASSERT_EQ(&v[1], &(r.front()));
        r.pop();
        ASSERT_TRUE(r.empty());
    }
    
    TEST(TakeTest, RValue) {
        auto r = std::vector<int> {10, 20, 30} | take(2);
        static_assert(std::is_same_v<int, decltype(r.front())>);
        ASSERT_FALSE(r.empty());
        ASSERT_EQ(10, r.front());
        r.pop();
        ASSERT_FALSE(r.empty());
        ASSERT_EQ(20, r.front());
        r.pop();
        ASSERT_TRUE(r.empty());
    }
    
    TEST(TakeTest, Empty) {
        auto r = std::vector<int> {} | take(5);
        ASSERT_TRUE(r.empty());
    }
    
    TEST(TakeTest, Over) {
        auto r = std::vector<int> {10} | take(5);
        ASSERT_FALSE(r.empty());
        r.pop();
        ASSERT_TRUE(r.empty());
    }
}