#include <gtest/gtest.h>
#include "ufo/sequence/take.hpp"
#include <vector>
#include <type_traits>
#include "delete_copy.hpp"

using namespace ufo;

namespace {
    TEST(TakeTest, FromLValue) {
        auto v = std::vector<int> {10, 20, 30};
        auto cw = container_wrapper(v);
        auto r = cw | take(2);
        static_assert(std::is_same_v<option<int &>, decltype(r.next())>);
        ASSERT_EQ(&v[0], &*r.next());
        ASSERT_EQ(&v[1], &*r.next());
        ASSERT_FALSE(r.next());
    }
    
    TEST(TakeTest, CopiedLValueNotChanged) {
        auto v = std::vector<int> {10, 20, 30};
        auto cw = container_wrapper(std::vector<int> {10, 20, 30});
        auto r = cw | take(2);
        ASSERT_EQ(10, *r.next());
        ASSERT_EQ(20, *r.next());
        ASSERT_FALSE(r.next());
        ASSERT_EQ(10, *cw.next());
        ASSERT_EQ(20, *cw.next());
        ASSERT_EQ(30, *cw.next());
        ASSERT_FALSE(cw.next());
    }
    
    TEST(TakeTest, FromRValue) {
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
    
    TEST(TakeTest, SequenceNotCopied) {
        auto r = std::vector<int> {2, 3, 4} | test::delete_copy | take(1);
        ASSERT_EQ(2, *r.next());
        ASSERT_FALSE(r.next());
    }
}
