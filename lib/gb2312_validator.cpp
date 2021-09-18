#include "gb2312_validator.h"
#include <iostream>
#include <fstream>
#include <exception>


static std::vector<std::vector<uint16_t>> str2u16(const std::string& str, bool include_crlf)
{
    std::vector<std::vector<uint16_t>> ans;

    std::vector<uint16_t> sentence;
    unsigned char p = 0;
    for(auto c: str) {
        if(!include_crlf && (c == '\r' || c == '\n')) {
            if(!sentence.empty())
                ans.push_back(sentence);
            sentence.clear();
        } else if (p != 0) {
            // expect a valid gb2312 string
            sentence.push_back((p << 8) + c);
            p = 0;
        } else if ((c & 0x80) == 0) {
            sentence.push_back(c);
        } else {
            p = c;
        }
    }

    if(!sentence.empty())
        ans.push_back(sentence);

    return ans;
}

bool GB2312Validator::load(unsigned char* buf, size_t bufsize, size_t& read) {
    return this->svm.load(buf, bufsize, read);
}

bool GB2312Validator::save(unsigned char* buf, size_t bufsize, size_t& writed) {
    return this->svm.save(buf, bufsize, writed);
}

bool GB2312Validator::feed_file(const std::string& filename) {
    std::ifstream ifile(filename);
    if(!ifile.is_open())
        return false;

    std::string sentence;
    std::istreambuf_iterator<char> inputBegin(ifile), inputEnd;
    for(;inputBegin != inputEnd;inputBegin++) {
        unsigned char c = *inputBegin;

        if(c == '\r' || c == '\n') {
            if(!sentence.empty())
                this->feed(sentence);
            sentence.clear();
        } else {
            sentence.push_back(c);
        }
    }

    if(!sentence.empty())
        this->feed(sentence);
    return true;
}

void GB2312Validator::feed(const std::string& gb2312sentence) {
    for(auto& s: str2u16(gb2312sentence, false))
        this->feed(s);
}

void GB2312Validator::feed(const std::vector<uint16_t>& sentence) {
    this->svm.feed_sentence(sentence.begin(), sentence.end());
}

void GB2312Validator::add_samples(const std::string& f, std::vector<sample_type>& samples)
{
    std::ifstream ifile(f);
    if(!ifile.is_open()) {
        std::cout << "fail to open file " << f << std::endl;
        return;
    }

    std::string sentence;
    std::istreambuf_iterator<char> inputBegin(ifile), inputEnd;
    for(;inputBegin != inputEnd;inputBegin++) {
        unsigned char c = *inputBegin;

        if(c == '\r' || c == '\n' || c == '\0') {
            if(!sentence.empty()) {
                auto nx = str2u16(sentence, true);
                samples.push_back(this->svm.feature2(nx[0].begin(), nx[0].end()));
            }
            sentence.clear();
        } else {
            sentence.push_back(c);
        }
    }

    if(!sentence.empty()) {
        auto nx = str2u16(sentence, true);
        samples.push_back(this->svm.feature2(nx[0].begin(), nx[0].end()));
    }
}

void GB2312Validator::svm_train_files(const std::vector<std::string>& valid, const std::vector<std::string>& invalid) {
    std::vector<sample_type> samples;
    std::vector<double> labels;
    for(auto& f: valid)
        this->add_samples(f, samples);
    labels = std::vector<double>(samples.size(), 1);

    for(auto& f: invalid)
        this->add_samples(f, samples);
    while(labels.size() < samples.size())
        labels.push_back(-1);

    this->svm.train_with_features(samples, labels);
}

bool GB2312Validator::validate(const std::string& str) {
    return true;
    auto nu = str2u16(str, true);
    if (nu.empty())
        return false;

    auto& nunu = nu[0];
    return this->svm.predict(nunu.begin(), nunu.end()) == -1;
}

GB2312Validator::~GB2312Validator() {
}

