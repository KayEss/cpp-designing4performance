#include <chrono>
#include <fstream>
#include <iostream>
#include <vector>


int main() {
    std::vector<char> data(10 << 20);
    std::ifstream("/dev/urandom", std::ios::binary).read(data.data(), data.size());
    const auto write = [&data](std::size_t size) {
        std::ofstream out("/dev/null", std::ios::binary);
        const auto started = std::chrono::steady_clock::now();
        std::size_t writes{};
        for ( std::size_t i{}; i < data.size(); i += size ) {
            out.write(&data[i], size);
            ++writes;
        }
        const auto ended = std::chrono::steady_clock::now();
        const auto ns = std::chrono::duration_cast<
            std::chrono::nanoseconds>(ended - started).count();
        std::cout << (size >> 10) << "KB per write " << writes <<
            " writes taking " << (ns / 1000) << "us (" << (ns / writes) << "ns each). "
            "Per byte " << (1000.0 * ns / data.size()) << "ns" << std::endl;
    };
    /// Perform lots of small allocations
    write(1 << 10);
    /// Perform fewer bigger allocations
    write(16 << 10); // 16 times bigger
    /// Perform even bigger allocations
    write(512 << 10); // 32 times bigger
}

