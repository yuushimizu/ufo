#include <gtest/gtest.h>
#include "ufo/sequence2/container_sequence.hpp"
#include "test_utility.hpp"
#include <type_traits>

namespace {
    TEST(ContainerSequenceTest, FromLValue) {
        auto v = std::vector {10, 20, 30};
        auto s = ufo::s2::container_sequence(v);
        ufo::test::sequence_assert<int &>(s);
        auto i = s.begin();
        ASSERT_EQ(&v[0], &*i.next());
        ASSERT_EQ(&v[1], &*i.next());
        ASSERT_EQ(&v[2], &*i.next());
        ASSERT_EQ(ufo::nullopt, i.next());
    }
    
    TEST(ContainerSequenceTest, FromConstLValue) {
        const auto v = std::vector {10, 20, 30};
        auto s = ufo::s2::container_sequence(v);
        ufo::test::sequence_assert<const int &>(s);
        auto i = s.begin();
        ASSERT_EQ(&v[0], &*i.next());
        ASSERT_EQ(&v[1], &*i.next());
        ASSERT_EQ(&v[2], &*i.next());
        ASSERT_EQ(ufo::nullopt, i.next());
    }
    
    TEST(ContainerSequenceTest, FromRValue) {
        auto s = ufo::s2::container_sequence(std::vector {45, 67, 89, 111});
        ufo::test::sequence_assert<const int &>(s);
        auto i = s.begin();
        ASSERT_EQ(45, *i.next());
        ASSERT_EQ(67, *i.next());
        ASSERT_EQ(89, *i.next());
        ASSERT_EQ(111, *i.next());
        ASSERT_EQ(ufo::nullopt, i.next());
    }
    
    TEST(ContainerSequenceTest, Empty) {
        auto s = ufo::s2::container_sequence(std::vector<int> {});
        auto i = s.begin();
        ASSERT_EQ(ufo::nullopt, i.next());
    }
    
    TEST(ContainerSequenceTest, MoveRValue) {
        auto v = std::vector<std::unique_ptr<int>> {};
        v.push_back(std::make_unique<int>(2));
        v.push_back(std::make_unique<int>(8));
        v.push_back(std::make_unique<int>(44));
        auto s = ufo::s2::container_sequence(std::move(v));
        ufo::test::sequence_assert<const std::unique_ptr<int> &>(s);
        auto i = s.begin();
        ASSERT_EQ(2, **i.next());
        ASSERT_EQ(8, **i.next());
        ASSERT_EQ(44, **i.next());
        ASSERT_EQ(ufo::nullopt, i.next());
    }
}
