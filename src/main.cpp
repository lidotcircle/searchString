#include <iostream>
#include <fstream>
#include <memory>
#include <string>
#include <tuple>
#include <map>
#include "search_string_gb2312.hpp"
#include "string_filter.h"
#include "gb2312_filter.h"
#include <cxxopts.hpp>
using namespace std;


static int train(string valid_dir, string invalid_dir, string outfile) {
    std::ofstream off(outfile);
    if(!off.is_open()) {
        std::cout << "can't open file " << outfile << endl;
        return 1;
    }

    auto gb = std::make_shared<GB2312Filter>();
    gb->svm_train_directories_recursively(valid_dir, invalid_dir);

    off << *gb;
    return 0;
}

int main(int argc, char** argv) {
    vector<string> train_dirs;
    string train_output;
    string model;
    vector<string> input_files;
    size_t min_length;
    bool disable_svm;

    cxxopts::Options options("StringFinder",
            "train a model to find string in binary files. "
            "currently support gb2312");
    options.add_options()
        ("t,train",  "specify directories to train a model", cxxopts::value<vector<string>>(train_dirs), "<dir,dir>")
        ("o,output", "model output path",                    cxxopts::value<string>(train_output), "<file>")

        ("m,model",   "model to search string",               cxxopts::value<string>(model), "<file>")
        ("min-length",  "minium string length in bytes",     cxxopts::value<size_t>(min_length)->default_value("4"))
        ("disable-svm",  "not using svm to classify strings",     cxxopts::value<bool>(disable_svm)->default_value("false"))
        ("i,inputs", "input files",                          cxxopts::value<vector<string>>(input_files))

        ("h,help",   "print help");
    options.parse_positional("inputs");
    options.positional_help("<files>");
    cxxopts::ParseResult result;
    try {
        result = options.parse(argc, argv);
    } catch (std::exception e) {
        cout << "bad arguments" << endl;
        cout << options.help() << endl;
        return 1;
    }

    if (result.count("help")) {
        cout << options.help() << endl;
        return 0;
    }

    if (!train_dirs.empty()) {
        if (train_output.empty()) {
            cout << options.help() << endl;
            return 1;
        }

        if (train_dirs.size() != 2) {
            cout << options.help() << endl;
        }

        return train(train_dirs[0], train_dirs[1], train_output);
    }

    auto gb = std::make_shared<GB2312Filter>();
    if (!model.empty()) {
        std::ifstream iss(model);
        if (!iss.is_open()) {
            cout << "can't open model file '" << model << "' to read" << endl;
            return 1;
        }
        try {
            iss >> *gb;
        } catch (std::exception e) {
            cout << "can't load model '" << model << "'" << endl;
            return 1;
        }
    }

    int status = 0;
    for (auto& filename: input_files) {
        std::ifstream inputFile(filename, std::ios::binary);

        if(!inputFile.is_open()) {
            cout << "fail to open file" << endl;
            status = 1;
            continue;
        }
        cout << "Search " << filename << ":" << endl;
        istream_iterator<char> inputBegin(inputFile), inputEnd;

        auto sbegin = gb2312Begin(inputBegin, inputEnd, true, true);
        auto send = sbegin.end();
        sbegin.add_filter(std::make_shared<MiniumLength>(min_length));
        if (!disable_svm)
            sbegin.add_filter(gb);

        for(;sbegin != send;sbegin++) {
            cout << std::get<0>(*sbegin) << ": " << std::get<2>(*sbegin) << endl;
        }
    }

    return status;
}

