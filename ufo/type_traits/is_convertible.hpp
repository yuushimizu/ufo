#ifndef ufo_type_traits_is_convertible
#define ufo_type_traits_is_convertible

namespace ufo {
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
