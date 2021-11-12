#ifndef _STRING_MAPPING_BASE64_H_
#define _STRING_MAPPING_BASE64_H_

#include "string_mapper.h"
#include "utils.h"

class Base64Mapper : public StringMapper {
private:
    bool decode;

public:
    Base64Mapper(bool decode);
    virtual ~Base64Mapper() = default;

    virtual std::vector<std::pair<size_t,std::string>> map(const std::string& str) const override;
};

#endif // _STRING_MAPPING_BASE64_H_
