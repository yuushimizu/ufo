#ifndef ufo_optref
#define ufo_optref

#include <functional>
#include <experimental/optional>

namespace ufo {
    template <typename T>
    class optref final {
    public:
        constexpr optref() noexcept : optional_ {} {
        }
        
        constexpr optref(T &reference) noexcept : optional_(std::ref(reference)) {
        }
        
        constexpr optref(std::experimental::nullopt_t) noexcept : optional_(std::experimental::nullopt) {
        }
        
        ~optref() noexcept = default;
        
        constexpr optref(const optref &) = default;
        
        constexpr optref(optref &&) noexcept = default;
        
        template <typename U>
        constexpr optref(const optref<U> &other) : optref(*other) {
        }
        
        constexpr optref &operator=(const optref &) noexcept = default;
        
        constexpr optref &operator=(optref &&) noexcept = default;
        
        constexpr optref &operator=(T &reference) noexcept {
            optional_ = std::ref(reference);
            return *this;
        }
        
        constexpr optref &operator=(std::experimental::nullopt_t) noexcept {
            optional_ = std::experimental::nullopt;
        }
        
        template <typename U>
        constexpr optref &operator=(const optref<U> &other) noexcept {
            *this = *other;
            return *this;
        }
        
        template <typename U>
        constexpr void emplace(U &reference) noexcept {
            optional_.emplace(reference);
        }
        
        constexpr void reset() noexcept {
            optional_ = std::experimental::nullopt;
        }
        
        constexpr operator bool() const noexcept {
            return static_cast<bool>(optional_);
        }
        
        constexpr T *operator->() const noexcept {
            return &optional_->get();
        }
        
        constexpr T &operator*() const noexcept {
            return optional_->get();
        }
        
    private:
        std::experimental::optional<std::reference_wrapper<T>> optional_;
        
        friend constexpr bool operator==(const optref &, const optref &);
    };
    
    template <typename LHS, typename RHS>
    constexpr bool operator==(const optref<LHS> &lhs, const optref<RHS> &rhs) noexcept {
        return lhs.optional_ == rhs.optional_;
    }
    
    template <typename LHS, typename RHS>
    constexpr bool operator!=(const optref<LHS> &lhs, const optref<RHS> &rhs) noexcept {
        return !(lhs == rhs);
    }
}

#endif
