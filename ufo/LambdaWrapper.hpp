#ifndef ufo_LambdaWrapper
#define ufo_LambdaWrapper

#include <functional>
#include "option.hpp"

namespace ufo {
    template <typename F>
    class LambdaWrapper {
    private:
        option<F> f_;
        
    public:
        constexpr LambdaWrapper(const F &f) : f_(f) {
        }
        
        constexpr LambdaWrapper(F &&f) noexcept : f_(std::move(f)) {
        }
        
        ~LambdaWrapper() = default;
        
        constexpr LambdaWrapper(const LambdaWrapper &) = default;
        
        constexpr LambdaWrapper(LambdaWrapper &&) noexcept = default;
        
        LambdaWrapper &operator=(const LambdaWrapper &other) {
            *this = LambdaWrapper(other);
            return *this;
        }
        
        LambdaWrapper &operator=(LambdaWrapper &&other) noexcept {
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
    
    template <typename F>
    class LambdaWrapper<F &>;
    
    template <typename F>
    class LambdaWrapper<F &&>;
    
    template <typename F>
    auto lambda_wrapper(F &&f) noexcept {
        return LambdaWrapper<std::decay_t<F>>(std::forward<F>(f));
    }
}

#endif
