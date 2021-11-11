#ifndef _STRING_FILTER_H_
#define _STRING_FILTER_H_

#include <string>


class StringFilter {
    public:
        /**
         * @return < 0 reject
         *         = 0 pass to next filter if it presents otherwise accept
         *         > 0 accept
         */
        virtual int filter(const std::string& str) const = 0;
        virtual ~StringFilter();
};

#endif // _STRING_FILTER_H_
