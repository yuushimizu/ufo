#ifndef ufo_type_traits
#define ufo_type_traits

namespace ufo {
    template <bool value>
    using nullptr_t_if_t = std::enable_if_t<value, std::nullptr_t>;
    
    template <template <typename ...> class Template, typename T>
    struct is_instantiation_of : std::false_type {};
    
    template <template <typename ...> class Template, typename ... Args>
    struct is_instantiation_of<Template, Template<Args ...>> : std::true_type {};
    
    template <template <typename ...> class Template, typename T>
    constexpr bool is_instantiation_of_v = is_instantiation_of<Template, T>::value;
}

#endif
