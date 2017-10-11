#ifndef ufo_type_traits
#define ufo_type_traits

#include <type_traits>

namespace ufo {
    template <bool value, typename T = std::nullptr_t>
    using enable_if_t = std::enable_if_t<value, T>;
    
    template <template <typename ...> class Template, typename T>
    struct is_instantiation_of : std::false_type {};
    
    template <template <typename ...> class Template, typename ... Args>
    struct is_instantiation_of<Template, Template<Args ...>> : std::true_type {};
    
    template <template <typename ...> class Template, typename T>
    constexpr bool is_instantiation_of_v = is_instantiation_of<Template, T>::value;
    
    template <typename T>
    struct template_deduce {
        using type = T;
    };
    
    template <typename T>
    struct template_deduce<T &&> {
        using type = T;
    };
    
    template <typename T>
    struct template_deduce<T &> {
        using type = T &;
    };
    
    template <typename T>
    using template_deduce_t = typename template_deduce<T>::type;
}

#endif
