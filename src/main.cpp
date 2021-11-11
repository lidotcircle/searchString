#include <iostream>
#include <iomanip>
#include <fstream>
#include <memory>
#include <string>
#include <tuple>
#include <map>
#include <cxxopts.hpp>
#include "search_string.h"
using namespace std;


static int train(string valid_dir, string invalid_dir, string outfile) {
    std::ofstream off(outfile, std::ios::binary);
    if(!off.is_open()) {
        std::cout << "can't open file " << outfile << endl;
        return 1;
    }

    auto gb = std::make_shared<GB2312SVMFilter>();
    gb->svm_train_directories_recursively(valid_dir, invalid_dir);

    off << *gb;
    return 0;
}

int main(int argc, char** argv) {
    string encoding;

    string train_output;
    vector<string> train_dirs;

    string model;
    vector<string> filters;
    vector<string> mappers;
    vector<string> input_files;

    cxxopts::Options options("StringFinder",
            "extract string from a binary file, "
            "support various encoding schema eg. ascii, gb2312");

    options.add_options("extract", {
        { "f,filter",   "string filters, applied in specified order", cxxopts::value<vector<string>>(filters), "<filters>" },
        { "m,mapper",   "string mappers, applied in specified order", cxxopts::value<vector<string>>(mappers), "<mappers>" },
        { "i,input",    "input file", cxxopts::value<vector<string>>(input_files) },
    });
    options.parse_positional("input");
    options.positional_help("<files>");

    options.add_options()
        ("e,encoding", "support ascii and gb2312", cxxopts::value<string>(encoding)->default_value("gb2312"), "<encoding>")
        ("h,help",   "print help");

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

    /* TODO
    if (!train_dirs.empty()) {
        if (train_output.empty()) {
            cout << options.help() << endl;
            return 1;
        }

        if (train_dirs.size() != 2) {
            cout << options.help() << endl;
            return 1;
        }

        return train(train_dirs[0], train_dirs[1], train_output);
    }

    auto gb = std::make_shared<GB2312SVMFilter>();
    if (!model.empty()) {
        std::ifstream iss(model, std::ios::binary);
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
    */

    if (input_files.empty()) {
        cout << options.help() << endl;
        return 1;
    }

    std::vector<std::shared_ptr<StringFilter>> filters_;
    std::vector<std::shared_ptr<StringMapper>> mappers_;
    for (auto& f : filters) {
        auto filter = FilterFactory::create(encoding, f);

        if (!filter) {
            cerr << "can't create filter '" << f << "'" << endl;
            return 1;
        }
        filters_.push_back(filter);
    }
    for (auto& m: mappers) {
        auto map = MapperFactory::create(encoding, m);

        if (!map) {
            cerr << "can't create mapper '" << m << "'" << endl;
            return 1;
        }
        mappers_.push_back(map);
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

        auto getter = make_string_getter<StringFinderGB2312>(inputBegin, inputEnd);
        for (auto f : filters_)
            getter.add_filter(f);
        for (auto m : mappers_)
            getter.add_mapper(m);

        for(auto& ss: getter) {
            cout << "0x" << std::hex << ss.first << ": " << ss.second << endl;
        }
    }

    return status;
}

