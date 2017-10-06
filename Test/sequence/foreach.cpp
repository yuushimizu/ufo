#include <gtest/gtest.h>
#include "ufo/sequence/foreach.hpp"
#include <vector>
#include <type_traits>
#include <functional>
#include "delete_copy.hpp"

using namespace ufo;

namespace {
    TEST(ForeachTest, LValue) {
        std::vector<int> v {10, 20, 30};
        std::vector<std::reference_wrapper<int>> r {};
        v | foreach([&r](int &n) {
            r.push_back(n);
        });
        ASSERT_EQ(&v[0], &r[0].get());
        ASSERT_EQ(&v[1], &r[1].get());
        ASSERT_EQ(&v[2], &r[2].get());
        ASSERT_EQ(3, r.size());
    }
    
    TEST(ForeachTest, RValue) {
        std::vector<int> r {};
        std::vector<int> {10, 20, 30} | test::delete_copy | foreach([&r](int n) {
            r.push_back(n);
        });
        ASSERT_EQ((std::vector<int> {10, 20, 30}), r);
    }
    
    TEST(ForeachTest, Twice) {
        std::vector<int> v {10, 20, 30};
        std::vector<int> r {};
        v | foreach([&r](int n) {
            r.push_back(n);
        });
        v | foreach([&r](int n) {
            r.push_back(n);
        });
        ASSERT_EQ((std::vector<int> {10, 20, 30, 10, 20, 30}), r);
    }
    
    TEST(ForeachTest, FunctionNotCopied) {
        std::vector<int> r {};
        std::vector<int> {10, 20, 30} | foreach(test::delete_function_copy([&r](auto n) {
            r.push_back(n);
        }));
        ASSERT_EQ((std::vector<int> {10, 20, 30}), r);
    }
}

