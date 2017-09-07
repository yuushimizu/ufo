#include <gtest/gtest.h>
#include <memory>
#include "ufo/memory.hpp"

using namespace ufo;

namespace {
    TEST(MakeUniquePtrTest, CallDeleter) {
        int count = 0;
        auto deleter = [&count](int *x) {
            delete x;
            ++count;
        };
        {
            std::unique_ptr<int, decltype(deleter)> p = make_unique_ptr(new int(5), deleter);
            ASSERT_EQ(5, *p);
        }
        ASSERT_EQ(1, count);
    }
}
