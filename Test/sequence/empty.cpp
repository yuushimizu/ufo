#include <gtest/gtest.h>
#include "ufo/sequence/empty.hpp"
#include <type_traits>

using namespace ufo;

namespace {
    TEST(EmptyTest, Int) {
        auto r = empty<int>;
        static_assert(std::is_same_v<option<int>, decltype(r.next())>);
        ASSERT_FALSE(r.next());
    }
    
    TEST(EmptyTest, Double) {
        auto r = empty<double>;
        static_assert(std::is_same_v<option<double>, decltype(r.next())>);
        ASSERT_FALSE(r.next());
    }
}

