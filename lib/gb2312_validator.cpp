#include "gb2312_validator.h"
#include <iostream>


bool GB2312Validator::load(unsigned char* buf, size_t bufsize, size_t& read) {
    return this->svm.load(buf, bufsize, read);
}

bool GB2312Validator::save(unsigned char* buf, size_t bufsize, size_t& writed) {
    return this->svm.save(buf, bufsize, writed);
}

void feed_file(const std::string& filename) {
}


void GB2312Validator::feed(const std::string& gb2312sentence) {
}
void GB2312Validator::feed(const std::vector<uint16_t>& sentence) {
}

void GB2312Validator::svm_train_files(const std::vector<std::string>& valid, const std::vector<std::string>& invalid) {
}
void GB2312Validator::svm_train(const std::vector<std::pair<std::vector<uint16_t>,bool>>& samples) {
}

bool GB2312Validator::validate(const std::string& str) {
    return true;
}

GB2312Validator::~GB2312Validator() {
}

