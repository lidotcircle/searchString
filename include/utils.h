#ifndef _UTILS_H_
#define _UTILS_H_

#include <vector>
#include <string>
#include <iostream>

std::vector<std::string> ls_files_recursively(const std::string& dir);
std::string str2hexstr(const std::string& str);
std::string hexstr2str(const std::string& hexstr);

std::vector<uint16_t> gb2312str2twobytes(const std::string& str);
std::string twobytes2gb2312str(const std::vector<uint16_t>& twobytes);

std::istream& safe_getline(std::istream& is, std::string& t);

#endif // _UTILS_H_