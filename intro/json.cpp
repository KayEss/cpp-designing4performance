#include "json.hpp"


inline void to_hex(std::string &into, fostlib::utf16 c, std::size_t digits = 3) {
    wchar_t l = ( c & 0xf ) + L'0';
    if ( l > L'9' ) l = l - L'0' - 10 + L'a';
    if ( digits )
        to_hex(into, c >> 4, digits - 1);
    into += l;
}


std::string escape(fostlib::string s) {
    std::string into;
    into += '"';
    for ( auto i : s ) {
        switch( i ) {
        case L'\n': into += "\\n"; break;
        case L'\r': into += "\\r"; break;
        case L'\t': into += "\\t"; break;
        case L'\\': into += "\\\\"; break;
        case L'\"': into += "\\\""; break;
        default:
            if ( i > 0x7f || i < 0x20 ) {
                fostlib::utf16 o[2];
                std::size_t l = fostlib::utf::encode(i, o, o + 2);
                into += "\\u";
                to_hex(into, o[0]);
                if ( l == 2 ) {
                    into += "\\u";
                    to_hex(into, o[1]);
                }
            } else {
                into += i;
            }
        }
    }
    into += '\"';
    return into;
}

