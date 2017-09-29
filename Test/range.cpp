#include <gtest/gtest.h>
#include "ufo/range.hpp"
#include "ufo/placeholder.hpp"
#include "ufo/range/transformed.hpp"
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
        auto r = find(v, _ == 3);
        ASSERT_TRUE(r);
        ASSERT_EQ(3, *r);
        *r = 4;
        ASSERT_EQ(3, v[2]);
    }
    
    TEST(RangeTest, FindConstLValueNotFound) {
        const std::vector<int> v {1, 2, 3, 4, 5};
        auto r = find(v, _ == 7);
        ASSERT_FALSE(r);
    }
    
    TEST(RangeTest, FindLValue) {
        std::vector<int> v {1, 2, 3, 4, 5};
        auto r = find(v, _ == 3);
        ASSERT_TRUE(r);
        ASSERT_EQ(3, *r);
        *r = 4;
        ASSERT_EQ(3, v[2]);
    }
    
    TEST(RangeTest, FindLValueNotFound) {
        std::vector<int> v {1, 2, 3, 4, 5};
        auto r = find(v, _ == 7);
        ASSERT_FALSE(r);
    }
    
    TEST(RangeTest, FindRValue) {
        auto r = find(std::vector<int> {1, 2, 3, 4, 5}, _ == 4);
        ASSERT_TRUE(r);
        ASSERT_EQ(4, *r);
    }
    
    TEST(RangeTest, FindRValueNotFound) {
        auto r = find(std::vector<int> {1, 2, 3, 4, 5}, _ == 7);
        ASSERT_FALSE(r);
    }
    
    TEST(RangeTest, FindRefConstLValue) {
        std::vector<int> v {1, 2, 3, 4, 5};
        const auto &cv = v;
        auto r = find_ref(cv, _ == 3);
        ASSERT_TRUE(r);
        ASSERT_EQ(&v[2], &*r);
        static_assert(std::is_same_v<const int &, decltype(*r)>);
    }
    
    TEST(RangeTest, FindRefLValue) {
        std::vector<int> v {1, 2, 3, 4, 5};
        auto r = find_ref(v, _ == 3);
        ASSERT_TRUE(r);
        ASSERT_EQ(&v[2], &*r);
        *r = 5;
        ASSERT_EQ(5, v[2]);
    }
    
    TEST(RangeTest, FindRefLValueNotFound) {
        std::vector<int> v {1, 2, 3, 4, 5};
        auto r = find_ref(v, _ == 7);
        ASSERT_FALSE(r);
    }
    
    TEST(RangeTest, FindRefRValue) {
        std::vector<int> v {1, 2, 3, 4, 5};
        auto r = find_ref(std::vector<int> {4, 0, 2} | transformed([&v](int i) -> int & {return v[i];}), _ == 3);
        ASSERT_TRUE(r);
        ASSERT_EQ(&v[2], &*r);
        *r = 7;
        ASSERT_EQ(7, v[2]);
    }
    
    TEST(RangeTest, FindRefRValueNotFound) {
        std::vector<int> v {1, 2, 3, 4, 5};
        auto r = find_ref(std::vector<int> {4, 0, 2} | transformed([&v](int i) -> int & {return v[i];}), _ == 7);
        ASSERT_FALSE(r);
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
