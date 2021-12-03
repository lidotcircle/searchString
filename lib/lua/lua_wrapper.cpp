#include <stdexcept>
#include <iostream>
#include <assert.h>
#include "lua/lua_wrapper.h"
using namespace std;

#if defined(linux) || defined(BSD) || defined(__APPLE__)
#define LIB_SUFFIX ".so"
#define LIB_PREFIX "lib"
#elif defined(_WIN32) || defined(_WIN64)
#define LIB_SUFFIX ".dll"
#define LIB_PREFIX ""
#endif

const LuaWrapper lua_wrapper;
constexpr static const char* libluanames[] = {
    "lua", "lua51", "lua52", "lua53"
    "lua5.1", "lua5.2", "lua5.3"
};

LuaWrapper::LuaWrapper()
{
    lhandle_t handle = nullptr;
    auto nnames = sizeof(libluanames) / sizeof(libluanames[0]);
    for (size_t i=0;i<nnames;i++) {
        auto name = libluanames[i];
        try {
            handle = loader_load(LIB_PREFIX + string(name) + LIB_SUFFIX);
            break;
        } catch (runtime_error&) { }
    }
   
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

LuaWrapper::~LuaWrapper()
{
    for(auto L: this->l_states)
        this->lua_close(L);
}

void LuaWrapper::resolve_functions() {
    if (this->library_handle == nullptr)
        return;

#define LFENTRY(rettype, funcname, ...) \
    this->_ ## funcname = (rettype (*)(__VA_ARGS__))loader_resolve_symbol(*this->library_handle, #funcname);
LUA_FUNCTION_LIST
#undef LFENTRY
}

static const char* NX_REGISTRY = "__NX_REGISTRY";
lua_State* LuaWrapper::luaL_newstate() const {
    if (this->library_handle == nullptr)
        throw std::runtime_error("luaL_newstate not found");
    auto state = this->_luaL_newstate();
    this->lua_newtable(state);
    this->lua_setglobal(state, NX_REGISTRY);
    return state;
}

void LuaWrapper::lua_close(lua_State* L) const {
    if (this->library_handle == nullptr)
        throw std::runtime_error("lua_close not found");
    this->_lua_close(L);
}

void LuaWrapper::luaL_openlibs(lua_State* L) const {
    if (this->library_handle == nullptr)
        throw std::runtime_error("luaL_openlibs not found");
    this->_luaL_openlibs(L);
}

int LuaWrapper::luaL_loadstring(lua_State* L, const char* s) const {
    if (this->library_handle == nullptr)
        throw std::runtime_error("luaL_loadstring not found");
    return this->_luaL_loadstring(L, s);
}

int LuaWrapper::lua_pcallk(lua_State* L, int nargs, int nresults, int errfunc, lua_KContext ctx, lua_KFunction k) const {
    if (this->library_handle == nullptr)
        throw std::runtime_error("lua_pcallk not found");
    return this->_lua_pcallk(L, nargs, nresults, errfunc, ctx, k);
}

void LuaWrapper::lua_callk(lua_State* L, int nargs, int nresults, lua_KContext ctx, lua_KFunction k) const {
    if (this->library_handle == nullptr)
        throw std::runtime_error("lua_callk not found");
    this->_lua_callk(L, nargs, nresults, ctx, k);
}

int LuaWrapper::lua_type(lua_State* L, int index) const {
    if (this->library_handle == nullptr)
        throw std::runtime_error("lua_type not found");
    return this->_lua_type(L, index);
}

int LuaWrapper::luaL_ref(lua_State* L, int registry) const {
    if (this->library_handle == nullptr)
        throw std::runtime_error("luaL_ref not found");

    if (registry == LuaWrapper::LUA_REGISTRYINDEX) {
        this->lua_checkstack(L, 2);
        this->lua_getglobal(L, NX_REGISTRY);
        this->lua_pushvalue(L, -2);
        auto n = this->luaL_ref(L, -2);
        this->lua_pop(L, 2);
        return n;
    } else {
        return this->_luaL_ref(L, registry);
    }
}

void LuaWrapper::luaL_unref(lua_State* L, int registry, int ref) const {
    if (this->library_handle == nullptr)
        throw std::runtime_error("luaL_unref not found");

    if (registry == LuaWrapper::LUA_REGISTRYINDEX) {
        this->lua_checkstack(L, 2);
        this->lua_getglobal(L, NX_REGISTRY);
        this->luaL_unref(L, -1, ref);
        this->lua_pop(L, 1);
    } else {
        this->_luaL_unref(L, registry, ref);
    }
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

void LuaWrapper::lua_createtable(lua_State* L, int narr, int nrec) const {
    if (this->library_handle == nullptr)
        throw std::runtime_error("lua_createtable not found");
    this->_lua_createtable(L, narr, nrec);
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

int LuaWrapper::lua_getfield(lua_State* L, int idx, const char* k) const {
    if (this->library_handle == nullptr)
        throw std::runtime_error("lua_getfield not found");
    return this->_lua_getfield(L, idx, k);
}

int LuaWrapper::lua_geti(lua_State* L, int idx, lua_Integer n) const {
    if (this->library_handle == nullptr)
        throw std::runtime_error("lua_geti not found");

    if (idx == LuaWrapper::LUA_REGISTRYINDEX) {
        this->lua_checkstack(L, 2);
        this->lua_getglobal(L, NX_REGISTRY);
        auto r = this->_lua_geti(L, -1, n);
        this->lua_rotate(L, -2, -1);
        this->lua_pop(L, 1);
        return r;
    } else {
        return this->_lua_geti(L, idx, n);
    }
}

int LuaWrapper::lua_rawget(lua_State* L, int idx) const {
    if (this->library_handle == nullptr)
        throw std::runtime_error("lua_rawget not found");
    return this->_lua_rawget(L, idx);
}

int LuaWrapper::lua_rawgeti(lua_State* L, int idx, lua_Integer n) const {
    if (this->library_handle == nullptr)
        throw std::runtime_error("lua_rawgeti not found");

    if (idx == LuaWrapper::LUA_REGISTRYINDEX) {
        this->lua_checkstack(L, 2);
        this->lua_getglobal(L, NX_REGISTRY);
        auto r = this->_lua_rawgeti(L, -1, n);
        this->lua_rotate(L, -2, -1);
        this->lua_pop(L, 1);
        return r;
    } else {
        return this->_lua_rawgeti(L, idx, n);
    }
}

void LuaWrapper::lua_seti(lua_State* L, int idx, lua_Integer n) const {
    if (this->library_handle == nullptr)
        throw std::runtime_error("lua_seti not found");

    this->_lua_seti(L, idx, n);
}

void LuaWrapper::lua_rawset(lua_State* L, int idx) const {
    if (this->library_handle == nullptr)
        throw std::runtime_error("lua_rawset not found");
    return this->_lua_rawset(L, idx);
}

void LuaWrapper::lua_rawseti(lua_State* L, int idx, lua_Integer n) const {
    if (this->library_handle == nullptr)
        throw std::runtime_error("lua_rawseti not found");
    return this->_lua_rawseti(L, idx, n);
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


LuaWrapper::operator bool() const {
    return this->library_handle != nullptr;
}


lua_State* LuaWrapper::luaL_newstate_close_by_wrapper() const {
    lua_State* state = this->luaL_newstate();
    auto _this = const_cast<LuaWrapper*>(this);
    _this->l_states.push_back(state);
    return state;
}

int LuaWrapper::luaL_dostring(lua_State* L, const char* s) const {
    return this->luaL_loadstring(L, s) || this->lua_pcall(L, 0, -1, 0);
}

int LuaWrapper::lua_pcall(lua_State* L, int nargs, int nresults, int errfunc) const {
    return this->_lua_pcallk(L, nargs, nresults, errfunc, 0, nullptr);
}

void LuaWrapper::lua_call(lua_State* L, int nargs, int nresults) const {
    this->_lua_callk(L, nargs, nresults, 0, nullptr);
}

lua_Integer LuaWrapper::lua_tointeger(lua_State* L, int idx) const {
    int isnum;
    int v = this->_lua_tointegerx(L, idx, &isnum);
    if (!isnum)
        throw std::runtime_error("lua_tointeger: not a number");
    return v;
}

void LuaWrapper::lua_newtable(lua_State* L) const {
    this->lua_createtable(L, 0, 0);
}

void LuaWrapper::lua_pop(lua_State *L, int n) const {
    this->lua_settop(L, -n - 1);
}

int LuaWrapper::lua_error_except(lua_State *L) const {
    if (this->library_handle == nullptr)
        throw std::runtime_error("lua_error_except not found");

    assert(this->lua_isstring(L, -1));
    auto err = this->lua_tostring(L, -1);
    throw std::runtime_error(err);
}

const char* LuaWrapper::lua_tostring(lua_State *L, int idx) const {
    size_t len;
    return this->lua_tolstring(L, idx, &len);
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

lua_Number LuaWrapper::lua_checknumber_except(lua_State *L, int idx) const {
    int isnum;
    auto ret = this->lua_tonumberx(L, idx, &isnum);
    if (!isnum) {
        this->lua_pushstring(L, "number expected");
        return this->lua_error_except(L);
    }

    return ret;
}
lua_Integer LuaWrapper::lua_checkinteger_except(lua_State *L, int idx) const {
    int isnum;
    auto ret = this->lua_tointegerx(L, idx, &isnum);
    if (!isnum) {
        this->lua_pushstring(L, "integer expected");
        return this->lua_error_except(L);
    }
    return ret;
}
const char* LuaWrapper::lua_checkstring_except(lua_State *L, int idx) const {
    if (!this->lua_isstring(L, idx)) {
        this->lua_pushstring(L, "string expected");
        this->lua_error_except(L);
        return nullptr;
    }

    return this->lua_tostring(L, idx);
}

