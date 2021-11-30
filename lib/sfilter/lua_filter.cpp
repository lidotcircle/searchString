#include "sfilter/lua_filter.h"
#include "sfilter/filter_factory.h"
#include "lua/lua_wrapper.h"
#include <stdexcept>
#include <string>
using namespace std;

LuaFilter::LuaFilter(lua_State* L, int funcref): L(L), filter_func_ref(funcref) {}

int LuaFilter::filter(const std::string& str) const {
    auto L = this->L;

    lua_wrapper.lua_pushlstring(L, str.c_str(), str.size());
    lua_wrapper.lua_pushvalue(L, this->filter_func_ref);

    if (lua_wrapper.lua_pcall(L, 1, 1, 0) != LuaWrapper::LUA_OK) {
        size_t n;
        const char* err = lua_wrapper.lua_tolstring(L, -1, &n);
        throw runtime_error(string(err, n));
    }

    if (!lua_wrapper.lua_isnumber(L, -1))
        throw runtime_error("filter function must return a number");

    auto v = lua_wrapper.lua_checknumber(L, -1);
    lua_wrapper.lua_pop(L, 1);
    if (v == 0)
        return 0;

    return v > 0 ? 1 : -1;
}


int lua_register_filter(lua_State * L) {
    if (lua_wrapper.lua_gettop(L) != 4)
        throw runtime_error("wrong number of arguments");
    string encoding  = lua_wrapper.lua_checkstring(L, 1);
    string filter_name = lua_wrapper.lua_checkstring(L, 2);
    string desc = lua_wrapper.lua_checkstring(L, 3);

    FilterFactory::register_filter(encoding, filter_name, desc, );
}
