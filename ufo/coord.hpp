#ifndef ufo_coord
#define ufo_coord

#include <cmath>
#include <functional>
#include <iostream>
#include "sequence/range.hpp"
#include "sequence/mapcat.hpp"
#include "hash.hpp"
#include "placeholder.hpp"

namespace ufo {
    template<typename F, typename ... Ts>
    constexpr auto transform_coord(const F &f, const Ts &... args);
    
    template<typename T>
    class coord final {
    public:
        constexpr coord(T x, T y) noexcept : x_(std::move(x)), y_(std::move(y)) {
        }

        constexpr explicit coord() noexcept : coord(T {}, T {}) {
        }
        
        constexpr T x() const noexcept {
            return x_;
        }
        
        constexpr T y() const noexcept {
            return y_;
        }

        template<typename Other>
        constexpr bool contains(const coord<Other> &other) const noexcept {
            return other.x_ >= 0 && other.y_ >= 0 && other.x_ < x_ && other.y_ < y_;
        }

        constexpr auto area() const noexcept {
            return x_ * y_;
        }
        
        template <typename Other>
        constexpr auto add_x(const Other &amount) const noexcept {
            return coord(x_ + amount, y_);
        }
        
        template <typename Other>
        constexpr auto add_y(const Other &amount) const noexcept {
            return coord(x_, y_ + amount);
        }
        
        template <typename Other>
        constexpr auto multiply(const coord<Other> &other) const noexcept {
            return transform_coord(std::multiplies<> {}, *this, other);
        }
        
        template <typename Other>
        constexpr auto divide(const coord<Other> &other) const noexcept {
            return transform_coord(std::divides<> {}, *this, other);
        }
        
        constexpr static auto zero() {
            return coord {};
        }
        
        constexpr static auto one() {
            return coord(1, 1);
        }
        
    private:
        T x_;
        T y_;
    };

    template<typename F, typename ... Ts>
    constexpr auto transform_coord(const F &f, const Ts &... args) {
        return coord(f(args.x() ...), f(args.y() ...));
    }

    template<typename LHS, typename RHS>
    constexpr bool operator==(const coord<LHS> &lhs, const coord<RHS> &rhs) noexcept {
        return lhs.x() == rhs.x() && lhs.y() == rhs.y();
    }

    template<typename LHS, typename RHS>
    constexpr bool operator!=(const coord<LHS> &lhs, const coord<RHS> &rhs) noexcept {
        return !(lhs == rhs);
    }

    template<typename T>
    constexpr coord<T> operator+(coord<T> coord) noexcept {
        return std::move(coord);
    }

    template<typename T>
    constexpr coord<T> operator-(const coord<T> &coord) noexcept {
        return transform_coord(-_, coord);
    }

    template<typename LHS, typename RHS>
    constexpr auto operator+(const coord<LHS> &lhs, const coord<RHS> &rhs) noexcept {
        return transform_coord(std::plus<> {}, lhs, rhs);
    }

    template<typename LHS, typename RHS>
    constexpr auto operator-(const coord<LHS> &lhs, const coord<RHS> &rhs) noexcept {
        return lhs + -rhs;
    }

    template<typename LHS, typename RHS>
    constexpr auto operator*(const coord<LHS> &lhs, const RHS &rhs) noexcept {
        return transform_coord(_ * rhs, lhs);
    }

    template<typename LHS, typename RHS>
    constexpr auto operator*(const LHS &lhs, const coord<RHS> &rhs) noexcept {
        return rhs * lhs;
    }

    template<typename LHS, typename RHS>
    constexpr auto operator/(const coord<LHS> &lhs, const RHS &rhs) noexcept {
        return transform_coord(_ / rhs, lhs);
    }
    
    template<typename T>
    constexpr auto range(const coord<T> &first, const coord<T> &last) noexcept {
        return range(first.y(), last.y()) | mapcat([x_range = range(first.x(), last.x())](auto y) {
            return x_range | map([y = std::move(y)](auto x) {return coord(std::move(x), y);});
        });
    }
    
    template <typename T>
    constexpr auto range(const coord<T> &last) noexcept {
        return range(coord(T {},  {}), last);
    }

    template<typename T>
    constexpr auto &operator<<(std::ostream &o, const coord<T> &coord) {
        return o << "(" << coord.x() << ", " << coord.y() << ")";
    }
}

template <typename T>
class std::hash<ufo::coord<T>> {
public:
    constexpr std::size_t operator()(const ufo::coord<T> &coord) const {
        return ufo::combined_hash(coord.x(), coord.y());
    }
};

#endif
