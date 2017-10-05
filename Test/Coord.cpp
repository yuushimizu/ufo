#include <gtest/gtest.h>
#include "ufo/Coord.hpp"
#include <type_traits>
#include <functional>
#include <sstream>
#include "ufo/placeholder.hpp"

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
    
    TEST(CoordTest, ContainsTrue) {
        ASSERT_EQ(true, coord(5, 10).contains(coord(3, 3)));
    }
    
    TEST(CoordTest, ContainsZero) {
        ASSERT_EQ(true, coord(7, 1).contains(coord(0, 0)));
    }
    
    TEST(CoordTest, ContainsSame) {
        ASSERT_EQ(false, coord(8, 4).contains(coord(8, 4)));
    }
    
    TEST(CoordTest, ContainsXOnly) {
        ASSERT_EQ(false, coord(7, 10).contains(coord(3, 13)));
    }
    
    TEST(CoordTest, ContainsYOnly) {
        ASSERT_EQ(false, coord(8, 5).contains(coord(17, 2)));
    }
    
    TEST(CoordTest, ContainsNegative) {
        ASSERT_EQ(false, coord(70, 30).contains(coord(-1, -1)));
    }
    
    TEST(CoordTest, ContainsPositiveEdge) {
        ASSERT_EQ(true, coord(6, 4).contains(coord(5, 3)));
    }
    
    TEST(CoordTest, Area) {
        ASSERT_EQ(70, coord(7, 10).area());
    }
    
    TEST(CoordTest, AddX) {
        ASSERT_EQ(coord(15, 4), coord(5, 4).add_x(10));
    }
    
    TEST(CoordTest, AddY) {
        ASSERT_EQ(coord(5, 14), coord(5, 4).add_y(10));
    }
    
    TEST(CoordTest, Multiply) {
        ASSERT_EQ(coord(20, 300), coord(2, 3).multiply(coord(10, 100)));
    }
    
    TEST(CoordTest, Divide) {
        ASSERT_EQ(coord(2, 3), coord(20, 300).divide(coord(10, 100)));
    }
    
    TEST(CoordTest, CoordDeduction) {
        auto c = coord(3.4, 5.6);
        ASSERT_DOUBLE_EQ(3.4, c.x());
        ASSERT_DOUBLE_EQ(5.6, c.y());
        ASSERT_TRUE((std::is_same_v<Coord<double>, decltype(c)>));
    }
    
    TEST(CoordTest, TransformCoord) {
        auto c = transform_coord(std::multiplies<> {}, coord(3, 7), coord(8, 9));
        ASSERT_EQ(3 * 8, c.x());
        ASSERT_EQ(7 * 9, c.y());
    }
    
    TEST(CoordTest, TransformCoordOne) {
        auto c = transform_coord(_ * 2, coord(10, 20));
        ASSERT_EQ(20, c.x());
        ASSERT_EQ(40, c.y());
    }
    
    TEST(CoordTest, OperatorEqualTrue) {
        ASSERT_EQ(true, coord(5, 8) == coord(5, 8));
    }
    
    TEST(CoordTest, OperatorEqualXOnly) {
        ASSERT_EQ(false, coord(13, 34) == coord(13, 44));
    }
    
    TEST(CoordTest, OperatorEqualYOnly) {
        ASSERT_EQ(false, coord(75, 38) == coord(33, 38));
    }
    
    TEST(CoordTest, OperatorEqualFalse) {
        ASSERT_EQ(false, coord(73, 77) == coord(12, 34));
    }
    
    TEST(CoordTest, OperatorNotEqualTrue) {
        ASSERT_EQ(false, coord(5, 8) != coord(5, 8));
    }
    
    TEST(CoordTest, OperatorNotEqualXOnly) {
        ASSERT_EQ(true, coord(13, 34) != coord(13, 44));
    }
    
    TEST(CoordTest, OperatorNotEqualYOnly) {
        ASSERT_EQ(true, coord(75, 38) != coord(33, 38));
    }
    
    TEST(CoordTest, OperatorNotEqualFalse) {
        ASSERT_EQ(true, coord(73, 77) != coord(12, 34));
    }
    
    TEST(CoordTest, OperatorUnaryPlus) {
        ASSERT_EQ(coord(10, 20), +coord(10, 20));
    }
    
    TEST(CoordTest, OperatorUnaryMinus) {
        ASSERT_EQ(coord(-3, -4), -coord(3, 4));
    }
    
    TEST(CoordTest, OperatorPlus) {
        ASSERT_EQ(coord(13, 27), coord(3, 7) + coord(10, 20));
    }

    TEST(CoordTest, OperatorMinus) {
        ASSERT_EQ(coord(20, 30), coord(28, 31) - coord(8, 1));
    }
    
    TEST(CoordTest, OperatorMultiply) {
        ASSERT_EQ(coord(20, 30), coord(2, 3) * 10);
    }
    
    TEST(CoordTest, OperatorMultiplyRight) {
        ASSERT_EQ(coord(10, 30), 2 * coord(5, 15));
    }
    
    TEST(CoordTest, OperatorDivide) {
        ASSERT_EQ(coord(2, 3), coord(20, 30) / 10);
    }
    
    TEST(CoordTest, Range) {
        std::vector<int> r {};
        for (auto c : range(Coord<int>(3, 4), Coord<int>(8, 7))) {
            r.emplace_back(c.x() * c.y());
        }
        ASSERT_EQ((std::vector<int> {12, 16, 20, 24, 28, 15, 20, 25, 30, 35, 18, 24, 30, 36, 42}), r);
    }
    
    TEST(CoordTest, RangeWithoutBegin) {
        std::vector<int> r {};
        for (auto c : range(Coord<int>(2, 3))) {
            r.emplace_back(c.x() * c.y());
        }
        ASSERT_EQ((std::vector<int> {0, 0, 0, 1, 0, 2}), r);
    }
    
    TEST(CoordTest, COORD_ZERO) {
        ASSERT_EQ(coord(0, 0), COORD_ZERO<int>);
    }
    
    TEST(CoordTest, COORD_ONE) {
        ASSERT_EQ(coord(1, 1), COORD_ONE<int>);
    }
    
    TEST(CoordTest, Output) {
        std::stringstream ss {};
        ss << Coord<int>(1, 2);
        ASSERT_EQ("(1, 2)", ss.str());
    }
}
