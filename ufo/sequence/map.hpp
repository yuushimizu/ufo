#ifndef ufo_sequence_map
#define ufo_sequence_map

#include <tuple>
#include "sequence.hpp"
#include "container_wrapper.hpp"
#include "sequence_operator.hpp"
#include "../lambda_wrapper.hpp"
#include "../option.hpp"

namespace ufo {
    template <typename F, typename ... Sequences>
    class Mapped : public sequence {
    private:
        LambdaWrapper<F> f_;
        std::tuple<Sequences ...> sequences_;
        
    public:
        template <typename ... Seqs>
        constexpr Mapped(F f, Seqs && ... sequences) : f_(std::move(f)), sequences_ {std::forward<Seqs>(sequences) ...} {
        }
        
        constexpr auto next() {
            return std::apply([this](auto & ... sequences) constexpr {
                return ([this](auto && ... results) constexpr -> decltype(make_option(this->f_(*std::forward<decltype(results)>(results) ...))) {
                    if ((... && static_cast<bool>(results))) return this->f_(*std::forward<decltype(results)>(results) ...);
                    return nullopt;
                })(sequences.next() ...);
            }, sequences_);
        }
    };
    
    template <typename F>
    constexpr auto map(F &&f) noexcept {
        return sequence_operator([](auto &&f, auto && ... sequences) {
            return Mapped<std::decay_t<decltype(f)>, std::decay_t<decltype(sequences)> ...>(std::forward<decltype(f)>(f), std::forward<decltype(sequences)>(sequences) ...);
        }, std::forward<F>(f));
    }
    
    template <typename F, typename ... Sequences>
    constexpr auto map(F &&f, Sequences && ... sequences) {
        return map(std::forward<F>(f))(std::forward<Sequences>(sequences) ...);
    }
}

#endif
