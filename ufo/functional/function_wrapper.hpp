#ifndef ufo_function_wrapper
#define ufo_function_wrapper

#include <functional>
#include "../box.hpp"

namespace ufo {
    template <typename F>
    class function_wrapper {
    private:
        box<F> f_;
        
    public:
        constexpr function_wrapper(const F &f) : f_(f) {
        }
        
        constexpr function_wrapper(F &&f) noexcept : f_(std::move(f)) {
        }
        
        ~function_wrapper() = default;
        
        constexpr function_wrapper(const function_wrapper &) = default;
        
        constexpr function_wrapper(function_wrapper &&) noexcept(std::is_nothrow_move_constructible_v<F>) = default;
        
        function_wrapper &operator=(const function_wrapper &other) {
            *this = other.f_.get();
            return *this;
        }
        
        function_wrapper &operator=(function_wrapper &&other) noexcept {
            *this = std::move(other.f_).get();
            return *this;
        }
        
        function_wrapper &operator=(const F &f) {
            f_.emplace(f);
            return *this;
        }
        
        function_wrapper &operator=(F &&f) noexcept {
            f_.emplace(std::move(f));
            return *this;
        }
        
        template <typename ... Args>
        constexpr decltype(auto) operator()(Args && ... args) const & noexcept(noexcept(std::invoke(f_.get(), std::forward<Args>(args) ...))) {
            return std::invoke(f_.get(), std::forward<Args>(args) ...);
        }
        
        template <typename ... Args>
        constexpr decltype(auto) operator()(Args && ... args) & noexcept(noexcept(std::invoke(f_.get(), std::forward<Args>(args) ...))) {
            return std::invoke(f_.get(), std::forward<Args>(args) ...);
        }
        
        template <typename ... Args>
        constexpr decltype(auto) operator()(Args && ... args) && noexcept(noexcept(std::invoke(std::move(f_).get(), std::forward<Args>(args) ...))) {
            return std::invoke(std::move(f_).get(), std::forward<Args>(args) ...);
        }
    };
}

#endif
