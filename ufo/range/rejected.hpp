#ifndef ufo_range_rejected
#define ufo_range_rejected

#include <utility>
#include "filtered.hpp"

namespace ufo {
    template <typename Predicate>
    auto rejected(Predicate predicate) {
        return filtered([predicate = std::move(predicate)](auto && ... x) {return !predicate(std::forward<decltype(x)>(x) ...);});
    }
}

#endif
