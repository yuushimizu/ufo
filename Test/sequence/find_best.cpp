#include <gtest/gtest.h>
#include "ufo/sequence/find_best.hpp"
#include <vector>
#include <type_traits>
#include <memory>
#include "delete_copy.hpp"

using namespace ufo;

namespace {
    TEST(FindBestTest, LValue) {
        auto v = std::vector<int> {7, 3, 1, 8, 9, 2};
        auto cw = container_wrapper(v);
        decltype(auto) r = cw | find_best([](int x, int y) {return x > y;});
        static_assert(std::is_same_v<option<int &>, decltype(r)>);
        ASSERT_EQ(&v[4], &*r);
    }
    
    TEST(FindBestTest, CopiedLValueNotChanged) {
        auto cw = container_wrapper(std::vector<int> {19, 22, 35});
        auto r = cw | find_best([](int n, int m) {return n % 10 > m % 10;});
        ASSERT_EQ(19, *r);
        ASSERT_EQ(19, *cw.next());
        ASSERT_EQ(22, *cw.next());
        ASSERT_EQ(35, *cw.next());
        ASSERT_FALSE(cw.next());
    }
    
    TEST(FindBestTest, RValue) {
        decltype(auto) r = std::vector<int> {40, 13, 24} | find_best([](auto n, auto m) {return n < m;});
        static_assert(std::is_same_v<option<int>, decltype(r)>);
        ASSERT_EQ(13, *r);
    }

    TEST(FindBestTest, Empty) {
        auto r = std::vector<int> {} | find_best([](auto n, auto m) {return n > m;});
        ASSERT_FALSE(r);
    }

    TEST(FindBestTest, OneValue) {
        auto r = std::vector<int> {56} | find_best([](auto n, auto m) {throw "unreached"; return true;});
        ASSERT_EQ(56, *r);
    }
    
    TEST(FindBestTest, SequenceNotCopied) {
        auto r = std::vector<int> {2, 3, 4} | test::delete_copy | find_best([](auto n, auto m) {return n > m;});
        ASSERT_EQ(4, *r);
    }
    
    TEST(FindBestTest, FunctionNotCopied) {
        auto r = std::vector<int> {2, 3, 4} | find_best(test::delete_function_copy([](auto n, auto m) {return n < m;}));
        ASSERT_EQ(2, *r);
    }
}
