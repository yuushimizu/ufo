#include <gtest/gtest.h>
#include "ufo/option.hpp"

using namespace ufo;

namespace {
    TEST(OptionTest, Value) {
        int x = 12;
        option<int> o(x);
        static_assert(std::is_same_v<int &, decltype(*o)>);
        ASSERT_TRUE(o);
        ASSERT_NE(&x, &*o);
        ASSERT_EQ(12, *o);
        *o = 14;
        ASSERT_EQ(14, *o);
    }
    
    TEST(OptionTest, Ref) {
        int x = 10;
        option<int &> o(x);
        static_assert(std::is_same_v<int &, decltype(*o)>);
        ASSERT_TRUE(o);
        ASSERT_EQ(&x, &*o);
    }
    
    TEST(OptionTest, ConstRef) {
        const int x = 10;
        option<const int &> o(x);
        static_assert(std::is_same_v<const int &, decltype(*o)>);
        ASSERT_EQ(&x, &*o);
    }
    
    TEST(OptionTest, ValueMapLValue) {
        option<int> o(10);
        decltype(auto) r = o.map([](int &n) {return n * 2;});
        static_assert(std::is_same_v<option<int>, decltype(r)>);
        ASSERT_EQ(10, *o);
        ASSERT_EQ(20, *r);
    }
    
    TEST(OptionTest, ValueMapLValueNullopt) {
        option<int> o {};
        ASSERT_EQ(nullopt, o.map([](int &n) {return n * 2;}));
    }
    
    TEST(OptionTest, ValueMapConstLValue) {
        const option<int> o(5);
        decltype(auto) r = o.map([](const int &n) {return n * 2;});
        static_assert(std::is_same_v<option<int>, decltype(r)>);
        ASSERT_EQ(5, *o);
        ASSERT_EQ(10, *r);
    }
    
    TEST(OptionTest, ValueMapConstLValueNullopt) {
        const option<int> o {};
        ASSERT_EQ(nullopt, o.map([](const int &n) {return n * 2;}));
    }
    
    TEST(OptionTest, ValueMapRValue) {
        decltype(auto) r = option<int>(7).map([](int &&n) {return n * 3;});
        static_assert(std::is_same_v<option<int>, decltype(r)>);
        ASSERT_EQ(21, *r);
    }
    
    TEST(OptionTest, ValueMapRValueNullopt) {
        ASSERT_EQ(nullopt, option<int> {}.map([](int &&n) {return n * 3;}));
    }
    
    TEST(OptionTest, RefMapLValue) {
        int x = 10;
        option<int &> o(x);
        decltype(auto) r = o.map([](int &n) {return n * 2;});
        static_assert(std::is_same_v<option<int>, decltype(r)>);
        ASSERT_EQ(10, *o);
        ASSERT_EQ(20, *r);
    }
    
    TEST(OptionTest, RefMapLValueNullopt) {
        option<int &> o {};
        ASSERT_EQ(nullopt, o.map([](int &n) {return n * 2;}));
    }
    
    TEST(OptionTest, RefMapConstLValue) {
        int x = 5;
        const option<int &> o(x);
        decltype(auto) r = o.map([](int &n) {return n * 2;});
        static_assert(std::is_same_v<option<int>, decltype(r)>);
        ASSERT_EQ(5, *o);
        ASSERT_EQ(10, *r);
    }
    
    TEST(OptionTest, RefMapConstLValueNullopt) {
        const option<int &> o {};
        ASSERT_EQ(nullopt, o.map([](int &n) {return n * 2;}));
    }
    
    TEST(OptionTest, RefMapRValue) {
        int x = 7;
        decltype(auto) r = option<int &>(x).map([](int &n) {return n * 3;});
        static_assert(std::is_same_v<option<int>, decltype(r)>);
        ASSERT_EQ(21, *r);
    }
    
    TEST(OptionTest, RefMapRValueNullopt) {
        ASSERT_EQ(nullopt, option<int &> {}.map([](int &n) {return n * 3;}));
    }
    
    TEST(OptionTest, MapReturnRef) {
        std::vector<int> v {10, 20, 30};
        decltype(auto) r = option<int>(0).map([&v](int i) -> decltype(auto) {return v[i];});
        static_assert(std::is_same_v<option<int &>, decltype(r)>);
        ASSERT_EQ(&v[0], &*r);
    }
    
    TEST(OptionTest, OptionOfOptionFalse) {
        option<option<int>> o {};
        ASSERT_FALSE(o);
    }
    
    TEST(OptionTest, OptionOfOptionTrueFalse) {
        option<option<int>> o(option<int> {});
        ASSERT_TRUE(o);
        ASSERT_FALSE(*o);
    }
    
    TEST(OptionTest, OptionOfOptionTrueTrue) {
        option<option<int>> o(5);
        ASSERT_TRUE(o);
        ASSERT_TRUE(*o);
        ASSERT_EQ(5, **o);
    }
}
