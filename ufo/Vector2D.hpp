#ifndef ufo_Vector2D
#define ufo_Vector2D

#include <vector>
#include "coord.hpp"
#include "iterator.hpp"
#include "sequence/range.hpp"
#include "sequence/container_wrapper.hpp"
#include "sequence/map.hpp"

namespace ufo {
    namespace vector2d_detail {
        template <typename Indices, typename Values>
        constexpr auto pairs(Indices &&indices, Values &&values) {
            return map([](auto &&index, auto &&value) {
                return std::tuple<coord<int>, sequence_element_t<Values>>(std::forward<decltype(index)>(index), std::forward<decltype(value)>(value));
            }, std::forward<Indices>(indices), std::forward<Values>(values));
        }
    }
    
    template <typename T>
    class Vector2D final {
    public:
        constexpr explicit Vector2D(coord<int> size, const T &initial_value) : size_(std::move(size)), values_(size_.area(), initial_value) {
        }
        
        constexpr explicit Vector2D(coord<int> size) : size_(std::move(size)), values_(size_.area()) {
        }
        
        constexpr explicit Vector2D() : size_ {}, values_ {} {
        }
        
        constexpr auto size() const {
            return size_;
        }
        
        constexpr const T &operator[](const coord<int> &index) const& {
            return values_[index.y() * size_.x() + index.x()];
        }
        
        constexpr T &operator[](const coord<int> &index) & {
            return values_[index.y() * size_.x() + index.x()];
        }
        
        constexpr T operator[](const coord<int> &index) && {
            return std::move(values_[index.y() * size_.x() + index.x()]);
        }
        
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
        
        constexpr auto indices() const {
            return range(size_);
        }
        
        constexpr auto values() const & {
            return container_wrapper(values_);
        }
        
        constexpr auto values() & {
            return container_wrapper(values_);
        }
        
        constexpr auto values() && {
            return container_wrapper(std::move(values_));
        }
        
        constexpr auto pairs() const & {
            return vector2d_detail::pairs(indices(), values());
        }
        
        constexpr auto pairs() & {
            return vector2d_detail::pairs(indices(), values());
        }
        
        constexpr auto pairs() && {
            return vector2d_detail::pairs(indices(), container_wrapper(std::move(values_)));
        }
        
        template <typename T1, typename T2>
        friend constexpr bool operator==(const Vector2D<T1> &lhs, const Vector2D<T2> &rhs);
        
    private:
        coord<int> size_;
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
}

#endif
