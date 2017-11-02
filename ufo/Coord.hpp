#ifndef ufo_Coord
#define ufo_Coord

#include <cmath>
#include <functional>
#include <iostream>
#include "sequence/range.hpp"
#include "sequence/mapcat.hpp"
#include "hash.hpp"
#include "placeholder.hpp"

namespace ufo {
    template <typename T>
    class Coord;
    
    template<typename T>
    constexpr Coord<T> coord(T x, T y) noexcept;
    
    template<typename F, typename ... Ts>
    constexpr auto transform_coord(const F &f, const Ts &... args);
    
    template<typename T>
    class Coord final {
    public:
        constexpr Coord(T x, T y) noexcept : x_(std::move(x)), y_(std::move(y)) {
        }

        constexpr Coord() noexcept : Coord(T {}, T {}) {
        }
        
        constexpr T x() const noexcept {
            return x_;
        }
        
        constexpr T y() const noexcept {
            return y_;
        }

        template<typename Other>
        constexpr bool contains(const Coord<Other> &other) const noexcept {
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
        constexpr auto multiply(const Coord<Other> &other) const noexcept {
            return transform_coord(std::multiplies<> {}, *this, other);
        }
        
        template <typename Other>
        constexpr auto divide(const Coord<Other> &other) const noexcept {
            return transform_coord(std::divides<> {}, *this, other);
        }
        
    private:
        T x_;
        T y_;
    };

    template<typename T>
    constexpr Coord<T> coord(T x, T y) noexcept {
        return {std::move(x), std::move(y)};
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
        return transform_coord(-_, coord);
    }

    template<typename LHS, typename RHS>
    constexpr auto operator+(const Coord<LHS> &lhs, const Coord<RHS> &rhs) noexcept {
        return transform_coord(std::plus<> {}, lhs, rhs);
    }

    template<typename LHS, typename RHS>
    constexpr auto operator-(const Coord<LHS> &lhs, const Coord<RHS> &rhs) noexcept {
        return lhs + -rhs;
    }

    template<typename LHS, typename RHS>
    constexpr auto operator*(const Coord<LHS> &lhs, const RHS &rhs) noexcept {
        return transform_coord(_ * rhs, lhs);
    }

    template<typename LHS, typename RHS>
    constexpr auto operator*(const LHS &lhs, const Coord<RHS> &rhs) noexcept {
        return rhs * lhs;
    }

    template<typename LHS, typename RHS>
    constexpr auto operator/(const Coord<LHS> &lhs, const RHS &rhs) noexcept {
        return transform_coord(_ / rhs, lhs);
    }
    
    template<typename T>
    constexpr auto range(const Coord<T> &first, const Coord<T> &last) noexcept {
        return range(first.y(), last.y()) | mapcat([first_x = first.x(), last_x = last.x()](auto &&y) {
            return range(first_x, last_x) | map([y = std::forward<decltype(y)>(y)](auto &&x) {return coord(std::forward<decltype(x)>(x), y);});
        });
    }
    
    template <typename T>
    constexpr auto range(const Coord<T> &last) noexcept {
        return range(coord(T {},  {}), last);
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
