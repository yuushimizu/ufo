#include <gtest/gtest.h>
#include <vector>
#include "ufo/range/filtered.hpp"
#include "ufo/placeholder.hpp"
#include "ufo/range/as_vector.hpp"

#include "ufo/TD.hpp"

using namespace ufo;

namespace {
    TEST(Filtered, FilterLValue) {
        std::vector<int> v {1, 2, 3, 4, 5, 6};
        ASSERT_EQ((std::vector<int> {2, 4, 6}), v | filtered([](auto &&n) {return n % 2 == 0;}) | as_vector());
    }
    
    TEST(Filtered, FilterRValue) {
        auto r = std::vector<int> {1, 2, 3, 4, 5} | filtered(_ % 2 == 0);
        ASSERT_EQ(*adl_begin(r), 2);
    }
}
