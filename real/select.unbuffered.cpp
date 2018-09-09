#include "json.hpp"
#include <iostream>
#include <fost/postgres>


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

    /// Turns off the stdout buffering
    std::setvbuf(stdout, NULL, _IONBF, 0);

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

