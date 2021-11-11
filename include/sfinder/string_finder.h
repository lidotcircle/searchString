#ifndef _STRING_DECODER_H_
#define _STRING_DECODER_H_

#include <vector>
#include <string>
#include <memory>
#include "../sfilter/string_filter.h"
#include "../smap/string_mapper.h"


class StringFinder
{
private:
    std::vector<std::shared_ptr<StringFilter>> filters;
    std::vector<std::shared_ptr<StringMapper>> mappers;
    std::vector<std::pair<size_t,std::string>> results;

    bool filter(const std::string &str);
    std::string map(const std::string &str);

protected:
    virtual std::vector<std::pair<size_t,std::string>>& pre_fetch() = 0;

public:
    virtual void feed_char(unsigned char c) = 0;
    virtual void feed_end() = 0;

    void add_mapper(std::shared_ptr<StringMapper> mapper);
    void add_filter(std::shared_ptr<StringFilter> filter);
    std::vector<std::pair<size_t,std::string>>& fetch();
};

#endif // _STRING_DECODER_H_
