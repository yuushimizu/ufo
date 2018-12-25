#include <gtest/gtest.h>
#include "ufo/coord.hpp"
#include <type_traits>
#include <functional>
#include <sstream>
#include "ufo/placeholder.hpp"

namespace {
    using ufo::_;
    
    TEST(CoordTest, DefaultConstructor) {
        auto c = ufo::coord<int> {};
        ASSERT_EQ(0, c.x());
        ASSERT_EQ(0, c.y());
    }
    
    TEST(CoodTest, XYConstructor) {
        auto c = ufo::coord(12, 43);
        ASSERT_EQ(12, c.x());
        ASSERT_EQ(43, c.y());
    }
    
    TEST(CoordTest, CopyConstructor) {
        auto c1 = ufo::coord(5, 8);
        auto c2 = ufo::coord(c1);
        ASSERT_EQ(5, c2.x());
        ASSERT_EQ(8, c2.y());
    }
    
    TEST(CoordTest, MoveConstructor) {
        auto c = ufo::coord(ufo::coord(90, 88));
        ASSERT_EQ(90, c.x());
        ASSERT_EQ(88, c.y());
    }
    
    TEST(CoordTest, CopyAssign) {
        auto c1 = ufo::coord(77, 88);
        auto c2 = ufo::coord<int> {};
        c2 = c1;
        ASSERT_EQ(77, c2.x());
        ASSERT_EQ(88, c2.y());
    }
    
    TEST(CoordTest, MoveAssign) {
        auto c = ufo::coord<int> {};
        c = ufo::coord(42, 24);
        ASSERT_EQ(42, c.x());
        ASSERT_EQ(24, c.y());
    }
    
    TEST(CoordTest, ContainsTrue) {
        ASSERT_EQ(true, ufo::coord(5, 10).contains(ufo::coord(3, 3)));
    }
    
    TEST(CoordTest, ContainsZero) {
        ASSERT_EQ(true, ufo::coord(7, 1).contains(ufo::coord(0, 0)));
    }
    
    TEST(CoordTest, ContainsSame) {
        ASSERT_EQ(false, ufo::coord(8, 4).contains(ufo::coord(8, 4)));
    }
    
    TEST(CoordTest, ContainsXOnly) {
        ASSERT_EQ(false, ufo::coord(7, 10).contains(ufo::coord(3, 13)));
    }
    
    TEST(CoordTest, ContainsYOnly) {
        ASSERT_EQ(false, ufo::coord(8, 5).contains(ufo::coord(17, 2)));
    }
    
    TEST(CoordTest, ContainsNegative) {
        ASSERT_EQ(false, ufo::coord(70, 30).contains(ufo::coord(-1, -1)));
    }
    
    TEST(CoordTest, ContainsPositiveEdge) {
        ASSERT_EQ(true, ufo::coord(6, 4).contains(ufo::coord(5, 3)));
    }
    
    TEST(CoordTest, Area) {
        ASSERT_EQ(70, ufo::coord(7, 10).area());
    }
    
    TEST(CoordTest, AddX) {
        ASSERT_EQ(ufo::coord(15, 4), ufo::coord(5, 4).add_x(10));
    }
    
    TEST(CoordTest, AddY) {
        ASSERT_EQ(ufo::coord(5, 14), ufo::coord(5, 4).add_y(10));
    }
    
    TEST(CoordTest, Multiply) {
        ASSERT_EQ(ufo::coord(20, 300), ufo::coord(2, 3).multiply(ufo::coord(10, 100)));
    }
    
    TEST(CoordTest, Divide) {
        ASSERT_EQ(ufo::coord(2, 3), ufo::coord(20, 300).divide(ufo::coord(10, 100)));
    }
    
    TEST(CoordTest, CoordDeduction) {
        auto c = ufo::coord(3.4, 5.6);
        ASSERT_DOUBLE_EQ(3.4, c.x());
        ASSERT_DOUBLE_EQ(5.6, c.y());
        ASSERT_TRUE((std::is_same_v<ufo::coord<double>, decltype(c)>));
    }
    
    TEST(CoordTest, TransformCoord) {
        auto c = transform_coord(std::multiplies<> {}, ufo::coord(3, 7), ufo::coord(8, 9));
        ASSERT_EQ(3 * 8, c.x());
        ASSERT_EQ(7 * 9, c.y());
    }
    
    TEST(CoordTest, TransformCoordOne) {
        auto c = transform_coord(_ * 2, ufo::coord(10, 20));
        ASSERT_EQ(20, c.x());
        ASSERT_EQ(40, c.y());
    }
    
    TEST(CoordTest, OperatorEqualTrue) {
        ASSERT_EQ(true, ufo::coord(5, 8) == ufo::coord(5, 8));
    }
    
    TEST(CoordTest, OperatorEqualXOnly) {
        ASSERT_EQ(false, ufo::coord(13, 34) == ufo::coord(13, 44));
    }
    
    TEST(CoordTest, OperatorEqualYOnly) {
        ASSERT_EQ(false, ufo::coord(75, 38) == ufo::coord(33, 38));
    }
    
    TEST(CoordTest, OperatorEqualFalse) {
        ASSERT_EQ(false, ufo::coord(73, 77) == ufo::coord(12, 34));
    }
    
    TEST(CoordTest, OperatorNotEqualTrue) {
        ASSERT_EQ(false, ufo::coord(5, 8) != ufo::coord(5, 8));
    }
    
    TEST(CoordTest, OperatorNotEqualXOnly) {
        ASSERT_EQ(true, ufo::coord(13, 34) != ufo::coord(13, 44));
    }
    
    TEST(CoordTest, OperatorNotEqualYOnly) {
        ASSERT_EQ(true, ufo::coord(75, 38) != ufo::coord(33, 38));
    }
    
    TEST(CoordTest, OperatorNotEqualFalse) {
        ASSERT_EQ(true, ufo::coord(73, 77) != ufo::coord(12, 34));
    }
    
    TEST(CoordTest, OperatorUnaryPlus) {
        ASSERT_EQ(ufo::coord(10, 20), +ufo::coord(10, 20));
    }
    
    TEST(CoordTest, OperatorUnaryMinus) {
        ASSERT_EQ(ufo::coord(-3, -4), -ufo::coord(3, 4));
    }
    
    TEST(CoordTest, OperatorPlus) {
        ASSERT_EQ(ufo::coord(13, 27), ufo::coord(3, 7) + ufo::coord(10, 20));
    }

    TEST(CoordTest, OperatorMinus) {
        ASSERT_EQ(ufo::coord(20, 30), ufo::coord(28, 31) - ufo::coord(8, 1));
    }
    
    TEST(CoordTest, OperatorMultiply) {
        ASSERT_EQ(ufo::coord(20, 30), ufo::coord(2, 3) * 10);
    }
    
    TEST(CoordTest, OperatorMultiplyRight) {
        ASSERT_EQ(ufo::coord(10, 30), 2 * ufo::coord(5, 15));
    }
    
    TEST(CoordTest, OperatorDivide) {
        ASSERT_EQ(ufo::coord(2, 3), ufo::coord(20, 30) / 10);
    }
    
    TEST(CoordTest, Range) {
        auto r = ufo::range(ufo::coord(3, 4), ufo::coord(8, 7));
        static_assert(std::is_same_v<ufo::option<ufo::coord<int>>, decltype(r.next())>);
        ASSERT_EQ(ufo::coord(3, 4), *r.next());
        ASSERT_EQ(ufo::coord(4, 4), *r.next());
        ASSERT_EQ(ufo::coord(5, 4), *r.next());
        ASSERT_EQ(ufo::coord(6, 4), *r.next());
        ASSERT_EQ(ufo::coord(7, 4), *r.next());
        ASSERT_EQ(ufo::coord(3, 5), *r.next());
        ASSERT_EQ(ufo::coord(4, 5), *r.next());
        ASSERT_EQ(ufo::coord(5, 5), *r.next());
        ASSERT_EQ(ufo::coord(6, 5), *r.next());
        ASSERT_EQ(ufo::coord(7, 5), *r.next());
        ASSERT_EQ(ufo::coord(3, 6), *r.next());
        ASSERT_EQ(ufo::coord(4, 6), *r.next());
        ASSERT_EQ(ufo::coord(5, 6), *r.next());
        ASSERT_EQ(ufo::coord(6, 6), *r.next());
        ASSERT_EQ(ufo::coord(7, 6), *r.next());
        ASSERT_FALSE(r.next());
    }
    
    TEST(CoordTest, RangeWithoutBegin) {
        auto r = ufo::range(ufo::coord(2, 3));
        static_assert(std::is_same_v<ufo::option<ufo::coord<int>>, decltype(r.next())>);
        ASSERT_EQ(ufo::coord(0, 0), *r.next());
        ASSERT_EQ(ufo::coord(1, 0), *r.next());
        ASSERT_EQ(ufo::coord(0, 1), *r.next());
        ASSERT_EQ(ufo::coord(1, 1), *r.next());
        ASSERT_EQ(ufo::coord(0, 2), *r.next());
        ASSERT_EQ(ufo::coord(1, 2), *r.next());
        ASSERT_FALSE(r.next());
    }
    
    TEST(CoordTest, Zero) {
        ASSERT_EQ(ufo::coord(0, 0), ufo::coord<int>::zero());
    }
    
    TEST(CoordTest, One) {
        ASSERT_EQ(ufo::coord(1, 1), ufo::coord<int>::one());
    }
    
    TEST(CoordTest, Output) {
        auto ss = std::stringstream {};
        ss << ufo::coord<int>(1, 2);
        ASSERT_EQ("(1, 2)", ss.str());
    }
}
