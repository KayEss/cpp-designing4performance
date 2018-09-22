#include "json.hpp"
#include <iostream>



void buffered() {

    std::vector<std::vector<json>> rs;

std::string buffer;
for ( const auto &row : rs ) {
    bool first{true};
    for ( const json &field : row ) {
        if ( not first ) buffer += ',';
        else first = false;
        stringify(field, buffer);
    }
    buffer += '\n';
    if ( buffer.size() > (60 << 10) ) {
        std::cout << buffer; buffer.clear();
    }
}
std::cout << buffer;


}
