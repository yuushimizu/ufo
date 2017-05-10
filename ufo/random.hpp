#ifndef ufo_Random
#define ufo_Random

#include <utility>
#include <random>
#include "Coord.hpp"
#include "Rect.hpp"

namespace ufo {
    template <typename T = int, typename Engine>
    T random_int(Engine &engine, T min, T max) {
        return std::uniform_int_distribution<T>(min, max)(engine);
    }
    
    template <typename T = int, typename Engine>
    T random_int(Engine &engine, T max) {
        return random_int<T>(engine, T {}, max);
    }
    
    template <typename T = double, typename Engine>
    T random_real(Engine &engine, T min, T max) {
        return std::uniform_real_distribution<T>(min, max)(engine);
    }
    
    template <typename T = double, typename Engine>
    T random_real(Engine &engine, T max) {
        return random_real<T>(engine, T {}, max);
    }
    
    template <typename T = double, typename Engine>
    T random_canonical(Engine &engine) {
        return random_real<T>(engine, 1.0);
    }
    
    template <typename Engine>
    bool random_bool(Engine &engine, double rate) {
        return random_canonical(engine) < rate;
    }
    
    template <typename Engine, typename Container>
    decltype(auto) sample_one(Engine &engine, const Container &container) {
        return container[random_int<std::size_t>(engine, container.size() - 1)];
    }
    
    template <typename Engine>
    Coord<int> random_int_coord(Engine &engine, const Coord<int> &min, const Coord<int> &max) {
        return transform_coord([&engine](int min, int max) {
            return random_int(engine, min, max);
        }, min, max);
    }
    
    template <typename Engine>
    Coord<int> random_int_coord(Engine &engine, const Coord<int> &max) {
        return random_int_coord(engine, Coord<int> {}, max);
    }
    
    template <typename Engine>
    Coord<double> random_double_coord(Engine &engine, const Coord<double> &min, const Coord<double> &max) {
        return transform_coord([&engine](double min, double max) {
            return random_real(engine, min, max);
        }, min, max);
    }
    
    template <typename Engine>
    Coord<double> random_double_coord(Engine &engine, const Coord<double> &max) {
        return random_double_coord(engine, Coord<double> {}, max);
    }
    
    template <typename Engine>
    Coord<int> random_int_coord(Engine &engine, const Rect<int> &rect) {
        return random_int_coord(engine, rect.origin(), rect.origin() + rect.size() - COORD_ONE<int>);
    }
    
    template <typename Engine>
    Coord<double> random_double_coord(Engine &engine, const Rect<double> &rect) {
        return random_double_coord(engine, rect.origin(), rect.origin() + rect.size());
    }
}

#endif
