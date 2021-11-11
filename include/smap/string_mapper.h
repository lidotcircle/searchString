#ifndef _STRING_MAPPER_H_
#define _STRING_MAPPER_H_

#include <string>


class StringMapper {
    public:
        virtual std::string map(const std::string&) = 0;
        virtual ~StringMapper();
};


#endif // _STRING_MAPPER_H_

