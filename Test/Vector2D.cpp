#include <gtest/gtest.h>
#include "ufo/Vector2D.hpp"
#include "ufo/Coord.hpp"
#include "ufo/range/as_vector.hpp"

using namespace ufo;

namespace {
    TEST(Vector2DTest, IndexedRange) {
        Vector2D<int> v(coord(2, 3));
        v[coord(0, 0)] = 0;
        v[coord(1, 0)] = 10;
        v[coord(0, 1)] = 1;
        v[coord(1, 1)] = 11;
        v[coord(0, 2)] = 2;
        v[coord(1, 2)] = 12;
        std::vector<Coord<int>> indices;
        std::vector<int> values;
        for (auto &&entry : indexed_range(v)) {
            indices.emplace_back(entry.index);
            values.emplace_back(entry.value);
        }
        ASSERT_EQ((std::vector<Coord<int>> {coord(0, 0), coord(1, 0), coord(0, 1), coord(1, 1), coord(0, 2), coord(1, 2)}), indices);
        ASSERT_EQ((std::vector<int> {0, 10, 1, 11, 2, 12}), values);
    }
    
    TEST(Vector2DTest, IndexedRangeReturnsReference) {
        Vector2D<int> v(coord(3, 2));
        for (auto &&entry : indexed_range(v)) {
            entry.value = entry.index.area();
        }
        ASSERT_EQ((std::vector<int> {0, 0, 0, 0, 1, 2}), v | as_vector());
    }
}
