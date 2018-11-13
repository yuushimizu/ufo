#include <gtest/gtest.h>
#include "ufo/sequence/foreach.hpp"
#include <vector>
#include <type_traits>
#include <functional>
#include "delete_copy.hpp"

using namespace ufo;

namespace {
    TEST(ForeachTest, LValue) {
        auto v = std::vector<int> {10, 20, 30};
        auto r = std::vector<std::reference_wrapper<int>> {};
        v | foreach([&r](int &n) {
            r.push_back(n);
        });
        ASSERT_EQ(&v[0], &r[0].get());
        ASSERT_EQ(&v[1], &r[1].get());
        ASSERT_EQ(&v[2], &r[2].get());
        ASSERT_EQ(3, r.size());
    }
    
    TEST(ForeachTest, RValue) {
        auto r = std::vector<int> {};
        std::vector<int> {10, 20, 30} | foreach([&r](int n) {
            r.push_back(n);
        });
        ASSERT_EQ((std::vector<int> {10, 20, 30}), r);
    }
    
    TEST(ForeachTest, Twice) {
        auto v = std::vector<int> {10, 20, 30};
        std::vector<int> r {};
        v | foreach([&r](int n) {
            r.push_back(n);
        });
        v | foreach([&r](int n) {
            r.push_back(n);
        });
        ASSERT_EQ((std::vector<int> {10, 20, 30, 10, 20, 30}), r);
    }
    
    TEST(ForeachTest, SequenceNotCopied) {
        auto r = std::vector<int> {};
        std::vector<int> {10, 20, 30} | test::delete_copy() | foreach([&r](int n) {
            r.push_back(n);
        });
        ASSERT_EQ((std::vector<int> {10, 20, 30}), r);
    }
    
    TEST(ForeachTest, FunctionNotCopied) {
        auto r = std::vector<int> {};
        std::vector<int> {10, 20, 30} | foreach(test::delete_function_copy([&r](auto n) {
            r.push_back(n);
        }));
        ASSERT_EQ((std::vector<int> {10, 20, 30}), r);
    }
    
    TEST(ForeachTest, MemberFunction) {
        struct Foo {
            int value;
            
            Foo(int value) : value(value) {
            }
            
            void do_doubling() {
                value *= 2;
            }
        };
        auto v = std::vector<Foo> {1, 3, 5};
        v | foreach(&Foo::do_doubling);
        ASSERT_EQ(2, v[0].value);
        ASSERT_EQ(6, v[1].value);
        ASSERT_EQ(10, v[2].value);
    }
}

