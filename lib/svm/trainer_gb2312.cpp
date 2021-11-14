#include "svm/trainer_gb2312.hpp"
#include <stdexcept>
#include "svm/supported_trainer_kernel.h"
using namespace std;


void train_gb2312_sentence_svm(size_t ngram,
                               const string& trainer,
                               const string& kernel,
                               const string& positive_example_dir, 
                               const string& negative_example_dir,
                               const string& output_model)
{
    if (ngram > 3)
        throw runtime_error("ngram must be <= 3");

#define NENTRY(ngram_l, ngram_, trainer_l, trainer_, kernel_l, kernel_) \
    if (ngram_l == ngram && trainer_l == trainer && kernel_l == kernel) { \
        GB2312SentenceSVMTrainer<ngram_,trainer_,kernel_> \
            trainer__(positive_example_dir, negative_example_dir, output_model); \
        trainer__.run_trainer(); \
        return; \
    }
    kernel_map();
#undef NENTRY

    throw runtime_error("invalid parameters");
}


#undef ngram_map
static const vector<string> trainers = {
#define ngram_map(trainer_l, trainer, kernel_l, kernel_) \
    trainer_l,
    trainer_map(a, b)
#undef ngram_map
};

#undef trainer_map
static const vector<string> kernels = {
#define trainer_map(kernel_l, kernel_) \
    kernel_l,
kernel_map()
#undef trainer_map
};

const std::vector<std::string>& trainer_list() {
    return trainers;
}
const std::vector<std::string>& kernel_list() {
    return kernels;
}

