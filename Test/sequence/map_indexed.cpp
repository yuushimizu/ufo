#include <gtest/gtest.h>
#include "ufo/sequence/map_indexed.hpp"
#include <vector>
#include <type_traits>
#include "delete_copy.hpp"

using namespace ufo;

namespace {
    TEST(MapIndexedTest, FromLValue) {
        std::vector<int> v {10, 20, 30};
        auto cw = container_wrapper(v);
        auto r = cw | map_indexed([](std::size_t i, int &n) {return std::tuple<std::size_t, int &>(i, n);});
        static_assert(std::is_same_v<option<std::tuple<std::size_t, int &>>, decltype(r.next())>);
        ASSERT_EQ((std::tuple<std::size_t, int &>(0, v[0])), *r.next());
        ASSERT_EQ((std::tuple<std::size_t, int &>(1, v[1])), *r.next());
        ASSERT_EQ((std::tuple<std::size_t, int &>(2, v[2])), *r.next());
        ASSERT_FALSE(r.next());
    }

    TEST(MapIndexedTest, CopiedLValueNotChanged) {
        auto cw = container_wrapper(std::vector<int> {10, 20});
        auto r = cw | map_indexed([](std::size_t i, int n) {return std::make_tuple(i, n);});
        ASSERT_EQ(std::make_tuple(0, 10), *r.next());
        ASSERT_EQ(std::make_tuple(1, 20), *r.next());
        ASSERT_FALSE(r.next());
        ASSERT_EQ(10, *cw.next());
        ASSERT_EQ(20, *cw.next());
        ASSERT_FALSE(cw.next());
    }
    
    TEST(MapIndexedTest, FromRValue) {
        auto r = std::vector<int> {11, 22, 33} | map_indexed([](std::size_t i, int n) {return int(i * 100 + n);});
        static_assert(std::is_same_v<option<int>, decltype(r.next())>);
        ASSERT_EQ(11, *r.next());
        ASSERT_EQ(122, *r.next());
        ASSERT_EQ(233, *r.next());
        ASSERT_FALSE(r.next());
    }
    
    TEST(MapIndexedTest, Empty) {
        auto r = std::vector<int> {} | map_indexed([](std::size_t i, int n) {return i + n;});
        ASSERT_FALSE(r.next());
    }
    
    TEST(MapIndexedTest, SequenceNotCopied) {
        auto r = std::vector<int> {1} | test::delete_copy | map_indexed([](std::size_t i, int n) {return std::make_tuple(i, n);});
        ASSERT_EQ(std::make_tuple(0, 1), *r.next());
        ASSERT_FALSE(r.next());
    }
    
    TEST(MapIndexedTest, FunctionNotCopied) {
        auto r = std::vector<int> {1} | map_indexed(test::delete_function_copy([](std::size_t i, int n) {return std::make_tuple(i, n);}));
        ASSERT_EQ(std::make_tuple(0, 1), *r.next());
        ASSERT_FALSE(r.next());
    }

    TEST(MapIndexedTest, MapToReference) {
        std::vector<int> v {10, 20, 30};
        auto r = std::vector<int> {2, 1, 0} | map_indexed([&v](std::size_t i, int n) -> decltype(auto) {return v[n];});
        static_assert(std::is_same_v<option<int &>, decltype(r.next())>);
        ASSERT_EQ(&v[2], &*r.next());
        ASSERT_EQ(&v[1], &*r.next());
        ASSERT_EQ(&v[0], &*r.next());
        ASSERT_FALSE(r.next());
    }

    TEST(MapIndexedTest, MapToNoncopyable) {
        auto r = std::vector<int> {10, 20} | map_indexed([](std::size_t i, int n) {return std::make_unique<int>(i + n);});
        static_assert(std::is_same_v<option<std::unique_ptr<int>>, decltype(r.next())>);
        std::unique_ptr<int> up1 = *r.next();
        ASSERT_EQ(10, *up1);
        std::unique_ptr<int> up2 = *r.next();
        ASSERT_EQ(21, *up2);
        ASSERT_FALSE(r.next());
    }
}

