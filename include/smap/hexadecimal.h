#ifndef _STRING_MAPPING_HEXADECIMAL_H_
#define _STRING_MAPPING_HEXADECIMAL_H_

#include "string_mapper.h"
#include "utils.h"

class HexMapper : public StringMapper {
private:
    bool decode;

public:
    HexMapper(bool decode);
    virtual ~HexMapper() = default;

    virtual std::vector<std::pair<size_t,std::string>> map(const std::string& str) const override;

    static const std::vector<int> register_handles;
};

#endif // _STRING_MAPPING_HEXADECIMAL_H_
