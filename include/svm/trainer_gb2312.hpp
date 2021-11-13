#ifndef _SENTENCE_SVM_TRAINER_GB2312_HPP_
#define _SENTENCE_SVM_TRAINER_GB2312_HPP_

#include "trainer.hpp"
#include "utils.h"


template<size_t N>
class GB2312SentenceSVMTrainer: public SentenceSVMTrainer<N,uint16_t> {
protected:
    std::vector<uint16_t> str2words(const std::string& str) const override
    {
        return gb2312str2twobytes(str);
    }

public:
    GB2312SentenceSVMTrainer(
            const std::string& positive_example_dir,
            const std::string& negative_example_dir,
            const std::string& output_model):
        SentenceSVMTrainer<N,uint16_t>(positive_example_dir,negative_example_dir,output_model) {}

};

void train_gb2312_sentence_svm(size_t ngram,
                               const std::string& positive_example_dir, 
                               const std::string& negative_example_dir,
                               const std::string& output_model);

#endif // _SENTENCE_SVM_TRAINER_GB2312_HPP_
