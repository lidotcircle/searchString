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
#include "search_string.h"

void print_transforms(const std::string& encoding);

int trainsvm_cmd(int argc, char** argv);

#endif // _STRING_MAIN_H_
