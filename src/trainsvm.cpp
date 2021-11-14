#include "main.h"
#include "search_string.h"
#include "svm/trainer_gb2312.hpp"
#include <stdexcept>
using namespace std;


void print_supported_trainers_and_kernels() {
    cout << "Supported trainers:" << endl;
    for (auto& t : trainer_list())
        cout << "    " << t << endl;
    cout << endl;

    cout << "Supported kernels:" << endl;
    for (auto& k : kernel_list())
        cout << "    " << k << endl;
}

int trainsvm_cmd(int argc, char **argv)
{
    string encoding;
    string model;
    string positive_example_dir;
    string negative_example_dir;
    string trainer;
    string kernel;
    size_t ngram;
    cxxopts::Options options("strfd trainsvm",
            "train a svm model for sentence validation");

    options.add_options()
        ("e,encoding", "support ascii and gb2312", cxxopts::value<string>(encoding)->default_value("gb2312"), "<encoding>")
        ("ngram",      "sentence feature extracted by ngram frequency", cxxopts::value<size_t>(ngram)->default_value("2"), "<ngram>")
        ("trainer",    "svm trainer, see dlib for details", cxxopts::value<string>(trainer)->default_value("c_ekm"), "<trainer>")
        ("kernel",     "svm kernel type, see dlib for details", cxxopts::value<string>(kernel)->default_value("radial_basis"), "<kernel>")
        ("info",       "list support tariners and kernels")
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

    if (result.count("info")) {
        print_supported_trainers_and_kernels();
        return 0;
    }

    if (positive_example_dir.empty() || negative_example_dir.empty()) {
        cerr << "bad arguments" << endl;
        cerr << options.help() << endl;
        return 1;
    }

    if (encoding != "gb2312") {
        cerr << "unsupport encoding" << endl;
        cerr << options.help() << endl;
        return 1;
    }

    cout << "training info:" << endl;
    cout << "  encoding:          " << encoding << endl;
    cout << "  ngram:             " << ngram << endl;
    cout << "  trainer:           " << trainer << endl;
    cout << "  trainer:           " << trainer << endl;
    cout << "  kernel:            " << kernel << endl;
    cout << "  output model:      " << model << endl;
    cout << "  positive exmpales: " << positive_example_dir << endl;
    cout << "  negative exmpales: " << negative_example_dir << endl;

    try {
        train_gb2312_sentence_svm(ngram, trainer, kernel, positive_example_dir, negative_example_dir, model);
        cout << "train finished" << endl;
    } catch (std::runtime_error& e) {
        cerr << e.what() << endl;
        return 1;
    }

    return 0;
}
