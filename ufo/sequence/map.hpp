#ifndef ufo_sequence_map
#define ufo_sequence_map

#include <tuple>
#include "sequence_wrapper.hpp"
#include "sequence.hpp"
#include "container_wrapper.hpp"
#include "sequence_operator.hpp"
#include "../LambdaWrapper.hpp"
#include "../option.hpp"

namespace ufo {
    template <typename F>
    constexpr auto map(F f) noexcept {
        return sequence_wrapper([f = std::move(f)](auto && ... sequences) constexpr {
            return ([&f](auto && ... results) constexpr {
                using result_option = decltype(make_option(f(*std::forward<decltype(results)>(results) ...)));
                if ((... && static_cast<bool>(results))) return make_option(f(*std::forward<decltype(results)>(results) ...));
                return result_option {};
            })(sequences.next() ...);
        });
    }
    
    template <typename F, typename ... Sequences>
    constexpr auto map(F &&f, Sequences && ... sequences) {
        return map(std::forward<F>(f))(std::forward<Sequences>(sequences) ...);
    }
}

#endif
