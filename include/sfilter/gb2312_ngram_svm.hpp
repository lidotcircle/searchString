#ifndef _STRING_SFILTER_GB2312_NGRAM_SVM_H_
#define _STRING_SFILTER_GB2312_NGRAM_SVM_H_

#include "ngram_svm_filter.hpp"
#include "utils.h"


template<size_t N,
    template<typename>typename TTrainer,
    template<typename>typename TKernel>
class GB2312NGramSVMFilter : public NGramSVMFilter<N,uint16_t,TTrainer,TKernel> {
    public:
        GB2312NGramSVMFilter(const std::string& model): NGramSVMFilter<N,uint16_t,TTrainer,TKernel>(model) {}

        int filter(const std::string& str) const override {
            auto vx = gb2312str2twobytes(str);
            return this->svm.predict(vx.begin(), vx.end()) ? 0 : -1;
        }

        static const std::vector<int> register_handles;
};

#endif // _STRING_SFILTER_GB2312_NGRAM_SVM_H_
