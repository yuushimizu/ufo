#include <gtest/gtest.h>
#include "ufo/partial.hpp"

using ufo::_;

namespace {
    TEST(PartialTest, Identity) {
        ASSERT_EQ(13, _(13));
    }
    
    TEST(PartialTest, LogicalNot) {
        auto f = !_;
        ASSERT_EQ(!false, f(false));
        ASSERT_EQ(!true, f(true));
    }
    
    TEST(PartialTest, AddressOf) {
        int x = 10;
        auto f = &_;
        ASSERT_EQ(&x, f(x));
    }
    
    TEST(PartialTest, Dereference) {
        int x = 5;
        int *p = &x;
        auto f = *_;
        ASSERT_EQ(*p, f(p));
        ASSERT_EQ(&*p, &f(p));
        f(p) = 3;
        ASSERT_EQ(3, x);
    }
    
    TEST(PartialTest, UnaryPlus) {
        struct X {
            int operator+() const {
                return 100;
            }
        } x;
        auto f = +_;
        ASSERT_EQ(+x, f(x));
    }
    
    TEST(PartialTest, Preincrement) {
        int x = 10;
        auto f = ++_;
        ASSERT_EQ(11, f(x));
        ASSERT_EQ(11, x);
    }
    
    TEST(PartialTest, Postincrement) {
        int x = 10;
        auto f = _++;
        ASSERT_EQ(10, f(x));
        ASSERT_EQ(11, x);
    }
    
    TEST(PartialTest, UnaryMinus) {
        auto f = -_;
        ASSERT_EQ(-42, f(42));
    }
    
    TEST(PartialTest, Predecrement) {
        int x = 10;
        auto f = --_;
        ASSERT_EQ(9, f(x));
        ASSERT_EQ(9, x);
    }
    
    TEST(PartialTest, Postdecrement) {
        int x = 10;
        auto f = _--;
        ASSERT_EQ(10, f(x));
        ASSERT_EQ(9, x);
    }
    
    TEST(PartialTest, BitwiseNot) {
        auto f = ~_;
        ASSERT_EQ(~50, f(50));
    }
    
    TEST(PartialTest, NotEqualLeft) {
        auto f = (_ != 10);
        ASSERT_TRUE(f(8));
        ASSERT_FALSE(f(10));
    }
    
    TEST(PartialTest, NotEqualRight) {
        auto f = (55 != _);
        ASSERT_TRUE(f(66));
        ASSERT_FALSE(f(55));
    }
    
    TEST(PartialTest, ModuloLeft) {
        auto f = (_ % 2);
        ASSERT_EQ(1, f(7));
    }
    
    TEST(PartialTest, ModuloRight) {
        auto f = (5 % _);
        ASSERT_EQ(2, f(3));
    }
    
    TEST(PartialTest, BitwiseAndLeft) {
        auto f = (_ & 2);
        ASSERT_EQ(100 & 2, f(100));
    }
    
    TEST(PartialTest, BitwiseAndRight) {
        auto f = (3 & _);
        ASSERT_EQ(3 & 5, f(5));
    }
    
    TEST(PartialTest, PlusLeft) {
        auto f = (_ + 1);
        ASSERT_EQ(15, f(14));
    }
    
    TEST(PartialTest, PlusRight) {
        auto f = (10 + _);
        ASSERT_EQ(23, f(13));
    }
    
    TEST(PartialTest, EqualLeft) {
        auto f = (_ == 10);
        ASSERT_TRUE(f(10));
        ASSERT_FALSE(f(11));
    }
    
    TEST(PartialTest, EqualRight) {
        auto f = (24 == _);
        ASSERT_TRUE(f(24));
        ASSERT_FALSE(f(20));
    }
    
    TEST(PartialTest, NotAndEqual) {
        auto f = !(_ == 3);
        ASSERT_TRUE(f(4));
        ASSERT_FALSE(f(3));
    }
    
    TEST(PartialTest, ModuloAndEqual) {
        auto f = (_ % 2 == 0);
        ASSERT_TRUE(f(8));
        ASSERT_FALSE(f(11));
    }
    
    TEST(PartialTest, NestedEqual) {
        auto f = (_ == 10 == false == true);
        ASSERT_TRUE(f(12));
        ASSERT_FALSE(f(10));
    }
}

