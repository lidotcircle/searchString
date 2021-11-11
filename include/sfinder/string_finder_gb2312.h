#ifndef _STRING_FINDER_GB2312_H_
#define _STRING_FINDER_GB2312_H_

#include "string_finder.h"


class StringFinderGB2312: public StringFinder {
private:
    std::vector<std::pair<size_t,std::string>> outputs;
    std::pair<size_t,std::string> complete_str;
    std::pair<size_t,std::string> half_str;
    size_t pos;
    bool is_end;

    void clear_complete();
    void clear_half();

protected:
    virtual std::vector<std::pair<size_t,std::string>>& pre_fetch() override;

public:
    StringFinderGB2312();

    virtual void feed_char(unsigned char c) override;
    virtual void feed_end() override;
};

#endif // _STRING_FINDER_GB2312_H_
