#ifndef _STRING_SFILTER_GB2312_SVM_FILTER_H_
#define _STRING_SFILTER_GB2312_SVM_FILTER_H_

#include <vector>
#include <iostream>
#include "../svm/sentence_svm_2gram.hpp"
#include "string_filter.h"


class GB2312SVMFilter;
std::ostream& operator<<(std::ostream& o, const GB2312SVMFilter& filter);
std::istream& operator>>(std::istream& i, GB2312SVMFilter& filter);

class GB2312SVMFilter: public StringFilter {
    private:
        using sample_type = SentenceSVM<uint16_t>::sample_type;
        SentenceSVM<uint16_t> svm;

        friend 
            std::ostream& operator<<(std::ostream& o, const GB2312SVMFilter& filter);
        friend 
            std::istream& operator>>(std::istream& i, GB2312SVMFilter& filter);

        void add_samples(const std::string& file, std::vector<sample_type>& samples);


    public:
        GB2312SVMFilter() = default;

        bool save(char* buf, size_t bufsize, size_t& write);
        bool load(char* buf, size_t bufsize, size_t& read);

        bool feed_file(const std::string& filenames);
        bool feed_files(const std::vector<std::string>& filenames);
        bool feed_directories_recursively(const std::vector<std::string>& filename);
        bool feed_directories_recursively(const std::string& filename);
        void feed(const std::string& gb2312sentence);
        void feed(const std::vector<uint16_t>& sentence);

        void svm_train(const std::vector<std::string>& valid_sentence, const std::vector<std::string>& invalid_sentence);
        void svm_train_files(const std::vector<std::string>& valid, const std::vector<std::string>& invalid);
        void svm_train_directories_recursively(const std::string& valid, const std::string& invalid);
        void svm_train_directories_recursively(const std::vector<std::string>& valid, const std::vector<std::string>& invalid);

        virtual int filter(const std::string& str) const override;
        virtual ~GB2312SVMFilter() override;

        static const std::vector<int> register_handles;
};

#endif // _STRING_SFILTER_GB2312_SVM_FILTER_H_
