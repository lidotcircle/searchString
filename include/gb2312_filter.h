#ifndef _GB2312_FILTER_H_
#define _GB2312_FILTER_H_

#include <vector>
#include <sentence_svm_2gram.hpp>
#include <iostream>
#include "string_filter.h"

class GB2312Filter;
std::ostream& operator<<(std::ostream& o, const GB2312Filter& filter);
std::istream& operator>>(std::istream& i, GB2312Filter& filter);

class GB2312Filter: public StringFilter {
    private:
        using sample_type = SentenceSVM<uint16_t>::sample_type;
        SentenceSVM<uint16_t> svm;

        friend 
            std::ostream& operator<<(std::ostream& o, const GB2312Filter& filter);
        friend 
            std::istream& operator>>(std::istream& i, GB2312Filter& filter);

        void add_samples(const std::string& file, std::vector<sample_type>& samples);


    public:
        GB2312Filter() = default;

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

        virtual bool filter(const std::string& str) override;
        virtual ~GB2312Filter() override;
};

#endif // _GB2312_FILTER_H_
