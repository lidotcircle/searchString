#include "main.h"
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
    string list;

    string train_output;
    vector<string> train_dirs;
    bool print_prefix = false;

    string model;
    vector<string> input_files;
    vector<string> filters;
    vector<string> mappers;
    vector<string> reducers;
    vector<string> transforms;

    cxxopts::Options options("StringFinder",
            "extract string from a binary file, "
            "support various encoding schema eg. ascii, gb2312");

    options.add_options("extract", {
        { "f,filter",    "string filters, applied in specified order", cxxopts::value<vector<string>>(filters),  "<filters>" },
        { "m,mapper",    "string mappers, applied in specified order", cxxopts::value<vector<string>>(mappers),  "<mappers>" },
        { "r,reducer",   "string mappers, applied in specified order", cxxopts::value<vector<string>>(reducers), "<reducers>" },
        { "t,transform", "transformations include filter, mapper, reducer. "
                         "prefixed by 'f','m','r' respectively. "
                         "this option is exclusive with -fmr", cxxopts::value<vector<string>>(transforms), "<transform>" },
        { "n,num",       "print offset",                               cxxopts::value<bool>(print_prefix) },
        { "i,input",     "input file", cxxopts::value<vector<string>>(input_files) },
    });
    options.parse_positional("input");
    options.positional_help("<files>");

    options.add_options()
        ("e,encoding", "support ascii and gb2312", cxxopts::value<string>(encoding)->default_value("gb2312"), "<encoding>")
        ("list",       "list filters, mappers and reducers", cxxopts::value<string>(list), "[encoding | all]")
        ("h,help",     "print help");

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

    if (result.count("filter") || result.count("mapper") || result.count("reducer")) {
        if (result.count("transform")) {
            cerr << "transform and filters, mappers and reducers are exclusive" << endl;
            cerr << options.help() << endl;
            return 1;
        }
    }

    if (transforms.empty()) {
        for (auto& f: filters)
            transforms.push_back("f" + f);
        for (auto& m: mappers)
            transforms.push_back("m" + m);
        for (auto& r: reducers)
            transforms.push_back("r" + r);
    }

    if (result.count("list")) {
        print_transforms(list);
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

    int status = 0;
    for (auto& filename: input_files) {
        std::ifstream inputFile(filename, std::ios::binary);

        if(!inputFile.is_open()) {
            cout << "fail to open file" << endl;
            status = 1;
            continue;
        }
        cout << "Search " << filename << ":" << endl;
        std::istreambuf_iterator<char> inputBegin(inputFile), inputEnd;

        try
        {
            auto getter = GetterFactory::create_by_exprs(encoding, transforms, inputBegin, inputEnd);
            for (auto &ss : *getter.get())
            {
                if (print_prefix)
                    cout << "0x" << std::hex << ss.first << ": ";

                cout << ss.second << endl;
            }
        }
        catch (std::runtime_error &e)
        {
            cout << e.what() << endl;
            status = 1;
        }
    }

    return status;
}

