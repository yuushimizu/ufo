#ifndef ufo_lambda_wrapper
#define ufo_lambda_wrapper

#include <functional>
#include "option.hpp"

namespace ufo {
    template <typename F>
    class lambda_wrapper {
    private:
        option<F> f_;
        
    public:
        constexpr lambda_wrapper(const F &f) : f_(f) {
        }
        
        constexpr lambda_wrapper(F &&f) noexcept : f_(std::move(f)) {
        }
        
        ~lambda_wrapper() = default;
        
        constexpr lambda_wrapper(const lambda_wrapper &) = default;
        
        constexpr lambda_wrapper(lambda_wrapper &&) noexcept = default;
        
        lambda_wrapper &operator=(const lambda_wrapper &other) {
            *this = lambda_wrapper(other);
            return *this;
        }
        
        lambda_wrapper &operator=(lambda_wrapper &&other) noexcept {
            f_.emplace(std::move(*other.f_));
            return *this;
        }
        
        template <typename ... Args>
        constexpr decltype(auto) operator()(Args && ... args) const & noexcept(noexcept(std::invoke(*f_, std::forward<Args>(args) ...))) {
            return std::invoke(*f_, std::forward<Args>(args) ...);
        }
        
        template <typename ... Args>
        constexpr decltype(auto) operator()(Args && ... args) & noexcept(noexcept(std::invoke(*f_, std::forward<Args>(args) ...))) {
            return std::invoke(*f_, std::forward<Args>(args) ...);
        }
        
        template <typename ... Args>
        constexpr decltype(auto) operator()(Args && ... args) && noexcept(noexcept(std::invoke(std::move(*f_), std::forward<Args>(args) ...))) {
            return std::invoke(std::move(*f_), std::forward<Args>(args) ...);
        }
    };
}

#endif
