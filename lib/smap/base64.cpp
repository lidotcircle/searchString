#include "smap/base64.h"
#include "utils.h"
using namespace std;


Base64Mapper::Base64Mapper(bool decode): decode( decode ) {}

vector<pair<size_t,string>> Base64Mapper::map(const std::string& str) const {
    if (decode) {
        try {
            auto ks = base64_decode(str);
            return { make_pair(0, string(ks.begin(), ks.end())) };
        } catch (const std::exception& e) {
            return { };
        }
    } else {
        return { make_pair(0, base64_encode(str.c_str(), str.size())) };
    }
}

