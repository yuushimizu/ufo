#include <gtest/gtest.h>
#include "ufo/sequence/max.hpp"
#include <vector>
#include <type_traits>
#include <memory>
#include "delete_copy.hpp"

using namespace ufo;

namespace {
    TEST(MaxTest, LValue) {
        std::vector<int> v {7, 3, 1, 8, 9, 2};
        auto cw = container_wrapper(v);
        decltype(auto) r = cw | max();
        static_assert(std::is_same_v<option<int &>, decltype(r)>);
        ASSERT_EQ(&v[4], &*r);
    }
    
    TEST(MaxTest, CopiedLValueNotChanged) {
        auto cw = container_wrapper(std::vector<int> {19, 22, 35});
        auto r = cw | max();
        ASSERT_EQ(35, *r);
        ASSERT_EQ(19, *cw.next());
        ASSERT_EQ(22, *cw.next());
        ASSERT_EQ(35, *cw.next());
        ASSERT_FALSE(cw.next());
    }
    
    TEST(MaxTest, RValue) {
        decltype(auto) r = std::vector<int> {40, 13, 24} | max();
        static_assert(std::is_same_v<option<int>, decltype(r)>);
        ASSERT_EQ(40, *r);
    }
    
    TEST(MaxTest, Empty) {
        auto r = std::vector<int> {} | max();
        ASSERT_FALSE(r);
    }
    
    TEST(MaxTest, OneValue) {
        auto r = std::vector<int> {56} | max();
        ASSERT_EQ(56, *r);
    }
    
    TEST(MaxTest, SequenceNotCopied) {
        auto r = std::vector<int> {2, 3, 4} | test::delete_copy | max();
        ASSERT_EQ(4, *r);
    }
}
