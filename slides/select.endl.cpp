#include "json.hpp"
#include <iostream>



void foo() {

std::vector<std::vector<json>> rs;

for ( auto &row : rs ) {
    bool first{true};
    for ( const json &field : row ) {
        if ( not first ) {
            std::cout << ',';
        } else {
            first = false;
        }
        std::cout << stringify(field);
    }
    std::cout << std::endl;
}


}
