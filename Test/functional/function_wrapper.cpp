#include <gtest/gtest.h>
#include "ufo/functional/function_wrapper.hpp"
#include <type_traits>
#include <memory>

using namespace ufo;

namespace {
    TEST(FunctionWrapperTest, ConstLValue) {
        const auto f = function_wrapper([](int n, int m) {return n + m * 2;});
        static_assert(std::is_same_v<int, decltype(f(1, 2))>);
        ASSERT_EQ(5, f(1, 2));
    }
    
    TEST(FunctionWrapperTest, LValue) {
        int x = 10;
        auto f = function_wrapper([&x](int n) mutable {x += n;});
        f(5);
        ASSERT_EQ(15, x);
    }
    
    TEST(FunctionWrapperTest, RValue) {
        ASSERT_EQ(8, function_wrapper([](int n) {return n * 2;})(4));
    }
    
    TEST(FunctionWrapperTest, CopyConstructor) {
        int x = 10;
        auto f = function_wrapper([&x, n = 10](int m) mutable {x += n * m;});
        auto f2 = f;
        f2(3);
        ASSERT_EQ(40, x);
    }
    
    TEST(FunctionWrapperTest, MoveConstructor) {
        int x = 10;
        auto f = function_wrapper([&x, n = std::make_unique<int>(10)](int m) mutable {x += *n * m;});
        auto f2 = std::move(f);
        f2(3);
        ASSERT_EQ(40, x);
    }
    
    TEST(FunctionWrapperTest, CopyAssign) {
        auto make_fn = [](int n) {return [n](int m) {return n * m;};};
        auto f = function_wrapper(make_fn(5));
        auto f2 = function_wrapper(make_fn(10));
        f2 = f;
        ASSERT_EQ(15, f2(3));
    }
    
    TEST(FunctionWrapperTest, MoveAssign) {
        auto make_fn = [](int n) {return [n = std::make_unique<int>(n)](int m) {return *n * m;};};
        auto f = function_wrapper(make_fn(5));
        auto f2 = function_wrapper(make_fn(10));
        f2 = std::move(f);
        ASSERT_EQ(15, f2(3));
    }
}
