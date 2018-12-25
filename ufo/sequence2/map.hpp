#ifndef ufo_sequence2_map
#define ufo_sequence2_map

#include "sequence_operator.hpp"
#include "sequence.hpp"
#include "../functional.hpp"
#include "../option.hpp"

namespace ufo::s2 {
    template <typename F>
    constexpr auto map(F f) noexcept {
        return sequence_operator(partial([](auto &&f, auto && ... seqs) {
            return sequence([f = function_wrapper(std::forward<F>(f)), seqs = std::make_tuple(forward_box<decltype(seqs)>(seqs) ...)] {
                return std::apply([&f](auto && ... seqs) {
                    return [&f, bases = std::tuple<decltype(seqs->begin()) ...> {seqs->emitter() ...}]() mutable {
                        auto options = std::apply([](auto & ... bases) {return std::make_tuple(bases.next() ...);}, bases);
                        return std::apply([&](auto & ... options) {
                            auto invoke = [&] {return forward_option(f(*options ...));};
                            return (... && static_cast<bool>(options)) ? invoke() : decltype(invoke()) {};
                        }, options);
                    };
                }, seqs);
            });
        }, std::move(f)));
    }
    
    template <typename F, typename ... Seqs>
    constexpr auto map(F &&f, Seqs && ... seqs) {
        return map(std::forward<F>(f))(std::forward<Seqs>(seqs) ...);
    }
}

#endif
