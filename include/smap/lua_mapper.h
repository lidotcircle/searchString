#ifndef _LULA_MAPPER_H_
#define _LULA_MAPPER_H_

#include "string_mapper.h"
#include "../lua/lua_wrapper.h"

class LuaMapper : public StringMapper {
private:
    lua_State* L;
    int func_ref;

public:
    LuaMapper(lua_State* L, int ref);
    ~LuaMapper();

    virtual std::vector<std::pair<size_t,std::string>> map(const std::string&) const override;
};

int lua_register_mapper(lua_State * L);

#endif // _LULA_MAPPER_H_
