#include "sfinder/string_finder_ascii.h"
#include <assert.h>

#define _IS_V_ASCII_CRLF(c)  ((c >= 0x20 && c <  0x7f) \
        || c == 0x09 || c == 0x0A || c == 0x0D)
#define is_ascii _IS_V_ASCII_CRLF


StringFinderASCII::StringFinderASCII(): pos(0), candidate_begin(0), is_end(false) {}

void StringFinderASCII::feed_char(unsigned char c) {
    if (is_ascii(c)) {
        if (this->candidate.empty())
            this->candidate_begin = this->pos;
        this->candidate.push_back(c);
    } else {
        if (!this->candidate.empty())
        {
            this->outputs.push_back(make_pair(
                        this->candidate_begin,
                        std::move(this->candidate)));
        }
    }
    pos++;
}

void StringFinderASCII::feed_end() {
    assert(!this->is_end);
    this->is_end = true;

    if (!this->candidate.empty()) {
        this->outputs.push_back(make_pair(
                    this->candidate_begin,
                    std::move(this->candidate)));
    }
}

std::vector<std::pair<size_t,std::string>>& StringFinderASCII::pre_fetch() {
    return this->outputs;
}

