#ifndef ufo_type_traits
#define ufo_type_traits

#include <type_traits>
#include "type_traits/deref_t.hpp"

namespace ufo {
    template <bool value, typename T = std::nullptr_t>
    using enable_if_t = std::enable_if_t<value, T>;
    
    template <template <typename ...> class Template, typename T>
    struct is_instantiation_of : std::false_type {};
    
    template <template <typename ...> class Template, typename ... Args>
    struct is_instantiation_of<Template, Template<Args ...>> : std::true_type {};
    
    template <template <typename ...> class Template, typename T>
    constexpr const bool is_instantiation_of_v = is_instantiation_of<Template, T>::value;
    
    template <typename From, typename To>
    struct is_convertible : std::is_convertible<From, To> {};
    
    template <typename From>
    struct is_convertible<From, void> : std::is_void<From> {};
    
    template <typename To>
    struct is_convertible<void, To> : std::is_void<To> {};
    
    template <>
    struct is_convertible<void, void> : std::true_type {};
    
    template <typename From, typename To>
    constexpr const bool is_convertible_v = is_convertible<From, To>::value;
}

#endif
