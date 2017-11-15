#include <gtest/gtest.h>
#include "ufo/sequence/as_container.hpp"
#include <vector>
#include <deque>
#include "delete_copy.hpp"

using namespace ufo;

namespace {
    TEST(AsContainerTest, FromLValue) {
        auto cw = container_wrapper(std::vector<int> {10, 20, 30});
        std::deque<int> r = cw | as_container();
        ASSERT_EQ((std::deque<int> {10, 20, 30}), r);
    }
    
    TEST(AsContainerTest, CopiedLValueNotChanged) {
        auto cw = container_wrapper(std::vector<int> {10, 20, 30});
        std::deque<int> r = cw | as_container();
        ASSERT_EQ((std::deque<int> {10, 20, 30}), r);
        ASSERT_EQ(10, *cw.next());
        ASSERT_EQ(20, *cw.next());
        ASSERT_EQ(30, *cw.next());
        ASSERT_FALSE(cw.next());
    }
    
    TEST(AsContainerTest, FromConstLValue) {
        const auto cw = container_wrapper(std::vector<int> {10, 20, 30});
        std::deque<int> r = cw | as_container();
        ASSERT_EQ((std::deque<int> {10, 20, 30}), r);
    }
    
    TEST(AsContainerTest, CopiedConstLValueNotChanged) {
        const auto cw = container_wrapper(std::vector<int> {10, 20, 30});
        std::deque<int> r = cw | as_container();
        ASSERT_EQ((std::deque<int> {10, 20, 30}), r);
        auto cw2 = cw;
        ASSERT_EQ(10, *cw2.next());
        ASSERT_EQ(20, *cw2.next());
        ASSERT_EQ(30, *cw2.next());
        ASSERT_FALSE(cw2.next());
    }
    
    TEST(AsContainerTest, FromRValue) {
        std::deque<int> r = std::vector<int> {10, 20, 30} | as_container();
        ASSERT_EQ((std::deque<int> {10, 20, 30}), r);
    }
    
    TEST(AsContainerTest, SequenceNotCopied) {
        std::deque<int> r = std::vector<int> {10, 20, 30} | test::delete_copy | as_container();
        ASSERT_EQ((std::deque<int> {10, 20, 30}), r);
    }
}
