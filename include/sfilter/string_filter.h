#ifndef _STRING_FILTER_H_
#define _STRING_FILTER_H_

#include <string>
#include <memory>
#include <vector>


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


bool
apply_filters(
        const std::vector<std::shared_ptr<StringFilter>>& filters,
        const std::string& str);

#endif // _STRING_FILTER_H_
