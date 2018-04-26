#ifndef ufo_sequence_repeatedly
#define ufo_sequence_repeatedly

#include "sequence.hpp"
#include "../option.hpp"
#include "../LambdaWrapper.hpp"

namespace ufo {
    template <typename F>
    class Repeatedly : public sequence {
    private:
        LambdaWrapper<F> f_;
        
    public:
        constexpr explicit Repeatedly(F f) noexcept : f_(std::move(f)) {
        }
        
        constexpr auto next() {
            return refoption(f_());
        }
    };
    
    template <typename F>
    constexpr auto repeatedly(F f) noexcept {
        return Repeatedly<F>(std::move(f));
    }
}

#endif
