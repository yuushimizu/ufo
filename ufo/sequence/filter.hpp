#ifndef ufo_sequence_filter
#define ufo_sequence_filter

#include "sequence.hpp"
#include "sequence_operator.hpp"
#include "../option.hpp"

namespace ufo {
    template <typename F, typename Sequence>
    class Filtered : public sequence {
    private:
        using reference = decltype(std::declval<Sequence>().front());
        
    public:
        constexpr Filtered(F f, Sequence sequence) : f_(std::move(f)), sequence_(std::move(sequence)) {
            normalize();
        }
        
        constexpr reference front() const {
            return *current_;
        }
        
        constexpr void pop() {
            sequence_.pop();
            normalize();
        }
        
        constexpr bool empty() const {
            return sequence_.empty();
        }
        
    private:
        F f_;
        Sequence sequence_;
        option<reference> current_;
        
        constexpr void normalize() {
            while (!(sequence_.empty())) {
                current_ = sequence_.front();
                if (f_(*current_)) return;
                sequence_.pop();
            }
        }
    };
    
    template <typename F>
    class Filter {
    public:
        constexpr Filter(F f) : f_(std::move(f)) {
        }
        
        template <typename Sequence>
        constexpr auto operator()(Sequence &&sequence) const & {
            return Filtered<F, Sequence>(f_, std::forward<Sequence>(sequence));
        }
        
        template <typename Sequence>
        constexpr auto operator()(Sequence &&sequence) & {
            return Filtered<F, Sequence>(f_, std::forward<Sequence>(sequence));
        }
        
        template <typename Sequence>
        constexpr auto operator()(Sequence &&sequence) && {
            return Filtered<F, Sequence>(std::move(f_), std::forward<Sequence>(sequence));
        }
        
    private:
        F f_;
    };
    
    template <typename F>
    constexpr auto filter(F f) {
        return sequence_operator(Filter<F>(std::move(f)));
    }
}

#endif

