#include "sfinder/unicode_converter.h"
using namespace std;


vector<int> decoder_utf16(const string& str, size_t& n, bool little_endian) {
    vector<int> result;

    for (size_t i=0; i<str.size(); i += 2) {
        if (str.size() < i + 1) {
            n = i;
            return result;
        }

        char c1 = str[i];
        char c2 = str[i+1];
        if (little_endian)
            std::swap(c1, c2);
        int code = (c1 << 8) + c2;

        if (code <= 0xD7FF || (0xE000 <= code && code <= 0xFFFF)) {
            result.push_back(code);
            continue;
        }

        if ((code & 0xD800) == 0xD800) {
            if (i + 3 >= str.size()) {
                n = i;
                return result;
            }
            auto c3 = str[i+2];
            auto c4 = str[i+3];
            if (little_endian)
                std::swap(c3, c4);
            int code2 = (c3 << 8) + c4;

            if ((code2 & 0xDC00) != 0xDC00) {
                n = i;
                return result;
            }

            i += 2;
            int v = ((code & 0x3FF) << 10) | (code2 & 0x3FF);
            v += 0x10000;
            result.push_back(v);
            continue;
        }
    }

    n = str.size();
    return result;
}

string encoder_utf16(const vector<int>& str, size_t& n, bool little_endian) {
    string result;

    for (size_t i = 0; i < str.size(); i++) {
        uint32_t val = str[i];

        if (val <= 0xD7FF || (0xE000 <= val && val <= 0xFFFF)) {
            char c1 = (val >> 8) & 0xFF;
            char c2 = val & 0xFF;
            if (little_endian)
                std::swap(c1, c2);

            result.push_back(c1);
            result.push_back(c2);
        } else if (val <= 0x10FFFF) {
            val -= 0x10000;
            char c1 = (val >> 10) + 0xD800;
            char c2 = (val & 0x3FF) + 0xDC00;
            if (little_endian)
                std::swap(c1, c2);

            result.push_back(c1);
            result.push_back(c2);
        } else {
            n = i;
            return result;
        }
    }

    n = str.size();
    return result;
}

