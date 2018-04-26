#ifndef ufo_option
#define ufo_option

#include <functional>
#include <experimental/optional>
#include "type_traits.hpp"

namespace ufo {
    using std::experimental::nullopt_t;
    using std::experimental::nullopt;
    
    template <typename T>
    class option;
    
    template <typename T>
    option(T &&) -> option<T>;
    
    template <typename Derived>
    class option_trait {
    protected:
        constexpr option_trait() noexcept = default;
        
    private:
        constexpr Derived *derived() {
            return static_cast<Derived *>(this);
        }
        
        constexpr const Derived *derived() const {
            return static_cast<const Derived *>(this);
        }
        
        template <typename Self, typename F>
        static constexpr auto map_(Self &&self, F &&f) {
            using result = decltype(option(std::invoke(std::forward<F>(f), *std::forward<Self>(self))));
            if (!self) return result {};
            return result(std::invoke(std::forward<F>(f), *std::forward<Self>(self)));
        }
        
        template <typename Self, typename F>
        static constexpr auto and_then_(Self &&self, F &&f) {
            using result = decltype(std::invoke(std::forward<F>(f), *std::forward<Self>(self)));
            if (!self) return result {};
            return std::invoke(std::forward<F>(f), *std::forward<Self>(self));
        }
        
        template <typename Self, typename U>
        static constexpr auto unwrap_or_(Self &&self, U &&value) {
            using result = std::decay_t<decltype(*std::forward<Self>(self))>;
            if (!self) return static_cast<result>(std::forward<U>(value));
            return *std::forward<Self>(self);
        }
        
    public:
        template <typename F>
        constexpr auto map(F &&f) const & {
            return map_(*derived(), std::forward<F>(f));
        }
        
        template <typename F>
        constexpr auto map(F &&f) & {
            return map_(*derived(), std::forward<F>(f));
        }
        
        template <typename F>
        constexpr auto map(F &&f) && {
            return map_(std::move(*derived()), std::forward<F>(f));
        }
        
        template <typename F>
        constexpr auto and_then(F &&f) const & {
            return and_then_(*derived(), std::forward<F>(f));
        }
        
        template <typename F>
        constexpr auto and_then(F &&f) & {
            return and_then_(*derived(), std::forward<F>(f));
        }
        
        template <typename F>
        constexpr auto and_then(F &&f) && {
            return and_then_(std::move(*derived()), std::forward<F>(f));
        }
        
        template <typename U>
        constexpr auto unwrap_or(U &&value) const & {
            return unwrap_or_(*derived(), std::forward<U>(value));
        }
        
        template <typename U>
        constexpr auto unwrap_or(U &&value) & {
            return unwrap_or_(*derived(), std::forward<U>(value));
        }
        
        template <typename U>
        constexpr auto unwrap_or(U &&value) && {
            return unwrap_or_(std::move(*derived()), std::forward<U>(value));
        }
    };
    
    template <typename T>
    class option : public option_trait<option<T>> {
        static_assert(std::is_nothrow_move_constructible_v<T>);
        
    private:
        std::experimental::optional<T> optional_;
        
    public:
        using value_type = T;
        
        constexpr explicit option() noexcept : optional_ {} {
        }
        
        ~option() noexcept = default;
        
        constexpr option(nullopt_t) noexcept : optional_(nullopt) {
        }
        
        constexpr option(const option &) = default;
        
        constexpr option(option &&) noexcept = default;
        
        constexpr option(const T &value) : optional_(value) {
        }
        
        constexpr option(T &&value) : optional_(std::move(value)) {
        }
        
        constexpr option &operator=(nullopt_t) noexcept {
            optional_ = nullopt;
            return *this;
        }
        
        constexpr option &operator=(const option &other) = default;
        
        constexpr option &operator=(option &&other) noexcept(std::is_nothrow_move_assignable_v<std::experimental::optional<T>>) = default;
        
        constexpr option &operator=(const T &value) {
            optional_ = value;
            return *this;
        }
        
        constexpr option &operator=(T &&value) {
            optional_ = std::move(value);
            return *this;
        }
        
        template <typename ... Args>
        constexpr void emplace(Args && ... args) {
            optional_.emplace(std::forward<Args>(args) ...);
        }
        
        constexpr void reset() noexcept {
            optional_.reset();
        }
        
        constexpr operator bool() const noexcept {
            return static_cast<bool>(optional_);
        }
        
        constexpr const T *operator->() const noexcept {
            return &*optional_;
        }
        
        constexpr T *operator->() noexcept {
            return &*optional_;
        }
        
        constexpr const T &operator*() const & noexcept {
            return *optional_;
        }
        
        constexpr T &operator*() & noexcept {
            return *optional_;
        }
        
        constexpr T &&operator*() && noexcept {
            return std::move(*optional_);
        }
        
        template <typename LHS, typename RHS>
        friend constexpr bool operator==(const option<LHS> &, const option<RHS> &) noexcept;
        
        template <typename U>
        friend constexpr bool operator==(const option<U> &, nullopt_t) noexcept;
        
        template <typename U>
        friend constexpr bool operator==(nullopt_t, const option<U> &) noexcept;
    };
    
    template <typename LHS, typename RHS>
    constexpr bool operator==(const option<LHS> &lhs, const option<RHS> &rhs) noexcept {
        return lhs.optional_ == rhs.optional_;
    }
    
    template <typename T>
    constexpr bool operator==(const option<T> &option, nullopt_t) noexcept {
        return option.optional_ == nullopt;
    }
    
    template <typename T>
    constexpr bool operator==(nullopt_t, const option<T> &option) noexcept {
        return nullopt == option.optional_;
    }
    
    template <typename LHS, typename RHS>
    constexpr bool operator!=(const option<LHS> &lhs, const option<RHS> &rhs) noexcept {
        return !(lhs == rhs);
    }
    
    template <typename T>
    class option<T &> : public option_trait<option<T &>> {
    private:
        T *pointer_;
        
    public:
        using value_type = T &;
        
        constexpr explicit option() noexcept : pointer_(nullptr) {
        }
        
        ~option() noexcept = default;
        
        constexpr option(T &reference) noexcept : pointer_(&reference) {
        }
        
        constexpr option(nullopt_t) noexcept : pointer_(nullptr) {
        }
        
        constexpr option(const option &) noexcept = default;
        
        constexpr option(option &&) noexcept = default;
        
        template <typename U>
        constexpr option(const option<U> &other) noexcept : option(*other) {
        }
        
        constexpr option &operator=(T &reference) noexcept {
            pointer_ = &reference;
            return *this;
        }
        
        constexpr option &operator=(nullopt_t) noexcept {
            pointer_ = nullptr;
            return *this;
        }
        
        constexpr option &operator=(const option &) noexcept = default;
        
        constexpr option &operator=(option &&) noexcept = default;
        
        template <typename U>
        constexpr option &operator=(const option<U> &other) noexcept {
            *this = *other;
            return *this;
        }
        
        template <typename U>
        constexpr void emplace(U &reference) noexcept {
            pointer_ = &reference;
        }
        
        constexpr void reset() noexcept {
            pointer_ = nullptr;
        }
        
        constexpr operator bool() const noexcept {
            return pointer_ != nullptr;
        }
        
        constexpr T *operator->() const noexcept {
            return pointer_;
        }
        
        constexpr T &operator*() const noexcept {
            return *pointer_;
        }
        
        constexpr auto deref() const noexcept {
            return this->map([](auto &x) -> std::decay_t<T> {return x;});
        }
        
        template <typename LHS, typename RHS>
        friend constexpr bool operator==(const option<LHS &> &lhs, const option<RHS &> &rhs) noexcept;
        
        template <typename U>
        friend constexpr bool operator==(const option<U &> &, nullopt_t) noexcept;
        
        template <typename U>
        friend constexpr bool operator==(nullopt_t, const option<U &> &) noexcept;
    };
    
    template <typename LHS, typename RHS>
    constexpr bool operator==(const option<LHS &> &lhs, const option<RHS &> &rhs) noexcept {
        return lhs.pointer_ == rhs.pointer_;
    }
    
    template <typename T>
    constexpr bool operator==(const option<T &> &option, nullopt_t) noexcept {
        return option.pointer_ == nullptr;
    }
    
    template <typename T>
    constexpr bool operator==(nullopt_t, const option<T &> &option) noexcept {
        return option.pointer_ == nullptr;
    }
}

#endif
