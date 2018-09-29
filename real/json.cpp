#include "json.hpp"


inline void to_hex(std::string &into, fostlib::utf16 c, std::size_t digits = 3) {
    wchar_t l = ( c & 0xf ) + L'0';
    if ( l > L'9' ) l = l - L'0' - 10 + L'a';
    if ( digits )
        to_hex(into, c >> 4, digits - 1);
    into += l;
}


void escape(f5::u8view s, std::string &into) {
    into += '"';
    for ( auto i : s ) {
        if ( i < 0x20 ) {
            switch( i ) {
            case L'\n': into += "\\n"; break;
            case L'\r': into += "\\r"; break;
            case L'\t': into += "\\t"; break;
            case L'\\': into += "\\\\"; break;
            case L'\"': into += "\\\""; break;
            default:
                into += "\\u";
                to_hex(into, i);
            }
        } else {
            into += i;
        }
    }
    into += '\"';
}

