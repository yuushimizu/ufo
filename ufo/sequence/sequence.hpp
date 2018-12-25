#ifndef ufo_sequence_sequence
#define ufo_sequence_sequence

#include <type_traits>

namespace ufo {
    class sequence {};
    
    template <typename T>
    constexpr const auto is_sequence_v = std::is_base_of_v<sequence, std::decay_t<T>>;
    
    template <typename T>
    using sequence_option_t = decltype(std::declval<T>().next());
    
    template <typename T>
    using sequence_element_t = typename sequence_option_t<T>::type;
}

#endif
