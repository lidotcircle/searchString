#ifndef _SENTENCE_SVM_TRAINER_HPP_
#define _SENTENCE_SVM_TRAINER_HPP_

#include "sentence_svm_ngram.hpp"
#include "utils.h"
#include <dlib/svm.h>
#include <fstream>

template<size_t N,typename TWord,
    template<typename>typename TTrainer,
    template<typename>typename TKernel>
class SentenceSVMTrainer
{
private:
    NGramSentenceSVM<N, TWord,TTrainer,TKernel> model;
    std::string positive_example_dir;
    std::string negative_example_dir;
    std::string output_model;
    using sample_type = typename NGramSentenceSVM<N, TWord, TTrainer, TKernel>::sample_type;

    void feed_positive_sentence(const std::string& file) {
        std::ifstream ifs(file, std::ios::binary);
        std::string line;

        while (safe_getline(ifs, line)) {
            std::vector<TWord> words = this->str2words(line);
            this->model.word_counter_feed(words.begin(), words.end());
        }
    }

    void add_to_sample(const std::string& file, std::vector<sample_type>& samples) {
        std::ifstream ifs(file, std::ios::binary);
        std::string line;

        while (safe_getline(ifs, line)) {
            std::vector<TWord> words = this->str2words(line);
            auto sample = this->model.feature(words.begin(), words.end());
            samples.push_back(sample);
        }
    }

protected:
    virtual std::vector<TWord> str2words(const std::string& str) const = 0;

public:
    SentenceSVMTrainer(const std::string& positive_example_dir,
                       const std::string& negative_example_dir,
                       const std::string& output_model):
        output_model(output_model),
        positive_example_dir(positive_example_dir),
        negative_example_dir(negative_example_dir)
    {
    }

    void run_trainer()
    {
        auto f1 = ls_files_recursively(positive_example_dir);
        auto f2 = ls_files_recursively(negative_example_dir);

        std::fstream out(output_model, std::ios::out | std::ios::binary);
        if (!out)
            throw std::runtime_error("cannot open output model file: " + output_model);

        for (auto& f : f1)
            this->feed_positive_sentence(f);

        std::vector<sample_type> samples;
        std::vector<double> labels;

        for (auto& f : f1)
            this->add_to_sample(f, samples);
        labels = std::vector<double>(samples.size(), 1.0);

        for (auto& f : f2)
            this->add_to_sample(f, samples);
        labels.insert(labels.end(), samples.size() - labels.size(), -1.0);

        this->model.train(samples, labels);
        out << this->model;
    }
};

#endif // _SENTENCE_SVM_TRAINER_HPP_
