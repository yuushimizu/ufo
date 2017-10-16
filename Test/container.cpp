#include <gtest/gtest.h>
#include "ufo/container.hpp"
#include "ufo/placeholder.hpp"
#include <vector>
#include <map>
#include "TD.hpp"

using namespace ufo;

namespace {
    TEST(ContainerTest, Remove) {
        std::vector<int> v {1, 2, 3, 4, 5};
        remove(v, _ == 3);
        ASSERT_EQ((std::vector<int> {1, 2, 4, 5}), v);
    }
    
    TEST(ContainerTest, RemoveNotMatched) {
        std::vector<int> v {1, 2, 3, 4};
        remove(v, _ == 8);
        ASSERT_EQ((std::vector<int> {1, 2, 3, 4}), v);
    }
    
    TEST(ContainerTest, Get) {
        std::map<int, std::string> m {{2, "two"}, {4, "four"}, {5, "five"}};
        auto r = get(m, 4);
        static_assert(std::is_same_v<option<std::string &>, decltype(r)>);
        ASSERT_EQ(&m.at(4), &*r);
    }
    
    TEST(ContainerTest, GetNotMatched) {
        std::map<int, std::string> m {{1, "one"}, {2, "two"}};
        ASSERT_EQ(nullopt, get(m, 4));
    }
    
    TEST(ContainerTest, GetConst) {
        const std::map<int, std::string> m {{1, "one"}, {3, "three"}};
        auto r = get(m, 1);
        static_assert(std::is_same_v<option<const std::string &>, decltype(r)>);
        ASSERT_EQ(&m.at(1), &*r);
    }
    
    TEST(ContainerTest, GetRValue) {
        std::map<int, std::string> m {{1, "one"}};
        static_assert(std::is_same_v<option<std::string>, decltype(get(std::move(m), 1))>);
        ASSERT_EQ(std::string("one"), *get(std::move(m), 1));
    }
}