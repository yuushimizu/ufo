#include <gtest/gtest.h>
#include "ufo/sequence/first.hpp"
#include <vector>
#include "ufo/sequence/container_wrapper.hpp"
#include "delete_copy.hpp"

using namespace ufo;

namespace {
    TEST(FirstTest, FromLValue) {
        auto v = std::vector<int> {1, 2, 3, 4, 5};
        auto cw = container_wrapper(v);
        decltype(auto) r = cw | first();
        static_assert(std::is_same_v<option<int &>, decltype(r)>);
        ASSERT_EQ(&v[0], &*r);
    }
    
    TEST(FirstTest, FromRValue) {
        decltype(auto) r = std::vector<int> {10, 20, 30} | first();
        static_assert(std::is_same_v<option<int>, decltype(r)>);
        ASSERT_EQ(10, *r);
    }
    
    TEST(FirstTest, Empty) {
        decltype(auto) r = std::vector<int> {} | first();
        ASSERT_EQ(nullopt, r);
    }
    
    TEST(FirstTest, SequenceNotCopied) {
        ASSERT_EQ(3, *(std::vector<int> {3, 4, 5} | test::delete_copy() | first()));
    }
    
    TEST(FirstTest, Noncopyable) {
        auto v = std::vector<std::unique_ptr<int>> {};
        v.push_back(std::make_unique<int>(12));
        v.push_back(std::make_unique<int>(34));
        static_assert(std::is_same_v<option<std::unique_ptr<int>>, decltype(std::move(v) | first())>);
        ASSERT_EQ(12, **(std::move(v) | first()));
    }
}
