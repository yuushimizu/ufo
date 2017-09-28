#ifndef ufo_Coord
#define ufo_Coord

#include <cmath>
#include <iostream>
#include "range/irange.hpp"
#include "range/transformed.hpp"
#include "hash.hpp"

namespace ufo {
    template<typename T>
    class Coord final {
    public:
        constexpr Coord(T x, T y) noexcept : x_(std::move(x)), y_(std::move(y)) {
        }

        constexpr Coord() noexcept : Coord(T {}, T {}) {
        }
        
        constexpr T x() const {
            return x_;
        }
        
        constexpr T y() const {
            return y_;
        }

        template<typename Other>
        constexpr bool contains(const Coord<Other> &other) const noexcept {
            return other.x_ >= 0 && other.y_ >= 0 && other.x_ < x_ && other.y_ < y_;
        }

        constexpr auto area() const noexcept {
            return x_ * y_;
        }
        
    private:
        T x_;
        T y_;
    };

    template<typename T>
    constexpr Coord<T> coord(T x, T y) noexcept {
        return Coord<T>(std::move(x), std::move(y));
    }

    template<typename F, typename ... Ts>
    constexpr auto transform_coord(const F &f, const Ts &... args) {
        return coord(f(args.x() ...), f(args.y() ...));
    }

    template<typename LHS, typename RHS>
    constexpr bool operator==(const Coord<LHS> &lhs, const Coord<RHS> &rhs) noexcept {
        return lhs.x() == rhs.x() && lhs.y() == rhs.y();
    }

    template<typename LHS, typename RHS>
    constexpr bool operator!=(const Coord<LHS> &lhs, const Coord<RHS> &rhs) noexcept {
        return !(lhs == rhs);
    }

    template<typename T>
    constexpr Coord<T> operator+(Coord<T> coord) noexcept {
        return std::move(coord);
    }

    template<typename T>
    constexpr Coord<T> operator-(const Coord<T> &coord) noexcept {
        return transform_coord([](auto &&n) {
            return -n;
        }, coord);
    }

    template<typename LHS, typename RHS>
    constexpr auto operator+(const Coord<LHS> &lhs, const Coord<RHS> &rhs) noexcept {
        return transform_coord([](auto &&lhs, auto &&rhs) {
            return lhs + rhs;
        }, lhs, rhs);
    }

    template<typename LHS, typename RHS>
    constexpr auto operator-(const Coord<LHS> &lhs, const Coord<RHS> &rhs) noexcept {
        return lhs + -rhs;
    }

    template<typename LHS, typename RHS>
    constexpr auto operator*(const Coord<LHS> &lhs, const RHS &rhs) noexcept {
        return coord(lhs.x() * rhs, lhs.y() * rhs);
    }

    template<typename LHS, typename RHS>
    constexpr auto operator*(const LHS &lhs, const Coord<RHS> &rhs) noexcept {
        return rhs * lhs;
    }

    template<typename LHS, typename RHS>
    constexpr auto operator/(const Coord<LHS> &lhs, const RHS &rhs) noexcept {
        return transform_coord([&rhs](auto &&lhs) {
            return lhs / rhs;
        }, lhs);
    }

    template<typename T, typename X>
    constexpr auto add_x(const Coord<T> &lhs, const X &rhs) noexcept {
        return coord(lhs.x() + rhs, lhs.y());
    }

    template<typename T, typename Y>
    constexpr auto add_y(const Coord<T> &lhs, const Y &rhs) noexcept {
        return coord(lhs.x(), lhs.y() + rhs);
    }

    template<typename LHS, typename RHS>
    constexpr auto multiply_each(const Coord<LHS> &lhs, const Coord<RHS> &rhs) noexcept {
        return transform_coord([](auto &&lhs, auto &&rhs) {
            return lhs * rhs;
        }, lhs, rhs);
    }

    template<typename LHS, typename RHS>
    constexpr auto divide_each(const Coord<LHS> &lhs, const Coord<RHS> &rhs) noexcept {
        return transform_coord([](auto &&lhs, auto &&rhs) {
            return lhs / rhs;
        }, lhs, rhs);
    }
    
    template <typename T>
    constexpr auto abs(const Coord<T> coord) {
        return transform_coord([](auto &&n) {return n >= 0 ? n : -n;}, coord);
    }

    template<typename T>
    constexpr auto range(const Coord<T> &last) noexcept {
        return irange(last.area()) | transformed(last](auto &&n) {
            return coord(T(n % last.x()), T(std::floor(n / last.x())));
        });
    }

    template<typename T>
    constexpr auto range(const Coord<T> &first, const Coord<T> &last) noexcept {
        return range(last - first) | transformed([first](auto &&coord) {
            return coord + first;
        });
    }

    template<typename T>
    constexpr auto COORD_ZERO = Coord<T>(T {}, T{});

    template<typename T>
    constexpr auto COORD_ONE = Coord<T>(1, 1);

    template<typename T>
    constexpr auto &operator<<(std::ostream &o, const Coord<T> &coord) {
        return o << "(" << coord.x() << ", " << coord.y() << ")";
    }
}

template <typename T>
class std::hash<ufo::Coord<T>> {
public:
    constexpr std::size_t operator()(const ufo::Coord<T> &coord) const {
        return ufo::combined_hash(coord.x(), coord.y());
    }
};

#endif
