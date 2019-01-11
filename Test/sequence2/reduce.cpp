#include <gtest/gtest.h>
#include "ufo/sequence2/reduce.hpp"
#include "ufo/sequence2/range.hpp"
#include "ufo/placeholder.hpp"
#include "test_utility.hpp"
#include "../test_utility.hpp"

namespace {
    using ufo::_;
    
    TEST(ReduceTest, Reduce) {
        ASSERT_EQ(55, ufo::s2::range(11) | ufo::s2::reduce(0, std::plus<> {}));
    }
    
    TEST(ReduceTest, Empty) {
        ASSERT_EQ(100, ufo::s2::range(0) | ufo::s2::reduce(100, std::plus<> {}));
    }
}
