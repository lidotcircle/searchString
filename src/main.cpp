#include <iostream>
#include <fstream>
#include <memory>
#include <string>
#include <tuple>
#include "search_string_gb2312.hpp"
#include "string_validator.h"
#include "gb2312_validator.h"
using namespace std;


static void usage(char* cmd) {
    cout << "Usage: " << string(cmd) << " <filename>" << endl;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        usage(argv[0]);
        return 1;
    }

    const char* filename = argv[1];
    std::ifstream inputFile(filename);
    std::istreambuf_iterator<char> inputBegin(inputFile), inputEnd;

    auto sbegin = gb2312Begin(inputBegin, inputEnd, true, true);
    auto send = sbegin.end();
    std::shared_ptr<StringValidator> n = std::make_shared<MiniumLength>(4);
    // sbegin.add_validator(std::make_shared<StringValidator>(x));
    // sbegin.add_validator(std::make_shared<StringValidator>(new GB2312Validator()));

    for(;sbegin != send;sbegin++) {
        cout << std::get<0>(*sbegin) << ": " << std::get<2>(*sbegin) << endl;
    }

    return 0;
}

