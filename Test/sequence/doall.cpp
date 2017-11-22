#include <gtest/gtest.h>
#include "ufo/sequence/doall.hpp"
#include <vector>
#include "ufo/sequence/take_while.hpp"
#include "delete_copy.hpp"

using namespace ufo;

namespace {
    TEST(DoallTest, LValue) {
        int x = 0;
        auto s = std::vector<int> {7, 5, 3, 8, 9, 2, 1} | take_while([&x](int n) mutable {
            if (n % 2 == 0) return false;
            x += n;
            return true;
        });
        ASSERT_EQ(0, x);
        s | doall();
        ASSERT_EQ(15, x);
        s | doall();
        ASSERT_EQ(30, x);
    }
    
    TEST(DoallTest, RValue) {
        int x = 0;
        std::vector<int> {3, 5, 4, 1, 5} | take_while([&x](int n) mutable {
            if (n % 2 == 0) return false;
            x += n;
            return true;
        }) | doall();
        ASSERT_EQ(8, x);
    }
    
    TEST(DoallTest, SequenceNotCopied) {
        int x = 0;
        std::vector<int> {3, 5, 4, 1, 5} | test::delete_copy | take_while([&x](int n) mutable {
            if (n % 2 == 0) return false;
            x += n;
            return true;
        }) | doall();
        ASSERT_EQ(8, x);
    }
}
