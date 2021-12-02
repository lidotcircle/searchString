#include "sfilter/lua_filter.h"
#include "sfilter/string_filter.h"
#include "sfilter/filter_factory.h"
#include "lua/lua_wrapper.h"
#include "utils.hpp"
#include <stdexcept>
#include <string>
using namespace std;
using namespace FilterFactory;

class LuaFilter: public StringFilter
{
private:
    lua_State* L;
    int filter_func_ref;
    string params;

public:
    LuaFilter(lua_State* L, int funcref, const string& params):
        L(L), filter_func_ref(funcref), params(params) {};

    virtual int filter(const std::string& str) const override {
        auto L = this->L;
        auto top = lua_wrapper.lua_gettop(L);
        defer([&]() { lua_wrapper.lua_settop(L, top); });

        lua_wrapper.lua_rawgeti(L, LuaWrapper::LUA_REGISTRYINDEX, this->filter_func_ref);
        lua_wrapper.lua_pushlstring(L, str.c_str(), str.size());
        if (this->params.empty()) {
            lua_wrapper.lua_pushnil(L);
        } else {
            lua_wrapper.lua_pushstring(L, params.c_str());
        }

        if (lua_wrapper.lua_pcall(L, 2, 1, 0) != LuaWrapper::LUA_OK) {
            size_t n;
            const char* err = lua_wrapper.lua_tolstring(L, -1, &n);
            throw runtime_error("lua_filter failed: " + string(err, n));
        }

        if (!lua_wrapper.lua_isnumber(L, -1))
            throw runtime_error("filter function must return a number");

        auto v = lua_wrapper.lua_checknumber_except(L, -1);
        if (v == 0)
            return 0;

        return v > 0 ? 1 : -1;
    }
};

class LuaFilterGenerator: public FilterFactory::FilterGenerator {
private:
    lua_State* L;
    int funcref;

public:
    LuaFilterGenerator(lua_State* L, int funcref): L(L), funcref(funcref) {}

    std::shared_ptr<StringFilter> operator()(const string& params) const override {
        return static_pointer_cast<StringFilter>(
                std::make_shared<LuaFilter>(this->L, this->funcref, params));
    }
};

int lua_register_filter(lua_State * L) {
    LUA_EXCEPTION_BEGIN();

    if (lua_wrapper.lua_gettop(L) != 4)
        throw runtime_error("wrong number of arguments");
    string encoding  = lua_wrapper.lua_checkstring_except(L, 1);
    string filter_name = lua_wrapper.lua_checkstring_except(L, 2);
    string desc = lua_wrapper.lua_checkstring_except(L, 3);
    if (!lua_wrapper.lua_isfunction(L, 4))
        throw runtime_error("filter function must be a function");

    int ref = lua_wrapper.luaL_ref(L, LuaWrapper::LUA_REGISTRYINDEX);
    FilterFactory::register_filter(encoding, filter_name, desc,
                                   std::static_pointer_cast<FilterGenerator>(std::make_shared<LuaFilterGenerator>(L, ref)));
    return 0;
    LUA_EXCEPTION_END();
}
