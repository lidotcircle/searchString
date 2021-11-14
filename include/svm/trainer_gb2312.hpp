#ifndef _SENTENCE_SVM_TRAINER_GB2312_HPP_
#define _SENTENCE_SVM_TRAINER_GB2312_HPP_

#include "trainer.hpp"
#include "utils.h"
#include <vector>
#include <string>


template<size_t N,
    template<typename>typename TTrainer,
    template<typename>typename TKernel>
class GB2312SentenceSVMTrainer: public SentenceSVMTrainer<N,uint16_t,TTrainer,TKernel> {
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
        SentenceSVMTrainer<N,uint16_t,TTrainer,TKernel>(positive_example_dir,negative_example_dir,output_model) {}

};

void train_gb2312_sentence_svm(size_t ngram,
                               const std::string& trainer,
                               const std::string& kernel,
                               const std::string& positive_example_dir, 
                               const std::string& negative_example_dir,
                               const std::string& output_model);

const std::vector<std::string>& trainer_list();
const std::vector<std::string>& kernel_list();

#endif // _SENTENCE_SVM_TRAINER_GB2312_HPP_
