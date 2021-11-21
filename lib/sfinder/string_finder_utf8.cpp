#include "sfinder/string_finder_utf8.h"
#include "sfinder/unicode_converter.h"
#include <assert.h>
using namespace std;

#define IS_CONTROL_CHAR(c) (0x00 <= (c) && (c) <= 0x1F && (c) != 0x09 && (c) != 0x0A && (c) != 0x0D)


StringFinderUTF8::StringFinderUTF8(): pos(0), state(0) {}

void StringFinderUTF8::clear_to_outputs() {
    this->state = 0;
    if (candidate.size() > 0) {
        this->outputs.push_back(make_pair(this->start_pos,candidate));
        this->candidate.clear();
    }
    this->pre_chars.clear();
}

void StringFinderUTF8::feed_char(unsigned char c) {
    this->pos++;
    if (IS_CONTROL_CHAR(c)) {
        this->clear_to_outputs();
        return;
    }

    switch (this->state) {
        case 0:
        {
            // U+0000 - U+007F
            if (c < 0x80) {
                this->candidate.push_back(c);
                this->state = 0;
            } else if ((c & 0x11100000) == 0x11000000) {
                this->pre_chars.push_back(c);
                this->state = 1;
            } else if ((c & 0x11110000) == 0x11100000) {
                this->pre_chars.push_back(c);
                this->state = 2;
            } else if ((c & 0x11111000) == 0x11110000) {
                this->pre_chars.push_back(c);
                this->state = 3;
            } else {
                this->clear_to_outputs();
            }

            if (this->candidate.empty())
                start_pos = pos - 1;
        } break;
        case 1:
        {
            // U+0080 - U+07FF
            assert(this->pre_chars.size() == 1);
            if ((c & 0x11000000) == 0x10000000) {
                int val = c & 0x00111111;;
                val |= (this->pre_chars[0] & 0x00011111) << 6;

                if (val < 0x80 || !is_desired_unicode_code_point(val)) {
                    this->clear_to_outputs();
                } else {
                    this->candidate.push_back(this->pre_chars[0]);
                    this->candidate.push_back(c);
                    this->pre_chars.clear();
                    this->state = 0;
                }
            } else {
                this->clear_to_outputs();
            }
        } break;
        case 2:
        {
            // U+0800 - U+FFFF
            assert(this->pre_chars.size() >= 1);
            if ((c & 0x11000000) == 0x10000000) {
                if (pre_chars.size() != 2) {
                    pre_chars.push_back(c);
                } else {
                    int val = c & 0x00111111;;
                    val |= (this->pre_chars[1] & 0x00111111) << 6;
                    val |= (this->pre_chars[0] & 0x00001111) << 12;
                    if (val < 0x800 || !is_desired_unicode_code_point(val)) {
                        this->clear_to_outputs();
                    } else {
                        this->candidate.push_back(this->pre_chars[0]);
                        this->candidate.push_back(this->pre_chars[1]);
                        this->candidate.push_back(c);
                        this->pre_chars.clear();
                        this->state = 0;
                    }
                    
                }
            } else {
                this->clear_to_outputs();
            }
        } break;
        case 3:
        {
            // U+10000 - U+10FFFF
            assert(this->pre_chars.size() >= 1);
            if ((c & 0x11000000) == 0x10000000) {
                if (pre_chars.size() != 3) {
                    pre_chars.push_back(c);
                } else {
                    int val = c & 0x00111111;;
                    val |= (this->pre_chars[2] & 0x00111111) << 6;
                    val |= (this->pre_chars[1] & 0x00111111) << 12;
                    val |= (this->pre_chars[0] & 0x00000111) << 18;

                    if (val < 0x10000 || val > 0x10FFFF || !is_desired_unicode_code_point(val)) {
                        this->clear_to_outputs();
                    } else {
                        this->candidate.push_back(this->pre_chars[0]);
                        this->candidate.push_back(this->pre_chars[1]);
                        this->candidate.push_back(this->pre_chars[2]);
                        this->candidate.push_back(c);
                        this->pre_chars.clear();
                        this->state = 0;
                    }
                }
            } else {
                this->clear_to_outputs();
            }
        } break;
        default:
            assert(false);
    }
}

void StringFinderUTF8::feed_end() {
    this->pos++;
    this->clear_to_outputs();
}

std::vector<std::pair<size_t,std::string>>& StringFinderUTF8::pre_fetch() {
    return this->outputs;
}

