#ifndef ufo_memory
#define ufo_memory

#include <memory>

namespace ufo {
    template <typename P, typename D>
    auto make_unique_ptr(P *p, D deleter) noexcept {
        return std::unique_ptr<P, D>(p, std::move(deleter));
    }
}

#endif
