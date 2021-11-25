#include "utils.h"
#include <regex>
#include <string>
using namespace std;


std::pair<std::regex,std::regex> parse_needfilter_regex(const std::string& need) {
    string f1 = need.substr(0, need.find("@"));
    string f2 = need.substr(need.find("@") + 1);
    if (need.find("@") == string::npos)
        f1 = need;

    if (f1.empty())
        f1 = ".*";
    if (f2.empty())
        f2 = ".*";

    return std::make_pair(std::regex(f1, std::regex_constants::ECMAScript),
                          std::regex(f2, std::regex_constants::ECMAScript));
}
