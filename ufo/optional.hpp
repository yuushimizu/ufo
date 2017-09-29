#ifndef ufo_optional
#define ufo_optional

#include <experimental/optional>
#include "optref.hpp"
#include "type_traits.hpp"

namespace ufo {
    using std::experimental::nullopt_t;
    using std::experimental::nullopt;
    
    template <typename T>
    struct OptionalType {
        using type = std::experimental::optional<T>;
    };
    
    template <typename T>
    struct OptionalType<T &> {
        using type = optref<T>;
    };
    
    template <typename T>
    using optional = typename OptionalType<T>::type;
}

#endif
