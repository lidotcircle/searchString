#ifndef _LUA_REDUCER_H_
#define _LUA_REDUCER_H_

#include "../lua/lua_wrapper.h"

int lua_register_reducer(lua_State *L);
int lua_reducer_list(lua_State *L);

#endif // _LUA_REDUCER_H_
