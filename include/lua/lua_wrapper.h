#ifndef _LUA_WRAPPER_H_
#define _LUA_WRAPPER_H_

#include <stdarg.h>
#include <vector>
#include <memory>
#include "loader.h"


struct lua_State;
typedef double    lua_Number;
typedef long      lua_Integer;
typedef ptrdiff_t lua_KContext; 
typedef int (*lua_CFunction) (lua_State *L);
typedef int (*lua_KFunction) (lua_State *L, int status, lua_KContext ctx);

#define LUA_FUNCTION_LIST \
    LFENTRY(lua_State*,    luaL_newstate) \
    LFENTRY(void,          lua_close,             lua_State* L) \
    LFENTRY(void,          luaL_openlibs,         lua_State* L) \
    \
    LFENTRY(int,           luaL_loadstring,       lua_State* L, const char* s) \
    LFENTRY(void,          lua_callk,             lua_State* L, int nargs, int nresults, lua_KContext ctx, lua_KFunction k) \
    LFENTRY(int,           lua_pcallk,            lua_State* L, int nargs, int nresults, int errfunc, lua_KContext ctx, lua_KFunction k) \
    \
    LFENTRY(int,           lua_type,              lua_State* L, int idx) \
    \
    LFENTRY(int,           luaL_ref,              lua_State* L, int t) \
    LFENTRY(void,          luaL_unref,            lua_State* L, int t, int ref) \
    \
    LFENTRY(int,           lua_absindex,          lua_State* L, int idx) \
    LFENTRY(int,           lua_gettop,            lua_State* L) \
    LFENTRY(void,          lua_settop,            lua_State* L, int idx) \
    LFENTRY(void,          lua_pushvalue,         lua_State* L, int idx) \
    LFENTRY(void,          lua_rotate,            lua_State* L, int idx, int n) \
    LFENTRY(void,          lua_copy,              lua_State* L, int fromidx, int toidx) \
    LFENTRY(int,           lua_checkstack,        lua_State* L, int n) \
    \
    LFENTRY(int,           lua_getglobal,         lua_State* L, const char* name) \
    LFENTRY(int,           lua_setglobal,         lua_State* L, const char* name) \
    \
    LFENTRY(void,          lua_createtable,       lua_State* L, int narr, int nrec) \
    LFENTRY(void,          lua_settable,          lua_State* L, int idx) \
    LFENTRY(void,          lua_gettable,          lua_State* L, int idx) \
    LFENTRY(size_t,        lua_rawlen,            lua_State* L, int idx) \
    LFENTRY(int,           lua_getfield,          lua_State* L, int idx, const char *k) \
    LFENTRY(int,           lua_geti,              lua_State* L, int idx, lua_Integer n) \
    LFENTRY(int,           lua_rawget,            lua_State* L, int idx) \
    LFENTRY(int,           lua_rawgeti,           lua_State* L, int idx, lua_Integer n) \
    \
    LFENTRY(void,          lua_pushnil,           lua_State* L) \
    LFENTRY(void,          lua_pushnumber,        lua_State* L, lua_Number  n) \
    LFENTRY(void,          lua_pushinteger,       lua_State* L, lua_Integer n) \
    LFENTRY(const char*,   lua_pushlstring,       lua_State* L, const char *s, size_t len) \
    LFENTRY(const char*,   lua_pushstring,        lua_State* L, const char *s) \
    LFENTRY(void,          lua_pushboolean,       lua_State* L, int b) \
    LFENTRY(void,          lua_pushlightuserdata, lua_State* L, void *p) \
    LFENTRY(void,          lua_pushcclosure,      lua_State* L, lua_CFunction fn, int n) \
    \
    LFENTRY(int,           lua_error,             lua_State* L) \
    \
    LFENTRY(lua_Number,    lua_tonumberx,         lua_State* L, int idx, int *isnum) \
    LFENTRY(lua_Integer,   lua_tointegerx,        lua_State* L, int idx, int *isnum) \
    LFENTRY(int,           lua_toboolean,         lua_State* L, int idx) \
    LFENTRY(const char*,   lua_tolstring,         lua_State* L, int idx, size_t *len) \
    LFENTRY(lua_CFunction, lua_tocfunction,       lua_State* L, int idx) \
    LFENTRY(void*,         lua_touserdata,        lua_State* L, int idx) \
    LFENTRY(lua_State*,    lua_tothread,          lua_State* L, int idx) \
    LFENTRY(const void*,   lua_topointer,         lua_State* L, int idx)

#define LFENTRY(rettype, funcname, ...) typedef rettype (*funcname ## _t)(__VA_ARGS__);
LUA_FUNCTION_LIST
#undef LFENTRY

class LuaWrapper
{
public:
    static constexpr int LUA_REGISTRYINDEX = -1000000 - 10000;
    static constexpr int LUA_OK = 0;
    static constexpr int LUA_YIELD = 1;
    static constexpr int LUA_ERRRUN = 2;
    static constexpr int LUA_ERRSYNTAX = 3;
    static constexpr int LUA_ERRMEM = 4;
    static constexpr int LUA_ERRGCMM = 5;
    static constexpr int LUA_ERRERR = 6;

    static constexpr int LUA_TNIL = 0;
    static constexpr int LUA_TBOOLEAN = 1;
    static constexpr int LUA_TLIGHTUSERDATA = 2;
    static constexpr int LUA_TNUMBER = 3;
    static constexpr int LUA_TSTRING = 4;
    static constexpr int LUA_TTABLE = 5;
    static constexpr int LUA_TFUNCTION = 6;
    static constexpr int LUA_TUSERDATA = 7;
    static constexpr int LUA_TTHREAD = 8;


private:
    std::shared_ptr<lhandle_t> library_handle;

#define LFENTRY(rettype, funcname, ...) funcname ## _t _ ## funcname = nullptr;
LUA_FUNCTION_LIST
#undef LFENTRY

    std::vector<lua_State*> l_states;

    void resolve_functions();

public:
    LuaWrapper();
    ~LuaWrapper();

#define LFENTRY(rettype, funcname, ...) rettype funcname(__VA_ARGS__) const;
LUA_FUNCTION_LIST
#undef LFENTRY

    lua_State* luaL_newstate_close_by_wrapper() const;

    int  luaL_dostring(lua_State* L, const char* s) const;
    int  lua_pcall (lua_State *L, int nargs, int nresults, int errfunc) const;
    void lua_call (lua_State *L, int nargs, int nresults) const;

    inline int lua_isnil(lua_State *L, int index) const { return lua_type(L, index) == LUA_TNIL ; }
    inline int lua_isboolean(lua_State *L, int index) const { return lua_type(L, index) == LUA_TBOOLEAN ; }
    inline int lua_islightuserdata(lua_State *L, int index) const { return lua_type(L, index) == LUA_TLIGHTUSERDATA ; }
    inline int lua_isnumber(lua_State *L, int index) const { return lua_type(L, index) == LUA_TNUMBER ; }
    inline int lua_isstring(lua_State *L, int index) const { return lua_type(L, index) == LUA_TSTRING ; }
    inline int lua_istable(lua_State *L, int index) const { return lua_type(L, index) == LUA_TTABLE ; }
    inline int lua_isfunction(lua_State *L, int index) const { return lua_type(L, index) == LUA_TFUNCTION ; }
    inline int lua_isuserdata(lua_State *L, int index) const { return lua_type(L, index) == LUA_TUSERDATA ; }
    inline int lua_isthread(lua_State *L, int index) const { return lua_type(L, index) == LUA_TTHREAD ; }
    inline int lua_isinteger(lua_State *L, int index) const { int ret; this->lua_tointegerx(L, index, &ret); return ret; }

    lua_Integer lua_tointeger(lua_State *L, int index) const;

    void lua_newtable(lua_State *L) const;
    void lua_pop(lua_State *L, int n) const;

    int lua_error_except(lua_State *L) const;

    const char *lua_tostring(lua_State *L, int idx) const;
    lua_Number  lua_checknumber(lua_State *L, int idx) const;
    lua_Integer lua_checkinteger(lua_State *L, int idx) const;
    const char* lua_checkstring(lua_State *L, int idx) const;

    lua_Number  lua_checknumber_except (lua_State *L, int idx) const;
    lua_Integer lua_checkinteger_except(lua_State *L, int idx) const;
    const char* lua_checkstring_except (lua_State *L, int idx) const;

    operator bool() const;
};

extern const LuaWrapper lua_wrapper;

#define LUA_EXCEPTION_BEGIN() \
    try {
#define LUA_EXCEPTION_END() \
    } catch (std::runtime_error& e) { \
        lua_wrapper.lua_pushstring(L, e.what()); \
        return lua_wrapper.lua_error(L); \
    }

#endif // _LUA_WRAPPER_H_
