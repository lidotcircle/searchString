#ifndef _LUA_FINDER_H_
#define _LUA_FINDER_H_

#include "../lua/lua_wrapper.h"

/**
 * register_finder(encoding, description, finder_factory)
 *   @finder_factory should be a function that returns a finder object
 *   @finder object can implement following functions:
 *   - init(string params) -> void optional 
 *       for initializing the finder object with params
 *   - feed(char c) -> nil or string or string[] or [integer, string] or [integer,string][] required
 *       for feeding the finder object with a character and it may return ...
 *   - end() -> nil or string or string[] or [integer, string] or [integer,string][] required
 *       just indicate the end of the input
 */
int lua_register_finder(lua_State *L);

#endif // _LUA_FINDER_H_
