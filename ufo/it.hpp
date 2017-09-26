#ifndef ufo_it
#define ufo_it

#include <utility>

namespace ufo {
    class It {};
    
    template <typename T>
    constexpr auto operator==(const It &it, T &&rhs) {
        return [rhs = std::forward<T>(rhs)](auto &&lhs) constexpr -> decltype(auto) {
            return std::forward<decltype(lhs)>(lhs) == rhs;
        };
    }
    
    template <typename T>
    constexpr auto operator==(T &&lhs, const It &it) {
        return[lhs = std::forward<T>(lhs)](auto &&rhs) constexpr -> decltype(auto) {
            return lhs == std::forward<decltype(rhs)>(rhs);
        };
    }
    
    constexpr const It it {};
}

#endif
