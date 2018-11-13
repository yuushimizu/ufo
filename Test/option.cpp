#include <gtest/gtest.h>
#include "ufo/option.hpp"
#include <memory>

using namespace ufo;

namespace {
    TEST(OptionTest, Value) {
        int x = 12;
        auto o = option<int>(x);
        static_assert(std::is_same_v<int &, decltype(*o)>);
        ASSERT_TRUE(o);
        ASSERT_NE(&x, &*o);
        ASSERT_EQ(12, *o);
        *o = 14;
        ASSERT_EQ(14, *o);
    }
    
    TEST(OptionTest, Ref) {
        int x = 10;
        auto o = option<int &>(x);
        static_assert(std::is_same_v<int &, decltype(*o)>);
        ASSERT_TRUE(o);
        ASSERT_EQ(&x, &*o);
    }
    
    TEST(OptionTest, ConstRef) {
        const int x = 10;
        auto o = option<const int &>(x);
        static_assert(std::is_same_v<const int &, decltype(*o)>);
        ASSERT_EQ(&x, &*o);
    }
    
    TEST(OptionTest, ValueMapLValue) {
        auto o = option<int>(10);
        decltype(auto) r = o.map([](int &n) {return n * 2.2;});
        static_assert(std::is_same_v<option<double>, decltype(r)>);
        ASSERT_EQ(10, *o);
        ASSERT_DOUBLE_EQ(22, *r);
    }
    
    TEST(OptionTest, ValueMapLValueNullopt) {
        auto o = option<int> {};
        ASSERT_EQ(nullopt, o.map([](int &n) {return n * 2;}));
    }
    
    TEST(OptionTest, ValueMapConstLValue) {
        const auto o = option<int>(5);
        decltype(auto) r = o.map([](const int &n) {return n * 2;});
        static_assert(std::is_same_v<option<int>, decltype(r)>);
        ASSERT_EQ(5, *o);
        ASSERT_EQ(10, *r);
    }
    
    TEST(OptionTest, ValueMapConstLValueNullopt) {
        const auto o = option<int> {};
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
        auto o = option<int &>(x);
        decltype(auto) r = o.map([](int &n) {return n * 2;});
        static_assert(std::is_same_v<option<int>, decltype(r)>);
        ASSERT_EQ(10, *o);
        ASSERT_EQ(20, *r);
    }
    
    TEST(OptionTest, RefMapLValueNullopt) {
        auto o = option<int &> {};
        ASSERT_EQ(nullopt, o.map([](int &n) {return n * 2;}));
    }
    
    TEST(OptionTest, RefMapConstLValue) {
        int x = 5;
        const auto o = option<int &>(x);
        decltype(auto) r = o.map([](int &n) {return n * 2;});
        static_assert(std::is_same_v<option<int>, decltype(r)>);
        ASSERT_EQ(5, *o);
        ASSERT_EQ(10, *r);
    }
    
    TEST(OptionTest, RefMapConstLValueNullopt) {
        const auto o = option<int &> {};
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
        auto v = std::vector<int> {10, 20, 30};
        decltype(auto) r = option<int>(0).map([&v](int i) -> decltype(auto) {return v[i];});
        static_assert(std::is_same_v<option<int &>, decltype(r)>);
        ASSERT_EQ(&v[0], &*r);
    }
    
    TEST(OptionTest, ValueMapMemberFunction) {
        struct Foo {
            int value() {
                return 123;
            }
        };
        decltype(auto) r = option<Foo>(Foo {}).map(&Foo::value);
        ASSERT_EQ(123, *r);
    }
    TEST(OptionTest, RefMapMemberFunction) {
        struct Foo {
            int value() {
                return 123;
            }
        };
        auto foo = Foo {};
        decltype(auto) r = option<Foo &>(foo).map(&Foo::value);
        ASSERT_EQ(123, *r);
    }
    
    TEST(OptionTest, ValueAndThenLValue) {
        auto o = option<int>(10);
        decltype(auto) r = o.and_then([](int &n) {return option(n * 2.2);});
        static_assert(std::is_same_v<option<double>, decltype(r)>);
        ASSERT_EQ(10, *o);
        ASSERT_DOUBLE_EQ(22, *r);
    }
    
    TEST(OptionTest, ValueAndThenLValueNullopt) {
        auto o = option<int> {};
        ASSERT_EQ(nullopt, o.and_then([](int &n) {return option(n * 2);}));
    }
    
    TEST(OptionTest, ValueAndThenConstLValue) {
        const auto o = option<int>(5);
        decltype(auto) r = o.and_then([](const int &n) {return option(n * 2);});
        static_assert(std::is_same_v<option<int>, decltype(r)>);
        ASSERT_EQ(5, *o);
        ASSERT_EQ(10, *r);
    }
    
    TEST(OptionTest, ValueAndThenConstLValueNullopt) {
        const auto o = option<int> {};
        ASSERT_EQ(nullopt, o.and_then([](const int &n) {return option(n * 2);}));
    }
    
    TEST(OptionTest, ValueAndThenRValue) {
        decltype(auto) r = option<int>(7).and_then([](int &&n) {return option(n * 3);});
        static_assert(std::is_same_v<option<int>, decltype(r)>);
        ASSERT_EQ(21, *r);
    }
    
    TEST(OptionTest, ValueAndThenRValueNullopt) {
        ASSERT_EQ(nullopt, option<int> {}.map([](int &&n) {return option(n * 3);}));
    }
    
    TEST(OptionTest, RefAndThenLValue) {
        int x = 10;
        auto o = option<int &>(x);
        decltype(auto) r = o.and_then([](int &n) {return option(n * 2);});
        static_assert(std::is_same_v<option<int>, decltype(r)>);
        ASSERT_EQ(10, *o);
        ASSERT_EQ(20, *r);
    }
    
    TEST(OptionTest, RefAndThenLValueNullopt) {
        auto o = option<int &> {};
        ASSERT_EQ(nullopt, o.and_then([](int &n) {return option(n * 2);}));
    }
    
    TEST(OptionTest, RefAndThenConstLValue) {
        int x = 5;
        const auto o = option<int &>(x);
        decltype(auto) r = o.and_then([](int &n) {return option(n * 2);});
        static_assert(std::is_same_v<option<int>, decltype(r)>);
        ASSERT_EQ(5, *o);
        ASSERT_EQ(10, *r);
    }
    
    TEST(OptionTest, RefAndThenConstLValueNullopt) {
        const auto o = option<int &> {};
        ASSERT_EQ(nullopt, o.map([](int &n) {return option(n * 2);}));
    }
    
    TEST(OptionTest, RefAndThenRValue) {
        int x = 7;
        decltype(auto) r = option<int &>(x).and_then([](int &n) {return option(n * 3);});
        static_assert(std::is_same_v<option<int>, decltype(r)>);
        ASSERT_EQ(21, *r);
    }
    
    TEST(OptionTest, RefAndThenRValueNullopt) {
        ASSERT_EQ(nullopt, option<int &> {}.and_then([](int &n) {return option(n * 3);}));
    }
    
    TEST(OptionTest, ValueAndThenMemberFunction) {
        struct Foo {
            auto value() {
                return option(123);
            }
        };
        decltype(auto) r = option<Foo>(Foo {}).and_then(&Foo::value);
        ASSERT_EQ(123, *r);
    }
    
    TEST(OptionTest, RefAndThenMemberFunction) {
        struct Foo {
            auto value() {
                return option(123);
            }
        };
        auto foo = Foo {};
        decltype(auto) r = option<Foo &>(foo).and_then(&Foo::value);
        ASSERT_EQ(123, *r);
    }
    
    TEST(OptionTest, UnwrapOrLValue) {
        auto o = option<int>(123);
        decltype(auto) r = o.unwrap_or(10);
        static_assert(std::is_same_v<int, decltype(r)>);
        ASSERT_EQ(123, r);
    }
    
    TEST(OptionTest, UnwrapOrLValueNullopt) {
        auto o = option<int> {};
        decltype(auto) r = o.unwrap_or(10);
        ASSERT_EQ(10, r);
    }
    
    TEST(OptionTest, UnwrapOrRValue) {
        decltype(auto) r = option<std::unique_ptr<int>>(std::make_unique<int>(5)).unwrap_or(std::make_unique<int>(84));
        static_assert(std::is_same_v<std::unique_ptr<int>, decltype(r)>);
        ASSERT_EQ(5, *r);
    }
    
    TEST(OptionTest, UnwrapOrRValueNullopt) {
        decltype(auto) r = option<std::unique_ptr<int>> {}.unwrap_or(std::make_unique<int>(84));
        static_assert(std::is_same_v<std::unique_ptr<int>, decltype(r)>);
        ASSERT_EQ(84, *r);
    }
    
    TEST(OptionTest, UnwrapOrRef) {
        int x = 10;
        auto o = option<int &>(x);
        decltype(auto) r = o.unwrap_or(5);
        static_assert(std::is_same_v<int, decltype(r)>);
        ASSERT_EQ(x, r);
    }
    
    TEST(OptionTest, UnwrapOrRefNullopt) {
        auto o = option<int &> {};
        int r = o.unwrap_or(42);
        ASSERT_EQ(42, r);
    }
    
    TEST(OptionTest, OptionOfOptionFalse) {
        auto o = option<option<int>> {};
        ASSERT_FALSE(o);
    }
    
    TEST(OptionTest, OptionOfOptionTrueFalse) {
        auto o = option<option<int>>(option<int> {});
        ASSERT_TRUE(o);
        ASSERT_FALSE(*o);
    }
    
    TEST(OptionTest, OptionOfOptionTrueTrue) {
        auto o = option<option<int>>(5);
        ASSERT_TRUE(o);
        ASSERT_TRUE(*o);
        ASSERT_EQ(5, **o);
    }
    
    TEST(OptionTest, ClassTemplateTypeDeduction) {
        static_assert(std::is_same_v<option<int>, decltype(option(1))>);
        int x = 12;
        static_assert(std::is_same_v<option<int>, decltype(option(x))>);
        const int y = 30;
        static_assert(std::is_same_v<option<int>, decltype(option(y))>);
        int z = 42;
        static_assert(std::is_same_v<option<int>, decltype(option(std::move(z)))>);
        SUCCEED();
    }
    
    TEST(OptionTest, ForwardOption) {
        static_assert(std::is_same_v<option<int>, forward_option_t<int>>);
        auto o1 = forward_option(1);
        static_assert(std::is_same_v<option<int>, decltype(o1)>);
        ASSERT_EQ(1, *o1);
        int x = 12;
        static_assert(std::is_same_v<option<int &>, forward_option_t<int &>>);
        auto o2 = forward_option(x);
        static_assert(std::is_same_v<option<int &>, decltype(o2)>);
        ASSERT_EQ(&x, &*o2);
        const int y = 34;
        static_assert(std::is_same_v<option<const int &>, forward_option_t<const int &>>);
        auto o3 = forward_option(y);
        static_assert(std::is_same_v<option<const int &>, decltype(o3)>);
        ASSERT_EQ(&y, &*o3);
        int z = 42;
        static_assert(std::is_same_v<option<int>, forward_option_t<int &&>>);
        auto o4 = forward_option(std::move(z));
        static_assert(std::is_same_v<option<int>, decltype(o4)>);
        ASSERT_EQ(42, *o4);
    }
}
