#ifndef ufo_OptionalReference
#define ufo_OptionalReference

#include <functional>
#include "optional.hpp"

namespace ufo {
    template <typename T>
    class OptionalReference final {
    public:
        constexpr OptionalReference() : optional_ {} {
        }
        
        constexpr OptionalReference(T &reference) : optional_(std::ref(reference)) {
        }
        
        constexpr OptionalReference(nullopt_t) : optional_(nullopt) {
        }
        
        ~OptionalReference() = default;
        
        constexpr OptionalReference(const OptionalReference &) = default;
        
        constexpr OptionalReference(OptionalReference &&) = default;
        
        template <typename U>
        constexpr OptionalReference(const OptionalReference<U> &other) : OptionalReference(*other) {
        }
        
        constexpr OptionalReference &operator=(const OptionalReference &) = default;
        
        constexpr OptionalReference &operator=(OptionalReference &&) = default;
        
        constexpr OptionalReference &operator=(T &reference) {
            optional_ = std::ref(reference);
            return *this;
        }
        
        constexpr OptionalReference &operator=(nullopt_t) {
            optional_ = nullopt;
        }
        
        template <typename U>
        constexpr OptionalReference &operator=(const OptionalReference<U> &other) {
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
        
        constexpr operator const T &() const {
            return optional_->get();
        }
        
        constexpr operator T &() {
            return optional_->get();
        }
        
    private:
        optional<std::reference_wrapper<T>> optional_;
        
        friend constexpr bool operator==(const OptionalReference &, const OptionalReference &);
    };
    
    template <typename LHS, typename RHS>
    constexpr bool operator==(const OptionalReference<LHS> &lhs, const OptionalReference<RHS> &rhs) {
        return lhs.optional_ == rhs.optional_;
    }
    
    template <typename LHS, typename RHS>
    constexpr bool operator!=(const OptionalReference<LHS> &lhs, const OptionalReference<RHS> &rhs) {
        return !(lhs == rhs);
    }
    
}

#endif
