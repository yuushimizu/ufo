#include <gtest/gtest.h>
#include "ufo/sequence/min.hpp"
#include <vector>
#include <type_traits>
#include <memory>
#include "delete_copy.hpp"

using namespace ufo;

namespace {
    TEST(MinTest, LValue) {
        auto v = std::vector<int> {7, 3, 1, 8, 9, 2};
        auto cw = container_wrapper(v);
        decltype(auto) r = cw | min();
        static_assert(std::is_same_v<option<int &>, decltype(r)>);
        ASSERT_EQ(&v[2], &*r);
    }
    
    TEST(MinTest, CopiedLValueNotChanged) {
        auto cw = container_wrapper(std::vector<int> {19, 22, 35});
        auto r = cw | min();
        ASSERT_EQ(19, *r);
        ASSERT_EQ(19, *cw.next());
        ASSERT_EQ(22, *cw.next());
        ASSERT_EQ(35, *cw.next());
        ASSERT_FALSE(cw.next());
    }
    
    TEST(MinTest, RValue) {
        decltype(auto) r = std::vector<int> {40, 13, 24} | min();
        static_assert(std::is_same_v<option<int>, decltype(r)>);
        ASSERT_EQ(13, *r);
    }
    
    TEST(MinTest, Empty) {
        auto r = std::vector<int> {} | min();
        ASSERT_FALSE(r);
    }
    
    TEST(MinTest, OneValue) {
        auto r = std::vector<int> {56} | min();
        ASSERT_EQ(56, *r);
    }
    
    TEST(MinTest, SequenceNotCopied) {
        auto r = std::vector<int> {2, 3, 4} | test::delete_copy | min();
        ASSERT_EQ(2, *r);
    }
}
