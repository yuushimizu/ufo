#ifndef ufo_Rect
#define ufo_Rect

#include <iostream>
#include "Coord.hpp"
#include "sequence/range.hpp"

namespace ufo {
    template <typename T>
    class Rect final {
    public:
        constexpr Rect(Coord<T> origin, Coord<T> size) noexcept : origin_(std::move(origin)), size_(std::move(size)) {
        }
        
        constexpr explicit Rect() noexcept : origin_ {}, size_ {} {
        }
        
        Coord<T> origin() const {
            return origin_;
        }
        
        Coord<T> size() const {
            return size_;
        }
        
        constexpr auto opposite_origin() const noexcept {
            return coord(origin().x() + size().x(), origin().y() + size().y());
        }
        
        template <typename CoordT>
        constexpr bool contains(const Coord<CoordT> &coord) noexcept {
            return coord.x() >= origin().x() && coord.y() >= origin().y() && coord.x() < opposite_origin().x() && coord.y() < opposite_origin().y();
        }
        
        constexpr auto range() noexcept {
            return ufo::range(origin(), origin() + size());
        }
        
        constexpr auto x_range() noexcept {
            return ufo::range(origin().x(), origin().x() + size().x());
        }
        
        constexpr auto y_range() noexcept {
            return ufo::range(origin().y(), origin().y() + size().y());
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
    
    template <typename T>
    auto &operator<<(std::ostream &o, const Rect<T> &rect) {
        return o << "(" << rect.origin() << ", " << rect.size() << ")";
    }
}

#endif
