#ifndef _CMDLINE_UTILS_H_
#define _CMDLINE_UTILS_H_

#include <regex>

std::pair<std::regex,std::regex> parse_regex(const std::string& need);

#endif // _CMDLINE_UTILS_H_