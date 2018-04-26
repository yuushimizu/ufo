#include <gtest/gtest.h>
#include "ufo/holder.hpp"

using namespace ufo;

namespace {
    TEST(HolderTest, Value) {
        auto h = holder<int>(123);
        static_assert(std::is_same_v<int &, decltype(h.get())>);
        static_assert(std::is_same_v<const int &, decltype(std::as_const(h).get())>);
        static_assert(std::is_same_v<int, decltype(std::move(h).get())>);
        ASSERT_EQ(123, h.get());
    }
    
    TEST(HolderTest, LValueReference) {
        int n = 100;
        auto h = holder<int &>(n);
        static_assert(std::is_same_v<int &, decltype(h.get())>);
        static_assert(std::is_same_v<int &, decltype(std::as_const(h).get())>);
        static_assert(std::is_same_v<int &, decltype(std::move(h).get())>);
        ASSERT_EQ(&n, &h.get());
    }
    
    TEST(HolderTest, ConstLValueReference) {
        const int n = 42;
        auto h = holder<const int &>(n);
        static_assert(std::is_same_v<const int &, decltype(h.get())>);
        static_assert(std::is_same_v<const int &, decltype(std::as_const(h).get())>);
        static_assert(std::is_same_v<const int &, decltype(std::move(h).get())>);
        ASSERT_EQ(&n, &h.get());
    }
    
    TEST(HolderTest, RValueReference) {
        auto h = holder<int &&>(50);
        static_assert(std::is_same_v<int &, decltype(h.get())>);
        static_assert(std::is_same_v<const int &, decltype(std::as_const(h).get())>);
        static_assert(std::is_same_v<int, decltype(std::move(h).get())>);
        ASSERT_EQ(50, h.get());
        static_assert(std::is_same_v<holder<int>, holder<int &&>>);
    }
}
