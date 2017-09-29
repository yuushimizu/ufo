#ifndef ufo_optref
#define ufo_optref

#include <functional>
#include "optional.hpp"

namespace ufo {
    template <typename T>
    class optref final {
    public:
        constexpr optref() : optional_ {} {
        }
        
        constexpr optref(T &reference) : optional_(std::ref(reference)) {
        }
        
        constexpr optref(nullopt_t) : optional_(nullopt) {
        }
        
        ~optref() = default;
        
        constexpr optref(const optref &) = default;
        
        constexpr optref(optref &&) = default;
        
        template <typename U>
        constexpr optref(const optref<U> &other) : optref(*other) {
        }
        
        constexpr optref &operator=(const optref &) = default;
        
        constexpr optref &operator=(optref &&) = default;
        
        constexpr optref &operator=(T &reference) {
            optional_ = std::ref(reference);
            return *this;
        }
        
        constexpr optref &operator=(nullopt_t) {
            optional_ = nullopt;
        }
        
        template <typename U>
        constexpr optref &operator=(const optref<U> &other) {
            *this = *other;
            return *this;
        }
        
        template <typename U>
        constexpr void emplace(U &reference) {
            optional_.emplace(reference);
        }
        
        constexpr void reset() {
            optional_ = nullopt;
        }
        
        constexpr operator bool() const {
            return static_cast<bool>(optional_);
        }
        
        constexpr const T *operator->() const {
            return &optional_->get();
        }
        
        constexpr T *operator->() {
            return &optional_->get();
        }
        
        constexpr const T &operator*() const {
            return optional_->get();
        }
        
        constexpr T &operator*() {
            return optional_->get();
        }
        
    private:
        optional<std::reference_wrapper<T>> optional_;
        
        friend constexpr bool operator==(const optref &, const optref &);
    };
    
    template <typename LHS, typename RHS>
    constexpr bool operator==(const optref<LHS> &lhs, const optref<RHS> &rhs) {
        return lhs.optional_ == rhs.optional_;
    }
    
    template <typename LHS, typename RHS>
    constexpr bool operator!=(const optref<LHS> &lhs, const optref<RHS> &rhs) {
        return !(lhs == rhs);
    }
}

#endif
