#ifndef ufo_it
#define ufo_it

#include <utility>
#include <type_traits>
#include "type_traits.hpp"

namespace ufo {
    namespace partial {
        class Placeholder {};
    
        template <typename F>
        class Partialed {
        public:
            constexpr Partialed(F f) : f_(std::move(f)) {
            }
            
            template <typename T>
            constexpr decltype(auto) operator()(T &&x) const& {
                return f_(std::forward<T>(x));
            }
            
            template <typename T>
            constexpr decltype(auto) operator()(T &&x) & {
                return f_(std::forward<T>(x));
            }
            
            template <typename T>
            constexpr decltype(auto) operator()(T &&x) && {
                return f_(std::forward<T>(x));
            }
            
        private:
            F f_;
        };
    
        template <typename F>
        constexpr auto partialed(F &&f) {
            return Partialed<F>(std::forward<F>(f));
        }
    
        template <typename F>
        class BinaryOperator {
        public:
            constexpr BinaryOperator(F f) : f_(std::move(f)) {
            }
            
            template <typename RHS>
            constexpr auto operator()(const Placeholder &, RHS &&rhs) const {
                return partialed([&f = f_, rhs = std::forward<RHS>(rhs)](auto &&lhs) constexpr -> decltype(auto) {
                    return f(std::forward<decltype(lhs)>(lhs), rhs);
                });
            }
                                 
            template <typename LHS>
            constexpr auto operator()(LHS &&lhs, const Placeholder &) const {
                return partialed([&f = f_, lhs = std::forward<LHS>(lhs)](auto &&rhs) constexpr -> decltype(auto) {
                    return f(lhs, std::forward<decltype(rhs)>(rhs));
                });
            }
            
            template <typename LHS, typename RHS, nullptr_t_if_t<is_instantiation_of_v<Partialed, LHS>> = nullptr>
            constexpr auto operator()(LHS &&lhs, RHS &&rhs) {
                return partialed([&f = f_, lhs = std::forward<LHS>(lhs), rhs = std::forward<RHS>(rhs)](auto &&arg) constexpr -> decltype(auto) {
                    return f(lhs(std::forward<decltype(arg)>(arg)), rhs);
                });
            }
            
            template <typename LHS, typename RHS, nullptr_t_if_t<is_instantiation_of_v<Partialed, RHS>> = nullptr>
            constexpr auto operator()(LHS &&lhs, RHS &&rhs) {
                return partialed([&f = f_, lhs = std::forward<LHS>(lhs), rhs = std::forward<RHS>(rhs)](auto &&arg) constexpr -> decltype(auto) {
                    return f(lhs, rhs(std::forward<decltype(arg)>(arg)));
                });
            }
            
        private:
            const F f_;
        };
        
        template <typename F>
        constexpr auto binary_operator(F &&f) {
            return BinaryOperator<F>(std::forward<F>(f));
        }
        
        template <typename LHS, typename RHS>
        constexpr const bool are_binary_operands_v = std::is_same_v<LHS, const Placeholder &> || std::is_same_v<RHS, const Placeholder &> || is_instantiation_of_v<Partialed, LHS> || is_instantiation_of_v<Partialed, RHS>;
        
        template <typename LHS, typename RHS, nullptr_t_if_t<are_binary_operands_v<LHS, RHS>> = nullptr>
        constexpr auto operator==(LHS &&lhs, RHS &&rhs) {
            return binary_operator([](auto &&lhs, auto &&rhs) constexpr -> decltype(auto) {
                return std::forward<decltype(lhs)>(lhs) == std::forward<decltype(rhs)>(rhs);
            })(std::forward<LHS>(lhs), std::forward<RHS>(rhs));
        }
        
        template <typename LHS, typename RHS, nullptr_t_if_t<are_binary_operands_v<LHS, RHS>> = nullptr>
        constexpr auto operator+(LHS &&lhs, RHS &&rhs) {
            return binary_operator([](auto &&lhs, auto &&rhs) constexpr -> decltype(auto) {
                return std::forward<decltype(lhs)>(lhs) + std::forward<decltype(rhs)>(rhs);
            })(std::forward<LHS>(lhs), std::forward<RHS>(rhs));
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
    }
    
    constexpr const partial::Placeholder _ {};
}

#endif
