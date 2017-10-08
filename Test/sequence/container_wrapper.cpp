#include <gtest/gtest.h>
#include "ufo/sequence/container_wrapper.hpp"
#include <type_traits>
#include <vector>
#include <array>

using namespace ufo;

namespace {
    TEST(ContainerWrapperTest, ConstLValue) {
        const std::vector<int> v {10, 20, 30};
        auto cw = container_wrapper(v);
        static_assert(std::is_same_v<ContainerWrapper<const std::vector<int> &>, decltype(cw)>);
        static_assert(std::is_same_v<const int &, decltype(cw.front())>);
        ASSERT_EQ(&v[0], &cw.front());
    }
    
    TEST(ContainerWrapperTest, LValue) {
        std::vector<int> v {10, 20, 30};
        auto cw = container_wrapper(v);
        static_assert(std::is_same_v<ContainerWrapper<std::vector<int> &>, decltype(cw)>);
        static_assert(std::is_same_v<int &, decltype(cw.front())>);
        ASSERT_EQ(&v[0], &cw.front());
    }
    
    TEST(ContainerWrapperTest, RValue) {
        auto cw = container_wrapper(std::vector<int> {10, 20, 30});
        static_assert(std::is_same_v<ContainerWrapper<std::vector<int>>, decltype(cw)>);
        static_assert(std::is_same_v<int, decltype(cw.front())>);
        ASSERT_EQ(10, cw.front());
    }
    
    TEST(ContainerWrapperTest, PopConstLValue) {
        const std::vector<int> v {10, 20, 30};
        auto cw = container_wrapper(v);
        cw.pop();
        ASSERT_EQ(&v[1], &cw.front());
    }
    
    TEST(ContainerWrapperTest, PopLValue) {
        std::vector<int> v {10, 20, 30};
        auto cw = container_wrapper(v);
        cw.pop();
        ASSERT_EQ(&v[1], &cw.front());
    }
    
    TEST(ContainerWrapperTest, PopRValue) {
        auto cw = container_wrapper(std::vector<int> {10, 20, 30});
        cw.pop();
        ASSERT_EQ(20, cw.front());
    }
    
    TEST(ContainerWrapperTest, EmptyConstLValue) {
        const std::vector<int> v {};
        auto cw = container_wrapper(v);
        ASSERT_TRUE(cw.empty());
    }
    
    TEST(ContainerWrapperTest, EmptyLValue) {
        std::vector<int> v {};
        auto cw = container_wrapper(v);
        ASSERT_TRUE(cw.empty());
    }
    
    TEST(ContainerWrapperTest, EmptyRValue) {
        auto cw = container_wrapper(std::vector<int> {});
        ASSERT_TRUE(cw.empty());
    }
    
    TEST(ContainerWrapperTest, EmptyConstLValueFalse) {
        const std::vector<int> v {10, 20, 30};
        auto cw = container_wrapper(v);
        ASSERT_FALSE(cw.empty());
    }
    
    TEST(ContainerWrapperTest, EmptyLValueFalse) {
        std::vector<int> v {10, 20, 30};
        auto cw = container_wrapper(v);
        ASSERT_FALSE(cw.empty());
    }
    
    TEST(ContainerWrapperTest, EmptyRValueFalse) {
        auto cw = container_wrapper(std::vector<int> {10, 20, 30});
        ASSERT_FALSE(cw.empty());
    }
    
    TEST(ContainerWrapperTest, EmptyPopConstLValue) {
        const std::vector<int> v {10};
        auto cw = container_wrapper(v);
        cw.pop();
        ASSERT_TRUE(cw.empty());
    }
    
    TEST(ContainerWrapperTest, EmptyPopLValue) {
        std::vector<int> v {10};
        auto cw = container_wrapper(v);
        cw.pop();
        ASSERT_TRUE(cw.empty());
    }
    
    TEST(ContainerWrapperTest, EmptyPopRValue) {
        auto cw = container_wrapper(std::vector<int> {10});
        cw.pop();
        ASSERT_TRUE(cw.empty());
    }
    
    TEST(ContainerWrapperTest, EmptyPopLValueFalse) {
        std::vector<int> v {10, 20};
        auto cw = container_wrapper(v);
        cw.pop();
        ASSERT_FALSE(cw.empty());
    }
    
    TEST(ContainerWrapperTest, EmptyPopRValueFalse) {
        auto cw = container_wrapper(std::vector<int> {10, 20});
        cw.pop();
        ASSERT_FALSE(cw.empty());
    }
    
    TEST(ContainerWrapperTest, CopyConstLValue) {
        const std::vector<int> v {10, 20, 30};
        auto cw1 = container_wrapper(v);
        auto cw2 = cw1;
        ASSERT_EQ(&v[0], &cw1.front());
        ASSERT_EQ(&v[0], &cw2.front());
        cw2.pop();
        ASSERT_EQ(&v[1], &cw2.front());
        cw2.pop();
        ASSERT_EQ(&v[2], &cw2.front());
        cw2.pop();
        ASSERT_TRUE(cw2.empty());
    }
    
    TEST(ContainerWrapperTest, CopyLValue) {
        std::vector<int> v {10, 20, 30};
        auto cw1 = container_wrapper(v);
        auto cw2 = cw1;
        ASSERT_EQ(&v[0], &cw1.front());
        ASSERT_EQ(&v[0], &cw2.front());
        cw2.pop();
        ASSERT_EQ(&v[1], &cw2.front());
        cw2.pop();
        ASSERT_EQ(&v[2], &cw2.front());
        cw2.pop();
        ASSERT_TRUE(cw2.empty());
    }

    TEST(ContainerWrapperTest, CopyRValue) {
        auto cw1 = container_wrapper(std::vector<int> {10, 20, 30});
        auto cw2 = cw1;
        ASSERT_EQ(10, cw1.front());
        ASSERT_EQ(10, cw2.front());
        cw2.pop();
        ASSERT_EQ(20, cw2.front());
        cw2.pop();
        ASSERT_EQ(30, cw2.front());
        cw2.pop();
        ASSERT_TRUE(cw2.empty());
    }
    
    TEST(ContainerWrapperTest, MoveConstLValue) {
        const std::vector<int> v {10, 20, 30};
        auto cw1 = container_wrapper(v);
        auto cw2 = std::move(cw1);
        ASSERT_EQ(&v[0], &cw2.front());
        cw2.pop();
        ASSERT_EQ(&v[1], &cw2.front());
        cw2.pop();
        ASSERT_EQ(&v[2], &cw2.front());
        cw2.pop();
        ASSERT_TRUE(cw2.empty());
    }
    
    TEST(ContainerWrapperTest, MoveLValue) {
        std::vector<int> v {10, 20, 30};
        auto cw1 = container_wrapper(v);
        auto cw2 = std::move(cw1);
        ASSERT_EQ(&v[0], &cw2.front());
        cw2.pop();
        ASSERT_EQ(&v[1], &cw2.front());
        cw2.pop();
        ASSERT_EQ(&v[2], &cw2.front());
        cw2.pop();
        ASSERT_TRUE(cw2.empty());
    }
    
    TEST(ContainerWrapperTest, MoveRValue) {
        auto cw1 = container_wrapper(std::vector<int> {10, 20, 30});
        auto cw2 = std::move(cw1);
        ASSERT_EQ(10, cw2.front());
        cw2.pop();
        ASSERT_EQ(20, cw2.front());
        cw2.pop();
        ASSERT_EQ(30, cw2.front());
        cw2.pop();
        ASSERT_TRUE(cw2.empty());
    }
    
    TEST(ContainerWrapperTest, CopyAssignConstLValue) {
        const std::vector<int> v {10, 20, 30};
        auto cw1 = container_wrapper(v);
        const std::vector<int> v2 {1, 2, 3};
        auto cw2 = container_wrapper(v2);
        cw2 = cw1;
        ASSERT_EQ(&v[0], &cw1.front());
        ASSERT_EQ(&v[0], &cw2.front());
        cw2.pop();
        ASSERT_EQ(&v[1], &cw2.front());
        cw2.pop();
        ASSERT_EQ(&v[2], &cw2.front());
        cw2.pop();
        ASSERT_TRUE(cw2.empty());
    }
    
    TEST(ContainerWrapperTest, CopyAssignLValue) {
        std::vector<int> v {10, 20, 30};
        auto cw1 = container_wrapper(v);
        std::vector<int> v2 {1, 2, 3};
        auto cw2 = container_wrapper(v2);
        cw2 = cw1;
        ASSERT_EQ(&v[0], &cw1.front());
        ASSERT_EQ(&v[0], &cw2.front());
        cw2.pop();
        ASSERT_EQ(&v[1], &cw2.front());
        cw2.pop();
        ASSERT_EQ(&v[2], &cw2.front());
        cw2.pop();
        ASSERT_TRUE(cw2.empty());
    }
    
    TEST(ContainerWrapperTest, CopyAssignRValue) {
        auto cw1 = container_wrapper(std::vector<int> {10, 20, 30});
        auto cw2 = container_wrapper(std::vector<int> {1, 2, 3});
        cw2 = cw1;
        ASSERT_EQ(10, cw1.front());
        ASSERT_EQ(10, cw2.front());
        cw2.pop();
        ASSERT_EQ(20, cw2.front());
        cw2.pop();
        ASSERT_EQ(30, cw2.front());
        cw2.pop();
        ASSERT_TRUE(cw2.empty());
    }
    
    TEST(ContainerWrapperTest, MoveAssignConstLValue) {
        const std::vector<int> v {10, 20, 30};
        auto cw1 = container_wrapper(v);
        const std::vector<int> v2 {1, 2, 3};
        auto cw2 = container_wrapper(v2);
        cw2 = std::move(cw1);
        ASSERT_EQ(&v[0], &cw2.front());
        cw2.pop();
        ASSERT_EQ(&v[1], &cw2.front());
        cw2.pop();
        ASSERT_EQ(&v[2], &cw2.front());
        cw2.pop();
        ASSERT_TRUE(cw2.empty());
    }
    
    TEST(ContainerWrapperTest, MoveAssignLValue) {
        std::vector<int> v {10, 20, 30};
        auto cw1 = container_wrapper(v);
        std::vector<int> v2 {1, 2, 3};
        auto cw2 = container_wrapper(v2);
        cw2 = std::move(cw1);
        ASSERT_EQ(&v[0], &cw2.front());
        cw2.pop();
        ASSERT_EQ(&v[1], &cw2.front());
        cw2.pop();
        ASSERT_EQ(&v[2], &cw2.front());
        cw2.pop();
        ASSERT_TRUE(cw2.empty());
    }
    
    TEST(ContainerWrapperTest, MoveAssignRValue) {
        auto cw1 = container_wrapper(std::vector<int> {10, 20, 30});
        auto cw2 = container_wrapper(std::vector<int> {1, 2, 3});
        cw2 = std::move(cw1);
        ASSERT_EQ(10, cw2.front());
        cw2.pop();
        ASSERT_EQ(20, cw2.front());
        cw2.pop();
        ASSERT_EQ(30, cw2.front());
        cw2.pop();
        ASSERT_TRUE(cw2.empty());
    }
    
    TEST(ContainerWrapperTest, MoveArrayConstLValue) {
        const std::array<int, 3> a {10, 20, 30};
        auto cw1 = container_wrapper(a);
        auto cw2 = std::move(cw1);
        ASSERT_EQ(&a[0], &cw2.front());
        cw2.pop();
        ASSERT_EQ(&a[1], &cw2.front());
        cw2.pop();
        ASSERT_EQ(&a[2], &cw2.front());
        cw2.pop();
        ASSERT_TRUE(cw2.empty());
    }
    
    TEST(ContainerWrapperTest, MoveArrayLValue) {
        std::array<int, 3> a {10, 20, 30};
        auto cw1 = container_wrapper(a);
        auto cw2 = std::move(cw1);
        ASSERT_EQ(&a[0], &cw2.front());
        cw2.pop();
        ASSERT_EQ(&a[1], &cw2.front());
        cw2.pop();
        ASSERT_EQ(&a[2], &cw2.front());
        cw2.pop();
        ASSERT_TRUE(cw2.empty());
    }
    
    TEST(ContainerWrapperTest, MoveArrayRValue) {
        auto cw1 = container_wrapper(std::array<int, 3> {10, 20, 30});
        auto cw2 = std::move(cw1);
        ASSERT_EQ(10, cw2.front());
        cw2.pop();
        ASSERT_EQ(20, cw2.front());
        cw2.pop();
        ASSERT_EQ(30, cw2.front());
        cw2.pop();
        ASSERT_TRUE(cw2.empty());
    }
    
    TEST(ContainerWrapperTest, MoveArrayAssignConstLValue) {
        const std::array<int, 3> a {10, 20, 30};
        auto cw1 = container_wrapper(a);
        const std::array<int, 3> a2 {1, 2, 3};
        auto cw2 = container_wrapper(a2);
        cw2 = std::move(cw1);
        ASSERT_EQ(&a[0], &cw2.front());
        cw2.pop();
        ASSERT_EQ(&a[1], &cw2.front());
        cw2.pop();
        ASSERT_EQ(&a[2], &cw2.front());
        cw2.pop();
        ASSERT_TRUE(cw2.empty());
    }
    
    TEST(ContainerWrapperTest, MoveAssignArrayLValue) {
        std::array<int, 3> a {10, 20, 30};
        auto cw1 = container_wrapper(a);
        std::array<int, 3> a2 {1, 2, 3};
        auto cw2 = container_wrapper(a2);
        cw2 = std::move(cw1);
        ASSERT_EQ(&a[0], &cw2.front());
        cw2.pop();
        ASSERT_EQ(&a[1], &cw2.front());
        cw2.pop();
        ASSERT_EQ(&a[2], &cw2.front());
        cw2.pop();
        ASSERT_TRUE(cw2.empty());
    }
    
    TEST(ContainerWrapperTest, MoveAssignArrayRValue) {
        auto cw1 = container_wrapper(std::array<int, 3> {10, 20, 30});
        auto cw2 = container_wrapper(std::array<int, 3> {1, 2, 3});
        cw2 = std::move(cw1);
        ASSERT_EQ(10, cw2.front());
        cw2.pop();
        ASSERT_EQ(20, cw2.front());
        cw2.pop();
        ASSERT_EQ(30, cw2.front());
        cw2.pop();
        ASSERT_TRUE(cw2.empty());
    }
}

