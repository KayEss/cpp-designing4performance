#include <chrono>
#include <iostream>
#include <memory>


int main() {
    const auto trial = [](const std::size_t size) {
        const std::size_t target = 1 << 20; // 1 MB
        std::size_t allocs{};
        const auto started = std::chrono::steady_clock::now();
        for ( auto times = 10; times; --times ) {
            for ( auto left = target; left; left -= size ) {
                if ( not malloc(size) ) std::exit(2);
                ++allocs;
            }
        }
        const auto ended = std::chrono::steady_clock::now();
        const auto ns = std::chrono::duration_cast<
            std::chrono::nanoseconds>(ended - started).count();
        std::cout << (size >> 10) << "KB per allocation. " <<
            allocs << " allocations: " << ns / 1000 << "us (" << (ns / allocs) << "ns each). "
            "Per byte: " << (1000.0 * ns / target) <<  "ps" << std::endl;
    };
    /// Perform lots of small allocations
    trial(1 << 10);
    /// Perform fewer bigger allocations
    trial(16 << 10); // 16 times bigger
    /// Perform even bigger allocations
    trial(512 << 10); // 32 times bigger
}

