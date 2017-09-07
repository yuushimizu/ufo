#include <gtest/gtest.h>
#include <type_traits>
#include "ufo/Coord.hpp"

using namespace ufo;

namespace {
    TEST(CoordTest, DefaultConstructor) {
        Coord<int> c {};
        ASSERT_EQ(0, c.x());
        ASSERT_EQ(0, c.y());
    }
    
    TEST(CoodTest, XYConstructor) {
        Coord<int> c(12, 43);
        ASSERT_EQ(12, c.x());
        ASSERT_EQ(43, c.y());
    }
    
    TEST(CoordTest, CopyConstructor) {
        Coord<int> c1(5, 8);
        Coord<int> c2(c1);
        ASSERT_EQ(5, c2.x());
        ASSERT_EQ(8, c2.y());
    }
    
    TEST(CoordTest, MoveConstructor) {
        Coord<int> c(Coord<int>(90, 88));
        ASSERT_EQ(90, c.x());
        ASSERT_EQ(88, c.y());
    }
    
    TEST(CoordTest, CopyAssign) {
        Coord<int> c1(77, 88);
        Coord<int> c2 {};
        c2 = c1;
        ASSERT_EQ(77, c2.x());
        ASSERT_EQ(88, c2.y());
    }
    
    TEST(CoordTest, MoveAssign) {
        Coord<int> c {};
        c = Coord<int>(42, 24);
        ASSERT_EQ(42, c.x());
        ASSERT_EQ(24, c.y());
    }
    
    TEST(CoordTest, Area) {
        ASSERT_EQ(70, Coord<int>(7, 10).area());
    }
    
    TEST(CoordTest, ContainsTrue) {
        ASSERT_EQ(true, Coord<int>(5, 10).contains(Coord<int>(3, 3)));
    }
    
    TEST(CoordTest, ContainsZero) {
        ASSERT_EQ(true, Coord<int>(7, 1).contains(Coord<int>(0, 0)));
    }
    
    TEST(CoordTest, ContainsSame) {
        ASSERT_EQ(false, Coord<int>(8, 4).contains(Coord<int>(8, 4)));
    }
    
    TEST(CoordTest, ContainsXOnly) {
        ASSERT_EQ(false, Coord<int>(7, 10).contains(Coord<int>(3, 13)));
    }
    
    TEST(CoordTest, ContainsYOnly) {
        ASSERT_EQ(false, Coord<int>(8, 5).contains(Coord<int>(17, 2)));
    }
    
    TEST(CoordTest, ContainsNegative) {
        ASSERT_EQ(false, Coord<int>(70, 30).contains(Coord<int>(-1, -1)));
    }
    
    TEST(CoordTest, ContainsPositiveEdge) {
        ASSERT_EQ(true, Coord<int>(6, 4).contains(Coord<int>(5, 3)));
    }
    
    TEST(CoordTest, CoordDeduction) {
        auto c = coord(3.4, 5.6);
        ASSERT_DOUBLE_EQ(3.4, c.x());
        ASSERT_DOUBLE_EQ(5.6, c.y());
        static_assert(std::is_same<Coord<double>, decltype(c)>());
    }
    
    TEST(CoordTest, TransformCoord) {
        auto c = transform_coord([](auto &&n, auto &&m) {return n * m;}, Coord<int>(3, 7), Coord<int>(8, 9));
        ASSERT_EQ(3 * 8, c.x());
        ASSERT_EQ(7 * 9, c.y());
    }
    
    TEST(CoordTest, TransformCoordOne) {
        auto c = transform_coord([](auto &&n) {return n * 2;}, Coord<int>(10, 20));
        ASSERT_EQ(20, c.x());
        ASSERT_EQ(40, c.y());
    }
    
    TEST(CoordTest, OperatorEqualTrue) {
        ASSERT_EQ(true, Coord<int>(5, 8) == Coord<int>(5, 8));
    }
    
    TEST(CoordTest, OperatorEqualXOnly) {
        ASSERT_EQ(false, Coord<int>(13, 34) == Coord<int>(13, 44));
    }
    
    TEST(CoordTest, OperatorEqualYOnly) {
        ASSERT_EQ(false, Coord<int>(75, 38) == Coord<int>(33, 38));
    }
    
    TEST(CoordTest, OperatorEqualFalse) {
        ASSERT_EQ(false, Coord<int>(73, 77) == Coord<int>(12, 34));
    }
    
    TEST(CoordTest, OperatorNotEqualTrue) {
        ASSERT_EQ(false, Coord<int>(5, 8) != Coord<int>(5, 8));
    }
    
    TEST(CoordTest, OperatorNotEqualXOnly) {
        ASSERT_EQ(true, Coord<int>(13, 34) != Coord<int>(13, 44));
    }
    
    TEST(CoordTest, OperatorNotEqualYOnly) {
        ASSERT_EQ(true, Coord<int>(75, 38) != Coord<int>(33, 38));
    }
    
    TEST(CoordTest, OperatorNotEqualFalse) {
        ASSERT_EQ(true, Coord<int>(73, 77) != Coord<int>(12, 34));
    }
    
    TEST(CoordTest, Range) {
        std::vector<int> r {};
        for (auto &&c : range(Coord<int>(3, 4), Coord<int>(8, 7))) {
            r.emplace_back(c.x() * c.y());
        }
        ASSERT_EQ((std::vector<int> {12, 16, 20, 24, 28, 15, 20, 25, 30, 35, 18, 24, 30, 36, 42}), r);
    }
    
    TEST(CoordTest, RangeWithoutBegin) {
        std::vector<int> r {};
        for (auto &&c : range(Coord<int>(2, 3))) {
            r.emplace_back(c.x() * c.y());
        }
        ASSERT_EQ((std::vector<int> {0, 0, 0, 1, 0, 2}), r);
    }
}
