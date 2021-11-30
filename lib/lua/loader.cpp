#include "lua/loader.h"
#include <stdexcept>


#if defined(linux) || defined(BSD) || defined(__APPLE__)
#define USE_DLOPEN
#endif

#if defined(USE_DLOPEN)
#ifndef RTLD_DEFAULT
#define RTLD_DEFAULT 0
#endif

#include <dlfcn.h>

void loader_unload (lhandle_t handle) {
    if(handle && (handle != RTLD_DEFAULT))
        dlclose(handle);
}

lhandle_t loader_load(const std::string& libname) {
    auto h = dlopen(libname.c_str(), RTLD_NOW);
    if (!h)
        throw std::runtime_error(dlerror());
    return h;
}

void *loader_resolve_symbol(lhandle_t handle, const std::string& symbol) {
    if(!handle) handle = RTLD_DEFAULT;
    void *f = dlsym(handle, symbol.c_str());
    if (!f)
        throw std::runtime_error(dlerror());
    return f;
}

#elif defined(_WIN32) || defined(_WIN64)

#include <Windows.h>

void loader_unload (lhandle_t handle) {
    if(handle)
        FreeLibrary((HINSTANCE)handle);
}

lhandle_t loader_load (const std::string& libname) {
    HINSTANCE lib = LoadLibraryA(libname.c_str());
    if (lib == NULL)
        throw std::runtime_error("LoadLibrary failed");
    return lib;
}

void *loader_resolve_symbol(lhandle_t handle, const std::string& symbol) {
    HINSTANCE module;
    void *f;
    module = (HINSTANCE)handle;
    if(!module) module = GetModuleHandleA(NULL);
    f = GetProcAddress(module, symbol.c_str());
    if (f == NULL)
        throw std::runtime_error("GetProcAddress failed: " + symbol);
    return f;
}

#else

void loader_unload (lhandle_t handle) {
    throw std::runtime_error("loader_unload not implemented");
}

lhandle_t loader_load (const std::string& libname) {
    throw std::runtime_error("loader_load not implemented");
}

void *loader_resolve_symbol(lhandle_t handle, const std::string& symbol) {
    throw std::runtime_error("loader_resolve_symbol not implemented");
}

#endif
