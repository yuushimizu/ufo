#include <gtest/gtest.h>
#include "ufo/sequence/take_while.hpp"
#include <vector>
#include <type_traits>
#include "ufo/placeholder.hpp"
#include "delete_copy.hpp"

using namespace ufo;

namespace {
    TEST(TakeWhileTest, FromLValue) {
        auto v = std::vector<int> {1, 7, 5, 8, 9, 3, 2};
        auto cw = container_wrapper(v);
        auto r = cw | take_while(_ % 2 != 0);
        static_assert(std::is_same_v<option<int &>, decltype(r.next())>);
        ASSERT_EQ(1, *r.next());
        ASSERT_EQ(7, *r.next());
        ASSERT_EQ(5, *r.next());
        ASSERT_FALSE(r.next());
    }
    
    TEST(TakeWhileTest, CopiedLValueNotChanged) {
        auto cw = container_wrapper(std::vector<int> {1, 2, 3, 4});
        auto r = cw | take_while(_ < 3);
        ASSERT_EQ(1, *r.next());
        ASSERT_EQ(2, *r.next());
        ASSERT_FALSE(r.next());
        ASSERT_EQ(1, *cw.next());
        ASSERT_EQ(2, *cw.next());
        ASSERT_EQ(3, *cw.next());
        ASSERT_EQ(4, *cw.next());
        ASSERT_FALSE(cw.next());
    }

    TEST(TakeWhileTest, FromRValue) {
        auto r = std::vector<int> {2, 4, 8, 9, 10, 12} | take_while(_ % 2 == 0);
        static_assert(std::is_same_v<option<int>, decltype(r.next())>);
        ASSERT_EQ(2, *r.next());
        ASSERT_EQ(4, *r.next());
        ASSERT_EQ(8, *r.next());
        ASSERT_FALSE(r.next());
    }
    
    TEST(TakeWhileTest, Empty) {
        auto r = std::vector<int> {} | take_while(_ < 10);
        ASSERT_FALSE(r.next());
    }
    
    TEST(TakeWhileTest, Unmatch) {
        auto r = std::vector<int> {10} | take_while(_ < 10);
        ASSERT_FALSE(r.next());
    }
    
    TEST(TakeWhileTest, SequenceNotCopied) {
        auto r = std::vector<int> {1, 2, 3} | test::delete_copy | take_while(_ < 2);
        ASSERT_EQ(1, *r.next());
        ASSERT_FALSE(r.next());
    }
    
    TEST(TakeWhileTest, FunctionNotCopied) {
        auto r = std::vector<int> {1, 2, 3} | take_while(test::delete_function_copy(_ < 2));
        ASSERT_EQ(1, *r.next());
        ASSERT_FALSE(r.next());
    }
    
    TEST(TakeWhileTest, MemberFunction) {
        struct Foo {
            int value;
            
            Foo(int value) : value(value) {
            }
            
            bool is_odd() const {
                return value % 2 != 0;
            }
        };
        auto r = std::vector<Foo> {7, 5, 6, 1, 3} | take_while(&Foo::is_odd);
        ASSERT_EQ(7, r.next()->value);
        ASSERT_EQ(5, r.next()->value);
        ASSERT_FALSE(r.next());
    }
}

