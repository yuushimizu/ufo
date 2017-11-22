#include <gtest/gtest.h>
#include "ufo/sequence/mapcat.hpp"
#include <vector>
#include <type_traits>
#include "ufo/sequence/container_wrapper.hpp"
#include "ufo/sequence/range.hpp"
#include "ufo/sequence/map.hpp"
#include "delete_copy.hpp"

using namespace ufo;

namespace {
    TEST(MapcatTest, FromLValue) {
        std::vector<std::vector<int>> v {{10, 20}, {30, 40}, {50}};
        auto cw = container_wrapper(v);
        auto r = cw | mapcat([](auto &&x) -> decltype(auto) {return x;});
        static_assert(std::is_same_v<option<int &>, decltype(r.next())>);
        ASSERT_EQ(&v[0][0], &*r.next());
        ASSERT_EQ(&v[0][1], &*r.next());
        ASSERT_EQ(&v[1][0], &*r.next());
        ASSERT_EQ(&v[1][1], &*r.next());
        ASSERT_EQ(&v[2][0], &*r.next());
        ASSERT_FALSE(r.next());
    }
    
    TEST(MapcatTest, CopiedLValueNotChanged) {
        auto cw = container_wrapper(std::vector<int> {2, 3});
        auto r = cw | mapcat([](auto n) {return range(n);});
        ASSERT_EQ(0, *r.next());
        ASSERT_EQ(1, *r.next());
        ASSERT_EQ(0, *r.next());
        ASSERT_EQ(1, *r.next());
        ASSERT_EQ(2, *r.next());
        ASSERT_FALSE(r.next());
        ASSERT_EQ(2, *cw.next());
        ASSERT_EQ(3, *cw.next());
        ASSERT_FALSE(r.next());
    }
    
    TEST(MapcatTest, FromRValue) {
        auto r = std::vector<int> {2, 1} | mapcat([](auto n) {return range(n);});
        ASSERT_EQ(0, *r.next());
        ASSERT_EQ(1, *r.next());
        ASSERT_EQ(0, *r.next());
        ASSERT_FALSE(r.next());
    }
    
    TEST(MapcatTest, Empty) {
        auto r = std::vector<int> {} | mapcat([](auto n) {return range(n);});
        ASSERT_FALSE(r.next());
    }
    
    TEST(MapcatTest, SequenceNotCopied) {
        auto r = std::vector<int> {1, 2} | test::delete_copy | mapcat([](auto n) {return range(n);});
        ASSERT_EQ(0, *r.next());
        ASSERT_EQ(0, *r.next());
        ASSERT_EQ(1, *r.next());
        ASSERT_FALSE(r.next());
    }
    
    TEST(MapcatTest, FunctionNotCopied) {
        auto r = std::vector<int> {1, 2} | mapcat(test::delete_function_copy([](auto n) {return range(n);}));
        ASSERT_EQ(0, *r.next());
        ASSERT_EQ(0, *r.next());
        ASSERT_EQ(1, *r.next());
        ASSERT_FALSE(r.next());
    }
    
    TEST(MapcatTest, Noncopyable) {
        auto r = std::vector<int> {1, 2} | mapcat([](auto n) {return range(n) | map([](auto n) {return std::make_unique<int>(n);});});
        static_assert(std::is_same_v<option<std::unique_ptr<int>>, decltype(r.next())>);
        ASSERT_EQ(0, **r.next());
        ASSERT_EQ(0, **r.next());
        ASSERT_EQ(1, **r.next());
        ASSERT_FALSE(r.next());
    }
    
    TEST(MapcatTest, MemberFunction) {
        struct Foo {
            int value;
            
            Foo(int value) : value(value) {
            }
            
            auto seq() const {
                return range(value);
            }
        };
        auto r = std::vector<Foo> {2, 3} | mapcat(&Foo::seq);
        ASSERT_EQ(0, *r.next());
        ASSERT_EQ(1, *r.next());
        ASSERT_EQ(0, *r.next());
        ASSERT_EQ(1, *r.next());
        ASSERT_EQ(2, *r.next());
        ASSERT_FALSE(r.next());
    }
}
