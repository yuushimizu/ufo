#ifndef ufo_test_sequence2_test_utility
#define ufo_test_sequence2_test_utility

#include "ufo/sequence2/sequence.hpp"
#include "../test_utility.hpp"

namespace ufo::test {
    template <typename Seq>
    constexpr auto nocopyseq(Seq &&seq) noexcept {
        return s2::sequence(nocopy([seq = std::forward<Seq>(seq)]() {
            return [i = seq.begin()]() mutable {
                return i.next();
            };
        }));
    }
    
    template <typename Seq>
    constexpr auto collect_values(const Seq &seq) noexcept {
        auto i = seq.begin();
        auto values = std::vector<std::decay_t<decltype(*i.next())>> {};
        while (auto &&element = i.next()) values.emplace_back(*std::forward<decltype(element)>(element));
        return values;
    }
    
    template <typename Seq>
    constexpr auto collect_values(const Seq &seq, std::size_t count) noexcept {
        auto i = seq.begin();
        auto values = std::vector<std::decay_t<decltype(*i.next())>> {};
        for (std::size_t n = 0; n < count; ++n) values.emplace_back(*i.next());
        return values;
    }
    
    template <typename T, typename Seq>
    constexpr auto sequence_assert(const Seq &seq) {
        static_assert(std::is_same_v<ufo::option<T>, decltype(seq.begin().next())>);
        static_assert(!std::is_copy_constructible_v<decltype(seq.begin())>);
        static_assert(!std::is_move_constructible_v<decltype(seq.begin())>);
        static_assert(!std::is_copy_assignable_v<decltype(seq.begin())>);
        static_assert(!std::is_move_assignable_v<decltype(seq.begin())>);
    }
}

#endif
