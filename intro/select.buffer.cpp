#include "json.hpp"
#include <iostream>
#include <fost/postgres>


void stringify(const fostlib::json &j, std::string &into) {
    j.apply_visitor(
        [&](std::monostate) {
            into += "null";
        },
        [&](bool b) {
            into += b ? "true" : "false";
        },
        [&](int64_t i) {
            into += std::to_string(i);
        },
        [&](double d) {
            into += std::to_string(d);
        },
        [&](f5::lstring s) {
            escape(s, into);
        },
        [&](fostlib::json::string_p s) {
            escape(*s, into);
        },
        [&](fostlib::json::object_p o) {
            into += '{';
            bool first{true};
            for ( const auto &p : *o ) {
                if ( not first ) {
                    into += ',';
                } else {
                    first = false;
                }
                into += escape(p.first);
                into += ':';
                stringify(p.second, into);
            }
            into += '}';
        },
        [&](fostlib::json::array_p a) {
            throw fostlib::exceptions::not_implemented(__PRETTY_FUNCTION__);
        }
    );
}


int main(int argc, char *argv[]) {
    if ( argc < 2 ) {
        std::cout << "Pass an SQL statement to execute" << std::endl;
        std::exit(1);
    }

    std::string buffer;
    buffer.reserve(64 * 1024);

    fostlib::pg::connection cnx;
    auto rs = cnx.exec(argv[1]);
    bool first{true};
    for ( auto &name : rs.columns() ) {
        if ( !first ) {
            buffer += ',';
        } else {
            first = false;
        }
        stringify(name, buffer);
    }
    buffer += '\n';
    for ( auto &row : rs ) {
        bool first{true};
        for ( const fostlib::json &field : row ) {
            if ( not first ) {
                buffer += ',';
            } else {
                first = false;
            }
            stringify(field, buffer);
        }
        buffer += '\n';
        if ( buffer.length() > 60 * 1024 ) {
            std::cout << buffer;
            buffer.clear();
        }
    }
    std::cout << buffer;
}

