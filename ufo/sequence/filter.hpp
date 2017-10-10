#ifndef ufo_sequence_filter
#define ufo_sequence_filter

#include "sequence.hpp"
#include "sequence_operator.hpp"
#include "../option.hpp"

namespace ufo {
    template <typename F, typename Sequence>
    class Filtered : public sequence {
    private:
        F f_;
        Sequence sequence_;
        
    public:
        constexpr Filtered(F f, Sequence sequence) noexcept : f_(std::move(f)), sequence_(std::move(sequence)) {
        }
        
        constexpr auto next() -> sequence_option_t<Sequence> {
            while (true) {
                auto value = sequence_.next();
                if (!value) return nullopt;
                if (f_(*value)) return value;
            }
        }
    };
    
    template <typename F>
    class Filter {
    private:
        F f_;
        
    public:
        constexpr Filter(F f) noexcept : f_(std::move(f)) {
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
    };
    
    template <typename F>
    constexpr auto filter(F f) noexcept {
        return sequence_operator(Filter<F>(std::move(f)));
    }
}

#endif

