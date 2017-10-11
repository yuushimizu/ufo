#ifndef ufo_option
#define ufo_option

#include <functional>
#include <experimental/optional>
#include <type_traits>

namespace ufo {
    using std::experimental::nullopt_t;
    using std::experimental::nullopt;
    
    template <typename T>
    class option;
    
    template <typename T>
    constexpr option<T> make_option(T &&value) noexcept;
    
    template <typename T>
    class option {
        static_assert(std::is_nothrow_move_constructible_v<T>);
        
    private:
        std::experimental::optional<T> optional_;
        
    public:
        constexpr option() noexcept : optional_ {} {
        }
        
        ~option() noexcept = default;
        
        constexpr option(nullopt_t) noexcept : optional_(nullopt) {
        }
        
        constexpr option(const option &) = default;
        
        constexpr option(option &&) noexcept = default;
        
        template <typename U>
        constexpr option(const option<U> &other) : option(*other) {
        }
        
        template <typename U>
        constexpr option(option<U> &&other) noexcept : option(std::move(*other)) {
        }
        
        template <typename U = T>
        constexpr option(U &&value) : optional_(std::forward<U>(value)) {
        }
        
        constexpr option &operator=(nullopt_t) noexcept {
            optional_ = nullopt;
            return *this;
        }
        
        constexpr option &operator=(const option &other) = default;
        
        constexpr option &operator=(option &&other) noexcept(std::is_nothrow_move_assignable_v<std::experimental::optional<T>>) = default;
        
        template <typename U>
        constexpr option &operator=(const option<U> &other) {
            *this = *other;
            return *this;
        }
        
        template <typename U>
        constexpr option &operator=(option<U> &&other) noexcept {
            *this = std::move(*other);
            return *this;
        }
        
        template <typename U = T>
        constexpr option &operator=(U &&value) {
            optional_ = std::forward<U>(value);
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
        
        template <typename F>
        constexpr auto map(F &&f) const & -> decltype(make_option(std::forward<F>(f)(*optional_))) {
            if (!*this) return nullopt;
            return make_option(std::forward<F>(f)(*optional_));
        }
        
        template <typename F>
        constexpr auto map(F &&f) & -> decltype(make_option(std::forward<F>(f)(*optional_))) {
            if (!*this) return nullopt;
            return make_option(std::forward<F>(f)(*optional_));
        }
        
        template <typename F>
        constexpr auto map(F &&f) && -> decltype(make_option(std::forward<F>(f)(std::move(*optional_)))) {
            if (!*this) return nullopt;
            return make_option(std::forward<F>(f)(std::move(*optional_)));
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
    class option<T &> {
    private:
        T *pointer_;
        
    public:
        constexpr option() noexcept : pointer_(nullptr) {
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
        
        template <typename F>
        constexpr auto map(F &&f) const & -> decltype(make_option(std::forward<F>(f)(*pointer_))) {
            if (!*this) return nullopt;
            return make_option(std::forward<F>(f)(*pointer_));
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
    
    template <typename T>
    constexpr option<T> make_option(T &&value) noexcept {
        return std::forward<T>(value);
    }
}

#endif