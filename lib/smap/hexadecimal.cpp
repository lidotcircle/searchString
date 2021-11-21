#include "smap/hexadecimal.h"
#include "smap/mapper_factory.h"
#include "utils.h"
#include <stdexcept>
using namespace std;


HexMapper::HexMapper(bool decode): decode( decode ) {}

vector<pair<size_t,string>> HexMapper::map(const std::string& str) const {
    if (decode) {
        try {
            auto ks = hexstr2str(str);
            return { make_pair(0, string(ks.begin(), ks.end())) };
        } catch (const std::exception&) {
            return { };
        }
    } else {
        return { make_pair(0, str2hexstr(str)) };
    }
}

static auto name = "hex";
static auto desc = "hex encoder or decoder, [:decode]";
static auto creator = [](const string& k) {
    bool decode = false;
    if (k == "decode")
        decode = true;
    else if (!k.empty())
        throw std::runtime_error("please specify a correct mode");
    return shared_ptr<StringMapper>(new HexMapper(decode));
};
const vector<int> HexMapper::register_handles = {
    MapperFactory::register_mapper("ascii",  name, desc, creator),
    MapperFactory::register_mapper("gb2312", name, desc, creator),
    MapperFactory::register_mapper("utf8", name, desc, creator),
    MapperFactory::register_mapper("utf16", name, desc, creator),
};
