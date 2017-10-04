#include <gtest/gtest.h>
#include "ufo/sequence/map.hpp"
#include <vector>
#include <type_traits>
#include "ufo/placeholder.hpp"
#include "delete_copy.hpp"

using namespace ufo;

namespace {
    TEST(MapTest, OperatorLValue) {
        std::vector<int> v {10, 20, 30};
        auto r = v | map(_ * 2);
        static_assert(std::is_same_v<int, decltype(r.first())>);
        ASSERT_FALSE(r.empty());
        ASSERT_EQ(20, r.first());
        r.pop();
        ASSERT_FALSE(r.empty());
        ASSERT_EQ(40, r.first());
        r.pop();
        ASSERT_FALSE(r.empty());
        ASSERT_EQ(60, r.first());
        r.pop();
        ASSERT_TRUE(r.empty());
    }
    
    TEST(MapTest, OperatorRValue) {
        auto r = std::vector<int> {11, 22, 33} | test::delete_copy | map(_ * 2);
        static_assert(std::is_same_v<int, decltype(r.first())>);
        ASSERT_FALSE(r.empty());
        ASSERT_EQ(22, r.first());
        r.pop();
        ASSERT_FALSE(r.empty());
        ASSERT_EQ(44, r.first());
        r.pop();
        ASSERT_FALSE(r.empty());
        ASSERT_EQ(66, r.first());
        r.pop();
        ASSERT_TRUE(r.empty());
    }
    
    TEST(MapTset, SingleSequence) {
        auto r = map(_ * 2, std::vector<int> {5, 10, 15});
        static_assert(std::is_same_v<int, decltype(r.first())>);
        ASSERT_FALSE(r.empty());
        ASSERT_EQ(10, r.first());
        r.pop();
        ASSERT_FALSE(r.empty());
        ASSERT_EQ(20, r.first());
        r.pop();
        ASSERT_FALSE(r.empty());
        ASSERT_EQ(30, r.first());
        r.pop();
        ASSERT_TRUE(r.empty());
    }
    
    TEST(MapTest, MultipleSequences) {
        auto r = map([](auto n, auto m) constexpr {return n + m;}, std::vector<int> {10, 20, 30}, std::vector<int> {3, 2, 1});
        static_assert(std::is_same_v<int, decltype(r.first())>);
        ASSERT_FALSE(r.empty());
        ASSERT_EQ(13, r.first());
        r.pop();
        ASSERT_FALSE(r.empty());
        ASSERT_EQ(22, r.first());
        r.pop();
        ASSERT_FALSE(r.empty());
        ASSERT_EQ(31, r.first());
        r.pop();
        ASSERT_TRUE(r.empty());
    }
    
    TEST(MapTest, Empty) {
        auto r = std::vector<int> {} | map(_ + 10);
        ASSERT_TRUE(r.empty());
    }
    
    TEST(MapTest, FunctionNotCopied) {
        auto r = std::vector<int> {1} | map(test::delete_function_copy([](auto n) {return n * 2;}));
        ASSERT_FALSE(r.empty());
        ASSERT_EQ(2, r.first());
        r.pop();
        ASSERT_TRUE(r.empty());
    }
}
