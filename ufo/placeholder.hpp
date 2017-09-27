#ifndef ufo_it
#define ufo_it

#include <utility>
#include <type_traits>
#include "type_traits.hpp"

namespace ufo {
    namespace placeholder {
        template <typename F>
        class Placeable {
        public:
            constexpr Placeable(F f) noexcept : f_(std::move(f)) {
            }
            
            template <typename ... Args>
            constexpr decltype(auto) operator()(Args && ... args) const& {
                return f_(std::forward<Args>(args) ...);
            }
            
            template <typename ... Args>
            constexpr decltype(auto) operator()(Args && ... args) & {
                return f_(std::forward<Args>(args) ...);
            }
            
            template <typename ... Args>
            constexpr decltype(auto) operator()(Args && ... args) && {
                return f_(std::forward<Args>(args) ...);
            }
            
        private:
            F f_;
        };
    
        template <typename F>
        constexpr auto placeable(F &&f) {
            return Placeable<F>(std::forward<F>(f));
        }
        
        template <typename T>
        constexpr const bool is_placeable_v = is_instantiation_of_v<Placeable, std::decay_t<T>>;
        
        template <typename T>
        using unary_operator_enable_if_t = enable_if_t<is_placeable_v<T>>;
        
#define UFO_PLACEHOLDER_DEFINE_UNARY_OPERATOR(op) \
template <typename F, unary_operator_enable_if_t<F> = nullptr> \
constexpr auto operator op(F &&f) noexcept { \
    return placeable([f = std::forward<F>(f)](auto &&arg) constexpr -> decltype(auto) { \
        return op f(std::forward<decltype(arg)>(arg)); \
    }); \
}
        
        UFO_PLACEHOLDER_DEFINE_UNARY_OPERATOR(!)
        UFO_PLACEHOLDER_DEFINE_UNARY_OPERATOR(&)
        UFO_PLACEHOLDER_DEFINE_UNARY_OPERATOR(*)
        UFO_PLACEHOLDER_DEFINE_UNARY_OPERATOR(+)
        UFO_PLACEHOLDER_DEFINE_UNARY_OPERATOR(++)
        UFO_PLACEHOLDER_DEFINE_UNARY_OPERATOR(-)
        UFO_PLACEHOLDER_DEFINE_UNARY_OPERATOR(--)
        UFO_PLACEHOLDER_DEFINE_UNARY_OPERATOR(~)
        
#undef UFO_PLACEHOLDER_DEFINE_UNARY_OPERATOR
        
        template <typename F, unary_operator_enable_if_t<F> = nullptr>
        constexpr auto operator ++(F &&f, int) noexcept {
            return placeable([f = std::forward<F>(f)](auto &&arg) constexpr -> decltype(auto) {
                return f(std::forward<decltype(arg)>(arg))++;
            });
        }
        
        template <typename F, unary_operator_enable_if_t<F> = nullptr>
        constexpr auto operator --(F &&f, int) noexcept {
            return placeable([f = std::forward<F>(f)](auto &&arg) constexpr -> decltype(auto) {
                return f(std::forward<decltype(arg)>(arg))--;
            });
        }
        
        template <typename F>
        class BinaryOperator {
        public:
            constexpr BinaryOperator(F f) noexcept : f_(std::move(f)) {
            }
            
            template <typename LHS, typename RHS, enable_if_t<is_placeable_v<LHS>> = nullptr>
            constexpr auto operator()(LHS &&lhs, RHS &&rhs) noexcept {
                return placeable([&f = f_, lhs = std::forward<LHS>(lhs), rhs = std::forward<RHS>(rhs)](auto &&arg) constexpr -> decltype(auto) {
                    return f(lhs(std::forward<decltype(arg)>(arg)), rhs);
                });
            }
            
            template <typename LHS, typename RHS, enable_if_t<is_placeable_v<RHS>> = nullptr>
            constexpr auto operator()(LHS &&lhs, RHS &&rhs) noexcept {
                return placeable([&f = f_, lhs = std::forward<LHS>(lhs), rhs = std::forward<RHS>(rhs)](auto &&arg) constexpr -> decltype(auto) {
                    return f(lhs, rhs(std::forward<decltype(arg)>(arg)));
                });
            }
            
        private:
            const F f_;
        };
        
        template <typename F>
        constexpr auto binary_operator(F &&f) noexcept {
            return BinaryOperator<F>(std::forward<F>(f));
        }
        
        template <typename LHS, typename RHS>
        constexpr const bool are_binary_operands_v = (is_placeable_v<LHS> && !is_placeable_v<RHS>) || (!is_placeable_v<LHS> && is_placeable_v<RHS>);
        
        template <typename LHS, typename RHS>
        using binary_operator_enable_if_t = enable_if_t<are_binary_operands_v<LHS, RHS>>;
        
        template <typename LHS, typename RHS, binary_operator_enable_if_t<LHS, RHS> = nullptr>
        constexpr auto operator,(LHS &&lhs, RHS &&rhs) noexcept {
            return binary_operator([](auto &&lhs, auto &&rhs) constexpr -> decltype(auto) {
                return std::forward<decltype(lhs)>(lhs), std::forward<decltype(rhs)>(rhs);
            })(std::forward<LHS>(lhs), std::forward<RHS>(rhs));
        }
        
#define UFO_PLACEHOLDER_DEFINE_BINARY_OPERATOR(op) \
template <typename LHS, typename RHS, binary_operator_enable_if_t<LHS, RHS> = nullptr> \
constexpr auto operator op(LHS &&lhs, RHS &&rhs) noexcept { \
    return binary_operator([](auto &&lhs, auto &&rhs) constexpr -> decltype(auto) { \
        return std::forward<decltype(lhs)>(lhs) op std::forward<decltype(rhs)>(rhs); \
    })(std::forward<LHS>(lhs), std::forward<RHS>(rhs)); \
}
        
        UFO_PLACEHOLDER_DEFINE_BINARY_OPERATOR(!=)
        UFO_PLACEHOLDER_DEFINE_BINARY_OPERATOR(%)
        UFO_PLACEHOLDER_DEFINE_BINARY_OPERATOR(&)
        UFO_PLACEHOLDER_DEFINE_BINARY_OPERATOR(&&)
        UFO_PLACEHOLDER_DEFINE_BINARY_OPERATOR(*)
        UFO_PLACEHOLDER_DEFINE_BINARY_OPERATOR(+)
        UFO_PLACEHOLDER_DEFINE_BINARY_OPERATOR(-)
        UFO_PLACEHOLDER_DEFINE_BINARY_OPERATOR(->*)
        UFO_PLACEHOLDER_DEFINE_BINARY_OPERATOR(/)
        UFO_PLACEHOLDER_DEFINE_BINARY_OPERATOR(<)
        UFO_PLACEHOLDER_DEFINE_BINARY_OPERATOR(<<)
        UFO_PLACEHOLDER_DEFINE_BINARY_OPERATOR(<=)
        UFO_PLACEHOLDER_DEFINE_BINARY_OPERATOR(==)
        UFO_PLACEHOLDER_DEFINE_BINARY_OPERATOR(>)
        UFO_PLACEHOLDER_DEFINE_BINARY_OPERATOR(>=)
        UFO_PLACEHOLDER_DEFINE_BINARY_OPERATOR(>>)
        UFO_PLACEHOLDER_DEFINE_BINARY_OPERATOR(^)
        UFO_PLACEHOLDER_DEFINE_BINARY_OPERATOR(|)
        UFO_PLACEHOLDER_DEFINE_BINARY_OPERATOR(||)
        
#undef UFO_PLACEHOLDER_DEFINE_BINARY_OPERATOR
        
    }
    
    constexpr const auto _ = placeholder::placeable([](auto &&x) -> decltype(auto) {return std::forward<decltype(x)>(x);});
}

#endif
