#include "lua/strfd.h"
#include "lua/lua_wrapper.h"
#include "sfilter/lua_filter.h"
#include "smap/lua_mapper.h"
#include "sreduce/lua_reducer.h"
#include "sfinder/lua_finder.h"
#include <iostream>
#include <memory>
#include <vector>
#include <stdexcept>
using namespace std;

static shared_ptr<lua_State> gluaL;

static vector<pair<string,lua_CFunction>> strfd_lua_funcs = {
    { "register_filter",  lua_register_filter },
    { "register_mapper",  lua_register_mapper },
    { "register_reducer", lua_register_reducer },
    { "register_finder",  lua_register_finder },
};

bool setup_lua_strfd(const std::string& luascript) {
    if (!lua_wrapper)
        return false;

    gluaL = shared_ptr<lua_State>(lua_wrapper.luaL_newstate_close_by_wrapper(), [] (lua_State* L) {
    });

    if (gluaL == nullptr)
        return false;

    lua_wrapper.luaL_openlibs(gluaL.get());
    lua_wrapper.lua_newtable(gluaL.get());
    for (auto &f : strfd_lua_funcs) {
        lua_wrapper.lua_pushstring(gluaL.get(), f.first.c_str());
        lua_wrapper.lua_pushcclosure(gluaL.get(), f.second, 0);
        lua_wrapper.lua_settable(gluaL.get(), -3);
    }
    lua_wrapper.lua_setglobal(gluaL.get(), "strfd");

    if (lua_wrapper.luaL_loadstring(gluaL.get(), luascript.c_str()) != LuaWrapper::LUA_OK ||
        lua_wrapper.lua_pcall(gluaL.get(), 0, 0, 0) != LuaWrapper::LUA_OK)
    {
        string err = lua_wrapper.lua_tostring(gluaL.get(), -1);
        throw std::runtime_error(err);
    }

    return true;
}
