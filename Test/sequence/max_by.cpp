#include <gtest/gtest.h>
#include "ufo/sequence/max_by.hpp"
#include <vector>
#include <type_traits>
#include <memory>
#include "delete_copy.hpp"

using namespace ufo;

namespace {
    TEST(MaxByTest, LValue) {
        std::vector<int> v {13, 48, 91, 26, 59, 33};
        auto cw = container_wrapper(v);
        decltype(auto) r = cw | max_by([](int &n) {
            return n % 10;
        });
        static_assert(std::is_same_v<option<int &>, decltype(r)>);
        ASSERT_EQ(&v[4], &*r);
    }
    
    TEST(MaxByTest, CopiedLValueNotChanged) {
        std::vector<int> v {10, 20, 30};
        auto cw = container_wrapper(std::vector<int> {19, 22, 35});
        auto r = cw | max_by([](int n) {return n % 10;});
        ASSERT_EQ(19, *r);
        ASSERT_EQ(19, *cw.next());
        ASSERT_EQ(22, *cw.next());
        ASSERT_EQ(35, *cw.next());
        ASSERT_FALSE(cw.next());
    }
    
    TEST(MaxByTest, RValue) {
        decltype(auto) r = std::vector<int> {40, 13, 24} | max_by([](auto n) {return -n;});
        static_assert(std::is_same_v<option<int>, decltype(r)>);
        ASSERT_EQ(13, *r);
    }
    
    TEST(MaxByTest, Empty) {
        auto r = std::vector<int> {} | max_by([](auto &&n) {return n % 10;});
        ASSERT_FALSE(r);
    }
    
    TEST(MaxByTest, OneValue) {
        auto r = std::vector<int> {56} | max_by([](auto &&n) {return -n;});
        ASSERT_EQ(56, *r);
    }
    
    TEST(MaxByTest, ReferenceKeyNotCopied) {
        struct Uncopyable {
        public:
            int value;
            
            Uncopyable(int value) : value(value) {
            };
            
            ~Uncopyable() = default;
            
            Uncopyable(const Uncopyable &) = delete;
            
            Uncopyable(Uncopyable &&other) : value(other.value) {
                other.value = 0;
            }
            
            Uncopyable &operator=(const Uncopyable &) = delete;
            
            Uncopyable &operator=(Uncopyable &&other) {
                value = other.value;
                other.value = 0;
                return *this;
            }
            
            bool operator<(const Uncopyable &other) {
                return value < other.value;
            }
            
            bool operator>(const Uncopyable &other) {
                return value > other.value;
            }
        };
        std::vector<Uncopyable> values {};
        values.emplace_back(20);
        values.emplace_back(40);
        values.emplace_back(30);
        values.emplace_back(10);
        auto r = std::vector<int> {3, 0, 1, 2} | max_by([&values](int n) -> decltype(auto) {return values[n];});
        ASSERT_EQ(1, *r);
        ASSERT_EQ(20, values[0].value);
        ASSERT_EQ(40, values[1].value);
        ASSERT_EQ(30, values[2].value);
        ASSERT_EQ(10, values[3].value);
    }
    
    TEST(MaxByTest, SequenceNotCopied) {
        auto r = std::vector<int> {2, 3, 4} | test::delete_copy | max_by([](auto n) {return n;});
        ASSERT_EQ(4, *r);
    }
    
    TEST(MaxByTest, FunctionNotCopied) {
        auto r = std::vector<int> {2, 3, 4} | max_by(test::delete_function_copy([](auto n) {return -n;}));
        ASSERT_EQ(2, *r);
    }
}
