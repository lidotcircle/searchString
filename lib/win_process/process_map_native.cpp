#if defined(_WIN32) || defined(_WIN64)
#include "win_process/process_map_native.h"
#include <stdexcept>
#include <Windows.h>
#include <string>
using namespace std;

#define CACHE_SIZE 4096


static string integer2hexstr(int64_t val) {
    string ret;
    char buf[16];
    snprintf(buf, sizeof(buf), "%llx", val);
    ret = buf;
    return ret;
}

static string addr2hexstr(void* addr) {
    return integer2hexstr(reinterpret_cast<int64_t>(addr));
}

ProcessMapNative::ProcessMapNative(ProcessHandle handle, void* base, size_t size, bool direct_write):
    process_handle(handle), baseaddress(base), map_size(size),
    cache(new char[CACHE_SIZE]), cache_size(0), cache_offset(std::string::npos), direct_write(direct_write), write_dirty(false)
{
}

ProcessMapNative::~ProcessMapNative()
{
    if (cache)
        delete[] cache;
}

void* ProcessMapNative::baseaddr() const
{
    return this->baseaddress;
}
    
size_t ProcessMapNative::size() const
{
    return this->map_size;
}

char ProcessMapNative::get_at(size_t offset) const {
    if (offset >= map_size)
        throw out_of_range("offset out of range");

    auto _this = const_cast<ProcessMapNative*>(this);

    size_t base = reinterpret_cast<size_t>(this->baseaddress);
    void* addr  = reinterpret_cast<void*>(base + offset);

    if (cache_offset <= offset && offset < cache_offset + cache_size) {
        return cache[offset - cache_offset];
    }

    MEMORY_BASIC_INFORMATION mbi;
    if (!VirtualQueryEx(*_this->process_handle.get(), addr, &mbi, sizeof(mbi))) {
        throw runtime_error("VirtualQueryEx failed");
    }
    DWORD alloc_protect = mbi.AllocationProtect, old_protect;

    if (!VirtualProtectEx(*this->process_handle.get(), addr, CACHE_SIZE, alloc_protect, &old_protect)) {
        throw runtime_error("X VirtualProtectEx failed: 0x" + addr2hexstr(addr));
    }

    SIZE_T n;
    auto result = ReadProcessMemory(*this->process_handle.get(), addr, cache, CACHE_SIZE, &n);
    VirtualProtectEx(*this->process_handle.get(), addr, CACHE_SIZE, old_protect, &old_protect);

    if (!result) {
        throw runtime_error("ReadProcessMemory failed at: 0x" + addr2hexstr(addr) + 
                            ", region base: 0x" + integer2hexstr(base));
    }
    _this->cache_size = n;

    _this->cache_offset = offset;
    return cache[offset - cache_offset];
}

void ProcessMapNative::set_at(size_t offset, char value) {
    if (offset >= map_size)
        throw out_of_range("offset out of range");

    size_t base = reinterpret_cast<size_t>(baseaddress);
    auto   addr = reinterpret_cast<void*>(base + offset);

    if (this->direct_write) {
        DWORD old_protect;
        if (!VirtualProtectEx(*this->process_handle.get(), addr, CACHE_SIZE, PAGE_READWRITE, &old_protect))
        {
            throw runtime_error("Y VirtualProtectEx failed: 0x" + addr2hexstr(addr));
        }

        auto result = WriteProcessMemory(*this->process_handle.get(), addr, &value, 1, nullptr);
        VirtualProtectEx(*this->process_handle.get(), addr, CACHE_SIZE, old_protect, &old_protect);
        
        if (!result) {
            throw runtime_error("WriteProcessMemory failed");
        }
        return;
    }

    if (offset < this->cache_offset || offset >= this->cache_offset + cache_size)
        this->get_at(offset);

    this->cache[offset - this->cache_offset] = value;
    this->write_dirty = true;
}

void ProcessMapNative::flush() {
    if (!this->write_dirty)
        return;

    size_t base = reinterpret_cast<size_t>(baseaddress);
    auto addr = reinterpret_cast<void*>(base + this->cache_offset);

    DWORD old_protect;
    if (!VirtualProtectEx(*this->process_handle.get(), addr, CACHE_SIZE, PAGE_EXECUTE_READWRITE, &old_protect))
    {
        throw runtime_error("VirtualProtectEx failed");
    }

    auto result = WriteProcessMemory(*this->process_handle.get(), addr, this->cache, cache_size, nullptr);
    VirtualProtectEx(*this->process_handle.get(), addr, CACHE_SIZE, old_protect, &old_protect);

    if (!result) {
        throw runtime_error("WriteProcessMemory failed");
    }
}

#endif // _WIN32 || _WIN64