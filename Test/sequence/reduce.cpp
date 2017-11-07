#include <gtest/gtest.h>
#include "ufo/sequence/reduce.hpp"
#include <vector>
#include <type_traits>
#include <functional>
#include "ufo/sequence/container_wrapper.hpp"
#include "delete_copy.hpp"

using namespace ufo;

namespace {
    TEST(ReduceTest, FromLValue) {
        std::vector<int> v {1, 2, 3, 4, 5};
        auto cw = container_wrapper(v);
        decltype(auto) r = cw | reduce([](auto acc, int &n) {return acc + n;}, 10);
        static_assert(std::is_same_v<int, decltype(r)>);
        ASSERT_EQ(25, r);
    }
    
    TEST(ReduceTest, CopiedLValueNotChanged) {
        auto cw = container_wrapper(std::vector<int> {10, 20, 30});
        ASSERT_EQ(60, cw | reduce(std::plus<> {}, 0));
        ASSERT_EQ(10, *cw.next());
        ASSERT_EQ(20, *cw.next());
        ASSERT_EQ(30, *cw.next());
        ASSERT_FALSE(cw.next());
    }
    
    TEST(ReduceTest, FromRValue) {
        decltype(auto) r = std::vector<int> {1, 2, 3, 4} | reduce(std::multiplies<> {}, 5);
        static_assert(std::is_same_v<int, decltype(r)>);
        ASSERT_EQ(120, r);
    }
    
    TEST(ReduceTest, Empty) {
        ASSERT_EQ(3, std::vector<int> {} | reduce(std::plus<> {}, 3));
    }
    
    TEST(ReduceTest, SequenceNotCopied) {
        ASSERT_EQ(10, (std::vector<int> {1, 2, 3, 4} | test::delete_copy | reduce(std::plus<> {}, 0)));
    }
    
    TEST(ReduceTest, FunctionNotCopied) {
        ASSERT_EQ(6, (std::vector<int> {1, 2, 3} | reduce(test::delete_function_copy(std::plus<> {}), 0)));
    }

    TEST(ReduceTest, Noncopyable) {
        decltype(auto) r = std::vector<int> {10, 20} | reduce([](std::unique_ptr<int> p, int n) {return std::make_unique<int>(*p + n);}, std::make_unique<int>(100));
        static_assert(std::is_same_v<std::unique_ptr<int>, decltype(r)>);
        ASSERT_EQ(130, *r);
    }
    
    TEST(ReduceTest, PreventSelfMoveResult) {
        struct A {
            int value = 10;
            
            A() noexcept = default;
            
            ~A() = default;
            
            A(const A &) = delete;
            
            A(A &&other) noexcept : value(other.value) {
                other.value = 0;
            }
            
            A &operator=(const A &) = delete;
            
            A &operator=(A &&other) noexcept {
                value = other.value;
                other.value = 0;
                return *this;
            }
        };
        auto r = std::vector<int> {1, 2, 3} | reduce([](A &&a, int) {return std::move(a);}, A {});
        ASSERT_EQ(10, r.value);
    }
}
