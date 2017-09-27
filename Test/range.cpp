#include <gtest/gtest.h>
#include "ufo/range.hpp"
#include <vector>
#include "ufo/partial.hpp"

using namespace ufo;

namespace {
    // TODO find_iterator
    
    TEST(RangeTest, ContainsConstLValue) {
        const std::vector<int> v {5, 10, 15, 20};
        ASSERT_TRUE(contains(v, _ == 10));
        ASSERT_FALSE(contains(v, _ == 30));
    }
    
    TEST(RangeTest, ContainsLValue) {
        std::vector<int> v {3, 7, 11, 13};
        ASSERT_TRUE(contains(v, _ == 7));
        ASSERT_FALSE(contains(v, _ == 5));
    }
    
    TEST(RangeTest, ContainsRValue) {
        ASSERT_TRUE(contains(std::vector<int> {1, 2, 3, 4, 5}, _ == 3));
        ASSERT_FALSE(contains(std::vector<int> {3, 4, 5, 6}, _ == 8));
    }
    
    // TODO iterator_value_optional
    
    // TODO iterator_reference_optional
    
    TEST(RangeTest, FindConstLValue) {
        const std::vector<int> v {1, 2, 3, 4, 5};
        auto found = find(v, _ == 3);
        ASSERT_TRUE(found);
        ASSERT_EQ(3, *found);
        auto not_found = find(v, _ == 7);
        ASSERT_EQ(std::experimental::nullopt, not_found);
    }
    
    // TODO find lvalue
    
    // TODO find rvalue
    
    // TODO find_reference
    
    // TODO remove
}