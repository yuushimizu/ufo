#include <gtest/gtest.h>
#include "ufo/sequence/skip.hpp"
#include <vector>
#include <type_traits>
#include "delete_copy.hpp"

using namespace ufo;

namespace {
    TEST(SkipTest, FromLValue) {
        auto v = std::vector<int> {10, 20, 30};
        auto cw = container_wrapper(v);
        auto r = cw | skip(1);
        static_assert(std::is_same_v<option<int &>, decltype(r.next())>);
        ASSERT_EQ(&v[1], &*r.next());
        ASSERT_EQ(&v[2], &*r.next());
        ASSERT_FALSE(r.next());
    }
    
    TEST(SkipTest, CopiedLValueNotChanged) {
        auto v = std::vector<int> {10, 20, 30};
        auto cw = container_wrapper(std::vector<int> {10, 20, 30});
        auto r = cw | skip(2);
        ASSERT_EQ(30, *r.next());
        ASSERT_FALSE(r.next());
        ASSERT_EQ(10, *cw.next());
        ASSERT_EQ(20, *cw.next());
        ASSERT_EQ(30, *cw.next());
        ASSERT_FALSE(cw.next());
    }
    
    TEST(SkipTest, FromRValue) {
        auto r = std::vector<int> {10, 20, 30} | skip(2);
        static_assert(std::is_same_v<option<int>, decltype(r.next())>);
        ASSERT_EQ(30, *r.next());
        ASSERT_FALSE(r.next());
    }
    
    TEST(SkipTest, Empty) {
        auto r = std::vector<int> {} | skip(5);
        ASSERT_FALSE(r.next());
    }
    
    TEST(SkipTest, Over) {
        auto r = std::vector<int> {10} | skip(5);
        ASSERT_FALSE(r.next());
    }
    
    TEST(SkipTest, SequenceNotCopied) {
        auto r = std::vector<int> {2, 3, 4} | test::delete_copy | skip(1);
        ASSERT_EQ(3, *r.next());
        ASSERT_EQ(4, *r.next());
        ASSERT_FALSE(r.next());
    }
}

