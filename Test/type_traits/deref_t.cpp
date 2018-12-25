#include <gtest/gtest.h>
#include "ufo/type_traits.hpp"

namespace {
    TEST(DerefTTest, Class) {
        struct X {
            int operator*() const & {
                return 42;
            }
            
            double operator*() & {
                return 1.2;
            }
            
            long operator*() && {
                return 45L;
            }
            
            X(const X &) = delete;
            X(X &&) = delete;
            X &operator=(const X &) = delete;
            X &operator=(X &&) = delete;
            
        private:
            X();
        };
        static_assert(std::is_same_v<long, ufo::deref_t<X>>);
        static_assert(std::is_same_v<double, ufo::deref_t<X &>>);
        static_assert(std::is_same_v<int, ufo::deref_t<const X &>>);
        static_assert(std::is_same_v<long, ufo::deref_t<X &&>>);
        SUCCEED();
    }
    
    TEST(DerefTTest, Pointer) {
        static_assert(std::is_same_v<int &, ufo::deref_t<int *>>);
        static_assert(std::is_same_v<const int &, ufo::deref_t<const int *>>);
        SUCCEED();
    }
}
