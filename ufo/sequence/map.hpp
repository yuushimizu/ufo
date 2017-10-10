#ifndef ufo_sequence_map
#define ufo_sequence_map

#include <tuple>
#include "sequence.hpp"
#include "container_wrapper.hpp"
#include "sequence_operator.hpp"
#include "../option.hpp"

namespace ufo {
    template <typename F, typename Sequence>
    class Mapped : public sequence {
    private:
        F f_;
        Sequence sequence_;
        
    public:
        constexpr Mapped(F f, Sequence sequence) : f_(std::move(f)), sequence_(std::move(sequence)) {
        }
        
        constexpr auto next() ->  decltype(sequence_.next().map(f_)) {
            return sequence_.next().map(f_);
        }
    };
    
    template <typename F>
    class Map {
    public:
        constexpr Map(F f) : f_(std::move(f)) {
        }
        
        template <typename Sequence>
        constexpr auto operator()(Sequence &&sequence) const & {
            return Mapped<F, Sequence>(f_, std::forward<Sequence>(sequence));
        }
        
        template <typename Sequence>
        constexpr auto operator()(Sequence &&sequence) & {
            return Mapped<F, Sequence>(f_, std::forward<Sequence>(sequence));
        }
        
        template <typename Sequence>
        constexpr auto operator()(Sequence &&sequence) && {
            return Mapped<F, Sequence>(std::move(f_), std::forward<Sequence>(sequence));
        }
        
    private:
        F f_;
    };
    
    template <typename F>
    constexpr auto map(F f) {
        return sequence_operator(Map<F>(std::move(f)));
    }
}

#endif
