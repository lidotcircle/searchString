#ifndef _GB2312_VALIDATOR_H_
#define _GB2312_VALIDATOR_H_

#include <vector>
#include <sentence_svm_2gram.hpp>
#include "string_validator.h"

class GB2312Validator: public StringValidator {
    private:
        using sample_type = SentenceSVM<uint16_t>::sample_type;
        SentenceSVM<uint16_t> svm;

        void add_samples(const std::string& file, std::vector<sample_type>& samples);


    public:
        GB2312Validator() = default;

        bool save(unsigned char* buf, size_t bufsize, size_t& write);
        bool load(unsigned char* buf, size_t bufsize, size_t& read);

        bool feed_file(const std::string& filename);
        void feed(const std::string& gb2312sentence);
        void feed(const std::vector<uint16_t>& sentence);

        void svm_train_files(const std::vector<std::string>& valid, const std::vector<std::string>& invalid);

        virtual bool validate(const std::string& str) override;
        virtual ~GB2312Validator() override;
};

#endif // _GB2312_VALIDATOR_H_

