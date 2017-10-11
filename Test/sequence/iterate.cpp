#include <gtest/gtest.h>
#include "ufo/sequence/iterate.hpp"
#include <type_traits>
#include "ufo/placeholder.hpp"
#include "delete_copy.hpp"

using namespace ufo;

namespace {
    TEST(IterateTest, Iteration) {
        auto r = iterate(_ * 2, 1);
        static_assert(std::is_same_v<option<int>, decltype(r.next())>);
        ASSERT_EQ(1, *r.next());
        ASSERT_EQ(2, *r.next());
        ASSERT_EQ(4, *r.next());
        ASSERT_EQ(8, *r.next());
        ASSERT_EQ(16, *r.next());
        ASSERT_EQ(32, *r.next());
    }
    
    TEST(IterateTest, FunctionNotCopied) {
        auto r = iterate(test::delete_function_copy(_ + 2), 0);
        ASSERT_EQ(0, *r.next());
        ASSERT_EQ(2, *r.next());
        ASSERT_EQ(4, *r.next());
    }
}
