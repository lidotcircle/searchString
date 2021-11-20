#ifndef _STRING_FINDER_UTF16_H_
#define _STRING_FINDER_UTF16_H_

#include "string_finder.h"
#include "utils.hpp"


class StringFinderUTF16: public StringFinder {
private:
    std::vector<std::pair<size_t,std::string>> outputs;
    std::vector<char> pre_chars;
    std::string candidate;
    bool little_endian;
    size_t pos;
    int state;

    void clear_to_outputs();

protected:
    virtual std::vector<std::pair<size_t,std::string>>& pre_fetch() override;

public:
    StringFinderUTF16(bool little_endian = false);

    virtual void feed_char(unsigned char c) override;
    virtual void feed_end() override;
};

#endif // _STRING_FINDER_UTF16_H_
