#include <memory>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>


struct json {
using array_type = std::vector<std::shared_ptr<json>>;
using object_type = std::unordered_map<std::string, std::shared_ptr<json>>;
using value_type = std::variant<
    std::monostate,
    bool, int64_t, double,
    std::string,
    array_type, object_type>;

    value_type value;
};


std::string escape(const std::string &);
std::string stringify(const json &);
