#include "sfilter/gb2312_svm.h"
#include "utils.h"
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

bool GB2312SVMFilter::load(char* buf, size_t bufsize, size_t& read) {
    return this->svm.load(buf, bufsize, read);
}

bool GB2312SVMFilter::save(char* buf, size_t bufsize, size_t& writed) {
    return this->svm.save(buf, bufsize, writed);
}

bool GB2312SVMFilter::feed_file(const std::string& filename) {
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

bool GB2312SVMFilter::feed_files(const std::vector<std::string>& filenames) {
    bool result = true;
    for(auto& f: filenames) {
        if(!this->feed_file(f))
            result = false;
    }

    return result;
}

bool GB2312SVMFilter::feed_directories_recursively(const std::vector<std::string>& dirs) {
    bool result = true;
    for(auto& f: dirs) {
        if(!this->feed_directories_recursively(f))
            result = false;
    }

    return result;
}

bool GB2312SVMFilter::feed_directories_recursively(const std::string& dir) {
    bool result = true;
    for(auto& f: ls_files_recursively(dir)) {
        if(!this->feed_file(f))
            result = false;
    }

    return result;
}

void GB2312SVMFilter::feed(const std::string& gb2312sentence) {
    for(auto& s: str2u16(gb2312sentence, false))
        this->feed(s);
}

void GB2312SVMFilter::feed(const std::vector<uint16_t>& sentence) {
    this->svm.feed_sentence(sentence.begin(), sentence.end());
}

void GB2312SVMFilter::add_samples(const std::string& f, std::vector<sample_type>& samples)
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
                assert(nx.size() > 0);
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

void GB2312SVMFilter::svm_train(const std::vector<std::string>& valid_sentence, const std::vector<std::string>& invalid_sentence) {
    std::vector<sample_type> samples;
    std::vector<double> labels;
    for(auto& s: valid_sentence) {
        auto nx = str2u16(s, true);
        if (nx.size() > 0) {
            samples.push_back(this->svm.feature2(nx[0].begin(), nx[0].end()));
            labels.push_back(1);
        }
    }
    for(auto& s: invalid_sentence) {
        auto nx = str2u16(s, true);
        if (nx.size() > 0) {
            samples.push_back(this->svm.feature2(nx[0].begin(), nx[0].end()));
            labels.push_back(-1);
        }
    }

    this->svm.train_with_features(samples, labels);
}

void GB2312SVMFilter::svm_train_files(const std::vector<std::string>& valid, const std::vector<std::string>& invalid) {
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

void GB2312SVMFilter::svm_train_directories_recursively(const std::vector<std::string>& valid, const std::vector<std::string>& invalid) {
    std::vector<std::string> _valid;
    std::vector<std::string> _invalid;

    for(auto& f: valid) {
        for(auto& _f: ls_files_recursively(f))
            _valid.push_back(_f);
    }

    for(auto& f: invalid) {
        for(auto& _f: ls_files_recursively(f))
            _invalid.push_back(_f);
    }

    this->svm_train_files(_valid, _invalid);
}

void GB2312SVMFilter::svm_train_directories_recursively(const std::string& valid, const std::string& invalid) {
    this->svm_train_files(
            ls_files_recursively(valid), 
            ls_files_recursively(invalid)
            );
}

int GB2312SVMFilter::filter(const std::string& str) const {
    auto nu = str2u16(str, true);
    if (nu.empty())
        return -1;

    auto& nunu = nu[0];
    auto _this = const_cast<GB2312SVMFilter*>(this);
    auto n = _this->svm.predict(nunu.begin(), nunu.end());
    return n > 0 ? 1 : -1;
}

GB2312SVMFilter::~GB2312SVMFilter() {
}

std::ostream& operator<<(std::ostream& o, const GB2312SVMFilter& filter) {
    o << filter.svm;
    return o;
}

std::istream& operator>>(std::istream& i, GB2312SVMFilter& filter) {
    i >> filter.svm;
    return i;
}

