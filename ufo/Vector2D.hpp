#ifndef ufo_Vector2D
#define ufo_Vector2D

#include <vector>
#include "Coord.hpp"
#include "iterator.hpp"
#include "range/combine.hpp"
#include "range/indexed.hpp"

namespace ufo {
    namespace vector2d_detail {
        template <typename T>
        auto indexed_range(T &vector) {
            return combine(range(vector.size()), vector, [](auto &&index, auto &&value) {return indexed_entry(std::forward<decltype(index)>(index), std::forward<decltype(value)>(value));});
        }
    }
    
    template <typename T>
    class Vector2D final {
    public:
        constexpr Vector2D(Coord<int> size, const T &initial_value) : size_(std::move(size)), values_(size_.area(), initial_value) {
        }
        
        constexpr explicit Vector2D(const Coord<int> &size) : size_(std::move(size)), values_(size_.area()) {
        }
        
        constexpr Vector2D() : size_ {}, values_ {} {
        }
        
        constexpr auto size() const {
            return size_;
        }
        
        constexpr const T &operator[](const Coord<int> &index) const& {
            return values_[index.y() * size_.x() + index.x()];
        }
        
        constexpr T &operator[](const Coord<int> &index) & {
            return values_[index.y() * size_.x() + index.x()];
        }
        
        constexpr T operator[](const Coord<int> &index) && {
            return std::move(values_[index.y() * size_.x() + index.x()]);
        }
        
        template <typename T1, typename T2>
        friend constexpr bool operator==(const Vector2D<T1> &lhs, const Vector2D<T2> &rhs);
        
        constexpr auto begin() const {
            return adl_begin(values_);
        }
        
        constexpr auto begin() {
            return adl_begin(values_);
        }
        
        constexpr auto end() const {
            return adl_end(values_);
        }
        
        constexpr auto end() {
            return adl_end(values_);
        }
        
    private:
        Coord<int> size_;
        std::vector<T> values_;
    };
    
    template <typename T1, typename T2>
    constexpr bool operator==(const Vector2D<T1> &lhs, const Vector2D<T2> &rhs) {
        return lhs.values_ == rhs.values_;
    }
    
    template <typename T1, typename T2>
    constexpr bool operator!=(const Vector2D<T1> &lhs, const Vector2D<T2> &rhs) {
        return !(lhs == rhs);
    }
    
    template <typename T>
    constexpr bool contains(const Vector2D<T> &vector, const Coord<int> &index) {
        return contains(vector.size(), index);
    }
    
    template <typename T>
    auto indexed_range(const Vector2D<T> &vector) {
        return vector2d_detail::indexed_range(vector);
    }
    
    template <typename T>
    auto indexed_range(Vector2D<T> &vector) {
        return vector2d_detail::indexed_range(vector);
    }
}

#endif
