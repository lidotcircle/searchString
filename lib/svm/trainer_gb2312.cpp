#include "svm/trainer_gb2312.hpp"
#include <stdexcept>
using namespace std;


void train_gb2312_sentence_svm(size_t ngram,
                               const std::string& positive_example_dir, 
                               const std::string& negative_example_dir,
                               const std::string& output_model)
{
    if (ngram > 3)
        throw runtime_error("ngram must be <= 3");

    switch (ngram) {
        case 1:
        {
            GB2312SentenceSVMTrainer<1> trainer(positive_example_dir, negative_example_dir, output_model);
            trainer.run_trainer();
        } break;
        case 2:
        {
            GB2312SentenceSVMTrainer<2> trainer(positive_example_dir, negative_example_dir, output_model);
            trainer.run_trainer();
        } break;
        case 3:
        {
            GB2312SentenceSVMTrainer<3> trainer(positive_example_dir, negative_example_dir, output_model);
            trainer.run_trainer();
        } break;
    }
}

