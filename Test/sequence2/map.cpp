#include <gtest/gtest.h>
#include "ufo/sequence2/map.hpp"
#include "ufo/sequence2/range.hpp"
#include "ufo/placeholder.hpp"
#include "../test_utility.hpp"
#include "test_utility.hpp"
#include <type_traits>
#include <memory>

#include "ufo/sequence2/foreach.hpp"
#include "ufo/sequence2/take.hpp"

namespace {
    using ufo::_;
    
    TEST(MapTest, Map) {
        auto s = ufo::s2::range(0, 5) | ufo::s2::map(_ * 3);
        ufo::test::sequence_assert<int>(s);
        ASSERT_EQ((std::vector {0, 3, 6, 9, 12}), ufo::test::collect_values(s));
    }
    
    TEST(MapTest, MapToReference) {
        auto data = std::vector {80, 140, 200};
        auto s = std::vector {2, 0, 1} | ufo::s2::map([&data](int i) -> int & {return data[i];});
        ufo::test::sequence_assert<int &>(s);
        auto i = s.begin();
        ASSERT_EQ(&data[2], &*i.next());
        ASSERT_EQ(&data[0], &*i.next());
        ASSERT_EQ(&data[1], &*i.next());
        ASSERT_EQ(ufo::nullopt, i.next());
    }
    TEST(MapTest, IterateTwice) {
        auto s = ufo::s2::range(3, 6) | ufo::s2::map(_ * 2);
        ASSERT_EQ((std::vector {6, 8, 10}), ufo::test::collect_values(s));
        ASSERT_EQ((std::vector {6, 8, 10}), ufo::test::collect_values(s));
    }
    
    TEST(MapTest, Empty) {
        auto s = ufo::s2::range(5, 0) | ufo::s2::map(_ * 8);
        ASSERT_EQ(ufo::nullopt, s.begin().next());
    }
    
    TEST(MapTest, FunctionIsCopiedIntoSequence) {
        auto s = []() {
            auto f = [x = std::make_shared<int>(8)](int n) {return n * *x;};
            auto op = ufo::s2::map(f);
            return ufo::s2::range(0, 3) | op;
        }();
        ASSERT_EQ((std::vector {0, 8, 16}), ufo::test::collect_values(s));
    }
    
    TEST(MapTest, FunctionIsMovedIntoSequence) {
        auto s = ufo::s2::range(0, 3) | ufo::s2::map([x = std::make_unique<int>(5)](int n) {return n * *x;});
        static_assert(!std::is_copy_constructible_v<decltype(s)>);
        ASSERT_EQ((std::vector {0, 5, 10}), ufo::test::collect_values(s));
    }
    
    TEST(MapTest, ReferenceWrapperIsCopied) {
        auto f = ufo::test::nocopy(_ * 2);
        auto s = ufo::s2::range(0, 4) | ufo::s2::map(std::ref(f));
        ASSERT_EQ((std::vector {0, 2, 4, 6}), ufo::test::collect_values(s));
        auto s2 = s;
        ASSERT_EQ((std::vector {0, 2, 4, 6}), ufo::test::collect_values(s2));
    }
    
    TEST(MapTest, MoveSequence) {
        auto s = ufo::test::nocopyseq(ufo::s2::range(0, 3)) | ufo::s2::map(_ * 7);
        static_assert(!std::is_copy_constructible_v<decltype(s)>);
        ASSERT_EQ((std::vector {0,7, 14}), ufo::test::collect_values(s));
    }
    
    TEST(MapTest, WithContainer) {
        auto s = std::vector {8, 9, 10} | ufo::s2::map(_ + 10);
        ASSERT_EQ((std::vector {18, 19, 20}), ufo::test::collect_values(s));
    }
    
    TEST(MapTest, MultipleSequences) {
        auto s = ufo::s2::map([](int a, int b, int c) {return a * 100 + b * 10 + c;}, std::vector {3, 5, 7}, std::vector {1, 2, 3}, std::vector {9, 8, 7});
        ufo::test::sequence_assert<int>(s);
        ASSERT_EQ((std::vector {319, 528, 737}), ufo::test::collect_values(s));
    }
    
    TEST(MapTest, MoveMultipleSequences) {
        auto s = ufo::s2::map([](int x, int y) {return x + y;}, ufo::test::nocopyseq(ufo::s2::range(1, 4)), ufo::test::nocopyseq(ufo::s2::range(11, 14)));
        ASSERT_EQ((std::vector {12, 14, 16}), ufo::test::collect_values(s));
    }
    
    TEST(MapTest, UnmatchedCount) {
        auto s = ufo::s2::map([](int x, int y) {return x + y;}, std::vector {10, 20}, std::vector {3, 4, 5});
        ASSERT_EQ((std::vector {13, 24}), ufo::test::collect_values(s));
    }
}
