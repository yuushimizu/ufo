#include <gtest/gtest.h>
#include "ufo/sequence/core.hpp"
#include <type_traits>
#include <vector>
#include "ufo/sequence/container_wrapper.hpp"

using namespace ufo;

namespace {
    TEST(FirstTest, LValue) {
        std::vector<int> v {10, 20, 30};
        static_assert(std::is_same_v<int &, decltype(v | first)>);
        ASSERT_EQ(&v[0], &(v | first));
    }
    
    TEST(FirstTset, RValue) {
        static_assert(std::is_same_v<int, decltype(std::vector<int> {10, 20, 30} | first)>);
        ASSERT_EQ(10, (std::vector<int> {10, 20, 30} | first));
    }
    
    TEST(FirstTest, LValueCall) {
        std::vector<int> v {10, 20, 30};
        static_assert(std::is_same_v<int &, decltype(first(v))>);
        ASSERT_EQ(&v[0], &first(v));
    }
    
    TEST(FirstTest, RValueCall) {
        static_assert(std::is_same_v<int, decltype(first(std::vector<int> {10, 20, 30}))>);
        ASSERT_EQ(10, first(std::vector<int> {10, 20, 30}));
    }
    
    TEST(RestTest, LValue) {
        std::vector<int> v {10, 20, 30};
        ASSERT_EQ(&v[1], &(v | rest | first));
    }
    
    TEST(RestTest, RValue) {
        ASSERT_EQ(20, (std::vector<int> {10, 20, 30} | rest | first));
    }
    
    TEST(RestTest, LValueCall) {
        std::vector<int> v {10, 20, 30};
        ASSERT_EQ(&v[1], &first(rest(v)));
    }
    
    TEST(RestTest, RValueCall) {
        ASSERT_EQ(20, first(rest(std::vector<int> {10, 20, 30})));
    }
    
    TEST(IsEmptyTest, LValueTrue) {
        std::vector<int> v {};
        ASSERT_TRUE(v | is_empty);
    }
    
    TEST(IsEmptyTest, RValueTrue) {
        ASSERT_TRUE((std::vector<int> {} | is_empty));
    }
    
    TEST(IsEmptyTest, LValueCallTrue) {
        std::vector<int> v {};
        ASSERT_TRUE(is_empty(v));
    }
    
    TEST(IsEmptyTest, RValueCallTrue) {
        ASSERT_TRUE(is_empty(std::vector<int> {}));
    }
    
    TEST(IsEmptyTest, LValueFalse) {
        std::vector<int> v {10, 20, 30};
        ASSERT_FALSE(v | is_empty);
    }
    
    TEST(IsEmptyTest, RValueFalse) {
        ASSERT_FALSE((std::vector<int> {10, 20, 30} | is_empty));
    }
    
    TEST(IsEmptyTest, LValueCallFalse) {
        std::vector<int> v {10, 20, 30};
        ASSERT_FALSE(is_empty(v));
    }
    
    TEST(IsEmptyTest, RValueCallFalse) {
        ASSERT_FALSE(is_empty(std::vector<int> {10, 20, 30}));
    }
}
