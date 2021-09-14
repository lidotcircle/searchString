#ifndef _STRING_FILTER_H_
#define _STRING_FILTER_H_

#include <string>


class StringFilter {
    public:
        virtual std::string filter(const std::string&) = 0;
        virtual ~StringFilter() = 0;
};


#endif // _STRING_FILTER_H_

