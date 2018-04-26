#include <gtest/gtest.h>
#include "ufo/sequence/find_best_by.hpp"
#include <vector>
#include <type_traits>
#include <memory>
#include "ufo/placeholder.hpp"
#include "delete_copy.hpp"

using namespace ufo;

namespace {
    TEST(FindBestByTest, LValue) {
        auto v = std::vector<int> {13, 48, 91, 26, 59, 33};
        auto cw = container_wrapper(v);
        decltype(auto) r = cw | find_best_by(_ % 10, [](int n, int m) {return n > m;});
        static_assert(std::is_same_v<option<int &>, decltype(r)>);
        ASSERT_EQ(&v[4], &*r);
    }
    
    TEST(FindBestByTest, CopiedLValueNotChanged) {
        auto cw = container_wrapper(std::vector<int> {19, 22, 35});
        auto r = cw | find_best_by(_ % 10, [](int n, int m) {return n < m;});
        ASSERT_EQ(22, *r);
        ASSERT_EQ(19, *cw.next());
        ASSERT_EQ(22, *cw.next());
        ASSERT_EQ(35, *cw.next());
        ASSERT_FALSE(cw.next());
    }
    
    TEST(FindBestByTest, RValue) {
        decltype(auto) r = std::vector<int> {40, 13, 24} | find_best_by(-_, [](int n, int m) {return n > m;});
        static_assert(std::is_same_v<option<int>, decltype(r)>);
        ASSERT_EQ(13, *r);
    }
    
    TEST(FindBestByTest, Empty) {
        auto r = std::vector<int> {} | find_best_by(_ % 10, [](int n, int m) {return n < m;});
        ASSERT_FALSE(r);
    }

    TEST(FindBestByTest, OneValue) {
        auto r = std::vector<int> {56} | find_best_by(_ % 5, [](auto n, auto m) {throw "unreached"; return true;});
        ASSERT_EQ(56, *r);
    }

    TEST(FindBestByTest, ReferenceKeyNotCopied) {
        struct Uncopyable {
        public:
            int value;
            
            Uncopyable(int value) : value(value) {
            };
            
            ~Uncopyable() = default;
            
            Uncopyable(const Uncopyable &) = delete;
            
            Uncopyable(Uncopyable &&other) : value(other.value) {
                other.value = 0;
            }
            
            Uncopyable &operator=(const Uncopyable &) = delete;
            
            Uncopyable &operator=(Uncopyable &&other) {
                value = other.value;
                other.value = 0;
                return *this;
            }
        };
        auto values = std::vector<Uncopyable> {};
        values.emplace_back(20);
        values.emplace_back(40);
        values.emplace_back(30);
        values.emplace_back(10);
        auto r = std::vector<int> {3, 0, 1, 2} | find_best_by([&values](int n) -> decltype(auto) {return values[n];}, [](auto &x, auto &y) {return x.value > y.value;});
        ASSERT_EQ(1, *r);
        ASSERT_EQ(20, values[0].value);
        ASSERT_EQ(40, values[1].value);
        ASSERT_EQ(30, values[2].value);
        ASSERT_EQ(10, values[3].value);
    }

    TEST(FindBestByTest, SequenceNotCopied) {
        auto r = std::vector<int> {2, 3, 4} | test::delete_copy | find_best_by([](auto n) {return n;}, [](auto &x, auto &y) {return x > y;});
        ASSERT_EQ(4, *r);
    }
    
    TEST(FindBestByTest, FunctionNotCopied) {
        auto r = std::vector<int> {2, 3, 4} | find_best_by(test::delete_function_copy([](auto n) {return -n;}), test::delete_function_copy([](auto &x, auto &y) {return x > y;}));
        ASSERT_EQ(2, *r);
    }
    
    TEST(FindBestByTest, RValueReferenceKey) {
        auto r = std::vector<int> {19, 28, 37} | find_best_by([](auto &&x) -> decltype(auto) {return std::move(x);}, [](auto &x, auto &y) {return x > y;});
        ASSERT_EQ(37, *r);
    }
    
    TEST(FindBestByTest, MemberFunction) {
        struct Foo {
            int value_;
            
            Foo(int value) : value_(value) {
            }
            
            int value() const {
                return value_;
            }
        };
        auto r = std::vector<Foo> {4, 8 , 2} | find_best_by(&Foo::value, [](auto x, auto y) {return x > y;});
        ASSERT_EQ(8, r->value());
    }
}
