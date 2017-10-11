#include <gtest/gtest.h>
#include "ufo/sequence/flatten.hpp"
#include <type_traits>
#include "ufo/sequence/map.hpp"
#include "ufo/sequence/range.hpp"
#include "delete_copy.hpp"

using namespace ufo;

namespace {
    TEST(FlattenTest, LValue) {
        std::vector<std::vector<int>> v {{10, 20}, {30, 40, 50}, {60}};
        auto r = v | flatten;
        static_assert(std::is_same_v<option<int &>, decltype(r.next())>);
        ASSERT_EQ(&v[0][0], &*r.next());
        ASSERT_EQ(&v[0][1], &*r.next());
        ASSERT_EQ(&v[1][0], &*r.next());
        ASSERT_EQ(&v[1][1], &*r.next());
        ASSERT_EQ(&v[1][2], &*r.next());
        ASSERT_EQ(&v[2][0], &*r.next());
        ASSERT_FALSE(r.next());
    }
    
    TEST(FlattenTest, RValue) {
        auto r = std::vector<std::vector<int>> {{10, 20}, {30}, {40}} | flatten;
        static_assert(std::is_same_v<option<int>, decltype(r.next())>);
        ASSERT_EQ(10, *r.next());
        ASSERT_EQ(20, *r.next());
        ASSERT_EQ(30, *r.next());
        ASSERT_EQ(40, *r.next());
        ASSERT_FALSE(r.next());
    }
    
    TEST(FlattenTest, OtherSequences) {
        auto r = std::vector<int> {1, 2, 3} | map([](auto n) {return range(n);}) | flatten;
        static_assert(std::is_same_v<option<int>, decltype(r.next())>);
        ASSERT_EQ(0, *r.next());
        ASSERT_EQ(0, *r.next());
        ASSERT_EQ(1, *r.next());
        ASSERT_EQ(0, *r.next());
        ASSERT_EQ(1, *r.next());
        ASSERT_EQ(2, *r.next());
        ASSERT_FALSE(r.next());
    }
    
    TEST(FlattenTest, SkipEmpty) {
        auto r = std::vector<std::vector<int>> {{1}, {}, {}, {2}} | flatten;
        ASSERT_EQ(1, *r.next());
        ASSERT_EQ(2, *r.next());
        ASSERT_FALSE(r.next());
    }
    
    TEST(FlattenTest, InnerEmpty) {
        auto r = std::vector<std::vector<int>> {{}, {}, {}} | flatten;
        ASSERT_FALSE(r.next());
    }
    
    TEST(FlattenTest, Empty) {
        auto r = std::vector<std::vector<int>> {} | flatten;
        ASSERT_FALSE(r.next());
    }
    
    TEST(FlattenTest, SequenceNotCopied) {
        auto r = std::vector<int> {1, 2} | map([](auto n) {return range(n) | test::delete_copy;}) | test::delete_copy | flatten;
        ASSERT_EQ(0, *r.next());
        ASSERT_EQ(0, *r.next());
        ASSERT_EQ(1, *r.next());
        ASSERT_FALSE(r.next());
    }
}
