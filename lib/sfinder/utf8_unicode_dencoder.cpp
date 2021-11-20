#include "sfinder/unicode_converter.h"
using namespace std;

constexpr int utf8_bound[4][2] = {
    {0x00,    0x7f},
    {0x80,    0x7ff},
    {0x800,   0xffff},
    {0x10000, 0x10ffff},
};

vector<int> decoder_utf8(const string& str, size_t& n) {
    vector<int> result;

    for (size_t i = 0; i < str.size(); ++i) {
        unsigned char c = str[i];

        if ((c & 0b10000000) == 0) {
            result.push_back(c);
            continue;
        }

        int val, len;
        if ((c & 0b11111000) == 0b11110000) {
            len = 3;
            val = (c & 0b00000111) << 18;
        } else if ((c & 0b11110000) == 0b11100000) {
            len = 2;
            val = (c & 0b00001111) << 12;
        } else if ((c & 0b11100000) == 0b11000000) {
            len = 1;
            val = (c & 0b00011111) << 6;
        } else {
            n = i;
            return result;
        }

        if (i + len >= str.size()) {
            n = i;
            return result;
        }

        for (int j = 0; j < len; ++j) {
            c = str[i+1+j];
            if ((c & 0b11000000) != 0b10000000) {
                n = i;
                return result;
            }
            val |= (c & 0b00111111) << (6 * (len - j - 1));
        }

        auto lb = utf8_bound[len][0];
        auto ub = utf8_bound[len][1];

        if (val < lb || val > ub) {
            n = i;
            return result;
        }

        i += len;
        result.push_back(val);
    }

    n = str.size();
    return result;
}

string encoder_utf8(const vector<int>& str, size_t& n) {
    string result;

    for (size_t i=0;i<str.size();i++) {
        uint32_t c = str[i];

        if (c < 0x80) {
            result.push_back(c);
        } else if (c < 0x800) {
            result.push_back(0b11000000 | (c >> 6));
            result.push_back(0b10000000 | (c & 0b00111111));
        } else if (c < 0x10000) {
            result.push_back(0b11100000 | (c >> 12));
            result.push_back(0b10000000 | ((c >> 6) & 0b00111111));
            result.push_back(0b10000000 | (c & 0b00111111));
        } else if (c < 0x110000) {
            result.push_back(0b11110000 | (c >> 18));
            result.push_back(0b10000000 | ((c >> 12) & 0b00111111));
            result.push_back(0b10000000 | ((c >> 6) & 0b00111111));
            result.push_back(0b10000000 | (c & 0b00111111));
        } else {
            n = i;
            return result;
        }
    }

    n = str.size();
    return result;
}

