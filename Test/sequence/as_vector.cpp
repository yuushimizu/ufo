#include <gtest/gtest.h>
#include "ufo/sequence/as_vector.hpp"
#include <deque>
#include <type_traits>
#include "delete_copy.hpp"

using namespace ufo;

namespace {
    TEST(AsVectorTest, FromLValue) {
        auto cw = container_wrapper(std::deque<int> {10, 20, 30});
        decltype(auto) r = cw | as_vector;
        static_assert(std::is_same_v<std::vector<int>, decltype(r)>);
        ASSERT_EQ((std::vector<int> {10, 20, 30}), r);
        // cw is not changed
        ASSERT_EQ(10, *cw.next());
        ASSERT_EQ(20, *cw.next());
        ASSERT_EQ(30, *cw.next());
    }
    
    TEST(AsVectorTest, FromRValue) {
        decltype(auto) r = std::deque<int> {10, 20, 30} | as_vector;
        static_assert(std::is_same_v<std::vector<int>, decltype(r)>);
        ASSERT_EQ((std::vector<int> {10, 20, 30}), r);
    }
    
    TEST(AsVectorTest, SequenceNotCopied) {
        std::vector<int> r = std::deque<int> {10, 20, 30} | test::delete_copy | as_vector;
        ASSERT_EQ((std::vector<int> {10, 20, 30}), r);
    }
}

