#ifndef _STRING_SFILTER_GENERIC_NGRAM_SVM_H_
#define _STRING_SFILTER_GENERIC_NGRAM_SVM_H_

#include "ngram_svm_filter.hpp"
#include "utils.h"
#include "../sfinder/unicode_converter.h"


template<size_t N,
    template<typename>typename TTrainer,
    template<typename>typename TKernel>
class GenericNGramSVMFilter : public NGramSVMFilter<N,int,TTrainer,TKernel> {
    private:
        std::string encoding;

    public:
        GenericNGramSVMFilter(const std::string& model, const std::string& encoding): NGramSVMFilter<N,int,TTrainer,TKernel>(model), encoding(encoding) {}

        int filter(const std::string& str) const override {
            vector<int> words;
            if (this->encoding == "gb2312") {
                words = gb2312str_to_unicode(str);
            } else if (this->encoding == "utf8" || this->encoding == "ascii") {
                size_t n;
                words = decoder_utf8(str, n);
            } else if (this->encoding == "utf16") {
                size_t n;
                words = decoder_utf16(str, n);
            }
            return this->svm.predict(words.begin(), words.end()) ? 0 : -1;
        }

        static const std::vector<int> register_handles;
};

#endif // _STRING_SFILTER_GENERIC_NGRAM_SVM_H_
