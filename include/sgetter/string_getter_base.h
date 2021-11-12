#ifndef _STRING_GETTER_BASE_H_
#define _STRING_GETTER_BASE_H_

#include <string>
#include <memory>
#include "../sfilter/string_filter.h"
#include "../smap/string_mapper.h"
#include "string_getter_iter.h"


class StringGetterBase {
protected:
    friend class StringGetterIter;
    virtual bool empty() const = 0;
    virtual std::pair<size_t,std::string>& top() = 0;
    virtual void pop() = 0;
    virtual void get_at_least_one_until_end() = 0;

public:
    virtual void add_filter(std::shared_ptr<StringFilter> filter) = 0;
    virtual void add_mapper(std::shared_ptr<StringMapper> mapper) = 0;

    virtual StringGetterIter begin();
    virtual StringGetterIter end();

    virtual ~StringGetterBase();
};

#endif // _STRING_GETTER_BASE_H_
