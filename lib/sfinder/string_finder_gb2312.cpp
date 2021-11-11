#include "sfinder/string_finder_gb2312.h"
#include <assert.h>


/** GB2312 string
 *  1. ascii string (without control characters but include TAB(0x09) LF(0x0A) CR(0x0D))
 *  2. first byte in 0xA1-0xF7 and second byte in 0xA1-0xFE
 */

#define _IS_V_ASCII_CRLF(c)  ((c >= 0x20 && c <  0x7f) \
        || c == 0x09 || c == 0x0A || c == 0x0D)
#define _IS_V_ASCII_NOCRLF(c)  ((c >= 0x20 && c <  0x7f) || c == 0x09)
#define _IS_V_FIRST(c)  (c >= 0xA1 && c <= 0xF7)
#define _IS_V_SECOND(c) (c >= 0xA1 && c <= 0xFE)
#define is_gb2312_multi(c) (_IS_V_FIRST(c) || _IS_V_SECOND(c))
#define is_ascii(c)        (_IS_V_ASCII_NOCRLF(c))
#define is_valid_gb2312(c) (is_gb2312_multi(c) || _IS_V_ASCII_NOCRLF(c))

StringFinderGB2312::StringFinderGB2312(): pos(0), is_end(false) {}

void StringFinderGB2312::clear_complete() {
    if (this->complete_str.second.empty())
        return;

    this->outputs.push_back(std::move(this->complete_str));
}

void StringFinderGB2312::clear_half() {
    if (this->half_str.second.size() > 1) {
        this->half_str.second.pop_back();
        this->outputs.push_back(std::move(this->half_str));
    }

    this->half_str.second.clear();
}

void StringFinderGB2312::feed_char(unsigned char c) {
    if (!is_valid_gb2312(c)) {
        this->clear_half();
        this->clear_complete();
    } else {
        if (is_ascii(c)) {
            this->clear_half();
            if (this->complete_str.second.empty())
                this->complete_str.first = pos;
            this->complete_str.second.push_back(c);
        } else if (_IS_V_FIRST(c)) {
            this->complete_str.second.push_back(c);
            if (!this->half_str.second.empty())
                this->half_str.second.push_back(c);

            auto tmp = std::move(this->half_str);
            this->half_str = std::move(this->complete_str);
            this->complete_str = std::move(tmp);
        } else {
            this->clear_complete();
            if (!this->half_str.second.empty()) {
                this->half_str.second.push_back(c);
                this->complete_str = std::move(this->half_str);
            }
        }
    }

    pos++;
}

void StringFinderGB2312::feed_end() {
    assert(!this->is_end);
    this->is_end = true;
}

std::vector<std::pair<size_t,std::string>>& StringFinderGB2312::pre_fetch() {
    return this->outputs;
}

