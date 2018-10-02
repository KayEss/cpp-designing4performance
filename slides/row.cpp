#include <memory>
#include <string>
#include <string_view>
#include <vector>


struct block {
    std::vector<unsigned char> data;
    std::vector<std::string_view> fields;
};


void process(std::string_view, std::string &);


void loop() {
std::string buffer;
std::size_t cols;
block b;

for ( std::size_t row{}; row * cols < b.fields.size(); ++row ) {
    for ( std::size_t field{}; field < cols; ++field ) {
        process(b.fields[row * cols + field], buffer);
    }
}


}


template<typename T>
class vector {
    std::size_t size_;
    T *data_;
};

class string_view {
    std::size_t size_;
    const char *data_;
};


template<typename T>
class shared_vector {
    std::size_t size_;
    std::shared_ptr<const T> data_;

    shared_vector(std::shared_ptr<const T>, std::size_t);
public:
    shared_vector();
    auto size() const { return size_; }


template<typename Allocator>
shared_vector(std::vector<T, Allocator> v)
: size_{v.size()},
    data_{v.data(), [o = std::move(v)](const auto &&) {}}
{
}


shared_vector split(const std::size_t s) {
    auto r = shared_vector{data_, s};
    data_ = std::shared_ptr<const T>{data_, data_.get() + s};
    return r;
}
    const T* begin() const;
    const T* end() const;
};

class shared_string {
    std::size_t size_;
    std::shared_ptr<const char> data_;

template<typename Traits, typename Allocator>
shared_string(std::basic_string<char, Traits, Allocator> s)
: size_{s.size()},
    data_{s.data(), [o = std::move(s)](const auto &&) {}}
{
}

public:
    operator std::string_view ();
};




void loop2() {
std::string buffer;
std::size_t cols;

using block_t = shared_vector<shared_string>;
block_t b;

for ( auto row{b.split(cols)}; row.size(); row = b.split(cols) ) {
    for ( auto field : row ) {
        process(field, buffer);
    }
}

}
