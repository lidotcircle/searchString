#ifndef _FINDER_FACTORY_H_
#define _FINDER_FACTORY_H_

#include <memory>
#include <string>
#include <map>
#include "string_finder.h"

namespace FinderFactory {

typedef std::unique_ptr<StringFinder> (*create_finder_func_t)(const std::string& params);

class FinderGenerator {
public:
    virtual std::unique_ptr<StringFinder> operator()(const std::string& params) = 0;
};

std::unique_ptr<StringFinder>
create(const std::string& finder, const std::string &params);

const std::vector<std::pair<std::string,std::string>>& get_supported_finders();

int
register_finder(const std::string& encoding,
                const std::string& description,
                std::shared_ptr<FinderGenerator> factory);

int
register_finder(const std::string& encoding,
                const std::string& description,
                create_finder_func_t factory);

};

#endif // _FINDER_FACTORY_H_
