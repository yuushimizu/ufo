#include <gtest/gtest.h>
#include "ufo/sequence/container_wrapper.hpp"
#include <type_traits>
#include <vector>
#include <array>

using namespace ufo;

namespace {
    TEST(ContainerWrapperTest, ConstLValue) {
        const auto v = std::vector<int> {10, 20, 30};
        auto cw = container_wrapper(v);
        static_assert(std::is_same_v<ContainerWrapper<const std::vector<int> &>, decltype(cw)>);
        static_assert(std::is_same_v<option<const int &>, decltype(cw.next())>);
        ASSERT_EQ(&v[0], &*cw.next());
        ASSERT_EQ(&v[1], &*cw.next());
        ASSERT_EQ(&v[2], &*cw.next());
        ASSERT_FALSE(cw.next());
    }
    
    TEST(ContainerWrapperTest, LValue) {
        auto v = std::vector<int> {10, 20, 30};
        auto cw = container_wrapper(v);
        static_assert(std::is_same_v<ContainerWrapper<std::vector<int> &>, decltype(cw)>);
        static_assert(std::is_same_v<option<int &>, decltype(cw.next())>);
        ASSERT_EQ(&v[0], &*cw.next());
        ASSERT_EQ(&v[1], &*cw.next());
        ASSERT_EQ(&v[2], &*cw.next());
        ASSERT_FALSE(cw.next());
    }
    
    TEST(ContainerWrapperTest, RValue) {
        auto cw = container_wrapper(std::vector<int> {10, 20, 30});
        static_assert(std::is_same_v<ContainerWrapper<std::vector<int>>, decltype(cw)>);
        static_assert(std::is_same_v<option<int>, decltype(cw.next())>);
        ASSERT_EQ(10, *cw.next());
        ASSERT_EQ(20, *cw.next());
        ASSERT_EQ(30, *cw.next());
        ASSERT_FALSE(cw.next());
    }
    
    TEST(ContainerWrapperTest, EmptyConstLValue) {
        const auto v = std::vector<int> {};
        auto cw = container_wrapper(v);
        ASSERT_FALSE(cw.next());
    }
    
    TEST(ContainerWrapperTest, EmptyLValue) {
        auto v = std::vector<int> {};
        auto cw = container_wrapper(v);
        ASSERT_FALSE(cw.next());
    }
    
    TEST(ContainerWrapperTest, EmptyRValue) {
        auto cw = container_wrapper(std::vector<int> {});
        ASSERT_FALSE(cw.next());
    }
    
    TEST(ContainerWrapperTest, CopyConstLValue) {
        const auto v = std::vector<int> {10, 20, 30};
        auto cw1 = container_wrapper(v);
        auto cw2 = cw1;
        ASSERT_EQ(&v[0], &*cw1.next());
        ASSERT_EQ(&v[0], &*cw2.next());
        ASSERT_EQ(&v[1], &*cw2.next());
        ASSERT_EQ(&v[2], &*cw2.next());
        ASSERT_FALSE(cw2.next());
    }
    
    TEST(ContainerWrapperTest, CopyLValue) {
        auto v = std::vector<int> {10, 20, 30};
        auto cw1 = container_wrapper(v);
        auto cw2 = cw1;
        ASSERT_EQ(&v[0], &*cw1.next());
        ASSERT_EQ(&v[0], &*cw2.next());
        ASSERT_EQ(&v[1], &*cw2.next());
        ASSERT_EQ(&v[2], &*cw2.next());
        ASSERT_FALSE(cw2.next());
    }

    TEST(ContainerWrapperTest, CopyRValue) {
        auto cw1 = container_wrapper(std::vector<int> {10, 20, 30});
        auto cw2 = cw1;
        ASSERT_EQ(10, *cw1.next());
        ASSERT_EQ(10, *cw2.next());
        ASSERT_EQ(20, *cw2.next());
        ASSERT_EQ(30, *cw2.next());
        ASSERT_FALSE(cw2.next());
    }
    
    TEST(ContainerWrapperTest, MoveConstLValue) {
        const auto v = std::vector<int> {10, 20, 30};
        auto cw1 = container_wrapper(v);
        auto cw2 = std::move(cw1);
        ASSERT_EQ(&v[0], &*cw2.next());
        ASSERT_EQ(&v[1], &*cw2.next());
        ASSERT_EQ(&v[2], &*cw2.next());
        ASSERT_FALSE(cw2.next());
    }
    
    TEST(ContainerWrapperTest, MoveLValue) {
        auto v = std::vector<int> {10, 20, 30};
        auto cw1 = container_wrapper(v);
        auto cw2 = std::move(cw1);
        ASSERT_EQ(&v[0], &*cw2.next());
        ASSERT_EQ(&v[1], &*cw2.next());
        ASSERT_EQ(&v[2], &*cw2.next());
        ASSERT_FALSE(cw2.next());
    }
    
    TEST(ContainerWrapperTest, MoveRValue) {
        auto cw1 = container_wrapper(std::vector<int> {10, 20, 30});
        auto cw2 = std::move(cw1);
        ASSERT_EQ(10, *cw2.next());
        ASSERT_EQ(20, *cw2.next());
        ASSERT_EQ(30, *cw2.next());
        ASSERT_FALSE(cw2.next());
    }
    
    TEST(ContainerWrapperTest, CopyAssignConstLValue) {
        const auto v = std::vector<int> {10, 20, 30};
        auto cw1 = container_wrapper(v);
        const auto v2 = std::vector<int> {1, 2, 3};
        auto cw2 = container_wrapper(v2);
        cw2 = cw1;
        ASSERT_EQ(&v[0], &*cw1.next());
        ASSERT_EQ(&v[0], &*cw2.next());
        ASSERT_EQ(&v[1], &*cw2.next());
        ASSERT_EQ(&v[2], &*cw2.next());
        ASSERT_FALSE(cw2.next());
    }
    
    TEST(ContainerWrapperTest, CopyAssignLValue) {
        auto v = std::vector<int> {10, 20, 30};
        auto cw1 = container_wrapper(v);
        auto v2 = std::vector<int> {1, 2, 3};
        auto cw2 = container_wrapper(v2);
        cw2 = cw1;
        ASSERT_EQ(&v[0], &*cw1.next());
        ASSERT_EQ(&v[0], &*cw2.next());
        ASSERT_EQ(&v[1], &*cw2.next());
        ASSERT_EQ(&v[2], &*cw2.next());
        ASSERT_FALSE(cw2.next());
    }
    
    TEST(ContainerWrapperTest, CopyAssignRValue) {
        auto cw1 = container_wrapper(std::vector<int> {10, 20, 30});
        auto cw2 = container_wrapper(std::vector<int> {1, 2, 3});
        cw2 = cw1;
        ASSERT_EQ(10, *cw1.next());
        ASSERT_EQ(10, *cw2.next());
        ASSERT_EQ(20, *cw2.next());
        ASSERT_EQ(30, *cw2.next());
        ASSERT_FALSE(cw2.next());
    }
    
    TEST(ContainerWrapperTest, MoveAssignConstLValue) {
        const auto v = std::vector<int> {10, 20, 30};
        auto cw1 = container_wrapper(v);
        const auto v2 = std::vector<int> {1, 2, 3};
        auto cw2 = container_wrapper(v2);
        cw2 = std::move(cw1);
        ASSERT_EQ(&v[0], &*cw2.next());
        ASSERT_EQ(&v[1], &*cw2.next());
        ASSERT_EQ(&v[2], &*cw2.next());
        ASSERT_FALSE(cw2.next());
    }
    
    TEST(ContainerWrapperTest, MoveAssignLValue) {
        auto v = std::vector<int> {10, 20, 30};
        auto cw1 = container_wrapper(v);
        auto v2 = std::vector<int> {1, 2, 3};
        auto cw2 = container_wrapper(v2);
        cw2 = std::move(cw1);
        ASSERT_EQ(&v[0], &*cw2.next());
        ASSERT_EQ(&v[1], &*cw2.next());
        ASSERT_EQ(&v[2], &*cw2.next());
        ASSERT_FALSE(cw2.next());
    }
    
    TEST(ContainerWrapperTest, MoveAssignRValue) {
        auto cw1 = container_wrapper(std::vector<int> {10, 20, 30});
        auto cw2 = container_wrapper(std::vector<int> {1, 2, 3});
        cw2 = std::move(cw1);
        ASSERT_EQ(10, *cw2.next());
        ASSERT_EQ(20, *cw2.next());
        ASSERT_EQ(30, *cw2.next());
        ASSERT_FALSE(cw2.next());
    }
    
    TEST(ContainerWrapperTest, MoveArrayConstLValue) {
        const auto a = std::array<int, 3> {10, 20, 30};
        auto cw1 = container_wrapper(a);
        auto cw2 = std::move(cw1);
        ASSERT_EQ(&a[0], &*cw2.next());
        ASSERT_EQ(&a[1], &*cw2.next());
        ASSERT_EQ(&a[2], &*cw2.next());
        ASSERT_FALSE(cw2.next());
    }
    
    TEST(ContainerWrapperTest, MoveArrayLValue) {
        auto a = std::array<int, 3> {10, 20, 30};
        auto cw1 = container_wrapper(a);
        auto cw2 = std::move(cw1);
        ASSERT_EQ(&a[0], &*cw2.next());
        ASSERT_EQ(&a[1], &*cw2.next());
        ASSERT_EQ(&a[2], &*cw2.next());
        ASSERT_FALSE(cw2.next());
    }
    
    TEST(ContainerWrapperTest, MoveArrayRValue) {
        auto cw1 = container_wrapper(std::array<int, 3> {10, 20, 30});
        auto cw2 = std::move(cw1);
        ASSERT_EQ(10, *cw2.next());
        ASSERT_EQ(20, *cw2.next());
        ASSERT_EQ(30, *cw2.next());
        ASSERT_FALSE(cw2.next());
    }
    
    TEST(ContainerWrapperTest, MoveArrayAssignConstLValue) {
        const auto a = std::array<int, 3> {10, 20, 30};
        auto cw1 = container_wrapper(a);
        const auto a2 = std::array<int, 3> {1, 2, 3};
        auto cw2 = container_wrapper(a2);
        cw2 = std::move(cw1);
        ASSERT_EQ(&a[0], &*cw2.next());
        ASSERT_EQ(&a[1], &*cw2.next());
        ASSERT_EQ(&a[2], &*cw2.next());
        ASSERT_FALSE(cw2.next());
    }
    
    TEST(ContainerWrapperTest, MoveAssignArrayLValue) {
        auto a = std::array<int, 3> {10, 20, 30};
        auto cw1 = container_wrapper(a);
        auto a2 = std::array<int, 3> {1, 2, 3};
        auto cw2 = container_wrapper(a2);
        cw2 = std::move(cw1);
        ASSERT_EQ(&a[0], &*cw2.next());
        ASSERT_EQ(&a[1], &*cw2.next());
        ASSERT_EQ(&a[2], &*cw2.next());
        ASSERT_FALSE(cw2.next());
    }
    
    TEST(ContainerWrapperTest, MoveAssignArrayRValue) {
        auto cw1 = container_wrapper(std::array<int, 3> {10, 20, 30});
        auto cw2 = container_wrapper(std::array<int, 3> {1, 2, 3});
        cw2 = std::move(cw1);
        ASSERT_EQ(10, *cw2.next());
        ASSERT_EQ(20, *cw2.next());
        ASSERT_EQ(30, *cw2.next());
        ASSERT_FALSE(cw2.next());
    }
    
    TEST(ContainerWrapperTest, NonCopyableElementsLValue) {
        auto v = std::vector<std::unique_ptr<int>> {};
        v.push_back(std::make_unique<int>(10));
        v.push_back(std::make_unique<int>(20));
        auto cw = container_wrapper(v);
        auto p = cw.next();
        ASSERT_EQ(10, **p);
    }
    
    TEST(ContainerWrapperTest, NonCopyableElementsRValue) {
        auto v = std::vector<std::unique_ptr<int>> {};
        v.push_back(std::make_unique<int>(10));
        v.push_back(std::make_unique<int>(20));
        auto cw = container_wrapper(std::move(v));
        auto p = std::move(*cw.next());
        ASSERT_EQ(10, *p);
    }
}

