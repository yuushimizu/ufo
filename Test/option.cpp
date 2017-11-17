#include <gtest/gtest.h>
#include "ufo/option.hpp"
#include <memory>

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
        decltype(auto) r = o.map([](int &n) {return n * 2.2;});
        static_assert(std::is_same_v<option<double>, decltype(r)>);
        ASSERT_EQ(10, *o);
        ASSERT_DOUBLE_EQ(22, *r);
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
        Foo foo {};
        decltype(auto) r = option<Foo &>(foo).map(&Foo::value);
        ASSERT_EQ(123, *r);
    }
    
    TEST(OptionTest, ValueAndThenLValue) {
        option<int> o(10);
        decltype(auto) r = o.and_then([](int &n) {return make_option(n * 2.2);});
        static_assert(std::is_same_v<option<double>, decltype(r)>);
        ASSERT_EQ(10, *o);
        ASSERT_DOUBLE_EQ(22, *r);
    }
     
    TEST(OptionTest, ValueAndThenLValueNullopt) {
        option<int> o {};
        ASSERT_EQ(nullopt, o.and_then([](int &n) {return make_option(n * 2);}));
    }
    
    TEST(OptionTest, ValueAndThenConstLValue) {
        const option<int> o(5);
        decltype(auto) r = o.and_then([](const int &n) {return make_option(n * 2);});
        static_assert(std::is_same_v<option<int>, decltype(r)>);
        ASSERT_EQ(5, *o);
        ASSERT_EQ(10, *r);
    }
     
    TEST(OptionTest, ValueAndThenConstLValueNullopt) {
        const option<int> o {};
        ASSERT_EQ(nullopt, o.and_then([](const int &n) {return make_option(n * 2);}));
    }

    TEST(OptionTest, ValueAndThenRValue) {
        decltype(auto) r = option<int>(7).and_then([](int &&n) {return make_option(n * 3);});
        static_assert(std::is_same_v<option<int>, decltype(r)>);
        ASSERT_EQ(21, *r);
    }
    
    TEST(OptionTest, ValueAndThenRValueNullopt) {
        ASSERT_EQ(nullopt, option<int> {}.map([](int &&n) {return make_option(n * 3);}));
    }
    
    TEST(OptionTest, RefAndThenLValue) {
        int x = 10;
        option<int &> o(x);
        decltype(auto) r = o.and_then([](int &n) {return make_option(n * 2);});
        static_assert(std::is_same_v<option<int>, decltype(r)>);
        ASSERT_EQ(10, *o);
        ASSERT_EQ(20, *r);
    }
    
    TEST(OptionTest, RefAndThenLValueNullopt) {
        option<int &> o {};
        ASSERT_EQ(nullopt, o.and_then([](int &n) {return make_option(n * 2);}));
    }

    TEST(OptionTest, RefAndThenConstLValue) {
        int x = 5;
        const option<int &> o(x);
        decltype(auto) r = o.and_then([](int &n) {return make_option(n * 2);});
        static_assert(std::is_same_v<option<int>, decltype(r)>);
        ASSERT_EQ(5, *o);
        ASSERT_EQ(10, *r);
    }
    
    TEST(OptionTest, RefAndThenConstLValueNullopt) {
        const option<int &> o {};
        ASSERT_EQ(nullopt, o.map([](int &n) {return make_option(n * 2);}));
    }
    
    TEST(OptionTest, RefAndThenRValue) {
        int x = 7;
        decltype(auto) r = option<int &>(x).and_then([](int &n) {return make_option(n * 3);});
        static_assert(std::is_same_v<option<int>, decltype(r)>);
        ASSERT_EQ(21, *r);
    }
    
    TEST(OptionTest, RefAndThenRValueNullopt) {
        ASSERT_EQ(nullopt, option<int &> {}.and_then([](int &n) {return make_option(n * 3);}));
    }
    
    TEST(OptionTest, ValueAndThenMemberFunction) {
        struct Foo {
            auto value() {
                return make_option(123);
            }
        };
        decltype(auto) r = option<Foo>(Foo {}).and_then(&Foo::value);
        ASSERT_EQ(123, *r);
    }
    
    TEST(OptionTest, RefAndThenMemberFunction) {
        struct Foo {
            auto value() {
                return make_option(123);
            }
        };
        Foo foo {};
        decltype(auto) r = option<Foo &>(foo).and_then(&Foo::value);
        ASSERT_EQ(123, *r);
    }
    
    TEST(OptionTest, UnwrapOrLValue) {
        option<int> o(123);
        decltype(auto) r = o.unwrap_or(10);
        static_assert(std::is_same_v<int, decltype(r)>);
        ASSERT_EQ(123, r);
    }
    
    TEST(OptionTest, UnwrapOrLValueNullopt) {
        option<int> o {};
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
        option<int &> o(x);
        int y = 5;
        decltype(auto) r = o.unwrap_or(y);
        static_assert(std::is_same_v<int &, decltype(r)>);
        ASSERT_EQ(&x, &r);
    }
    
    TEST(OptionTest, UnwrapOrRefNullopt) {
        option<int &> o {};
        int y = 42;
        int &r = o.unwrap_or(y);
        ASSERT_EQ(&y, &r);
    }
    
    TEST(OptionTest, Deref) {
        int x = 123;
        option<int &> o(x);
        decltype(auto) r = o.deref();
        static_assert(std::is_same_v<option<int>, decltype(r)>);
        ASSERT_EQ(123, *r);
        ASSERT_NE(&x, &*r);
    }
    
    TEST(OptionTest, DerefNullopt) {
        option<int &> o {};
        ASSERT_EQ(nullopt, o.deref());
    }
    
    TEST(OptionTest, DerefConst) {
        const int x = 42;
        option<const int &> o(x);
        decltype(auto) r = o.deref();
        static_assert(std::is_same_v<option<int>, decltype(r)>);
        ASSERT_EQ(42, *r);
        ASSERT_NE(&x, &*r);
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
