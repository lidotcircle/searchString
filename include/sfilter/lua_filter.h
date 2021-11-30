#ifndef _LUA_FILTER_H_
#define _LUA_FILTER_H_

#include "string_filter.h"
#include "../lua/lua_wrapper.h"
#include <memory>

class LuaFilter: StringFilter
{
private:
    lua_State* L;
    int filter_func_ref;

public:
    LuaFilter(lua_State* L, int funcref);
    virtual int filter(const std::string& str) const override;
};

int lua_register_filter(lua_State * L);

#endif // _LUA_FILTER_H_