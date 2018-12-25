#ifndef ufo_functional_identity
#define ufo_functional_identity

namespace ufo {
    constexpr inline auto identity = [](auto &&x) noexcept -> auto && {return std::forward<decltype(x)>(x);};
}

#endif
