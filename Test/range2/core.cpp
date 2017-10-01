#include <gtest/gtest.h>
#include "ufo/range2/core.hpp"
#include <type_traits>
#include <vector>
#include "ufo/range2/container_wrapper.hpp"

using namespace ufo;

namespace {
    TEST(ContainerWrapperTest, FirstLValue) {
        std::vector<int> v {10, 20, 30};
        static_assert(std::is_same_v<int &, decltype(v | first)>);
        ASSERT_EQ(&v[0], &(v | first));
    }
    
    TEST(ContainerWrapperTest, FirstRValue) {
        static_assert(std::is_same_v<int, decltype(std::vector<int> {10, 20, 30} | first)>);
        ASSERT_EQ(10, (std::vector<int> {10, 20, 30} | first));
    }
    
    TEST(ContainerWrapperTest, FirstLValueCall) {
        std::vector<int> v {10, 20, 30};
        static_assert(std::is_same_v<int &, decltype(first(v))>);
        ASSERT_EQ(&v[0], &first(v));
    }
    
    TEST(ContainerWrapperTest, FirstRValueCall) {
        static_assert(std::is_same_v<int, decltype(first(std::vector<int> {10, 20, 30}))>);
        ASSERT_EQ(10, first(std::vector<int> {10, 20, 30}));
    }
}
