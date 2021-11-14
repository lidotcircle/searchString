#ifndef _STRING_FILTER_NGRAM_SVM_FILTER_HPP
#define _STRING_FILTER_NGRAM_SVM_FILTER_HPP

#include "string_filter.h"
#include "svm/sentence_svm_ngram.hpp"
#include <fstream>
#include <stdexcept>

template<size_t N, typename T,
    template<typename>typename TTrainer,
    template<typename>typename TKernel>
class NGramSVMFilter: public StringFilter {
    protected:
        NGramSentenceSVM<N, T, TTrainer, TKernel> svm;

    public:
        NGramSVMFilter(const std::string& model) {
            std::ifstream ifs(model, std::ios::binary);
            if (!ifs)
                 throw std::runtime_error("NGramSVMFilter: can't open model file: " + model);

            ifs >> this->svm;
        }

        ~NGramSVMFilter() = default;
};

#endif // _STRING_FILTER_NGRAM_SVM_FILTER_HPP
