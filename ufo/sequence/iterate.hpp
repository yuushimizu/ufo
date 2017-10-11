#ifndef ufo_sequence_iterate
#define ufo_sequence_iterate

#include "sequence.hpp"
#include "../option.hpp"
#include "../lambda_wrapper.hpp"

namespace ufo {
    template <typename F, typename T>
    class Iterate : public sequence {
    private:
        LambdaWrapper<F> f_;
        T current_;
        
    public:
        constexpr Iterate(F f, T initial_value) noexcept : f_(std::move(f)), current_(std::move(initial_value)) {
        }
        
        ~Iterate() = default;
        
        Iterate(const Iterate &) = default;
        
        Iterate(Iterate &&) noexcept = default;
        
        Iterate &operator=(const Iterate &) = default;
        
        Iterate &operator=(Iterate &&) noexcept = default;
        
        constexpr option<T> next() {
            auto value = current_;
            current_ = f_(std::move(current_));
            return std::move(value);
        }
    };
    
    template <typename F, typename T>
    constexpr auto iterate(F f, T initial_value) noexcept {
        return Iterate<F, T>(std::move(f), std::move(initial_value));
    }
}

#endif
