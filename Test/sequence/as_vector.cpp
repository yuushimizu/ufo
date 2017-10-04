#include <gtest/gtest.h>
#include "ufo/sequence/as_vector.hpp"
#include <deque>
#include <type_traits>
#include "delete_copy.hpp"

using namespace ufo;

namespace {
    TEST(AsVectorTest, LValue) {
        std::deque<int> d {10, 20, 30};
        decltype(auto) r = d | as_vector;
        ASSERT_EQ((std::vector<int> {10, 20, 30}), r);
    }
    
    TEST(AsVectorTest, RValue) {
        decltype(auto) r = std::deque<int> {10, 20, 30} | test::delete_copy | as_vector;
        ASSERT_EQ((std::vector<int> {10, 20, 30}), r);
    }
}

