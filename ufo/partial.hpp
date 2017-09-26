#ifndef ufo_it
#define ufo_it

#include <utility>
#include <type_traits>
#include "type_traits.hpp"

namespace ufo {
    class Placeholder {};
    
    template <typename F>
    class Partialed {
    public:
        Partialed(F f) : f_(std::move(f)) {
        }
        
        template <typename T>
        decltype(auto) operator()(T &&x) const& {
            return f_(std::forward<T>(x));
        }
        
        template <typename T>
        decltype(auto) operator()(T &&x) & {
            return f_(std::forward<T>(x));
        }
        
        template <typename T>
        decltype(auto) operator()(T &&x) && {
            return f_(std::forward<T>(x));
        }

    private:
        F f_;
    };
    
    template <typename F>
    auto partialed(F &&f) {
        return Partialed<F>(std::forward<F>(f));
    }
    
    template <typename T>
    constexpr auto operator==(const Placeholder &, T &&rhs) {
        return partialed([rhs = std::forward<T>(rhs)](auto &&lhs) constexpr -> decltype(auto) {
            return std::forward<decltype(lhs)>(lhs) == rhs;
        });
    }
    
    template <typename T>
    constexpr auto operator==(T &&lhs, const Placeholder &) {
        return partialed([lhs = std::forward<T>(lhs)](auto &&rhs) constexpr -> decltype(auto) {
            return lhs == std::forward<decltype(rhs)>(rhs);
        });
    }
    
    template <typename T, typename RHS, std::enable_if_t<is_instantiation_of_v<Partialed, T>, std::nullptr_t> = nullptr>
    constexpr auto operator==(T &&lhs, RHS &&rhs) {
        return partialed([lhs = std::forward<T>(lhs), rhs = std::forward<RHS>(rhs)](auto &&x) constexpr -> decltype(auto) {
            return lhs(x) == rhs;
        });
    }
    
    template <typename LHS, typename T, std::enable_if_t<is_instantiation_of_v<Partialed, T>, std::nullptr_t> = nullptr>
    constexpr auto operator==(LHS &&lhs, T &&rhs) {
        return partialed([lhs = std::forward<LHS>(lhs), rhs = std::forward<T>(rhs)](auto &&x) constexpr -> decltype(auto) {
            return lhs == rhs(x);
        });
    }
    
    template <typename T>
    constexpr auto operator%(const Placeholder &, T &&rhs) {
        return partialed([rhs = std::forward<T>(rhs)](auto &&lhs) constexpr -> decltype(auto) {
            return std::forward<decltype(lhs)>(lhs) % rhs;
        });
    }
    
    template <typename T>
    constexpr auto operator%(T &&lhs, const Placeholder &) {
        return partialed([lhs = std::forward<T>(lhs)](auto &&rhs) constexpr -> decltype(auto) {
            return lhs % std::forward<decltype(rhs)>(rhs);
        });
    }
    
    constexpr const Placeholder _ {};
}

#endif
