#include "smap/lua_mapper.h"
#include "smap/string_mapper.h"
#include "smap/mapper_factory.h"
#include "utils.hpp"
using namespace std;
using namespace MapperFactory;


class LuaMapper : public StringMapper {
private:
    lua_State* L;
    int func_ref;
    string params;

public:
    LuaMapper(lua_State* L, int ref, const string& params);
    ~LuaMapper();

    virtual std::vector<std::pair<size_t,std::string>> map(const std::string&) const override;
};



LuaMapper::LuaMapper(lua_State* L, int ref, const string& params): L(L), func_ref(ref), params(params) {
}

LuaMapper::~LuaMapper() {
}

vector<pair<size_t,std::string>> LuaMapper::map(const string& str) const {
    auto L = this->L;
    vector<pair<size_t,std::string>> result;
    auto t = lua_wrapper.lua_gettop(L);
    defer([&]() { lua_wrapper.lua_settop(L, t); });

    lua_wrapper.lua_rawgeti(L, LuaWrapper::LUA_REGISTRYINDEX, this->func_ref);
    lua_wrapper.lua_pushlstring(L, str.c_str(), str.size());
    if (this->params.empty())
        lua_wrapper.lua_pushnil(L);
    else
        lua_wrapper.lua_pushlstring(L, this->params.c_str(), this->params.size());

    // return value
    // 1: nil
    // 2: string
    // 3: integer, string
    // 4: string[]
    // 5: [integer, string][]
    if (lua_wrapper.lua_pcall(L, 2, 2, 0) != LuaWrapper::LUA_OK) {
        size_t n;
        const char* err = lua_wrapper.lua_tolstring(L, -1, &n);
        throw runtime_error("lua_mapper failed: " + string(err, n));
    }

    if (lua_wrapper.lua_isstring(L, -2)) {
        result.push_back(make_pair(0,lua_wrapper.lua_tostring(L, -2)));
    } else if (lua_wrapper.lua_isinteger(L, -2)) {
        if (!lua_wrapper.lua_isstring(L, -1)) {
            throw runtime_error("lua_mapper failed: expect second "
                                "return value to be string");
        }

        auto v1 = lua_wrapper.lua_tointeger(L, -2);
        auto v2 = lua_wrapper.lua_tostring(L, -1);
        result.push_back(make_pair(v1,v2));
    } else if (lua_wrapper.lua_istable(L, -2)) {
        size_t len = lua_wrapper.lua_rawlen(L, -2);
        for (size_t i = 1; i <= len; i++) {
            lua_wrapper.lua_rawgeti(L, -2, i);

            if (lua_wrapper.lua_isstring(L, -1)) {
                auto v1 = lua_wrapper.lua_tostring(L, -1);
                result.push_back(make_pair(0,v1));
                lua_wrapper.lua_pop(L, 1);
            } else if (lua_wrapper.lua_istable(L, -1)) {
                if (lua_wrapper.lua_rawlen(L, -1) != 2) {
                    throw runtime_error("lua_mapper failed: expect table "
                                        "to have two elements");
                }

                lua_wrapper.lua_rawgeti(L, -1, 1);
                if (!lua_wrapper.lua_isinteger(L, -1)) {
                    throw runtime_error("lua_mapper failed: expect first "
                                        "element of table to be integer");
                }
                auto v1 = lua_wrapper.lua_tointeger(L, -1);
                lua_wrapper.lua_pop(L, 1);

                lua_wrapper.lua_rawgeti(L, -1, 2);
                if (!lua_wrapper.lua_isstring(L, -1)) {
                    throw runtime_error("lua_mapper failed: expect second "
                                        "element of table to be string");
                }
                auto v2 = lua_wrapper.lua_tostring(L, -1);
                lua_wrapper.lua_pop(L, 2);
                result.push_back(make_pair(v1,v2));
            } else {
                throw runtime_error("lua_mapper failed: expect string or [integer, string]");
            }
        }
    }

    return std::move(result);
}

class LuaMapperGenerator: public MapperGenerator {
private:
    lua_State* L;
    int funcref;

public:
    LuaMapperGenerator(lua_State* L, int funcref): L(L), funcref(funcref) {}

    std::shared_ptr<StringMapper> operator()(const string& params) const override {
        return std::make_shared<LuaMapper>(this->L, this->funcref, params);
    }
};

int lua_register_mapper(lua_State * L) {
    LUA_EXCEPTION_BEGIN();

    if (lua_wrapper.lua_gettop(L) != 4)
        throw runtime_error("wrong number of arguments");
    string encoding  = lua_wrapper.lua_checkstring_except(L, 1);
    string filter_name = lua_wrapper.lua_checkstring_except(L, 2);
    string desc = lua_wrapper.lua_checkstring_except(L, 3);
    if (!lua_wrapper.lua_isfunction(L, 4))
        throw runtime_error("mapper function must be a function");

    int ref = lua_wrapper.luaL_ref(L, LuaWrapper::LUA_REGISTRYINDEX);
    register_mapper(encoding, filter_name, desc,
                    make_shared<LuaMapperGenerator>(L, ref));
    return 0;
    LUA_EXCEPTION_END();
}
