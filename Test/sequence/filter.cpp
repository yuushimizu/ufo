#include <gtest/gtest.h>
#include "ufo/sequence/filter.hpp"
#include <vector>
#include <type_traits>
#include "ufo/placeholder.hpp"
#include "delete_copy.hpp"

using namespace ufo;

namespace {
    TEST(FilterTest, FromLValue) {
        std::vector<int> v {1, 2, 3, 4, 5};
        auto cw = container_wrapper(v);
        auto r = cw | filter(_ % 2 == 0);
        static_assert(std::is_same_v<option<int &>, decltype(r.next())>);
        ASSERT_EQ(&v[1], &*r.next());
        ASSERT_EQ(&v[3], &*r.next());
        ASSERT_FALSE(r.next());
    }
    
    TEST(FilterTest, CopiedLValueNotChanged) {
        auto cw = container_wrapper(std::vector<int> {1, 2, 3});
        auto r = cw | filter(_ % 2 == 0);
        ASSERT_EQ(2, *r.next());
        ASSERT_FALSE(r.next());
        ASSERT_EQ(1, *cw.next());
        ASSERT_EQ(2, *cw.next());
        ASSERT_EQ(3, *cw.next());
        ASSERT_FALSE(cw.next());
        
    }
    
    TEST(FilterTest, FromRValue) {
        auto r = std::vector<int> {1, 2, 3, 4, 5} | filter(_ % 2 != 0);
        static_assert(std::is_same_v<option<int>, decltype(r.next())>);
        ASSERT_EQ(1, *r.next());
        ASSERT_EQ(3, *r.next());
        ASSERT_EQ(5, *r.next());
        ASSERT_FALSE(r.next());
    }
    
    TEST(FilterTest, Empty) {
        auto r = std::vector<int> {} | filter(_ == 0);
        ASSERT_FALSE(r.next());
    }
    
    TEST(FilterTest, NotMatched) {
        auto r = std::vector<int> {1, 2, 3} | filter(_ > 5);
        ASSERT_FALSE(r.next());
    }
    
    TEST(FilterTest, SequenceNotCopied) {
        auto r = std::vector<int> {5, 10, 15} | test::delete_copy | filter(_ % 2 == 1);
        ASSERT_EQ(5, *r.next());
        ASSERT_EQ(15, *r.next());
        ASSERT_FALSE(r.next());
    }
    
    TEST(FilterTest, FunctionNotCopied) {
        auto r = std::vector<int> {1, 2, 3} | filter(test::delete_function_copy(_ % 2 == 0));
        ASSERT_EQ(2, *r.next());
        ASSERT_FALSE(r.next());
    }
}
