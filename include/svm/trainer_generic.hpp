#ifndef _SENTENCE_SVM_TRAINER_GENERIC_HPP_
#define _SENTENCE_SVM_TRAINER_GENERIC_HPP_

#include "trainer.hpp"
#include "utils.h"
#include "../sfinder/unicode_converter.h"
#include <vector>
#include <string>


template<size_t N,
    template<typename>typename TTrainer,
    template<typename>typename TKernel>
class GenericSentenceSVMTrainer: public SentenceSVMTrainer<N,int,TTrainer,TKernel> {
private:
    std::string encoding;

protected:
    std::vector<int> str2words(const std::string& str) const override
    {
        if (encoding == "gb2312") {
            return gb2312str_to_unicode(str);
        } else if (encoding == "utf8" || encoding == "ascii") {
            size_t n;
            return decoder_utf8(str, n);
        } else if (encoding == "utf16") {
            size_t n;
            return decoder_utf16(str, n);
        } else {
            throw std::runtime_error("unsupported encoding schema");
        }
    }

public:
    GenericSentenceSVMTrainer(
            const std::string& encoding,
            const std::string& positive_example_dir,
            const std::string& negative_example_dir,
            const std::string& output_model):
        SentenceSVMTrainer<N,int,TTrainer,TKernel>(positive_example_dir,negative_example_dir,output_model),
        encoding(encoding) {}

};

void train_sentence_svm(size_t ngram,
                        const std::string& encoding,
                        const std::string& trainer,
                        const std::string& kernel,
                        const std::string& positive_example_dir, 
                        const std::string& negative_example_dir,
                        const std::string& output_model);

const std::vector<std::string>& trainer_list();
const std::vector<std::string>& kernel_list();

#endif // _SENTENCE_SVM_TRAINER_GENERIC_HPP_