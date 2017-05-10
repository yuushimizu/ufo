#ifndef ufo_Rect
#define ufo_Rect

#include <iostream>
#include "Coord.hpp"
#include "range/irange.hpp"

namespace ufo {
    template <typename T>
    class Rect final {
    public:
        constexpr Rect(Coord<T> origin, Coord<T> size) noexcept : origin_(std::move(origin)), size_(std::move(size)) {
        }
        
        constexpr Rect() noexcept : origin_ {}, size_ {} {
        }
        
        Coord<T> origin() const {
            return origin_;
        }
        
        Coord<T> size() const {
            return size_;
        }
        
        constexpr auto opposite_origin() const noexcept {
            return coord(origin_.x() + size_.x(), origin_.y() + size_.y());
        }
        
    private:
        Coord<T> origin_;
        Coord<T> size_;
    };
    
    template <typename LHS, typename RHS>
    constexpr bool operator==(const Rect<LHS> &lhs, const Rect<RHS> &rhs) noexcept {
        return lhs.origin() == rhs.origin() && lhs.size() == rhs.size();
    }
    
    template <typename LHS, typename RHS>
    constexpr bool operator!=(const Rect<LHS> &lhs, const Rect<RHS> &rhs) noexcept {
        return !(lhs == rhs);
    }
    
    template <typename RectT, typename PointT>
    constexpr bool contains(const Rect<RectT> &rect, const Coord<PointT> &point) noexcept {
        return point.x() >= rect.origin().x() && point.y() >= rect.origin().y() && point.x() < rect.opposite_origin().x() && point.y() < rect.opposite_origin().y();
    }
    
    template <typename T>
    auto range(const Rect<T> &rect) noexcept {
        return range(rect.origin(), rect.origin() + rect.size());
    }
    
    template <typename T>
    constexpr auto x_range(const Rect<T> &rect) noexcept {
        return irange(rect.origin().x(), rect.origin().x() + rect.size().x());
    }
    
    template <typename T>
    constexpr auto y_range(const Rect<T> &rect) noexcept {
        return irange(rect.origin().y(), rect.origin().y() + rect.size().y());
    }
    
    template <typename T>
    auto &operator<<(std::ostream &o, const Rect<T> &rect) {
        return o << "(" << rect.origin() << ", " << rect.size() << ")";
    }
}

#endif
