#include "sreduce/lua_reducer.h"
#include "sreduce/reducer_factory.h"
#include "utils.hpp"
#include <stdexcept>
using namespace std;
using namespace ReducerFactory;



/**
 * the lua object should implement the following methods:
 *     void init(string params)
 *     void reduce(int off, string str);
 *     vector<pair<int, string>> reduce_end();
 */
class LuaReducer : public StringReducer {
private:
    lua_State *L;
    int obj_ref;

public:
    LuaReducer(lua_State* L, int obj_ref, const string& params): L(L), obj_ref(obj_ref) {
        auto t = lua_wrapper.lua_gettop(L);
        defer([&]() { lua_wrapper.lua_settop(L, t); });

        lua_wrapper.lua_rawgeti(L, LuaWrapper::LUA_REGISTRYINDEX, obj_ref);
        lua_wrapper.lua_pushstring(L, "init");
        lua_wrapper.lua_gettable(L, -2);
        if (lua_wrapper.lua_isnil(L, -1))
            return;

        if (!lua_wrapper.lua_isfunction(L, -1))
            throw runtime_error("init function is not a function");

        lua_wrapper.lua_pushvalue(L, -2);
        lua_wrapper.lua_pushstring(L, params.c_str());
        if (lua_wrapper.lua_pcall(L, 2, 0, 0) != LuaWrapper::LUA_OK)
            throw runtime_error("init function failed");
    }
    virtual ~LuaReducer() = default;

    virtual void reduce(const std::pair<size_t,std::string>& pair) override {
        auto t = lua_wrapper.lua_gettop(L);
        defer([&]() { lua_wrapper.lua_settop(L, t); });

        lua_wrapper.lua_rawgeti(L, LuaWrapper::LUA_REGISTRYINDEX, obj_ref);
        lua_wrapper.lua_pushstring(L, "reduce");
        lua_wrapper.lua_gettable(L, -2);
        if (!lua_wrapper.lua_isfunction(L, -1))
            throw std::runtime_error("reduce function not found");

        lua_wrapper.lua_pushvalue(L, -2);
        lua_wrapper.lua_pushinteger(L, pair.first);
        lua_wrapper.lua_pushstring(L, pair.second.c_str());
        if (lua_wrapper.lua_pcall(L, 3, 0, 0) != LuaWrapper::LUA_OK)
            throw std::runtime_error("reduce function error");
    }

    virtual std::vector<std::pair<size_t,std::string>> reduce_end() override {
        auto t = lua_wrapper.lua_gettop(L);
        defer([&]() { lua_wrapper.lua_settop(L, t); });
        lua_wrapper.lua_checkstack(L, 4);

        lua_wrapper.lua_rawgeti(L, LuaWrapper::LUA_REGISTRYINDEX, obj_ref);
        lua_wrapper.lua_pushstring(L, "reduce_end");
        lua_wrapper.lua_gettable(L, -2);
        if (!lua_wrapper.lua_isfunction(L, -1))
            throw std::runtime_error("reduce_end function not found");

        lua_wrapper.lua_pushvalue(L, -2);
        if (lua_wrapper.lua_pcall(L, 1, 1, 0) != LuaWrapper::LUA_OK)
            throw std::runtime_error("reduce_end function error");

        std::vector<std::pair<size_t,std::string>> ret;
        if (!lua_wrapper.lua_istable(L, -1))
            throw std::runtime_error("reduce_end function return value is not a array");

        size_t len = lua_wrapper.lua_rawlen(L, -1);
        for (size_t i = 1; i <= len; i++) {
            lua_wrapper.lua_rawgeti(L, -1, i);
            if (lua_wrapper.lua_istable(L, -1)) {
                if (lua_wrapper.lua_rawlen(L, -1) != 2)
                    throw std::runtime_error("reduce_end function return value is not a array of length 2");

                lua_wrapper.lua_rawgeti(L, -1, 1);
                lua_wrapper.lua_rawgeti(L, -2, 2);

                if (lua_wrapper.lua_isinteger(L, -2) && lua_wrapper.lua_isstring(L, -1)) {
                    ret.push_back(std::make_pair(lua_wrapper.lua_tointeger(L, -2), lua_wrapper.lua_tostring(L, -1)));
                    lua_wrapper.lua_pop(L, 2);
                } else {
                    throw std::runtime_error("reduce_end function return value is not a array of integer and string");
                }
            } else if (lua_wrapper.lua_isstring(L, -1)) {
                ret.push_back(make_pair(0, lua_wrapper.lua_tostring(L, -1)));
            } else {
                throw std::runtime_error("reduce_end function return value is not a array of integer and string");
            }
            lua_wrapper.lua_pop(L, 1);
        }

        return ret;
    }
};

class LuaReducerGenerator : public ReducerGenerator {
private:
    lua_State *L;
    int gen_ref;

public:
    LuaReducerGenerator(lua_State* L, int gen_ref): L(L), gen_ref(gen_ref) {}
    virtual ~LuaReducerGenerator() = default;

    virtual unique_ptr<StringReducer> operator()(const std::string& params) override {
        auto t = lua_wrapper.lua_gettop(L);
        defer([&]() { lua_wrapper.lua_settop(L, t); });

        lua_wrapper.lua_rawgeti(L, LuaWrapper::LUA_REGISTRYINDEX, gen_ref);
        if (lua_wrapper.lua_pcall(L, 0, 1, 0) != LuaWrapper::LUA_OK)
            throw runtime_error("lua reducer generator function error");

        if (!lua_wrapper.lua_istable(L, -1))
            throw runtime_error("lua reducer generator function return value is not a table");

        auto objref = lua_wrapper.luaL_ref(L, LuaWrapper::LUA_REGISTRYINDEX);
        return unique_ptr<StringReducer>(new LuaReducer(L, objref, params));
    }
};

int lua_register_reducer(lua_State *L) {
    LUA_EXCEPTION_BEGIN();

    if (lua_wrapper.lua_gettop(L) != 4)
        throw runtime_error("wrong number of arguments");
    string encoding  = lua_wrapper.lua_checkstring_except(L, 1);
    string filter_name = lua_wrapper.lua_checkstring_except(L, 2);
    string desc = lua_wrapper.lua_checkstring_except(L, 3);
    if (!lua_wrapper.lua_isfunction(L, 4))
        throw runtime_error("mapper function must be a function");

    int ref = lua_wrapper.luaL_ref(L, LuaWrapper::LUA_REGISTRYINDEX);
    register_reducer(encoding, filter_name, desc,
                    make_shared<LuaReducerGenerator>(L, ref));
    return 0;
    LUA_EXCEPTION_END();
}

