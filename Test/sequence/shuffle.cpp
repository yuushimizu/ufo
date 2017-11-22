#include <gtest/gtest.h>
#include "ufo/sequence/shuffle.hpp"
#include <random>
#include "ufo/sequence/range.hpp"

using namespace ufo;

namespace {
    TEST(ShuffleTest, Shuffle) {
        std::mt19937_64 random(0);
        decltype(auto) r = range(5) | shuffle(random);
        static_assert(std::is_same_v<std::vector<int>, decltype(r)>);
        ASSERT_EQ(5, r.size());
    }
}
