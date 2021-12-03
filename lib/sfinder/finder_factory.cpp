#include "sfinder/finder_factory.h"
#include "sfinder/string_finder_ascii.h"
#include "sfinder/string_finder_gb2312.h"
#include "sfinder/string_finder_utf16.h"
#include "sfinder/string_finder_utf8.h"
#include <stdexcept>
using namespace std;


static int keep_initialization_procedures()
{
    static vector<int> handles = {
        StringFinderASCII::register_handle,
        StringFinderGB2312::register_handle,
        StringFinderUTF16::register_handle,
        StringFinderUTF8::register_handle
    };

    int ret = 0;
    for (auto h: handles)
        ret += h;

    return ret;
}


namespace FinderFactory {

static map<string,shared_ptr<FinderGenerator>> finder_generators;
static vector<pair<string,string>> finder_names_descs;

class FinderGeneratorFuncWrapper: public FinderGenerator {
private:
    create_finder_func_t func;

public:
    FinderGeneratorFuncWrapper(create_finder_func_t func): func(func) {}
    virtual unique_ptr<StringFinder> operator()(const string& params) override{
        return func(params);
    }
};

std::unique_ptr<StringFinder>
create(const std::string& finder, const std::string &params) {
    int n = keep_initialization_procedures();

    auto it = finder_generators.find(finder);
    if (it == finder_generators.end()) {
        throw std::runtime_error("Unknown encoding: " + finder + ", magic = " + std::to_string(n));
    }
    return it->second->operator()(params);
}

const vector<pair<string,string>>& get_supported_finders() {
    return finder_names_descs;
}

int
register_finder(const std::string& encoding,
                const std::string& description,
                std::shared_ptr<FinderGenerator> factory)
{
    if (finder_generators.find(encoding) != finder_generators.end()) {
        throw std::runtime_error("Duplicate encoding: " + encoding);
    }
    finder_generators[encoding] = factory;
    finder_names_descs.push_back(make_pair(encoding,description));
    return finder_names_descs.size();
}

int
register_finder(const std::string& encoding,
                const std::string& description,
                create_finder_func_t factory)
{
return register_finder(encoding, description,
                       std::make_shared<FinderGeneratorFuncWrapper>(factory));
}

};
