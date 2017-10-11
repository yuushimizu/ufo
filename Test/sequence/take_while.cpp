#include <gtest/gtest.h>
#include "ufo/sequence/take_while.hpp"
#include <vector>
#include <type_traits>
#include "ufo/placeholder.hpp"
#include "delete_copy.hpp"

using namespace ufo;

namespace {
    TEST(TakeWhileTest, LValue) {
        std::vector<int> v {1, 7, 5, 8, 9, 3, 2};
        auto r = v | take_while(_ % 2 != 0);
        static_assert(std::is_same_v<option<int &>, decltype(r.next())>);
        ASSERT_EQ(1, *r.next());
        ASSERT_EQ(7, *r.next());
        ASSERT_EQ(5, *r.next());
        ASSERT_FALSE(r.next());
    }

    TEST(TakeWhileTest, RValue) {
        auto r = std::vector<int> {2, 4, 8, 9, 10, 12} | take_while(_ % 2 == 0);
        static_assert(std::is_same_v<option<int>, decltype(r.next())>);
        ASSERT_EQ(2, *r.next());
        ASSERT_EQ(4, *r.next());
        ASSERT_EQ(8, *r.next());
        ASSERT_FALSE(r.next());
    }
    
    TEST(TakeWhileTest, Empty) {
        auto r = std::vector<int> {} | take_while(_ < 10);
        ASSERT_FALSE(r.next());
    }
    
    TEST(TakeWhileTest, Unmatch) {
        auto r = std::vector<int> {10} | take_while(_ < 10);
        ASSERT_FALSE(r.next());
    }
    
    TEST(TakeWhileTest, SequenceNotCopied) {
        auto r = std::vector<int> {1, 2, 3} | test::delete_copy | take_while(_ < 2);
        ASSERT_EQ(1, *r.next());
        ASSERT_FALSE(r.next());
    }
    
    TEST(TakeWhileTest, FunctionNotCopied) {
        auto r = std::vector<int> {1, 2, 3} | take_while(test::delete_function_copy(_ < 2));
        ASSERT_EQ(1, *r.next());
        ASSERT_FALSE(r.next());
    }
}

