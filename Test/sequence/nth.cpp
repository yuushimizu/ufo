#include <gtest/gtest.h>
#include "ufo/sequence/nth.hpp"
#include <vector>
#include <type_traits>
#include "delete_copy.hpp"

using namespace ufo;

namespace {
    TEST(NthTest, FromLValue) {
        std::vector<int> v {10, 20, 30};
        auto cw = container_wrapper(v);
        decltype(auto) r = cw | nth(1);
        static_assert(std::is_same_v<option<int &>, decltype(r)>);
        ASSERT_EQ(&v[1], &*r);
    }
    
    TEST(NthTest, CopiedLValueNotChanged) {
        std::vector<int> v {10, 20, 30};
        auto cw = container_wrapper(std::vector<int> {10, 20, 30});
        decltype(auto) r = cw | nth(2);
        ASSERT_EQ(30, *r);
        ASSERT_EQ(10, *cw.next());
        ASSERT_EQ(20, *cw.next());
        ASSERT_EQ(30, *cw.next());
        ASSERT_FALSE(cw.next());
    }
    
    TEST(NthTest, FromRValue) {
        decltype(auto) r = std::vector<int> {10, 20, 30} | nth(2);
        static_assert(std::is_same_v<option<int>, decltype(r)>);
        ASSERT_EQ(30, *r);
    }
    
    TEST(NthTest, Empty) {
        auto r = std::vector<int> {} | nth(5);
        ASSERT_FALSE(r);
    }
    
    TEST(NthTest, Over) {
        auto r = std::vector<int> {10} | nth(5);
        ASSERT_FALSE(r);
    }
    
    TEST(NthTest, SequenceNotCopied) {
        auto r = std::vector<int> {2, 3, 4} | test::delete_copy | nth(1);
        ASSERT_EQ(3, *r);
    }
}
