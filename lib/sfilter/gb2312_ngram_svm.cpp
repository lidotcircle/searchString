#include "sfilter/gb2312_ngram_svm.hpp"
#include "sfilter/filter_factory.h"
#include "svm/supported_trainer_kernel.h"
#include "utils.h"
#include <assert.h>
using namespace std;


static auto name = "svm";
static auto desc = "model[:ngram[:trainer[:kernel]]] svm classifier, ngram between 1 to 3";
static auto creator = [](const string& param) {
    string file, trainer, kernel;
    int ngram = 2;
    auto vmm = split_string(param, ":");
    assert(vmm.size() > 0);

    file = vmm[0];
    if (vmm.size() > 1) {
        auto ngram_str = vmm[1];

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

#define NENTRY(ngram_l, ngram_, trainer_l, trainer_, kernel_l, kernel_) \
    if (ngram_l == ngram && trainer_l == trainer && kernel_l == kernel) { \
        return std::shared_ptr<StringFilter>(\
                new GB2312NGramSVMFilter<ngram_,trainer_,kernel_>(file)); \
    }
    kernel_map();
#undef NENTRY

    throw runtime_error("invalid parameters for svm");
};

template<>
const vector<int> GB2312NGramSVMFilter<1,dlib::svm_c_trainer,dlib::radial_basis_kernel>::register_handles = {
    FilterFactory::register_filter("gb2312", name, desc, creator),
};
