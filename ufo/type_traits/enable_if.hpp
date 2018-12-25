#ifndef ufo_type_traits_enable_If
#define ufo_type_traits_enable_If

namespace ufo {
    using enabler_t = std::nullptr_t;
    
    constexpr const inline enabler_t enabler = nullptr;
    
    template <bool value, typename T = enabler_t>
    using enable_if_t = std::enable_if_t<value, T>;
}

#endif
