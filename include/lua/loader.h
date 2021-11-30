#ifndef _STRING_SEARCH_LOADEER_H_
#define _STRING_SEARCH_LOADEER_H_

#include <string>

typedef void* lhandle_t;

lhandle_t loader_load  (const std::string& path);
void      loader_unload(lhandle_t handle);

void* loader_resolve_symbol(lhandle_t handle, const std::string& funcname);

#endif // _STRING_SEARCH_LOADEER_H_