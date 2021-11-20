#include "sfinder/string_finder_utf16.h"
#include <assert.h>
using namespace std;

#define IS_CONTROL_CHAR(c) (0x00 <= (c) && (c) <= 0x1F && (c) != 0x09 && (c) != 0x0A && (c) != 0x0D)


StringFinderUTF16::StringFinderUTF16(bool little_endian): little_endian(little_endian), pos(0), state(0) {}

void StringFinderUTF16::clear_to_outputs() {
    this->state = 0;
    if (candidate.size() > 0) {
        this->outputs.push_back(make_pair(this->pos - candidate.size(),candidate));
        this->candidate.clear();
    }
    this->pre_chars.clear();
}

void StringFinderUTF16::feed_char(unsigned char c) {
    this->pos++;
    if (this->pre_chars.size() % 2 == 0) {
        this->pre_chars.push_back(c);
        return;
    }

    unsigned char c1 = this->pre_chars.back();
    unsigned char c2 = c;
    if (this->little_endian)
        std::swap(c1,c2);

    uint16_t val = (c1 << 8) | c2;
    switch (this->state) {
        case 0:
        {
        } break;
        case 1:
        {
        } break;
    }
}

void StringFinderUTF16::feed_end() {
    this->pos++;
    this->clear_to_outputs();
}

std::vector<std::pair<size_t,std::string>>& StringFinderUTF16::pre_fetch() {
    return this->outputs;
}

