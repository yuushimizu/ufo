#ifndef ufo_sequence_as_container
#define ufo_sequence_as_container

#include "sequence_operator.hpp"
#include "foreach.hpp"
#include "../type_traits.hpp"

namespace ufo {
    template <typename Sequence>
    class AsContainer {
    private:
        Sequence sequence_;
        
    public:
        constexpr AsContainer(Sequence &&sequence) noexcept : sequence_(std::forward<Sequence>(sequence)) {
        }
        
        template <typename Container>
        operator Container() && {
            Container result {};
            std::forward<Sequence>(sequence_) | foreach([&result](auto &&v) {
                result.push_back(std::forward<decltype(v)>(v));
            });
            return result;
        }
    };
    
    constexpr const auto as_container = sequence_operator([](auto &&sequence) constexpr noexcept {
        return AsContainer<template_deduce_t<decltype(sequence)>>(std::forward<decltype(sequence)>(sequence));
    });
}

#endif
