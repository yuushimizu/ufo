#include <gtest/gtest.h>
#include <vector>
#include <type_traits>
#include "ufo/range/transformed.hpp"
#include "ufo/range/as_vector.hpp"

using namespace ufo;

namespace {
    TEST(TransformedTest, LValue) {
        std::vector<int> v {10, 20, 30};
        ASSERT_EQ((std::vector<int> {20, 40, 60}), v | transformed([](auto &&n) {return n * 2;}) | as_vector());
        ASSERT_EQ((std::vector<int> {10, 20, 30}), v);
    }
    
    TEST(TransformedTest, RValue) {
        ASSERT_EQ((std::vector<int> {10, 80, 70, 40}), (std::vector<int> {1, 8, 7, 4} | transformed([](auto &&n) {return n * 10;}) | as_vector()));
    }
    
    TEST(TransformedTest, ToReference) {
        std::vector<int> v {5, 8, 9};
        std::vector<int> r {};
        for (int &n : std::vector<int> {2, 1, 0} | transformed([&v](auto &&i) -> int & {return v[i];})) {
            r.emplace_back(n);
            n *= 2;
        }
        ASSERT_EQ((std::vector<int> {9, 8, 5}), r);
        ASSERT_EQ((std::vector<int> {10, 16, 18}), v);
    }
    
    TEST(TransformedTest, ConstToNonConst) {
        const std::vector<int> idxs {0, 1, 2};
        std::vector<int> v {10, 20, 30};
        std::vector<int> r {};
        for (int &n : idxs | transformed([&v](auto &&i) -> int & {return v[i];})) {
            r.emplace_back(n);
        }
        ASSERT_EQ((std::vector<int> {10, 20, 30}), r);
    }
    
    TEST(TransformedTest, NonConstToConst) {
        std::vector<int> idxs {0, 1, 2};
        const std::vector<int> v{5, 10, 15};
        std::vector<int> r {};
        for (const int &n : idxs | transformed([&v](auto &&i) -> const int & {return v[i];})) {
            r.emplace_back(n);
        }
        ASSERT_EQ((std::vector<int> {5, 10, 15}), r);
    }
    
    TEST(TransformedTest, Twice) {
        ASSERT_EQ((std::vector<int> {20, 40, 60}), (std::vector<int> {1, 2, 3} | transformed([](auto &&n) {return n * 2;}) | transformed([](auto &&n) {return n * 10;}) | as_vector()));
    }
    
    TEST(TransformedTest, Empty) {
        ASSERT_EQ((std::vector<std::string> {}), (std::vector<std::string> {} | transformed([](auto &&s) {return s + "-";}) | as_vector()));
    }
}
