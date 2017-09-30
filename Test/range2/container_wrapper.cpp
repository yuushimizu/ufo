#include <gtest/gtest.h>
#include "ufo/range2/container_wrapper.hpp"
#include <type_traits>

using namespace ufo;

namespace {
    TEST(ContainerWrapperTest, ConstLValue) {
        const std::vector<int> v {10, 20, 30};
        auto cw = container_wrapper(v);
        static_assert(std::is_same_v<ContainerWrapper<const std::vector<int> &>, decltype(cw)>);
        static_assert(std::is_same_v<const int &, decltype(cw.first())>);
        ASSERT_EQ(&v[0], &cw.first());
    }
    
    TEST(ContainerWrapperTest, LValue) {
        std::vector<int> v {10, 20, 30};
        auto cw = container_wrapper(v);
        static_assert(std::is_same_v<ContainerWrapper<std::vector<int> &>, decltype(cw)>);
        static_assert(std::is_same_v<int &, decltype(cw.first())>);
        ASSERT_EQ(&v[0], &cw.first());
    }
    
    TEST(ContainerWrapperTest, RValue) {
        auto cw = container_wrapper(std::vector<int> {10, 20, 30});
        static_assert(std::is_same_v<ContainerWrapper<std::vector<int>>, decltype(cw)>);
        static_assert(std::is_same_v<int, decltype(cw.first())>);
        ASSERT_EQ(10, cw.first());
    }
    
    TEST(ContainerWrapperTest, CopyConstLValue) {
        const std::vector<int> v {10, 20, 30};
        auto cw1 = container_wrapper(v);
        auto cw2 = cw1;
        ASSERT_EQ(&v[0], &cw1.first());
        ASSERT_EQ(&v[0], &cw2.first());
    }
    
    TEST(ContainerWrapperTest, CopyLValue) {
        std::vector<int> v {10, 20, 30};
        auto cw1 = container_wrapper(v);
        auto cw2 = cw1;
        ASSERT_EQ(&v[0], &cw1.first());
        ASSERT_EQ(&v[0], &cw2.first());
    }

    TEST(ContainerWrapperTest, CopyRValue) {
        auto cw1 = container_wrapper(std::vector<int> {10, 20, 30});
        auto cw2 = cw1;
        ASSERT_EQ(10, cw1.first());
        ASSERT_EQ(10, cw2.first());
    }
    
    TEST(ContainerWrapperTest, MoveConstLValue) {
        const std::vector<int> v {10, 20, 30};
        auto cw1 = container_wrapper(v);
        auto cw2 = std::move(cw1);
        ASSERT_EQ(&v[0], &cw2.first());
    }
    
    TEST(ContainerWrapperTest, MoveLValue) {
        std::vector<int> v {10, 20, 30};
        auto cw1 = container_wrapper(v);
        auto cw2 = std::move(cw1);
        ASSERT_EQ(&v[0], &cw2.first());
    }
    
    TEST(ContainerWrapperTest, MoveRValue) {
        auto cw1 = container_wrapper(std::vector<std::string> {"foo", "bar", "baz"});
        auto cw2 = std::move(cw1);
        ASSERT_EQ("bar", cw2.first());
    }
}

