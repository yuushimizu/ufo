#include <gtest/gtest.h>
#include "ufo/range.hpp"
#include "ufo/placeholder.hpp"
#include <vector>
#include <map>

using namespace ufo;
using placeholder::_;

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
    
    // TODO iterator_optional
    
    // TODO iterator_optref
    
    TEST(RangeTest, FindConstLValue) {
        const std::vector<int> v {1, 2, 3, 4, 5};
        auto found = find(v, _ == 3);
        ASSERT_TRUE(found);
        ASSERT_EQ(3, *found);
        auto not_found = find(v, _ == 7);
        ASSERT_FALSE(not_found);
    }
    
    TEST(RangeTest, FindLValue) {
        std::vector<int> v {1, 2, 3, 4, 5};
        auto found = find(v, _ == 3);
        ASSERT_TRUE(found);
        ASSERT_EQ(3, *found);
        *found = 10;
        ASSERT_EQ(3, v[2]);
        auto not_found = find(v, _ == 7);
        ASSERT_FALSE(not_found);
    }
    
    // TODO find rvalue
    
    // TODO find_ref
    
    TEST(RangeTest, FindRefLValue) {
        const std::vector<int> v {1, 2, 3, 4, 5};
        auto found = find_ref(v, _ == 3);
        ASSERT_TRUE(found);
        ASSERT_EQ(&v[2], &*found);
        auto not_found = find_ref(v, _ == 7);
        ASSERT_FALSE(not_found);
    }
    
    // TODO remove
    
    TEST(RangeTest, GetConstLValue) {
        const std::map<int ,std::string> m {{5, "foo"}, {8, "bar"}, {11, "baz"}};
        auto found = get(m, 8);
        ASSERT_TRUE(found);
        ASSERT_EQ("bar", *found);
        auto not_found = get(m, 99);
        ASSERT_FALSE(not_found);
    }
    
    // TODO get lvalue
    
    // TODO get rvalue
    
    TEST(RangeTest, GetRefConstLValue) {
        /*
        const std::map<int, std::string> m {{2, "two"}, {4, "four"}};
        auto found = get_ref(m, 4);
        ASSERT_TRUE(found);
        ASSERT_EQ("foud", *found);
        auto not_found = get(m, 123);
        ASSERT_EQ(nullopt, not_found);
         */
    }
    
    TEST(RangeTest, GetRefLValue) {
        std::map<int, std::string> m {{1, "one"}, {3, "three"}};
//        auto found = get_ref(m, 3);
//        ASSERT_TRUE(found);
//        ASSERT_EQ(&m.at(3), &*found);
        auto not_found = get_ref(m, 88);
        ASSERT_FALSE(not_found);
    }
    
    // get_ref rvalue
}
