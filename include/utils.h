#ifndef _UTILS_H_
#define _UTILS_H_

#include <vector>
#include <string>

std::vector<std::string> ls_files_recursively(const std::string& dir);
std::string str2hexstr(const std::string& str);
std::string hexstr2str(const std::string& hexstr);

#endif // _UTILS_H_

