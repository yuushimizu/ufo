#include <gtest/gtest.h>
#include "ufo/sequence/map.hpp"
#include <vector>
#include <type_traits>
#include "ufo/placeholder.hpp"
#include "delete_copy.hpp"

using namespace ufo;

namespace {
    TEST(MapTest, FromLValue) {
        auto v = std::vector<int> {10, 20, 30};
        auto cw = container_wrapper(v);
        auto r = cw | map(_ * 2);
        static_assert(std::is_same_v<option<int>, decltype(r.next())>);
        ASSERT_EQ(20, *r.next());
        ASSERT_EQ(40, *r.next());
        ASSERT_EQ(60, *r.next());
        ASSERT_FALSE(r.next());
    }
    
    TEST(MapTest, CopiedLValueNotChanged) {
        auto cw = container_wrapper(std::vector<int> {10, 20});
        auto r = cw | map(_ * 2);
        ASSERT_EQ(20, *r.next());
        ASSERT_EQ(40, *r.next());
        ASSERT_FALSE(r.next());
        ASSERT_EQ(10, *cw.next());
        ASSERT_EQ(20, *cw.next());
        ASSERT_FALSE(cw.next());
    }
    
    TEST(MapTest, FromRValue) {
        auto r = std::vector<int> {11, 22, 33} | map(_ * 2);
        static_assert(std::is_same_v<option<int>, decltype(r.next())>);
        ASSERT_EQ(22, *r.next());
        ASSERT_EQ(44, *r.next());
        ASSERT_EQ(66, *r.next());
        ASSERT_FALSE(r.next());
    }
    
    TEST(MapTest, Empty) {
        auto r = std::vector<int> {} | map(_ + 10);
        ASSERT_FALSE(r.next());
    }
    
    TEST(MapTest, SequenceNotCopied) {
        auto r = std::vector<int> {1} | test::delete_copy() | map(_ * 2);
        ASSERT_EQ(2, *r.next());
        ASSERT_FALSE(r.next());
    }
    
    TEST(MapTest, FunctionNotCopied) {
        auto r = std::vector<int> {1} | map(test::delete_function_copy(_ * 2));
        ASSERT_EQ(2, *r.next());
        ASSERT_FALSE(r.next());
    }
    
    TEST(MapTest, MapToReference) {
        auto v = std::vector<int> {10, 20, 30};
        auto r = std::vector<int> {2, 1, 0} | map([&v](int index) -> decltype(auto) {return v[index];});
        static_assert(std::is_same_v<option<int &>, decltype(r.next())>);
        ASSERT_EQ(&v[2], &*r.next());
        ASSERT_EQ(&v[1], &*r.next());
        ASSERT_EQ(&v[0], &*r.next());
        ASSERT_FALSE(r.next());
    }
    
    TEST(MapTest, MapToNoncopyable) {
        auto r = std::vector<int> {10, 20} | map([](int n) {return std::make_unique<int>(n);});
        static_assert(std::is_same_v<option<std::unique_ptr<int>>, decltype(r.next())>);
        std::unique_ptr<int> up1 = *r.next();
        ASSERT_EQ(10, *up1);
        std::unique_ptr<int> up2 = *r.next();
        ASSERT_EQ(20, *up2);
        ASSERT_FALSE(r.next());
    }
    
    TEST(MapTest, MultipleSequences) {
        auto v = std::vector<int> {10, 20};
        auto r = map([](int &n, double m, bool neg) {return (n * m) * (neg ? -1 : 1);}, v, std::vector<double> {0.7, 0.5}, std::vector<bool> {true, false});
        static_assert(std::is_same_v<option<double>, decltype(r.next())>);
        ASSERT_DOUBLE_EQ(-7, *r.next());
        ASSERT_DOUBLE_EQ(10, *r.next());
        ASSERT_FALSE(r.next());
    }
    
    TEST(MapTest, UnmatchedLength) {
        auto r = map([](int n, int m) {return n + m;}, std::vector<int> {10, 20, 30}, std::vector<int> {5, 6});
        ASSERT_EQ(15, *r.next());
        ASSERT_EQ(26, *r.next());
        ASSERT_FALSE(r.next());
    }
    
    TEST(MapTest, MemberFunction) {
        struct Foo {
            int value_;
            
            Foo(int value) : value_(value) {
            }
            
            int value() const {
                return value_;
            }
        };
        auto r = std::vector<Foo> {Foo(5), Foo(8), Foo(3)} | map(&Foo::value);
        ASSERT_EQ(5, *r.next());
        ASSERT_EQ(8, *r.next());
        ASSERT_EQ(3, *r.next());
        ASSERT_FALSE(r.next());
    }
}
