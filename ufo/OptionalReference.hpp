#ifndef ufo_OptionalReference
#define ufo_OptionalReference

#include <functional>
#include <experimental/optional>

namespace ufo {
    template <typename T>
    class OptionalReference final {
    public:
        OptionalReference() : optional_ {} {
        }
        
        OptionalReference(T &reference) : optional_(std::ref(reference)) {
        }
        
        OptionalReference(std::experimental::nullopt_t) : optional_(std::experimental::nullopt) {
        }
        
        ~OptionalReference() = default;
        
        OptionalReference(const OptionalReference &) = default;
        
        OptionalReference(OptionalReference &&) = default;
        
        template <typename U>
        OptionalReference(const OptionalReference<U> &other) : OptionalReference(*other) {
        }
        
        OptionalReference &operator=(const OptionalReference &) = default;
        
        OptionalReference &operator=(OptionalReference &&) = default;
        
        OptionalReference &operator=(T &reference) {
            optional_ = std::ref(reference);
            return *this;
        }
        
        OptionalReference &operator=(std::experimental::nullopt_t) {
            optional_ = std::experimental::nullopt;
        }
        
        template <typename U>
        OptionalReference &operator=(const OptionalReference<U> &other) {
            *this = *other;
            return *this;
        }
        
        template <typename U>
        void emplace(U &reference) {
            optional_.emplace(reference);
        }
        
        void reset() {
            optional_ = std::experimental::nullopt;
        }
        
        operator bool() const {
            return static_cast<bool>(optional_);
        }
        
        const T *operator->() const {
            return &optional_->get();
        }
        
        T *operator->() {
            return &optional_->get();
        }
        
        const T &operator*() const {
            return optional_->get();
        }
        
        T &operator*() {
            return optional_->get();
        }
        
        operator const T &() const {
            return optional_->get();
        }
        
        operator T &() {
            return optional_->get();
        }
        
    private:
        std::experimental::optional<std::reference_wrapper<T>> optional_;
        
        friend bool operator==(const OptionalReference &, const OptionalReference &);
    };
    
    template <typename LHS, typename RHS>
    bool operator==(const OptionalReference<LHS> &lhs, const OptionalReference<RHS> &rhs) {
        return lhs.optional_ == rhs.optional_;
    }
    
    template <typename LHS, typename RHS>
    bool operator!=(const OptionalReference<LHS> &lhs, const OptionalReference<RHS> &rhs) {
        return !(lhs == rhs);
    }
    
}

#endif
