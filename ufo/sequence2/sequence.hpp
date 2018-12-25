#ifndef ufo_sequence_sequence
#define ufo_sequence_sequence

#include <functional>
#include "iterator.hpp"
#include "../type_traits.hpp"
#include "../functional.hpp"

namespace ufo::s2 {
    template <typename F>
    class sequence {
    public:
        constexpr sequence(F f) noexcept : emitter_(std::move(f)) {
        }
        
        constexpr sequence(const sequence &) = default;
        
        constexpr sequence(sequence &&) = default;
        
        constexpr sequence &operator=(const sequence &) = default;
        
        constexpr sequence &operator=(sequence &&) = default;
        
        ~sequence() noexcept = default;
        
        constexpr const auto &emitter() const noexcept {
            return emitter_;
        }
        
        constexpr auto begin() const noexcept {
            return iterator(emitter_);
        }
        
    private:
        iterator_emitter<F> emitter_;
    };
    
    template <typename T>
    constexpr inline const bool is_sequence_v = is_instantiation_of_v<sequence, T>;
}

#endif
