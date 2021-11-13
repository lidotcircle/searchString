#include "main.h"
#include "search_string.h"
#include "svm/trainer_gb2312.hpp"
#include <stdexcept>
using namespace std;


int trainsvm_cmd(int argc, char **argv)
{
    string encoding;
    string model;
    string positive_example_dir;
    string negative_example_dir;
    size_t ngram;
    cxxopts::Options options("strfd trainsvm",
            "train a svm model for sentence validation");

    options.add_options()
        ("e,encoding", "support ascii and gb2312", cxxopts::value<string>(encoding)->default_value("gb2312"), "<encoding>")
        ("ngram",      "sentence feature extracted by ngram frequency", cxxopts::value<size_t>(ngram)->default_value("2"), "<ngram>")
        ("p,positive", "positive example directory", cxxopts::value<string>(positive_example_dir), "<positive>")
        ("n,negative", "negative example directory", cxxopts::value<string>(negative_example_dir), "<negative>")
        ("o,output",   "output model", cxxopts::value<string>(model)->default_value("model.dat"), "<output>")
        ("h,help",     "print help");

    cxxopts::ParseResult result;
    try {
        result = options.parse(argc, argv);
    } catch (std::exception e) {
        cerr << "bad arguments" << endl;
        cerr << options.help() << endl;
        return 1;
    }

    if (result.count("help")) {
        cout << options.help() << endl;
        return 0;
    }

    if (positive_example_dir.empty() || negative_example_dir.empty()) {
        cerr << "bad arguments" << endl;
        cerr << options.help() << endl;
        return 1;
    }

    cout << "training info:" << endl;
    cout << "  encoding:          " << encoding << endl;
    cout << "  output model:      " << model << endl;
    cout << "  positive exmpales: " << positive_example_dir << endl;
    cout << "  negative exmpales: " << negative_example_dir << endl;

    try {
        train_gb2312_sentence_svm(ngram, positive_example_dir, negative_example_dir, model);
    } catch (std::runtime_error& e) {
        cerr << e.what() << endl;
        return 1;
    }

    return 0;
}
