#include <gtest/gtest.h>
#include "ufo/option.hpp"
#include "ufo/TD.hpp"

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
}
