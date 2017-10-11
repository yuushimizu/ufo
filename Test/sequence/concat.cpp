#include <gtest/gtest.h>
#include "ufo/sequence/concat.hpp"
#include <vector>
#include <deque>
#include <type_traits>
#include "delete_copy.hpp"

using namespace ufo;

namespace {
    TEST(ConcatTest, LValue) {
        std::vector<int> v {10, 20};
        std::deque<int> d {30, 40, 50};
        auto r = concat(v, d);
        static_assert(std::is_same_v<option<int &>, decltype(r.next())>);
        ASSERT_EQ(&v[0], &*r.next());
        ASSERT_EQ(&v[1], &*r.next());
        ASSERT_EQ(&d[0], &*r.next());
        ASSERT_EQ(&d[1], &*r.next());
        ASSERT_EQ(&d[2], &*r.next());
        ASSERT_FALSE(r.next());
    }
    
    TEST(ConcatTest, RValue) {
        auto r = concat(std::vector<int> {11, 22, 33}, std::deque<int> {44, 55});
        static_assert(std::is_same_v<option<int>, decltype(r.next())>);
        ASSERT_EQ(11, *r.next());
        ASSERT_EQ(22, *r.next());
        ASSERT_EQ(33, *r.next());
        ASSERT_EQ(44, *r.next());
        ASSERT_EQ(55, *r.next());
        ASSERT_FALSE(r.next());
    }
    
    TEST(ConcatTest, Empty) {
        auto r = concat(std::vector<int> {}, std::vector<int> {});
        ASSERT_FALSE(r.next());
    }
    
    TEST(ConcatTest, FrontEmpty) {
        auto r = concat(std::vector<int> {}, std::vector<int> {5});
        ASSERT_EQ(5, *r.next());
        ASSERT_FALSE(r.next());
    }
    
    TEST(ConcatTest, BackEmpty) {
        auto r = concat(std::vector<int> {5}, std::vector<int> {});
        ASSERT_EQ(5, *r.next());
        ASSERT_FALSE(r.next());
    }
    
    TEST(ConcatTest, MultipleSequences) {
        auto r = concat(std::vector<int> {10}, std::vector<int> {20}, std::vector<int> {30});
        ASSERT_EQ(10, *r.next());
        ASSERT_EQ(20, *r.next());
        ASSERT_EQ(30, *r.next());
        ASSERT_FALSE(r.next());
    }
    
    TEST(ConcatTest, SingleSequence) {
        auto r = concat(std::vector<int> {10, 20});
        ASSERT_EQ(10, *r.next());
        ASSERT_EQ(20, *r.next());
        ASSERT_FALSE(r.next());
    }
    
    TEST(ConcatTest, SequenceNotCopied) {
        auto r = concat(std::vector<int> {10} | test::delete_copy, std::vector<int> {20} | test::delete_copy);
        ASSERT_EQ(10, *r.next());
        ASSERT_EQ(20, *r.next());
        ASSERT_FALSE(r.next());
    }
}
