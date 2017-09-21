#include <gtest/gtest.h>
#include <vector>
#include "ufo/range/join.hpp"
#include "ufo/range/as_vector.hpp"

using namespace ufo;

namespace {
    TEST(Join, JoinOneRValue) {
        ASSERT_EQ((std::vector<int> {10, 20}), join(std::vector<int> {10, 20}) | as_vector());
    }
    
    TEST(Join, JoinTwoRValue) {
        ASSERT_EQ((std::vector<int> {1, 2, 3, 4, 5}), join(std::vector<int> {1, 2}, std::vector<int> {3, 4, 5}) | as_vector());
    }
    
    TEST(Join, JoinThreeRValue) {
        ASSERT_EQ((std::vector<int> {8, 7, 6, 5, 4, 3, 2, 1}), join(std::vector<int> {8, 7, 6}, std::vector<int> {5, 4, 3}, std::vector<int> {2, 1}) | as_vector());
    }
    
    TEST(Join, JoinOneLValue) {
        std::vector<int> v {5, 10, 15};
        ASSERT_EQ((std::vector<int> {5, 10, 15}), join(v) | as_vector());
    }
    
    TEST(Join, JoinTwoLValue) {
        std::vector<int> v1 {1, 2, 3};
        std::vector<int> v2 {4, 5, 6};
        ASSERT_EQ((std::vector<int> {1, 2, 3, 4, 5, 6}), join(v1, v2) | as_vector());
    }
    
    TEST(Join, JoinThreeLValue) {
        std::vector<int> v1 {10, 20};
        std::vector<int> v2 {30, 40};
        std::vector<int> v3 {50, 60};
        ASSERT_EQ((std::vector<int> {10, 20, 30, 40, 50, 60}), join(v1, v2, v3) | as_vector());
    }
}
