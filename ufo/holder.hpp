#ifndef ufo_holder
#define ufo_holder

#include <functional>
#include <type_traits>

namespace ufo {
    template <typename T>
    class value_holder {
        static_assert(std::is_nothrow_move_constructible_v<T>);
        
    private:
        T value_;
        
    public:
        using value_type = T;
        
        constexpr value_holder(const T &value) : value_(value) {
        }
        
        constexpr value_holder(T &&value) noexcept : value_(std::move(value)) {
        }
        
        ~value_holder() noexcept = default;
        
        constexpr value_holder(const value_holder &) = default;
        
        constexpr value_holder(value_holder &&) noexcept = default;
        
        constexpr value_holder &operator=(const value_holder &) = default;
        
        constexpr value_holder &operator=(value_holder &&other) noexcept = default;
        
        constexpr value_holder &operator=(const T &value) {
            value_ = value;
            return *this;
        }
        
        constexpr value_holder &operator=(T &&value) {
            value_ = std::move(value);
            return *this;
        }
        
        constexpr const T &get() const & noexcept {
            return value_;
        }
        
        constexpr T &get() & noexcept {
            return value_;
        }
        
        constexpr T get() && noexcept {
            return std::move(value_);
        }
    };
    
    template <typename T>
    class reference_holder {
    private:
        std::reference_wrapper<T> ref_;
        
    public:
        using value_type = T &;
        
        constexpr reference_holder(T &value) : ref_(value) {
        }
        
        ~reference_holder() noexcept = default;
        
        constexpr reference_holder(const reference_holder &) = default;
        
        constexpr reference_holder(reference_holder &&) noexcept = default;
        
        constexpr reference_holder &operator=(const reference_holder &) = default;
        
        constexpr reference_holder &operator=(reference_holder &&other) noexcept = default;
        
        constexpr reference_holder &operator=(T &value) {
            ref_ = value;
            return *this;
        }
        
        constexpr T &get() const noexcept {
            return ref_;
        }
    };
    
    template <typename T>
    struct holder_ {
        using type = value_holder<T>;
    };
    
    template <typename T>
    struct holder_<T &> {
        using type = reference_holder<T>;
    };
    
    template <typename T>
    struct holder_<T &&> {
        using type = value_holder<T>;
    };
    
    template <typename T>
    using holder = typename holder_<T>::type;
}

#endif
