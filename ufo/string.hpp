#ifndef ufo_string
#define ufo_string

#include <string>

namespace ufo::string {
    template <typename T, typename Delimiter>
    auto join(T &&strings, const Delimiter &delimiter) {
        std::string result("");
        bool first = true;
        for (auto &&s : strings) {
            if (first) {
                first = false;
            } else {
                result += delimiter;
            }
            result += s;
        }
        return result;
    }
}

#endif
