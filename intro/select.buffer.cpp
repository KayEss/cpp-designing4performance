#include <iostream>
#include <fost/postgres>


void stringify(const fostlib::json &j, std::string &into) {
    fostlib::json::unparse(into, j, false);
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

