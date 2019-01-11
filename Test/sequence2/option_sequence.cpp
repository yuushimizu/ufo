#include <gtest/gtest.h>
#include "ufo/sequence2/option_sequence.hpp"
#include "test_utility.hpp"
#include <type_traits>

namespace {
    TEST(OptionSequenceTest, Value) {
        auto s = ufo::s2::option_sequence(ufo::option<int>(123));
        ufo::test::sequence_assert<int>(s);
        auto i = s.begin();
        ASSERT_EQ(123, *i.next());
        ASSERT_EQ(ufo::nullopt, i.next());
    }
    
    TEST(OptionSequenceTest, ValueNullopt) {
        auto s = ufo::s2::option_sequence(ufo::option<int> {});
        auto i = s.begin();
        ASSERT_EQ(ufo::nullopt, i.next());
    }
    
    TEST(OptionSequenceTest, Reference) {
        int x = 50;
        auto s = ufo::s2::option_sequence(ufo::option<int &>(x));
        ufo::test::sequence_assert<int &>(s);
        auto i = s.begin();
        ASSERT_EQ(&x, &*i.next());
        ASSERT_EQ(ufo::nullopt, i.next());
    }
    
    TEST(OptionSequenceTest, ReferenceNullopt) {
        auto s = ufo::s2::option_sequence(ufo::option<int &>());
        auto i = s.begin();
        ASSERT_EQ(ufo::nullopt, i.next());
    }
}
