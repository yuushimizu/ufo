#include <gtest/gtest.h>
#include "ufo/scope_exit.hpp"

using namespace ufo;

namespace {
    TEST(ScopeExitTest, Destructor) {
        int n = 10;
        {
            auto add = scope_exit([&n]() mutable noexcept {
                ++n;
            });
            ASSERT_EQ(10, n);
        }
        ASSERT_EQ(11, n);
    }
}


