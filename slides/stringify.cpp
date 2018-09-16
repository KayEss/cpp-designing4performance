#include "json.hpp"

struct stringer {
    std::string operator () (std::monostate) {
        return "null";
    }
    std::string operator () (bool b) {
        return b ? "true" : "false";
    }
    std::string operator () (int64_t i) {
        return std::to_string(i);
    }
    std::string operator () (double d) {
        return std::to_string(d);
    }
    std::string operator () (const std::string &s) {
        return escape(s);
    }
    std::string operator () (const json::object_type &o) {
        return "{}";
    }
    std::string operator () (const json::array_type &a) {
        return "[]";
    }
};

std::string stringify(const json &v) {
    return std::visit(stringer{}, v.value);
}

std::string escape(const std::string &s) {
    return s;
}

