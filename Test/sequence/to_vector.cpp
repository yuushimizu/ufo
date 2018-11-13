#include <gtest/gtest.h>
#include "ufo/sequence/to_vector.hpp"
#include <deque>
#include <type_traits>
#include "delete_copy.hpp"

using namespace ufo;

namespace {
    TEST(ToVectorTest, FromLValue) {
        auto cw = container_wrapper(std::deque<int> {10, 20, 30});
        decltype(auto) r = cw | to_vector();
        static_assert(std::is_same_v<std::vector<int>, decltype(r)>);
        ASSERT_EQ((std::vector<int> {10, 20, 30}), r);
        // cw is not changed
        ASSERT_EQ(10, *cw.next());
        ASSERT_EQ(20, *cw.next());
        ASSERT_EQ(30, *cw.next());
    }
    
    TEST(ToVectorTest, FromRValue) {
        decltype(auto) r = std::deque<int> {10, 20, 30} | to_vector();
        static_assert(std::is_same_v<std::vector<int>, decltype(r)>);
        ASSERT_EQ((std::vector<int> {10, 20, 30}), r);
    }
    
    TEST(ToVectorTest, SequenceNotCopied) {
        std::vector<int> r = std::deque<int> {10, 20, 30} | test::delete_copy() | to_vector();
        ASSERT_EQ((std::vector<int> {10, 20, 30}), r);
    }
}

