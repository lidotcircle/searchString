#include <iostream>
#include <fstream>
#include <string>
#include <tuple>
#include "search_string_gb2312.hpp"
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

    auto sbegin = gb2312Begin(inputBegin, inputEnd, true, true, 10);
    auto send = sbegin.end();

    for(;sbegin != send;sbegin++) {
        cout << std::get<0>(*sbegin) << ": " << std::get<2>(*sbegin) << endl;
    }

    return 0;
}

