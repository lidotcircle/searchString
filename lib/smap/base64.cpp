#include "smap/base64.h"
#include "smap/mapper_factory.h"
#include "utils.h"
using namespace std;


Base64Mapper::Base64Mapper(bool decode): decode( decode ) {}

vector<pair<size_t,string>> Base64Mapper::map(const std::string& str) const {
    if (decode) {
        try {
            auto ks = base64_decode(str);
            return { make_pair(0, string(ks.begin(), ks.end())) };
        } catch (const std::exception&) {
            return { };
        }
    } else {
        return { make_pair(0, base64_encode(str.c_str(), str.size())) };
    }
}

static auto name = "base64";
static auto desc = "base64 encoder or decoder, [:decode]";
static auto creator = [](const string& k) {
    bool decode = false;
    if (k == "decode")
        decode = true;
    else if (!k.empty())
        throw std::runtime_error("please specify a correct mode");
    return shared_ptr<StringMapper>(new Base64Mapper(decode));
};
const vector<int> Base64Mapper::register_handles = {
    MapperFactory::register_mapper("ascii",  name, desc, creator),
    MapperFactory::register_mapper("gb2312", name, desc, creator),
};
