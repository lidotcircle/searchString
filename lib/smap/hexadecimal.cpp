#include "smap/hexadecimal.h"
#include "utils.h"
using namespace std;


HexMapper::HexMapper(bool decode): decode( decode ) {}

vector<pair<size_t,string>> HexMapper::map(const std::string& str) const {
    if (decode) {
        try {
            auto ks = hexstr2str(str);
            return { make_pair(0, string(ks.begin(), ks.end())) };
        } catch (const std::exception& e) {
            return { };
        }
    } else {
        return { make_pair(0, str2hexstr(str)) };
    }
}

