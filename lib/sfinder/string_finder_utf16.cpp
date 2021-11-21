#include "sfinder/string_finder_utf16.h"
#include "sfinder/unicode_converter.h"
#include <assert.h>
using namespace std;

#define IS_CONTROL_CHAR(c) (0x00 <= (c) && (c) <= 0x1F && (c) != 0x09 && (c) != 0x0A && (c) != 0x0D)


StringFinderUTF16::StringFinderUTF16(bool little_endian): little_endian(little_endian), pos(0) {}

void StringFinderUTF16::clear_to_outputs() {
    if (s1.candidate.size() > 0) {
        outputs.push_back(make_pair(s1.start_pos, std::move(s1.candidate)));
    }
    s1.candidate.clear();
    s1.pre_chars.clear();
}

void StringFinderUTF16::clear_to_outputs2() {
    if (s2.candidate.size() > 0) {
        outputs.push_back(make_pair(s2.start_pos, std::move(s2.candidate)));
    }
    s2.candidate.clear();
    s2.pre_chars.clear();
}

void StringFinderUTF16::feed_char(unsigned char c) {
    this->pos++;

    if (this->s1.candidate.empty())
        this->s1.start_pos = this->pos - 1;
    this->s1.pre_chars.push_back(c);

    if (this->pos != 1) {
        if (this->s2.candidate.empty())
            this->s2.start_pos = this->pos - 1;
        this->s2.pre_chars.push_back(c);
    }

    if (s1.pre_chars.size() > 0 &&  s1.pre_chars.size() % 2 == 0) {
        size_t n;
        auto words = decoder_utf16(s1.pre_chars, n, this->little_endian);
        assert(words.size() <= 1);

        if (n == s1.pre_chars.size()) {
            assert(words.size() == 1);
            if (IS_CONTROL_CHAR(words[0]) || !is_desired_unicode_code_point(words[0])) {
                clear_to_outputs();
            } else {
                this->s1.candidate += s1.pre_chars;
                s1.pre_chars.clear();
            }
        } else if (s1.pre_chars.size() == 4) {
            clear_to_outputs();
        }
    }

    if (s2.pre_chars.size() > 0 &&  s2.pre_chars.size() % 2 == 0) {
        size_t n;
        auto words = decoder_utf16(s2.pre_chars, n, this->little_endian);
        assert(words.size() <= 1);

        if (n == s2.pre_chars.size()) {
            assert(words.size() == 1);
            if (IS_CONTROL_CHAR(words[0]) || !is_desired_unicode_code_point(words[0])) {
                clear_to_outputs2();
            } else {
                this->s2.candidate += s2.pre_chars;
                s2.pre_chars.clear();
            }
        } else if (s2.pre_chars.size() == 4) {
            clear_to_outputs2();
        }
    }

}

void StringFinderUTF16::feed_end() {
    this->clear_to_outputs();
    this->clear_to_outputs2();
}

std::vector<std::pair<size_t,std::string>>& StringFinderUTF16::pre_fetch() {
    return this->outputs;
}

