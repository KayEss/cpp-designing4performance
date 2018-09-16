#include <cstdlib>

int main() {
    const auto trial = [](const std::size_t size) {
        for ( auto left = 10 << 20; left; left -= size ) {
            if ( not malloc(size) ) std::exit(2);
        }
    };
}

