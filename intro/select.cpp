#include <iostream>
#include <fost/postgres>


fostlib::string stringify(const fostlib::json &j) {
    return fostlib::json::unparse(j, false);
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

