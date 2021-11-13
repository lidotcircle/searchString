#include "sfilter/gb2312_ngram_svm.hpp"
#include "sfilter/filter_factory.h"
using namespace std;


static auto name = "svm";
static auto desc = "ngram:model svm classifier, ngram between 1 to 3";
static auto creator = [](const string& param) {
    string ngram, file;
    auto pos = param.find(':');
    if (pos != std::string::npos) {
        ngram = param.substr(0, pos);
        file = param.substr(pos + 1);
        if (ngram != "1" && ngram != "2" && ngram != "3") 
            throw std::runtime_error("ngram should be from 1 to 3");
    }

    switch (ngram[0]) {
        case '1':
            return std::shared_ptr<StringFilter>(new GB2312NGramSVMFilter<1>(file));
        case '2':
            return std::shared_ptr<StringFilter>(new GB2312NGramSVMFilter<2>(file));
        case '3':
            return std::shared_ptr<StringFilter>(new GB2312NGramSVMFilter<3>(file));
        default:
            throw std::runtime_error("impossible");
    }
};

template<>
const vector<int> GB2312NGramSVMFilter<1>::register_handles = {
    FilterFactory::register_filter("gb2312", name, desc, creator),
};
