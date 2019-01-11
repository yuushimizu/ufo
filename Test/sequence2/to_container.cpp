#include <gtest/gtest.h>
#include "ufo/sequence2/to_container.hpp"
#include "ufo/sequence2/range.hpp"
#include "test_utility.hpp"
#include <type_traits>
#include <vector>
#include <list>
#include <deque>

namespace {
    TEST(ToContainerTest, ToVector) {
        std::vector<int> v = ufo::s2::range(5) | ufo::s2::to_container();
        ASSERT_EQ((std::vector {0, 1, 2, 3, 4,}), v);
    }
    
    TEST(ToContainerTest, ToList) {
        std::list<int> l = ufo::s2::range(5) | ufo::s2::to_container();
        ASSERT_EQ((std::list {0, 1, 2, 3, 4}), l);
    }
    
    TEST(ToContainerTest, ToDeque) {
        std::deque<int> d = ufo::s2::range(5) | ufo::s2::to_container();
        ASSERT_EQ((std::deque {0, 1, 2, 3, 4}), d);
    }
}
