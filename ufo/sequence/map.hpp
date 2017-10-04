#ifndef ufo_sequence_map
#define ufo_sequence_map

#include <tuple>
#include "sequence.hpp"
#include "container_wrapper.hpp"
#include "sequence_operator.hpp"

namespace ufo {
    template <typename F, typename ... Sequences>
    class Mapped : public sequence {
    public:
        constexpr Mapped(F f, Sequences ... sequences) : f_(std::move(f)), sequences_(std::move(sequences) ...) {
        }
        
        constexpr decltype(auto) first() const {
            return std::apply([this](const auto & ... sequences) constexpr {
                return this->f_(sequences.first() ...);
            }, sequences_);
        }
        
        constexpr void pop() {
            std::apply([](auto & ... sequences) constexpr {
                (void) (..., sequences.pop());
            }, sequences_);
        }
        
        constexpr bool empty() const {
            return std::apply([](const auto & ... sequences) constexpr {
                return (... && sequences.empty());
            }, sequences_);
        }
        
    private:
        F f_;
        std::tuple<Sequences ...> sequences_;
    };
    
    template <typename F, typename ... Sequences>
    constexpr auto map(F f, Sequences ... sequences) {
        return [](auto f, auto ... sequences) constexpr {
            return Mapped<F, decltype(sequences) ...>(std::move(f), std::move(sequences) ...);
        }(std::move(f), container_wrapper(std::move(sequences)) ...);
    }
    
    template <typename F>
    class Map {
    public:
        constexpr Map(F f) : f_(std::move(f)) {
        }
        
        template <typename Sequence>
        constexpr auto operator()(Sequence &&sequence) const & {
            return map(f_, std::forward<Sequence>(sequence));
        }
        
        template <typename Sequence>
        constexpr auto operator()(Sequence &&sequence) & {
            return map(f_, std::forward<Sequence>(sequence));
        }
        
        template <typename Sequence>
        constexpr auto operator()(Sequence &&sequence) && {
            return map(std::move(f_), std::forward<Sequence>(sequence));
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
