#ifndef _LUA_WRAPPER_H_
#define _LUA_WRAPPER_H_

#include <stdarg.h>
#include <memory>
#include "loader.h"


struct lua_State;
typedef double    lua_Number;
typedef long      lua_Integer;
typedef ptrdiff_t lua_KContext; 
typedef int (*lua_CFunction) (lua_State *L);
typedef int (*lua_KFunction) (lua_State *L, int status, lua_KContext ctx);

typedef lua_State* (*luaL_newstate_t)();
typedef void (*lua_close_t)(lua_State* L);

typedef int (*luaL_loadstring_t) (lua_State *L, const char *s); 
typedef void (*lua_callk_t) (lua_State *L, int nargs, int nresults,
                           lua_KContext ctx, lua_KFunction k);
typedef int  (*lua_pcallk_t) (lua_State *L, int nargs, int nresults, int errfunc,
                           lua_KContext ctx, lua_KFunction k);

typedef int (*lua_type_t) (lua_State *L, int index);

typedef int  (*luaL_ref_t) (lua_State *L, int t);
typedef void (*luaL_unref_t) (lua_State *L, int t, int ref);

typedef int   (*lua_absindex_t) (lua_State *L, int idx);
typedef int   (*lua_gettop_t) (lua_State *L);
typedef void  (*lua_settop_t) (lua_State *L, int idx);
typedef void  (*lua_pushvalue_t) (lua_State *L, int idx);
typedef void  (*lua_rotate_t) (lua_State *L, int idx, int n);
typedef void  (*lua_copy_t) (lua_State *L, int fromidx, int toidx);
typedef int   (*lua_checkstack_t) (lua_State *L, int n);

typedef int (*lua_getglobal_t)(lua_State* L, const char* name);
typedef int (*lua_setglobal_t)(lua_State* L, const char* name);

typedef void (*lua_createtable_t)(lua_State* L, int narr, int nrec);
typedef void (*lua_settable_t) (lua_State *L, int idx);
typedef void (*lua_gettable_t) (lua_State *L, int idx);
typedef size_t (*lua_rawlen_t)   (lua_State *L, int idx);

typedef void        (*lua_pushnil_t)     (lua_State *L);
typedef void        (*lua_pushnumber_t)  (lua_State *L, lua_Number  n);
typedef void        (*lua_pushinteger_t) (lua_State *L, lua_Integer n);
typedef const char *(*lua_pushlstring_t) (lua_State *L, const char *s, size_t len);
typedef const char *(*lua_pushstring_t)  (lua_State *L, const char *s);
typedef void        (*lua_pushboolean_t)  (lua_State *L, int b);
typedef void        (*lua_pushlightuserdata_t) (lua_State *L, void *p);
typedef void        (*lua_pushcclosure_t) (lua_State *L, lua_CFunction fn, int n);

typedef lua_Number      (*lua_tonumberx_t) (lua_State *L, int idx, int *isnum);
typedef lua_Integer     (*lua_tointegerx_t) (lua_State *L, int idx, int *isnum);
typedef int             (*lua_toboolean_t) (lua_State *L, int idx);
typedef const char     *(*lua_tolstring_t) (lua_State *L, int idx, size_t *len);
typedef size_t          (*lua_rawlen_t) (lua_State *L, int idx);
typedef lua_CFunction   (*lua_tocfunction_t) (lua_State *L, int idx);
typedef void	       *(*lua_touserdata_t) (lua_State *L, int idx);
typedef lua_State      *(*lua_tothread_t) (lua_State *L, int idx);
typedef const void     *(*lua_topointer_t) (lua_State *L, int idx);


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

    luaL_newstate_t _luaL_newstate = nullptr;
    lua_close_t    _lua_close = nullptr;

    luaL_loadstring_t _luaL_loadstring = nullptr;
    lua_pcallk_t  _lua_pcallk = nullptr;
    lua_callk_t   _lua_callk = nullptr;
    lua_CFunction _luaL_openlibs = nullptr;

    lua_type_t   _lua_type = nullptr;

    luaL_ref_t   _luaL_ref = nullptr;
    luaL_unref_t _luaL_unref = nullptr;

    lua_absindex_t _lua_absindex = nullptr;
    lua_gettop_t   _lua_gettop = nullptr;
    lua_settop_t   _lua_settop = nullptr;
    lua_pushvalue_t _lua_pushvalue = nullptr;
    lua_rotate_t   _lua_rotate = nullptr;
    lua_copy_t     _lua_copy = nullptr;
    lua_checkstack_t _lua_checkstack = nullptr;

    lua_getglobal_t _lua_getglobal = nullptr;
    lua_setglobal_t _lua_setglobal = nullptr;

    lua_createtable_t _lua_createtable = nullptr;
    lua_settable_t _lua_settable = nullptr;
    lua_gettable_t _lua_gettable = nullptr;
    lua_rawlen_t   _lua_rawlen   = nullptr;
    
    lua_pushnil_t     _lua_pushnil = nullptr;
    lua_pushnumber_t  _lua_pushnumber = nullptr;
    lua_pushinteger_t _lua_pushinteger = nullptr;
    lua_pushlstring_t _lua_pushlstring = nullptr;
    lua_pushstring_t  _lua_pushstring = nullptr;
    lua_pushboolean_t  _lua_pushboolean = nullptr;
    lua_pushlightuserdata_t _lua_pushlightuserdata = nullptr;
    lua_pushcclosure_t _lua_pushcclosure = nullptr;

    lua_CFunction _lua_error = nullptr;

    lua_tonumberx_t _lua_tonumberx = nullptr;
    lua_tointegerx_t _lua_tointegerx = nullptr;
    lua_toboolean_t _lua_toboolean = nullptr;
    lua_tolstring_t _lua_tolstring = nullptr;
    lua_rawlen_t    _lua_rawlen = nullptr;
    lua_tocfunction_t _lua_tocfunction = nullptr;
    lua_touserdata_t _lua_touserdata = nullptr;
    lua_tothread_t _lua_tothread = nullptr;
    lua_topointer_t _lua_topointer = nullptr;

    void resolve_functions();

public:
    LuaWrapper();

    lua_State* luaL_newstate() const;
    void lua_close(lua_State* L) const;

    int luaL_loadstring(lua_State *L, const char *s) const;
    int luaL_dostring(lua_State* L, const char* s) const;
    int lua_pcallk(lua_State *L, int nargs, int nresults, int errfunc,
                    lua_KContext ctx, lua_KFunction k) const;
    int lua_pcall (lua_State *L, int nargs, int nresults, int errfunc) const;
    void lua_callk(lua_State *L, int nargs, int nresults,
                   lua_KContext ctx, lua_KFunction k) const;
    void lua_call (lua_State *L, int nargs, int nresults) const;
    void luaL_openlibs(lua_State* L) const;

    int lua_type(lua_State *L, int index) const;

    inline int lua_isnil(lua_State *L, int index) const { return lua_type(L, index) == LUA_TNIL ; }
    inline int lua_isboolean(lua_State *L, int index) const { return lua_type(L, index) == LUA_TBOOLEAN ; }
    inline int lua_islightuserdata(lua_State *L, int index) const { return lua_type(L, index) == LUA_TLIGHTUSERDATA ; }
    inline int lua_isnumber(lua_State *L, int index) const { return lua_type(L, index) == LUA_TNUMBER ; }
    inline int lua_isstring(lua_State *L, int index) const { return lua_type(L, index) == LUA_TSTRING ; }
    inline int lua_istable(lua_State *L, int index) const { return lua_type(L, index) == LUA_TTABLE ; }
    inline int lua_isfunction(lua_State *L, int index) const { return lua_type(L, index) == LUA_TFUNCTION ; }
    inline int lua_isuserdata(lua_State *L, int index) const { return lua_type(L, index) == LUA_TUSERDATA ; }
    inline int lua_isthread(lua_State *L, int index) const { return lua_type(L, index) == LUA_TTHREAD ; }

    int luaL_ref(lua_State *L, int t) const;
    void luaL_unref(lua_State *L, int t, int ref) const;

    int lua_absindex(lua_State *L, int idx) const;
    int lua_gettop(lua_State *L) const;
    void lua_settop(lua_State *L, int index) const;
    void lua_pushvalue(lua_State *L, int index) const;
    void lua_rotate(lua_State *L, int index, int n) const;
    void lua_copy(lua_State *L, int fromidx, int toidx) const;
    int  lua_checkstack(lua_State *L, int extra) const;
    void lua_pop(lua_State *L, int n) const;

    int lua_getglobal(lua_State* L, const char* name) const;
    int lua_setglobal(lua_State* L, const char* name) const;

    void lua_newtable(lua_State *L) const;
    void lua_settable(lua_State *L, int idx) const;
    void lua_gettable(lua_State *L, int idx) const;
    size_t lua_rawlen(lua_State *L, int idx) const;

    void lua_pushnil(lua_State *L) const;
    void lua_pushnumber(lua_State *L, lua_Number  n) const;
    void lua_pushinteger(lua_State *L, lua_Integer n) const;
    const char *lua_pushlstring(lua_State *L, const char *s, size_t len) const;
    const char *lua_pushstring(lua_State *L, const char *s) const;
    void lua_pushboolean(lua_State *L, int b) const;
    void lua_pushlightuserdata(lua_State *L, void *p) const;
    void lua_pushcclosure(lua_State *L, lua_CFunction fn, int n) const;

    int lua_error(lua_State *L) const;

    lua_Number lua_tonumberx(lua_State *L, int idx, int *isnum) const;
    lua_Integer lua_tointegerx(lua_State *L, int idx, int *isnum) const;
    int lua_toboolean(lua_State *L, int idx) const;
    const char *lua_tolstring(lua_State *L, int idx, size_t *len) const;
    const char *lua_tostring(lua_State *L, int idx) const;
    lua_CFunction lua_tocfunction(lua_State *L, int idx) const;
    void *lua_touserdata(lua_State *L, int idx) const;
    lua_State *lua_tothread(lua_State *L, int idx) const;
    const void *lua_topointer(lua_State *L, int idx) const;

    lua_Number lua_checknumber(lua_State *L, int idx) const;
    lua_Integer lua_checkinteger(lua_State *L, int idx) const;
    const char* lua_checkstring(lua_State *L, int idx) const;

    operator bool() const;
};

extern const LuaWrapper lua_wrapper;

#endif // _LUA_WRAPPER_H_