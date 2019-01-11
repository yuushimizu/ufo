#include <gtest/gtest.h>
#include "ufo/sequence2/to_vector.hpp"
#include "ufo/sequence2/range.hpp"
#include "test_utility.hpp"
#include <type_traits>

namespace {
    TEST(ToVectorTest, ToVector) {
        auto v = ufo::s2::range(8) | ufo::s2::to_vector();
        static_assert(std::is_same_v<std::vector<int>, decltype(v)>);
        ASSERT_EQ((std::vector {0, 1, 2, 3, 4, 5, 6, 7}), v);
    }
    
    TEST(ToVectorTest, Reference) {
        std::vector o {10, 20, 30};
        auto v = o | ufo::s2::to_vector();
        static_assert(std::is_same_v<std::vector<int>, decltype(v)>);
        ASSERT_EQ((std::vector {10, 20, 30}), o);
    }
}
