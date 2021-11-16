#ifndef _STRING_MAIN_H_
#define _STRING_MAIN_H_

#include <iostream>
#include <iomanip>
#include <fstream>
#include <memory>
#include <string>
#include <tuple>
#include <map>
#include <cxxopts.hpp>
#include <stdexcept>
#include "search_string.h"

void print_transforms(const std::string& encoding);

int trainsvm_cmd(int argc, char** argv);

int search_in_pefile(const std::string& pefile,
                     const std::string& encoding,
                     const std::vector<std::string>& transforms,
                     bool print_prefix);

#endif // _STRING_MAIN_H_
