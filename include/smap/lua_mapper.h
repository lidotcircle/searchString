#ifndef _LULA_MAPPER_H_
#define _LULA_MAPPER_H_

#include "../lua/lua_wrapper.h"

int lua_register_mapper(lua_State * L);
int lua_mapper_list(lua_State * L);

#endif // _LULA_MAPPER_H_
