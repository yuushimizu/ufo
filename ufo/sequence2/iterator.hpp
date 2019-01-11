#ifndef ufo_sequence_iterator
#define ufo_sequence_iterator

#include "../functional.hpp"

namespace ufo::s2 {
    template <typename F>
    class iterator;
    
    template <typename F>
    class iterator_emitter {
    public:
        constexpr iterator_emitter(F f) noexcept : f_(std::move(f)) {
        }
        
        friend iterator<F>;
        
    private:
        function_wrapper<F> f_;
    };
    
    template <typename F>
    class iterator {
    public:
        constexpr iterator(const iterator_emitter<F> &emitter) : f_(emitter.f_()) {
        }
        
        constexpr iterator(const iterator &) = delete;
        
        constexpr iterator(iterator &&) = delete;
        
        constexpr iterator &operator=(const iterator &) = delete;
        
        constexpr iterator &operator=(iterator &&) = delete;
        
        ~iterator() noexcept = default;
    
        constexpr auto next() noexcept(noexcept(f_())) {
            return std::invoke(f_);
        }
        
    private:
        std::invoke_result_t<F> f_;
    };
}

#endif
