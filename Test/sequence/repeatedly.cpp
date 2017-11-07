#include <gtest/gtest.h>
#include "ufo/sequence/repeatedly.hpp"
#include <type_traits>
#include "ufo/option.hpp"
#include "delete_copy.hpp"

using namespace ufo;

namespace {
    TEST(RepeatedlyTest, Repeatedly) {
        auto r = repeatedly([x = 0]() mutable {
            return x++;
        });
        static_assert(std::is_same_v<option<int>, decltype(r.next())>);
        ASSERT_EQ(0, *r.next());
        ASSERT_EQ(1, *r.next());
        ASSERT_EQ(2, *r.next());
        ASSERT_EQ(3, *r.next());
    }
    
    TEST(RepeatedlyTest, FunctionNotCopied) {
        auto r = repeatedly(test::delete_function_copy([]() {return 42;}));
        ASSERT_EQ(42, *r.next());
        ASSERT_EQ(42, *r.next());
        ASSERT_EQ(42, *r.next());
    }
}
