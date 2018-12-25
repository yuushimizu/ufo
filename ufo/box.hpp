#ifndef ufo_box
#define ufo_box

#include <functional>
#include <variant>
#include "type_traits.hpp"

namespace ufo {
    namespace detail {
        template <typename T>
        class box {
        public:
            using type = T;
            
            constexpr box(T value) noexcept : value_(std::move(value)) {
            }
            
            constexpr const T &get() const & {
                return std::get<T>(value_);
            }
            
            constexpr T &get() & {
                return std::get<T>(value_);
            }
            
            constexpr T &&get() && {
                return std::get<T>(std::move(value_));
            }
            
            constexpr const T *operator->() const {
                return &std::get<T>(value_);
            }
            
            constexpr T *operator->() {
                return &std::get<T>(value_);
            }
            
            template <typename ... Args>
            constexpr void emplace(Args && ... args) {
                value_.template emplace<T>(std::forward<Args>(args) ...);
            }
            
        private:
            std::variant<T> value_;
        };
        
        template <typename T>
        class box<T &> {
        public:
            using type = T &;
            
            constexpr box(T &reference) noexcept : reference_(reference) {
            }
            
            constexpr T &get() const {
                return reference_.get();
            }
            
            constexpr T *operator->() const {
                return &reference_.get();
            }
            
            constexpr void emplace(T &reference) {
                reference_ = reference;
            }
            
        private:
            std::reference_wrapper<T> reference_;
        };
        
        template <typename T>
        struct box_type {
            using type = box<T>;
        };
        
        template <typename T>
        struct box_type<T &> {
            using type = box<T &>;
        };
        
        template <typename T>
        struct box_type<T &&> {
            using type = box<T>;
        };
    }
    
    template <typename T>
    using box = typename detail::box_type<T>::type;
    
    template <typename T, enable_if_t<std::is_lvalue_reference_v<T>> = enabler>
    constexpr auto forward_box(T element) noexcept {
        return box<T>(element);
    }
    
    template <typename T, enable_if_t<!std::is_lvalue_reference_v<T>> = enabler>
    constexpr auto forward_box(T &element) noexcept {
        return box<T>(std::move(element));
    }
    
    template <typename T, enable_if_t<!std::is_lvalue_reference_v<T>> = enabler>
    constexpr auto forward_box(T &&element) noexcept {
        return box<T>(std::move(element));
    }
}

#endif
