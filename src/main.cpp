#include <iostream>
#include <fstream>
#include <string>
#include "search_gb2312_string.hpp"
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

    auto pairs = searchGB2312(inputBegin, inputEnd, 6, 0);
    for(auto& p: pairs) {
        cout << p.first << " " << p.second << endl;
    }

    return 0;
}

