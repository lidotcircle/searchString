#include "sfilter/generic_ngram_svm.hpp"
#include "sfilter/filter_factory.h"
#include "svm/supported_trainer_kernel.h"
#include "utils.h"
#include <assert.h>
#include <ctype.h>
#include <stdexcept>
#include <tuple>
using namespace std;


static auto parse_param(const std::string& param) {
    string file, trainer = "c_ekm", kernel = "rbf";
    int ngram = 2;
    auto vmm = split_string(param, ":");
    assert(vmm.size() > 0);

    file = vmm[0];
    if (vmm.size() > 1) {
        auto ngram_str = vmm[1];
        if (ngram_str.empty() || !isdigit(ngram_str[0]))
            throw std::runtime_error("please specify a correct ngram value");

        size_t pos = 0;
        ngram = std::stoi(ngram_str, &pos);
        if (pos != ngram_str.size() || ngram <= 0)
            throw std::runtime_error("please specify a correct ngram value");
    }
    if (vmm.size() > 2) {
        trainer = vmm[2];
    }
    if (vmm.size() > 3) {
        kernel = vmm[3];
    }

    if (vmm.size() > 4) {
        throw std::runtime_error("too many parameters for svm specification");
    }

    return make_tuple(file, ngram, trainer, kernel);
}

static auto name = "svm";
static auto desc = "model[:ngram=2[:trainer=c_ekm[:kernel=rbf]]] svm classifier, ngram between 1 to 3";
template<const char* encoding>
static auto svm_creator = [](const string& param) {
    string file, trainer, kernel;
    int ngram;
    std::tie(file, ngram, trainer, kernel) = parse_param(param);

#define NENTRY(ngram_l, ngram_, trainer_l, trainer_, kernel_l, kernel_) \
    if (ngram_l == ngram && trainer_l == trainer && kernel_l == kernel) { \
        return std::shared_ptr<StringFilter>(\
                new GenericNGramSVMFilter<ngram_,trainer_,kernel_>(file, encoding)); \
    }
    kernel_map();
#undef NENTRY

    throw runtime_error("invalid parameters for svm");
};

static const char encoding_gb2312[] = "gb2312";
static const char encoding_ascii[]  = "ascii";
static const char encoding_utf8[]   = "utf8";
static const char encoding_utf16[]  = "utf16";
static const auto fn_gb2312 = svm_creator<encoding_gb2312>;
static const auto fn_utf8   = svm_creator<encoding_utf8>;
static const auto fn_ascii  = svm_creator<encoding_ascii>;
static const auto fn_utf16  = svm_creator<encoding_utf16>;
template<>
const vector<int> GenericNGramSVMFilter<1,dlib::svm_c_trainer,dlib::radial_basis_kernel>::register_handles = {
    FilterFactory::register_filter("gb2312", name, desc, fn_gb2312),
    FilterFactory::register_filter("ascii",  name, desc, fn_ascii),
    FilterFactory::register_filter("utf8",   name, desc, fn_utf8),
    FilterFactory::register_filter("utf16",  name, desc, fn_utf16),
};
