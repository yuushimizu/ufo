#include <gtest/gtest.h>
#include "ufo/sequence/remove_nullopt.hpp"
#include <vector>
#include "ufo/option.hpp"
#include "delete_copy.hpp"

using namespace ufo;

namespace {
    TEST(RemoveNulloptTest, FromLValue) {
        auto v = std::vector<option<int>> {3, nullopt, 4, nullopt, 5};
        auto cw = container_wrapper(v);
        auto r = cw | remove_nullopt();
        static_assert(std::is_same_v<option<int &>, decltype(r.next())>);
        ASSERT_EQ(&*v[0], &*r.next());
        ASSERT_EQ(&*v[2], &*r.next());
        ASSERT_EQ(&*v[4], &*r.next());
        ASSERT_FALSE(r.next());
    }
    
    TEST(RemoveNulloptTest, CopiedLValueNotChanged) {
        auto cw = container_wrapper(std::vector<option<int>> {2, nullopt, 3});
        auto r = cw | remove_nullopt();
        ASSERT_EQ(2, *r.next());
        ASSERT_EQ(3, *r.next());
        ASSERT_FALSE(r.next());
        ASSERT_EQ(option<int>(2), *cw.next());
        ASSERT_EQ(nullopt, *cw.next());
        ASSERT_EQ(option<int>(3), *cw.next());
        ASSERT_FALSE(r.next());
    }
    
    TEST(RemoveNulloptTest, FromRValue) {
        auto r = std::vector<option<int>> {2, nullopt, 1} | remove_nullopt();
        ASSERT_EQ(2, *r.next());
        ASSERT_EQ(1, *r.next());
        ASSERT_FALSE(r.next());
    }

    TEST(RemoveNulloptTest, Empty) {
        auto r = std::vector<option<int>> {} | remove_nullopt();
        ASSERT_FALSE(r.next());
    }

    TEST(RemoveNulloptTest, SequenceNotCopied) {
        auto r = std::vector<option<int>> {1, 2} | test::delete_copy | remove_nullopt();
        ASSERT_EQ(1, *r.next());
        ASSERT_EQ(2, *r.next());
        ASSERT_FALSE(r.next());
    }
}
