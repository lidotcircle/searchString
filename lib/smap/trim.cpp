#include "smap/trim.h"
#include "smap/mapper_factory.h"
#include "utils.h"
using namespace std;


static string trim_string(const string& str, bool left, bool right) {
    string::size_type start = 0;
    string::size_type end = str.size();
    if (left) {
        start = str.find_first_not_of(" \t\r\n");
        if (start == string::npos) {
            return "";
        }
    }
    if (right) {
        end = str.find_last_not_of(" \t\r\n");
        if (end == string::npos) {
            return "";
        }
    }
    return str.substr(start, end - start + 1);
}

TrimMapper::TrimMapper(bool left, bool right): trim_left(left), trim_right(right) {}

vector<pair<size_t,string>> TrimMapper::map(const std::string& str) const {
    return { {0,trim_string(str, this->trim_left, this->trim_right) } };
}

static auto name = "trim";
static auto desc = "trim left of string or right of string or both, [left | right]";
static auto creator = [](const string& k) {
    bool left = true, right = true;
    if (k == "left")
        right = false;
    else if (k == "right")
        left = false;
    else if (!k.empty())
        throw std::runtime_error("please specify a correct mode");
    return shared_ptr<StringMapper>(new TrimMapper(left, right));
};
const vector<int> TrimMapper::register_handles = {
    MapperFactory::register_mapper("ascii",  name, desc, creator),
    MapperFactory::register_mapper("gb2312", name, desc, creator),
};
