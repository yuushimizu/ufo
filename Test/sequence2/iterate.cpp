#include <gtest/gtest.h>
#include "ufo/sequence2/iterate.hpp"
#include "ufo/placeholder.hpp"
#include "test_utility.hpp"
#include <type_traits>

namespace {
    using ufo::_;
    
    TEST(IterateTest, Iterate) {
        auto s = ufo::s2::iterate(_ * 2, 1);
        ufo::test::sequence_assert<int>(s);
        ASSERT_EQ((std::vector {1, 2, 4, 8}), ufo::test::collect_values(s, 4));
    }
}
