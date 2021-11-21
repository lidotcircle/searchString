#ifndef _STRING_FINDER_UTF8_H_
#define _STRING_FINDER_UTF8_H_

#include "string_finder.h"
#include "utils.hpp"


class StringFinderUTF8: public StringFinder {
private:
    std::vector<std::pair<size_t,std::string>> outputs;
    std::vector<char> pre_chars;
    std::string candidate;
    size_t pos, start_pos;
    int state;

    void clear_to_outputs();

protected:
    virtual std::vector<std::pair<size_t,std::string>>& pre_fetch() override;

public:
    StringFinderUTF8();

    virtual void feed_char(unsigned char c) override;
    virtual void feed_end() override;
};


#endif // _STRING_FINDER_UTF8_H_
