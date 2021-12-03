#include "sfinder/lua_finder.h"
#include "sfinder/string_finder.h"
#include "sfinder/finder_factory.h"
#include "utils.hpp"
#include <vector>
#include <string>
using namespace std;
using namespace FinderFactory;


class LuaFinder: public StringFinder {
private:
    vector<pair<size_t,string>> outputs;
    lua_State* L;
    int obj_ref;
    void handle_return_value();

protected:
    virtual std::vector<std::pair<size_t,std::string>>& pre_fetch() override;

public:
    virtual void feed_char(unsigned char c) override;
    virtual void feed_end() override;

    LuaFinder(lua_State* L, int obj_ref, const string& params);
    virtual ~LuaFinder();
};


LuaFinder::LuaFinder(lua_State* L, int obj_ref, const string& params): L(L), obj_ref(obj_ref)
{
    auto top = lua_wrapper.lua_gettop(L);
    defer([&](){lua_wrapper.lua_settop(L,top);});

    lua_wrapper.lua_rawgeti(L, LuaWrapper::LUA_REGISTRYINDEX, obj_ref);
    lua_wrapper.lua_pushstring(L, "init");
    lua_wrapper.lua_gettable(L, -2);

    if (lua_wrapper.lua_isnil(L, -1))
        return;

    if (!lua_wrapper.lua_isfunction(L, -1))
        throw std::runtime_error("init() must be a function");

    lua_wrapper.lua_pushvalue(L, -2);
    lua_wrapper.lua_pushstring(L, params.c_str());
    if (lua_wrapper.lua_pcall(L, 2, 0, 0) != LuaWrapper::LUA_OK) {
        throw std::runtime_error("lua finder object init failed: " +
                                 string(lua_wrapper.lua_tostring(L, -1)));
    }
}

LuaFinder::~LuaFinder()
{
    lua_wrapper.luaL_unref(L, LuaWrapper::LUA_REGISTRYINDEX, obj_ref);
}

vector<pair<size_t,string>>& LuaFinder::pre_fetch() { return this->outputs; }

void LuaFinder::feed_char(unsigned char c)
{
    auto top = lua_wrapper.lua_gettop(L);
    defer([&](){lua_wrapper.lua_settop(L,top);});


    lua_wrapper.lua_rawgeti(L, LuaWrapper::LUA_REGISTRYINDEX, obj_ref);
    lua_wrapper.lua_pushstring(L, "feed");
    lua_wrapper.lua_gettable(L, -2);

    if (!lua_wrapper.lua_isfunction(L, -1))
        throw std::runtime_error("feed(char) must be implemented");

    lua_wrapper.lua_pushvalue(L, -2);
    lua_wrapper.lua_pushinteger(L, c);
    if (lua_wrapper.lua_pcall(L, 2, 2, 0) != LuaWrapper::LUA_OK) {
        throw std::runtime_error("lua finder object feed(c) failed: " +
                                 string(lua_wrapper.lua_tostring(L, -1)));
    }

    this->handle_return_value();
}

void LuaFinder::feed_end()
{
    auto top = lua_wrapper.lua_gettop(L);
    defer([&](){lua_wrapper.lua_settop(L,top);});


    lua_wrapper.lua_rawgeti(L, LuaWrapper::LUA_REGISTRYINDEX, obj_ref);
    lua_wrapper.lua_pushstring(L, "end");
    lua_wrapper.lua_gettable(L, -2);

    if (!lua_wrapper.lua_isfunction(L, -1))
        throw std::runtime_error("end() must be implemented");

    lua_wrapper.lua_pushvalue(L, -2);
    if (lua_wrapper.lua_pcall(L, 1, 2, 0) != LuaWrapper::LUA_OK) {
        throw std::runtime_error("lua finder object end() failed: " +
                                 string(lua_wrapper.lua_tostring(L, -1)));
    }

    this->handle_return_value();
}

void LuaFinder::handle_return_value() {
    if (lua_wrapper.lua_isnil(L, -2))
        return;

    auto ff = [&]() {
        if (lua_wrapper.lua_isstring(L, -2)) {
            string str = lua_wrapper.lua_tostring(L, -2);
            this->outputs.push_back(make_pair(0, str));
        } else if (lua_wrapper.lua_isinteger(L, -2)) {
            if (!lua_wrapper.lua_isstring(L, -1))
                throw std::runtime_error("first value is integer, expect second be string");
            size_t pos = lua_wrapper.lua_tointeger(L, -2);
            string str = lua_wrapper.lua_tostring(L, -1);
            this->outputs.push_back(make_pair(pos, str));
        } else {
            throw std::runtime_error("unexpected value type");
        }
    };

    if (lua_wrapper.lua_istable(L, -2)) {
        size_t len = lua_wrapper.lua_rawlen(L, -2);
        for (size_t i=1;i<len;i++) {
            lua_wrapper.lua_rawgeti(L, -2, i);

            if (lua_wrapper.lua_isstring(L, -1)) {
                lua_wrapper.lua_pushnil(L);
                ff();
                lua_wrapper.lua_pop(L, 2);
            } else if (lua_wrapper.lua_istable(L, -1)) {
                if (lua_wrapper.lua_rawlen(L, -1) != 2)
                    throw std::runtime_error("should be a tuple of integer and string");
                lua_wrapper.lua_rawgeti(L, -1, 1);
                lua_wrapper.lua_rawgeti(L, -1, 2);

                if (lua_wrapper.lua_isinteger(L, -2) && lua_wrapper.lua_isstring(L, -1)) {
                    ff();
                    lua_wrapper.lua_pop(L, 2);
                } else {
                    throw std::runtime_error("should be a tuple of integer and string");
                }
                lua_wrapper.lua_pop(L, 1);
            }
        }
    } else {
        ff();
    }
}

class LuaFinderGenerator: public FinderGenerator {
private:
    lua_State *L;
    int gen_ref;

public:
    LuaFinderGenerator(lua_State* L, int gen_ref): L(L), gen_ref(gen_ref) {}
    virtual ~LuaFinderGenerator() = default;

    virtual unique_ptr<StringFinder> operator()(const std::string& params) override {
        auto t = lua_wrapper.lua_gettop(L);
        defer([&]() { lua_wrapper.lua_settop(L, t); });

        lua_wrapper.lua_rawgeti(L, LuaWrapper::LUA_REGISTRYINDEX, gen_ref);
        if (lua_wrapper.lua_pcall(L, 0, 1, 0) != LuaWrapper::LUA_OK)
            throw runtime_error("lua finder generator function error");

        if (!lua_wrapper.lua_istable(L, -1))
            throw runtime_error("lua finder generator function return value is not a table");

        auto objref = lua_wrapper.luaL_ref(L, LuaWrapper::LUA_REGISTRYINDEX);
        return unique_ptr<StringFinder>(new LuaFinder(L, objref, params));
    }
};

int lua_register_finder(lua_State* L) {
    LUA_EXCEPTION_BEGIN();

    if (lua_wrapper.lua_gettop(L) != 3)
        throw runtime_error("wrong number of arguments");
    string encoding  = lua_wrapper.lua_checkstring_except(L, 1);
    string desc = lua_wrapper.lua_checkstring_except(L, 2);
    if (!lua_wrapper.lua_isfunction(L, 3))
        throw runtime_error("finder factory must be a function");

    int ref = lua_wrapper.luaL_ref(L, LuaWrapper::LUA_REGISTRYINDEX);
    register_finder(encoding, desc,
                    make_shared<LuaFinderGenerator>(L, ref));
    return 0;
    LUA_EXCEPTION_END();
}
