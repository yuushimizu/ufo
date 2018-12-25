#include <gtest/gtest.h>
#include "ufo/box.hpp"

namespace {
    TEST(BoxTest, ForwardBoxMoveRValueToNonReference) {
        auto b = ufo::forward_box<std::unique_ptr<int>>(std::make_unique<int>(20));
        static_assert(std::is_same_v<ufo::box<std::unique_ptr<int>>, decltype(b)>);
        static_assert(std::is_same_v<std::unique_ptr<int> &, decltype(b.get())>);
        ASSERT_EQ(20, *b.get());
    }
    
    TEST(BoxTest, ForwardBoxMoveLValueToNonReference) {
        auto x = std::make_unique<int>(30);
        auto address = x.get();
        auto b = ufo::forward_box<std::unique_ptr<int>>(x);
        static_assert(std::is_same_v<ufo::box<std::unique_ptr<int>>, decltype(b)>);
        static_assert(std::is_same_v<std::unique_ptr<int> &, decltype(b.get())>);
        ASSERT_EQ(address, b.get().get());
        ASSERT_EQ(30, *b.get());
        ASSERT_NE(&x, &b.get());
    }
    
    TEST(BoxTest, ForwardBoxMoveRValueToRValue) {
        auto b = ufo::forward_box<std::unique_ptr<int> &&>(std::make_unique<int>(40));
        static_assert(std::is_same_v<ufo::box<std::unique_ptr<int>>, decltype(b)>);
        static_assert(std::is_same_v<std::unique_ptr<int> &, decltype(b.get())>);
        ASSERT_EQ(40, *b.get());
    }
    
    TEST(BoxTest, ForwardBoxMoveLValueToRValue) {
        auto x = std::make_unique<int>(11);
        auto address = x.get();
        auto b = ufo::forward_box<std::unique_ptr<int> &&>(x);
        static_assert(std::is_same_v<ufo::box<std::unique_ptr<int>>, decltype(b)>);
        static_assert(std::is_same_v<std::unique_ptr<int> &, decltype(b.get())>);
        ASSERT_EQ(address, b.get().get());
        ASSERT_EQ(11, *b.get());
        ASSERT_NE(&x, &b.get());
    }
    
    TEST(BoxTest, ForwardBoxRefLValueToLValue) {
        auto x = std::make_unique<int>(13);
        auto b = ufo::forward_box<std::unique_ptr<int> &>(x);
        static_assert(std::is_same_v<ufo::box<std::unique_ptr<int> &>, decltype(b)>);
        static_assert(std::is_same_v<std::unique_ptr<int> &, decltype(b.get())>);
        ASSERT_EQ(&x, &b.get());
    }
}
