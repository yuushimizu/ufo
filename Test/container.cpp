#include <gtest/gtest.h>
#include "ufo/container.hpp"
#include "ufo/placeholder.hpp"
#include <list>
#include <vector>
#include <map>

using namespace ufo;

namespace {
    TEST(ContainerTest, PushFrontAll) {
        auto l = std::list<int> {1, 2, 3};
        push_front_all(l, 5, 6, 7);
        ASSERT_EQ(l, (std::list<int> {5, 6, 7, 1, 2, 3}));
    }
    
    TEST(ContainerTest, PushFronAllNoElement) {
        auto l = std::list<int> {4, 5};
        push_front_all(l);
        ASSERT_EQ(l, (std::list<int> {4, 5}));
    }
    
    TEST(ContainerTest, Remove) {
        auto v = std::vector<int> {1, 2, 3, 4, 5};
        remove(v, _ == 3);
        ASSERT_EQ((std::vector<int> {1, 2, 4, 5}), v);
    }
    
    TEST(ContainerTest, RemoveNotMatched) {
        auto v = std::vector<int> {1, 2, 3, 4};
        remove(v, _ == 8);
        ASSERT_EQ((std::vector<int> {1, 2, 3, 4}), v);
    }
    
    TEST(ContainerTest, Sort) {
        auto v = std::vector<int> {4, 8, 1, 2, 0, 5, 3};
        sort(v, [](auto n, auto m) {return n > m;});
        ASSERT_EQ((std::vector<int> {8, 5, 4, 3, 2, 1, 0}), v);
    }
    
    TEST(ContainerTest, StableSort) {
        auto v = std::vector<int> {-1, 4, -3, 2, 1, -4, 5, -5, 3, -2};
        stable_sort(v, [](auto n, auto m) {return std::abs(n) < std::abs(m);});
        ASSERT_EQ((std::vector<int> {-1, 1, 2, -2, -3, 3, 4, -4, 5, -5}), v);
    }
    
    TEST(ContainerTest, Get) {
        auto m = std::map<int, std::string> {{2, "two"}, {4, "four"}, {5, "five"}};
        auto r = get(m, 4);
        static_assert(std::is_same_v<option<std::string &>, decltype(r)>);
        ASSERT_EQ(&m.at(4), &*r);
    }
    
    TEST(ContainerTest, GetNotMatched) {
        auto m = std::map<int, std::string> {{1, "one"}, {2, "two"}};
        ASSERT_EQ(nullopt, get(m, 4));
    }
    
    TEST(ContainerTest, GetConst) {
        const auto m = std::map<int, std::string> {{1, "one"}, {3, "three"}};
        auto r = get(m, 1);
        static_assert(std::is_same_v<option<const std::string &>, decltype(r)>);
        ASSERT_EQ(&m.at(1), &*r);
    }
    
    TEST(ContainerTest, GetRValue) {
        auto m = std::map<int, std::string> {{1, "one"}};
        static_assert(std::is_same_v<option<std::string>, decltype(get(std::move(m), 1))>);
        ASSERT_EQ(std::string("one"), *get(std::move(m), 1));
    }
}
