#ifndef ufo_memory
#define ufo_memory

#include <memory>

namespace ufo {
    template <typename P, typename D>
    auto make_unique_ptr(P *p, D deleter) noexcept {
        return std::unique_ptr<P, D>(p, std::move(deleter));
    }
    
    struct owner_before_compare {
        template <typename LHS, typename RHS>
        bool operator()(const LHS &lhs, const RHS &rhs) const noexcept {
            return lhs.owner_before(rhs);
        }
    };
}

#endif
