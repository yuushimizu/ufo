#include <gtest/gtest.h>
#include "ufo/sequence/take.hpp"
#include <vector>
#include <type_traits>

using namespace ufo;

namespace {
    TEST(TakeTest, LValue) {
        std::vector<int> v {10, 20, 30};
        auto r = v | take(2);
        static_assert(std::is_same_v<option<int &>, decltype(r.next())>);
        ASSERT_EQ(&v[0], &*r.next());
        ASSERT_EQ(&v[1], &*r.next());
        ASSERT_FALSE(r.next());
    }
    
    TEST(TakeTest, RValue) {
        auto r = std::vector<int> {10, 20, 30} | take(2);
        static_assert(std::is_same_v<option<int>, decltype(r.next())>);
        ASSERT_EQ(10, *r.next());
        ASSERT_EQ(20, *r.next());
        ASSERT_FALSE(r.next());
    }
    
    TEST(TakeTest, Empty) {
        auto r = std::vector<int> {} | take(5);
        ASSERT_FALSE(r.next());
    }
    
    TEST(TakeTest, Over) {
        auto r = std::vector<int> {10} | take(5);
        ASSERT_EQ(10, *r.next());
        ASSERT_FALSE(r.next());
    }
}
