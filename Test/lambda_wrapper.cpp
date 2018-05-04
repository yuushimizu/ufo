#include <gtest/gtest.h>
#include "ufo/lambda_wrapper.hpp"
#include <type_traits>
#include <memory>

using namespace ufo;

namespace {
    TEST(LambdaWrapperTest, ConstLValue) {
        const auto l = lambda_wrapper([](int n, int m) {return n + m * 2;});
        static_assert(std::is_same_v<int, decltype(l(1, 2))>);
        ASSERT_EQ(5, l(1, 2));
    }
    
    TEST(LambdaWrapperTest, LValue) {
        int x = 10;
        auto l = lambda_wrapper([&x](int n) mutable {x += n;});
        l(5);
        ASSERT_EQ(15, x);
    }
    
    TEST(LambdaWrapperTest, RValue) {
        ASSERT_EQ(8, lambda_wrapper([](int n) {return n * 2;})(4));
    }
    
    TEST(LambdaWrapperTest, CopyConstructor) {
        int x = 10;
        auto l = lambda_wrapper([&x, n = 10](int m) mutable {x += n * m;});
        auto l2 = l;
        l2(3);
        ASSERT_EQ(40, x);
    }
    
    TEST(LambdaWrapperTest, MoveConstructor) {
        int x = 10;
        auto l = lambda_wrapper([&x, n = std::make_unique<int>(10)](int m) mutable {x += *n * m;});
        auto l2 = std::move(l);
        l2(3);
        ASSERT_EQ(40, x);
    }
    
    TEST(LambdaWrapperTest, CopyAssign) {
        auto f = [](int n) {return [n](int m) {return n * m;};};
        auto l = lambda_wrapper(f(5));
        auto l2 = lambda_wrapper(f(10));
        l2 = l;
        ASSERT_EQ(15, l2(3));
    }
    
    TEST(LambdaWrapperTest, MoveAssign) {
        auto f = [](int n) {return [n = std::make_unique<int>(n)](int m) {return *n * m;};};
        auto l = lambda_wrapper(f(5));
        auto l2 = lambda_wrapper(f(10));
        l2 = std::move(l);
        ASSERT_EQ(15, l2(3));
    }
}
