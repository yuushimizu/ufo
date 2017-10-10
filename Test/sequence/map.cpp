#include <gtest/gtest.h>
#include "ufo/sequence/map.hpp"
#include <vector>
#include <type_traits>
#include "ufo/placeholder.hpp"
#include "delete_copy.hpp"

using namespace ufo;

namespace {
    TEST(MapTest, LValue) {
        std::vector<int> v {10, 20, 30};
        auto r = v | map(_ * 2);
        static_assert(std::is_same_v<option<int>, decltype(r.next())>);
        ASSERT_EQ(20, *r.next());
        ASSERT_EQ(40, *r.next());
        ASSERT_EQ(60, *r.next());
        ASSERT_FALSE(r.next());
    }
    
    TEST(MapTest, RValue) {
        auto r = std::vector<int> {11, 22, 33} | test::delete_copy | map(_ * 2);
        static_assert(std::is_same_v<option<int>, decltype(r.next())>);
        ASSERT_EQ(22, *r.next());
        ASSERT_EQ(44, *r.next());
        ASSERT_EQ(66, *r.next());
        ASSERT_FALSE(r.next());
    }
    
    TEST(MapTest, Empty) {
        auto r = std::vector<int> {} | map(_ + 10);
        ASSERT_FALSE(r.next());
    }
    
    TEST(MapTest, FunctionNotCopied) {
        auto r = std::vector<int> {1} | map(test::delete_function_copy([](auto n) {return n * 2;}));
        ASSERT_EQ(2, *r.next());
        ASSERT_FALSE(r.next());
    }
}
