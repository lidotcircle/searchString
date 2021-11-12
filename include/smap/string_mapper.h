#ifndef _STRING_MAPPER_H_
#define _STRING_MAPPER_H_

#include <string>
#include <vector>
#include <memory>


class StringMapper {
    public:
        virtual std::vector<std::pair<size_t,std::string>> map(const std::string&) const = 0;
        virtual ~StringMapper();
};


std::vector<std::pair<size_t,std::string>>
apply_mappers(
        const std::vector<std::shared_ptr<StringMapper>>& mappers,
        const std::string&);

#endif // _STRING_MAPPER_H_

