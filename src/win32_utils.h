#if (defined(_WIN32) || defined(_WIN64)) && !defined(_WIN32_UTILS_H_)
#define _WIN32_UTILS_H_

#include <string>

int GetPIDByProcessName(const std::string& processName);

#endif // _WIN32_UTILS_H_