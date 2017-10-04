#ifndef ufo_sequence_iterate
#define ufo_sequence_iterate

#include "sequence.hpp"

namespace ufo {
    template <typename F, typename T>
    class Iterate : public sequence {
    public:
        constexpr Iterate(F f, T initial_value) : f_(std::move(f)), current_(std::move(initial_value)) {
        }
        
        constexpr auto first() const {
            return current_;
        }
        
        constexpr void pop() {
            current_ = f_(std::move(current_));
        }
        
        constexpr bool empty() const {
            return false;
        }
        
    private:
        F f_;
        T current_;
    };
    
    template <typename F, typename T>
    constexpr auto iterate(F f, T initial_value) {
        return Iterate<F, T>(std::move(f), std::move(initial_value));
    }
}

#endif
