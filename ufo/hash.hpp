#ifndef ufo_hash
#define ufo_hash

namespace ufo {
    template <typename T>
    constexpr std::size_t combine_hash(const std::size_t &seed, const T &hash) {
        return seed ^ hash + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }

    template <typename T>
    constexpr std::size_t combined_hash(const T &value) {
        return std::hash<T>()(value);
    }

    template <typename First, typename ... Rest>
    constexpr std::size_t combined_hash(const First &first, const Rest & ... rest) {
        return combine_hash(combined_hash(first), combined_hash(rest ...));
    }
}

#endif
