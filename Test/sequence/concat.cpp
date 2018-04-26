#include <gtest/gtest.h>
#include "ufo/sequence/concat.hpp"
#include <vector>
#include <deque>
#include <type_traits>
#include "delete_copy.hpp"

using namespace ufo;

namespace {
    TEST(ConcatTest, FromLValue) {
        auto v = std::vector<int> {10, 20};
        auto cwv = container_wrapper(v);
        auto d = std::deque<int> {30, 40, 50};
        auto cwd = container_wrapper(d);
        auto r = concat(cwv, cwd);
        static_assert(std::is_same_v<option<int &>, decltype(r.next())>);
        ASSERT_EQ(&v[0], &*r.next());
        ASSERT_EQ(&v[1], &*r.next());
        ASSERT_EQ(&d[0], &*r.next());
        ASSERT_EQ(&d[1], &*r.next());
        ASSERT_EQ(&d[2], &*r.next());
        ASSERT_FALSE(r.next());
    }
    
    TEST(ConcatTest, CopiedLValueNotChanged) {
        auto cwv = container_wrapper(std::vector<int> {10});
        auto cwd = container_wrapper(std::vector<int> {20, 30});
        auto r = concat(cwv, cwd);
        ASSERT_EQ(10, *r.next());
        ASSERT_EQ(20, *r.next());
        ASSERT_EQ(30, *r.next());
        ASSERT_FALSE(r.next());
        ASSERT_EQ(10, *cwv.next());
        ASSERT_FALSE(cwv.next());
        ASSERT_EQ(20, *cwd.next());
        ASSERT_EQ(30, *cwd.next());
        ASSERT_FALSE(cwd.next());
    }
    
    TEST(ConcatTest, FromRValue) {
        auto r = concat(std::vector<int> {11, 22, 33}, std::deque<int> {44, 55});
        static_assert(std::is_same_v<option<int>, decltype(r.next())>);
        ASSERT_EQ(11, *r.next());
        ASSERT_EQ(22, *r.next());
        ASSERT_EQ(33, *r.next());
        ASSERT_EQ(44, *r.next());
        ASSERT_EQ(55, *r.next());
        ASSERT_FALSE(r.next());
    }
    
    TEST(ConcatTest, FromLValueAndRValue) {
        auto cw = container_wrapper(std::vector<int> {10});
        auto r = concat(cw, std::vector<int> {20} | test::delete_copy);
        ASSERT_EQ(10, *r.next());
        ASSERT_EQ(20, *r.next());
        ASSERT_FALSE(r.next());
        ASSERT_EQ(10, *cw.next());
        ASSERT_FALSE(cw.next());
    }
    
    TEST(ConcatTest, FromRValueAndLValue) {
        auto cw = container_wrapper(std::vector<int> {20});
        auto r = concat(std::vector<int> {10} | test::delete_copy, cw);
        ASSERT_EQ(10, *r.next());
        ASSERT_EQ(20, *r.next());
        ASSERT_FALSE(r.next());
        ASSERT_EQ(20, *cw.next());
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
