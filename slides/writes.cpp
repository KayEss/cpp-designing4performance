#include <fstream>
#include <vector>

int main() {
std::vector<char> data(10 << 20);
std::ifstream("/dev/urandom", std::ios::binary).read(data.data(), data.size());
const auto trial = [&data](std::size_t size) {
    std::ofstream out("/dev/null", std::ios::binary);
    for ( std::size_t i{}; i < data.size(); i += size ) {
        out.write(&data[i], size);
    }
};
}
