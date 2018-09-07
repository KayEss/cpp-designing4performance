#include <fost/unicode>

void escape(f5::u8view, std::string &);

inline std::string escape(f5::u8view s) {
    std::string into;
    escape(s, into);
    return into;
}

