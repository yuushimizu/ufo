#ifndef ufo_math
#define ufo_math

namespace ufo {
    auto digits(int n) {
        return n == 0 ? 1 : static_cast<int>(std::floor(std::log10(n))) + 1;
    }
}

#endif
