#ifndef _STRING_MAPPER_H_
#define _STRING_MAPPER_H_

#include <string>
#include <vector>


class StringMapper {
    public:
        virtual std::vector<std::pair<size_t,std::string>> map(const std::string&) const = 0;
        virtual ~StringMapper();
};


#endif // _STRING_MAPPER_H_

