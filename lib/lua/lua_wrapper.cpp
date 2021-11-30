#include <stdexcept>
#include <iostream>
#include "lua/lua_wrapper.h"
using namespace std;

#if defined(linux) || defined(BSD) || defined(__APPLE__)
#define LIB_SUFFIX ".so"
#elif defined(_WIN32) || defined(_WIN64)
#define LIB_SUFFIX ".dll"
#endif

const LuaWrapper lua_wrapper;

LuaWrapper::LuaWrapper()
{
    lhandle_t handle = loader_load("lua" LIB_SUFFIX);
    if (handle == nullptr)
        handle = loader_load("lua51" LIB_SUFFIX);
    if (handle == nullptr)
        handle = loader_load("lua52" LIB_SUFFIX);
    if (handle == nullptr)
        handle = loader_load("lua53" LIB_SUFFIX);
    
    if (handle != nullptr) {
        this->library_handle = std::shared_ptr<lhandle_t>(new lhandle_t(handle), [](lhandle_t* handle) {
            loader_unload(*handle);
        });
    }

    try {
        this->resolve_functions();
    } catch (std::runtime_error & e) {
        this->library_handle.reset();
        cerr << "Failed to load lua library: " << e.what() << endl;
    }
}

void LuaWrapper::resolve_functions() {
    if (this->library_handle == nullptr)
        return;

    this->_luaL_newstate = (luaL_newstate_t)loader_resolve_symbol(*this->library_handle, "luaL_newstate");
    this->_lua_close = (lua_close_t)loader_resolve_symbol(*this->library_handle, "lua_close");

    this->_luaL_loadstring = (luaL_loadstring_t)loader_resolve_symbol(*this->library_handle, "luaL_loadstring");
    this->_lua_pcallk = (lua_pcallk_t)loader_resolve_symbol(*this->library_handle, "lua_pcallk");
    this->_lua_callk = (lua_callk_t)loader_resolve_symbol(*this->library_handle, "lua_callk");
    this->_luaL_openlibs = (lua_CFunction)loader_resolve_symbol(*this->library_handle, "luaL_openlibs");

    this->_lua_type = (lua_type_t)loader_resolve_symbol(*this->library_handle, "lua_type");

    this->_luaL_ref = (luaL_ref_t)loader_resolve_symbol(*this->library_handle, "luaL_ref");
    this->_luaL_unref = (luaL_unref_t)loader_resolve_symbol(*this->library_handle, "luaL_unref");

    this->_lua_absindex = (lua_absindex_t)loader_resolve_symbol(*this->library_handle, "lua_absindex");
    this->_lua_gettop = (lua_gettop_t)loader_resolve_symbol(*this->library_handle, "lua_gettop");
    this->_lua_settop = (lua_settop_t)loader_resolve_symbol(*this->library_handle, "lua_settop");
    this->_lua_pushvalue = (lua_pushvalue_t)loader_resolve_symbol(*this->library_handle, "lua_pushvalue");
    this->_lua_rotate = (lua_rotate_t)loader_resolve_symbol(*this->library_handle, "lua_rotate");
    this->_lua_copy = (lua_copy_t)loader_resolve_symbol(*this->library_handle, "lua_copy");
    this->_lua_checkstack = (lua_checkstack_t)loader_resolve_symbol(*this->library_handle, "lua_checkstack");

    this->_lua_getglobal = (lua_getglobal_t)loader_resolve_symbol(*this->library_handle, "lua_getglobal");
    this->_lua_setglobal = (lua_setglobal_t)loader_resolve_symbol(*this->library_handle, "lua_setglobal");

    this->_lua_createtable = (lua_createtable_t)loader_resolve_symbol(*this->library_handle, "lua_createtable");
    this->_lua_gettable = (lua_gettable_t)loader_resolve_symbol(*this->library_handle, "lua_gettable");
    this->_lua_gettable = (lua_gettable_t)loader_resolve_symbol(*this->library_handle, "lua_gettable");
    this->_lua_rawlen   = (lua_rawlen_t)loader_resolve_symbol(*this->library_handle, "lua_rawlen");

    this->_lua_pushnil = (lua_pushnil_t)loader_resolve_symbol(*this->library_handle, "lua_pushnil");
    this->_lua_pushnumber = (lua_pushnumber_t)loader_resolve_symbol(*this->library_handle, "lua_pushnumber");
    this->_lua_pushinteger = (lua_pushinteger_t)loader_resolve_symbol(*this->library_handle, "lua_pushinteger");
    this->_lua_pushlstring = (lua_pushlstring_t)loader_resolve_symbol(*this->library_handle, "lua_pushlstring");
    this->_lua_pushstring = (lua_pushstring_t)loader_resolve_symbol(*this->library_handle, "lua_pushstring");
    this->_lua_pushboolean = (lua_pushboolean_t)loader_resolve_symbol(*this->library_handle, "lua_pushboolean");
    this->_lua_pushlightuserdata = (lua_pushlightuserdata_t)loader_resolve_symbol(*this->library_handle, "lua_pushlightuserdata");
    this->_lua_pushcclosure = (lua_pushcclosure_t)loader_resolve_symbol(*this->library_handle, "lua_pushcclosure");
}

lua_State* LuaWrapper::luaL_newstate() const {
    if (this->library_handle == nullptr)
        throw std::runtime_error("luaL_newstate not found");
    return this->_luaL_newstate();
}

void LuaWrapper::lua_close(lua_State* L) const {
    if (this->library_handle == nullptr)
        throw std::runtime_error("lua_close not found");
    this->_lua_close(L);
}

int LuaWrapper::luaL_loadstring(lua_State* L, const char* s) const {
    if (this->library_handle == nullptr)
        throw std::runtime_error("luaL_loadstring not found");
    return this->_luaL_loadstring(L, s);
}

int LuaWrapper::luaL_dostring(lua_State* L, const char* s) const {
    if (this->library_handle == nullptr)
        throw std::runtime_error("luaL_dostring not found");
    return this->luaL_loadstring(L, s) || this->lua_pcall(L, 0, -1, 0);
}

int LuaWrapper::lua_pcallk(lua_State* L, int nargs, int nresults, int errfunc, lua_KContext ctx, lua_KFunction k) const {
    if (this->library_handle == nullptr)
        throw std::runtime_error("lua_pcallk not found");
    return this->_lua_pcallk(L, nargs, nresults, errfunc, ctx, k);
}

int LuaWrapper::lua_pcall(lua_State* L, int nargs, int nresults, int errfunc) const {
    if (this->library_handle == nullptr)
        throw std::runtime_error("lua_pcall not found");
    return this->_lua_pcallk(L, nargs, nresults, errfunc, 0, nullptr);
}

void LuaWrapper::lua_callk(lua_State* L, int nargs, int nresults, lua_KContext ctx, lua_KFunction k) const {
    if (this->library_handle == nullptr)
        throw std::runtime_error("lua_callk not found");
    this->_lua_callk(L, nargs, nresults, ctx, k);
}

void LuaWrapper::lua_call(lua_State* L, int nargs, int nresults) const {
    if (this->library_handle == nullptr)
        throw std::runtime_error("lua_call not found");
    this->_lua_callk(L, nargs, nresults, 0, nullptr);
}

void LuaWrapper::luaL_openlibs(lua_State* L) const {
    if (this->library_handle == nullptr)
        throw std::runtime_error("luaL_openlibs not found");
    this->_luaL_openlibs(L);
}

int LuaWrapper::lua_type(lua_State* L, int index) const {
    if (this->library_handle == nullptr)
        throw std::runtime_error("lua_type not found");
    return this->_lua_type(L, index);
}

int LuaWrapper::luaL_ref(lua_State* L, int registry) const {
    if (this->library_handle == nullptr)
        throw std::runtime_error("luaL_ref not found");
    return this->_luaL_ref(L, registry);
}

void LuaWrapper::luaL_unref(lua_State* L, int registry, int ref) const {
    if (this->library_handle == nullptr)
        throw std::runtime_error("luaL_unref not found");
    this->_luaL_unref(L, registry, ref);
}

int LuaWrapper::lua_absindex(lua_State *L, int idx) const {
    if (this->library_handle == nullptr)
        throw std::runtime_error("lua_absindex not found");
    return this->_lua_absindex(L, idx);
}
int LuaWrapper::lua_gettop(lua_State *L) const {
    if (this->library_handle == nullptr)
        throw std::runtime_error("lua_gettop not found");
    return this->_lua_gettop(L);
}
void LuaWrapper::lua_settop(lua_State *L, int index) const {
    if (this->library_handle == nullptr)
        throw std::runtime_error("lua_settop not found");
    this->_lua_settop(L, index);
}
void LuaWrapper::lua_pushvalue(lua_State *L, int index) const {
    if (this->library_handle == nullptr)
        throw std::runtime_error("lua_pushvalue not found");
    this->_lua_pushvalue(L, index);
}
void LuaWrapper::lua_rotate(lua_State *L, int index, int n) const {
    if (this->library_handle == nullptr)
        throw std::runtime_error("lua_rotate not found");
    this->_lua_rotate(L, index, n);
}
void LuaWrapper::lua_copy(lua_State *L, int fromidx, int toidx) const {
    if (this->library_handle == nullptr)
        throw std::runtime_error("lua_copy not found");
    this->_lua_copy(L, fromidx, toidx);
}
int LuaWrapper::lua_checkstack(lua_State *L, int extra) const {
    if (this->library_handle == nullptr)
        throw std::runtime_error("lua_checkstack not found");
    return this->_lua_checkstack(L, extra);
}

void LuaWrapper::lua_pop(lua_State *L, int n) const {
    if (this->library_handle == nullptr)
        throw std::runtime_error("lua_pop not found");
    this->lua_settop(L, -n - 1);
}

int LuaWrapper::lua_getglobal(lua_State* L, const char* name) const {
    if (this->library_handle == nullptr)
        throw std::runtime_error("lua_getglobal not found");
    return this->_lua_getglobal(L, name);
}

int LuaWrapper::lua_setglobal(lua_State* L, const char* name) const {
    if (this->library_handle == nullptr)
        throw std::runtime_error("lua_setglobal not found");
    return this->_lua_setglobal(L, name);
}

void LuaWrapper::lua_newtable(lua_State* L) const {
    if (this->library_handle == nullptr)
        throw std::runtime_error("lua_newtable not found");
    this->_lua_createtable(L, 0, 0);
}

void LuaWrapper::lua_settable(lua_State* L, int idx) const {
    if (this->library_handle == nullptr)
        throw std::runtime_error("lua_settable not found");
    this->_lua_settable(L, idx);
}

void LuaWrapper::lua_gettable(lua_State* L, int idx) const {
    if (this->library_handle == nullptr)
        throw std::runtime_error("lua_gettable not found");
    this->_lua_gettable(L, idx);
}

size_t LuaWrapper::lua_rawlen(lua_State* L, int idx) const {
    if (this->library_handle == nullptr)
        throw std::runtime_error("lua_rawlen not found");
    return this->_lua_rawlen(L, idx);
}

void LuaWrapper::lua_pushnil(lua_State* L) const {
    if (this->library_handle == nullptr)
        throw std::runtime_error("lua_pushnil not found");
    this->_lua_pushnil(L);
}

void LuaWrapper::lua_pushnumber(lua_State* L, lua_Number  n) const {
    if (this->library_handle == nullptr)
        throw std::runtime_error("lua_pushnumber not found");
    this->_lua_pushnumber(L, n);
}

void LuaWrapper::lua_pushinteger(lua_State* L, lua_Integer n) const {
    if (this->library_handle == nullptr)
        throw std::runtime_error("lua_pushinteger not found");
    this->_lua_pushinteger(L, n);
}

const char* LuaWrapper::lua_pushlstring(lua_State* L, const char* s, size_t len) const {
    if (this->library_handle == nullptr)
        throw std::runtime_error("lua_pushlstring not found");
    return this->_lua_pushlstring(L, s, len);
}

const char* LuaWrapper::lua_pushstring(lua_State* L, const char* s) const {
    if (this->library_handle == nullptr)
        throw std::runtime_error("lua_pushstring not found");
    return this->_lua_pushstring(L, s);
}

void LuaWrapper::lua_pushboolean(lua_State* L, int b) const {
    if (this->library_handle == nullptr)
        throw std::runtime_error("lua_pushboolean not found");
    this->_lua_pushboolean(L, b);
}

void LuaWrapper::lua_pushlightuserdata(lua_State* L, void* p) const {
    if (this->library_handle == nullptr)
        throw std::runtime_error("lua_pushlightuserdata not found");
    this->_lua_pushlightuserdata(L, p);
}   

void LuaWrapper::lua_pushcclosure(lua_State* L, lua_CFunction fn, int n) const {
    if (this->library_handle == nullptr)
        throw std::runtime_error("lua_pushcclosure not found"); 
    this->_lua_pushcclosure(L, fn, n);
}

int LuaWrapper::lua_error(lua_State *L) const {
    if (this->library_handle == nullptr)
        throw std::runtime_error("lua_error not found");
    return this->_lua_error(L);
}

lua_Number LuaWrapper::lua_tonumberx(lua_State *L, int idx, int *isnum) const {
    if (this->library_handle == nullptr)
        throw std::runtime_error("lua_tonumberx not found");
    return this->_lua_tonumberx(L, idx, isnum);
}
lua_Integer LuaWrapper::lua_tointegerx(lua_State *L, int idx, int *isnum) const {
    if (this->library_handle == nullptr)
        throw std::runtime_error("lua_tointegerx not found");
    return this->_lua_tointegerx(L, idx, isnum);
}
int LuaWrapper::lua_toboolean(lua_State *L, int idx) const {
    if (this->library_handle == nullptr)
        throw std::runtime_error("lua_toboolean not found");
    return this->_lua_toboolean(L, idx);
}
const char* LuaWrapper::lua_tolstring(lua_State *L, int idx, size_t *len) const {
    if (this->library_handle == nullptr)
        throw std::runtime_error("lua_tolstring not found");
    return this->_lua_tolstring(L, idx, len);
}
const char* LuaWrapper::lua_tostring(lua_State *L, int idx) const {
    size_t len;
    return this->lua_tolstring(L, idx, &len);
}
lua_CFunction LuaWrapper::lua_tocfunction(lua_State *L, int idx) const {
    if (this->library_handle == nullptr)
        throw std::runtime_error("lua_tocfunction not found");
    return this->_lua_tocfunction(L, idx);
}
void* LuaWrapper::lua_touserdata(lua_State *L, int idx) const {
    if (this->library_handle == nullptr)
        throw std::runtime_error("lua_touserdata not found");
    return this->_lua_touserdata(L, idx);
}
lua_State* LuaWrapper::lua_tothread(lua_State *L, int idx) const {
    if (this->library_handle == nullptr)
        throw std::runtime_error("lua_tothread not found");
    return this->_lua_tothread(L, idx);
}
const void* LuaWrapper::lua_topointer(lua_State *L, int idx) const {
    if (this->library_handle == nullptr)
        throw std::runtime_error("lua_topointer not found");
    return this->_lua_topointer(L, idx);
}

lua_Number LuaWrapper::lua_checknumber(lua_State *L, int idx) const {
    int isnum;
    auto ret = this->lua_tonumberx(L, idx, &isnum);
    if (!isnum) {
        this->lua_pushstring(L, "number expected");
        return this->lua_error(L);
    }

    return ret;
}
lua_Integer LuaWrapper::lua_checkinteger(lua_State *L, int idx) const {
    int isnum;
    auto ret = this->lua_tointegerx(L, idx, &isnum);
    if (!isnum) {
        this->lua_pushstring(L, "integer expected");
        return this->lua_error(L);
    }
    return ret;
}
const char* LuaWrapper::lua_checkstring(lua_State *L, int idx) const {
    if (!this->lua_isstring(L, idx)) {
        this->lua_pushstring(L, "string expected");
        this->lua_error(L);
        return nullptr;
    }

    return this->lua_tostring(L, idx);
}

LuaWrapper::operator bool() const {
    return this->library_handle != nullptr;
}
