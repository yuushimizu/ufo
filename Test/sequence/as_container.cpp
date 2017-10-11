#include <gtest/gtest.h>
#include "ufo/sequence/as_container.hpp"
#include <vector>
#include <deque>
#include "delete_copy.hpp"
#include "ufo/TD.hpp"

using namespace ufo;

namespace {
    TEST(AsContainerTest, LValue) {
        auto v = container_wrapper(std::vector<int> {10, 20, 30});
        std::deque<int> r = v | as_container;
        ASSERT_EQ((std::deque<int> {10, 20, 30}), r);
        ASSERT_EQ(10, *v.next());
        ASSERT_EQ(20, *v.next());
        ASSERT_EQ(30, *v.next());
        ASSERT_FALSE(v.next());
    }
    
    TEST(AsContainerTest, ConstLValue) {
        const auto v = container_wrapper(std::vector<int> {10, 20, 30});
        std::deque<int> r = v | as_container;
        ASSERT_EQ((std::deque<int> {10, 20, 30}), r);
        auto v2 = v;
        ASSERT_EQ(10, *v2.next());
        ASSERT_EQ(20, *v2.next());
        ASSERT_EQ(30, *v2.next());
        ASSERT_FALSE(v2.next());
    }
    
    TEST(AsContainerTest, RValue) {
        std::deque<int> r = std::vector<int> {10, 20, 30} | as_container;
        ASSERT_EQ((std::deque<int> {10, 20, 30}), r);
    }
    
    TEST(AsContainerTest, SequenceNotCopied) {
        std::deque<int> r = std::vector<int> {10, 20, 30} | test::delete_copy | as_container;
        ASSERT_EQ((std::deque<int> {10, 20, 30}), r);
    }
}
