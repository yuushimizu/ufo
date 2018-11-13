#include <gtest/gtest.h>
#include "ufo/sequence/find.hpp"
#include <vector>
#include <type_traits>
#include "delete_copy.hpp"
#include "placeholder.hpp"

using namespace ufo;

namespace {
    TEST(FindTest, FromLValue) {
        auto v = std::vector<int> {3, 5, 8, 1, 7};
        auto cw = container_wrapper(v);
        decltype(auto) r = cw | find(_ % 2 == 0);
        static_assert(std::is_same_v<option<int &>, decltype(r)>);
        ASSERT_EQ(&v[2], &*r);
    }

    TEST(FindTest, CopiedLValueNotChanged) {
        auto v = std::vector<int> {10, 20, 30};
        auto cw = container_wrapper(std::vector<int> {10, 20, 30});
        decltype(auto) r = cw | find(_ == 30);
        ASSERT_EQ(30, *r);
        ASSERT_EQ(10, *cw.next());
        ASSERT_EQ(20, *cw.next());
        ASSERT_EQ(30, *cw.next());
        ASSERT_FALSE(cw.next());
    }

    TEST(FindTest, FromRValue) {
        decltype(auto) r = std::vector<int> {10, 20, 30} | find(_ % 4 == 0);
        static_assert(std::is_same_v<option<int>, decltype(r)>);
        ASSERT_EQ(20, *r);
    }

    TEST(FindTest, Empty) {
        auto r = std::vector<int> {} | find(_ == 2);
        ASSERT_FALSE(r);
    }

    TEST(FindTest, NotMatched) {
        auto r = std::vector<int> {10, 20, 30} | find(_ % 7 == 0);
        ASSERT_FALSE(r);
    }

    TEST(FindTest, SequenceNotCopied) {
        auto r = std::vector<int> {2, 3, 4} | test::delete_copy() | find(_ == 3);
        ASSERT_EQ(3, *r);
    }
    
    TEST(FindTest, FunctionNotCopied) {
        auto r = std::vector<int> {2, 3, 4} | find(test::delete_function_copy(_ == 4));
        ASSERT_EQ(4, *r);
    }
    
    TEST(FindTest, MemberFunction) {
        struct Foo {
            int value;
            
            Foo(int value) : value(value) {
            }
            
            bool is_even() const {
                return value % 2 == 0;
            }
        };
        auto r = std::vector<Foo> {5, 8, 2} | find(&Foo::is_even);
        ASSERT_EQ(8, r->value);
    }
}

