#include <iostream>
#include <fost/postgres>
#include <fost/unicode>


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

std::string stringify(const fostlib::json &j) {
    return j.apply_visitor(
        [](std::monostate) -> std::string {
            return "null";
        },
        [](bool b) -> std::string {
            return b ? "true" : "false";
        },
        [](int64_t i) {
            return std::to_string(i);
        },
        [](double d) {
            return std::to_string(d);
        },
        [](f5::lstring s) {
            return escape(fostlib::string(s));
        },
        [](fostlib::json::string_p s) {
            return escape(*s);
        },
        [](fostlib::json::object_p o) {
            std::string r{"{"};
            bool first{true};
            for ( const auto &p : *o ) {
                if ( not first ) {
                    r += ',';
                } else {
                    first = false;
                }
                r += escape(p.first) + ':' + stringify(p.second);
            }
            r += '}';
            return r;
        },
        [](fostlib::json::array_p a) -> std::string {
            throw fostlib::exceptions::not_implemented(__PRETTY_FUNCTION__);
        }
    );
}


int main(int argc, char *argv[]) {
    if ( argc < 2 ) {
        std::cout << "Pass an SQL statement to execute" << std::endl;
        std::exit(1);
    }

    fostlib::pg::connection cnx;
    auto rs = cnx.exec(argv[1]);
    bool first{true};
    for ( auto &name : rs.columns() ) {
        if ( !first ) {
            std::cout << ',';
        } else {
            first = false;
        }
        std::cout << stringify(name);
    }
    std::cout << '\n';
    for ( auto &row : rs ) {
        bool first{true};
        for ( const fostlib::json &field : row ) {
            if ( not first ) {
                std::cout << ',';
            } else {
                first = false;
            }
            std::cout << stringify(field);
        }
        std::cout << '\n';
    }
}

