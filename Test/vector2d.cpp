#include <gtest/gtest.h>
#include "ufo/vector2d.hpp"
#include <type_traits>
#include "ufo/coord.hpp"

using namespace ufo;

namespace {
    TEST(Vector2DTest, PairsLValue) {
        auto v = vector2d<int>(coord(3, 2));
        v[coord(0, 0)] = 0;
        v[coord(1, 0)] = 10;
        v[coord(2, 0)] = 20;
        v[coord(0, 1)] = 1;
        v[coord(1, 1)] = 11;
        v[coord(2, 1)] = 21;
        decltype(auto) r = v.pairs();
        static_assert(std::is_same_v<option<std::tuple<coord<int>, int &>>, decltype(r.next())>);
        auto t0 = *r.next();
        ASSERT_EQ(coord(0, 0), std::get<0>(t0));
        ASSERT_EQ(&v[coord(0, 0)], &std::get<1>(t0));
        auto t1 = *r.next();
        ASSERT_EQ(coord(1, 0), std::get<0>(t1));
        ASSERT_EQ(&v[coord(1, 0)], &std::get<1>(t1));
        auto t2 = *r.next();
        ASSERT_EQ(coord(2, 0), std::get<0>(t2));
        ASSERT_EQ(&v[coord(2, 0)], &std::get<1>(t2));
        auto t3 = *r.next();
        ASSERT_EQ(coord(0, 1), std::get<0>(t3));
        ASSERT_EQ(&v[coord(0, 1)], &std::get<1>(t3));
        auto t4 = *r.next();
        ASSERT_EQ(coord(1, 1), std::get<0>(t4));
        ASSERT_EQ(&v[coord(1, 1)], &std::get<1>(t4));
        auto t5 = *r.next();
        ASSERT_EQ(coord(2, 1), std::get<0>(t5));
        ASSERT_EQ(&v[coord(2, 1)], &std::get<1>(t5));
        ASSERT_FALSE(r.next());
    }
    
    TEST(Vector2DTest, PairsRValue) {
        auto v = vector2d<int>(coord(3, 2));
        v[coord(0, 0)] = 0;
        v[coord(1, 0)] = 10;
        v[coord(2, 0)] = 20;
        v[coord(0, 1)] = 1;
        v[coord(1, 1)] = 11;
        v[coord(2, 1)] = 21;
        decltype(auto) r = std::move(v).pairs();
        static_assert(std::is_same_v<option<std::tuple<coord<int>, int>>, decltype(r.next())>);
        auto t0 = *r.next();
        ASSERT_EQ(coord(0, 0), std::get<0>(t0));
        ASSERT_EQ(0, std::get<1>(t0));
        auto t1 = *r.next();
        ASSERT_EQ(coord(1, 0), std::get<0>(t1));
        ASSERT_EQ(10, std::get<1>(t1));
        auto t2 = *r.next();
        ASSERT_EQ(coord(2, 0), std::get<0>(t2));
        ASSERT_EQ(20, std::get<1>(t2));
        auto t3 = *r.next();
        ASSERT_EQ(coord(0, 1), std::get<0>(t3));
        ASSERT_EQ(1, std::get<1>(t3));
        auto t4 = *r.next();
        ASSERT_EQ(coord(1, 1), std::get<0>(t4));
        ASSERT_EQ(11, std::get<1>(t4));
        auto t5 = *r.next();
        ASSERT_EQ(coord(2, 1), std::get<0>(t5));
        ASSERT_EQ(21, std::get<1>(t5));
        ASSERT_FALSE(r.next());
    }
}
