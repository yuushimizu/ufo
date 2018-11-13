#ifndef ufo_sequence_map
#define ufo_sequence_map

#include <tuple>
#include <functional>
#include "sequence_operator.hpp"
#include "sequence_wrapper.hpp"
#include "../option.hpp"

namespace ufo {
    template <typename F>
    constexpr auto map(F &&f) noexcept {
        return sequence_operator([](auto f, auto && ... sequences) constexpr {
            return sequence_wrapper([f = std::move(f)](auto & ... sequences) constexpr {
                return ([&f](auto && ... results) constexpr {
                    auto invoke = [&]() {return forward_option(std::invoke(f, *std::forward<decltype(results)>(results) ...));};
                    return (... && static_cast<bool>(results)) ? invoke() : decltype(invoke()) {};
                })(sequences.next() ...);
            }, std::forward<decltype(sequences)>(sequences) ...);
        }, std::forward<F>(f));
    }
    
    template <typename F, typename ... Sequences>
    constexpr auto map(F &&f, Sequences && ... sequences) {
        return map(std::forward<F>(f))(std::forward<Sequences>(sequences) ...);
    }
}

#endif
