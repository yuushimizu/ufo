#include <gtest/gtest.h>
#include "ufo/range2/as_container.hpp"
#include <vector>
#include <deque>
#include "delete_copy.hpp"

using namespace ufo;

namespace {
    TEST(AsContainerTest, LValue) {
        std::vector<int> v {10, 20, 30};
        std::deque<int> r = v | as_container;
        ASSERT_EQ((std::deque<int> {10, 20, 30}), r);
    }
    
    TEST(AsContainerTest, RValue) {
        std::deque<int> r = std::vector<int> {10, 20, 30} | test::delete_copy | as_container;
        ASSERT_EQ((std::deque<int> {10, 20, 30}), r);
    }
}
