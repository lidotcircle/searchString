#include "sfinder/string_finder_utf8.h"
#include "sfinder/unicode_converter.h"
#include "sfinder/finder_factory.h"
#include <string>
#include <assert.h>
using namespace std;

#define IS_CONTROL_CHAR(c) (0x00 <= (c) && (c) <= 0x1F && (c) != 0x09 && (c) != 0x0A && (c) != 0x0D)


StringFinderUTF8::StringFinderUTF8(): pos(0) {}

void StringFinderUTF8::clear_to_outputs() {
    if (candidate.size() > 0) {
        this->outputs.push_back(make_pair(this->start_pos,candidate));
        this->candidate.clear();
    }
    this->pre_chars.clear();
}

void StringFinderUTF8::feed_char(unsigned char c) {
    this->pos++;

    this->pre_chars.push_back(c);
    size_t n;
    auto code_point = decoder_utf8(this->pre_chars, n);
    assert(code_point.size() <= 1);
    if (n == this->pre_chars.size()) {
        assert(code_point.size() == 1);
        auto cdp = code_point[0];

        if (IS_CONTROL_CHAR(cdp) || !is_desired_unicode_code_point(cdp)) {
            this->clear_to_outputs();
        } else {
            if (this->candidate.size() == 0)
                this->start_pos = this->pos - this->pre_chars.size();

            for (auto c: this->pre_chars)
                this->candidate.push_back(c);
            this->pre_chars.clear();
        }
    } else if (this->pre_chars.size() == 4) {
        this->clear_to_outputs();
    }
}

void StringFinderUTF8::feed_end() {
    this->pos++;
    this->clear_to_outputs();
}

std::vector<std::pair<size_t,std::string>>& StringFinderUTF8::pre_fetch() {
    return this->outputs;
}

const int StringFinderUTF8::register_handle = FinderFactory::register_finder(
        "utf8",
        "printable utf8 string",
        [](const string& params) {
            return std::unique_ptr<StringFinder>(new StringFinderUTF8());
        });
