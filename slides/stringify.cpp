#include "json.hpp"

std::string stringify(const json &v) {
    struct stringer {
        std::string operator () (std::monostate) {
            return "null";
        }
        std::string operator () (bool b) {
            return b ? "true" : "false";
        }
        std::string operator () (double d) {
            return std::to_string(d);
        }
        std::string operator () (const json::array_type &a) {
            std::string s;
            for ( const auto &i : a ) {
                if ( not s.empty() ) s += ',';
                s += stringify(*i);
            }
            return "[" + s + "]";
        }
        std::string operator () (int64_t i) {
            return std::to_string(i);
        }
        std::string operator () (const std::string &s) {
            return escape(s);
        }
        std::string operator () (const json::object_type &o) {
            return "{}";
        }
    };
    return std::visit(stringer{}, v.value);
}

void stringify(const json &v, std::string&s) {
    struct stringer {
        std::string &s;
        void operator () (std::monostate) {
            s += "null";
        }
        void operator () (bool b) {
            s += b ? "true" : "false";
        }
        void operator () (double d) {
            s+= std::to_string(d);
        }
        void operator () (const json::array_type &a) {
            s += '[';
            bool first{false};
            for ( const auto &i : a ) {
                if ( not first ) s += ',';
                else first = false;
                stringify(*i, s);
            }
            s += ']';
        }
        void operator () (int64_t i) {
            s += std::to_string(i);
        }
        void operator () (const std::string &e) {
            s += escape(e);
        }
        void operator () (const json::object_type &o) {
            s += "{}";
        }
    };
    std::visit(stringer{s}, v.value);
}

std::string escape(const std::string &s) {
    return s;
}

