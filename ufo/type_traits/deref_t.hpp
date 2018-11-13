#ifndef ufo_type_traits_deref_t
#define ufo_type_traits_deref_t

#include <type_traits>

namespace ufo {
    template <typename T>
    using deref_t = decltype(*std::declval<T>());
}

#endif
