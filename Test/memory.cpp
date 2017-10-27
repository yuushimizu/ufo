#include <gtest/gtest.h>
#include <memory>
#include <set>
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
    
    TEST(OwnerBeforeCompareTest, Set) {
        auto shared = std::make_shared<int>(10);
        std::set<std::shared_ptr<int>, owner_before_compare> s {};
        s.insert(shared);
        ASSERT_EQ(1, s.count(shared));
    }
}
