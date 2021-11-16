#if defined(_WIN32) || defined(_WIN64)
#include "win_process/process_map_native.h"
#include <stdexcept>
#include <Windows.h>
#include <string>
using namespace std;

#define CACHE_SIZE 4096


ProcessMapNative::ProcessMapNative(ProcessHandle handle, void* base, size_t size, bool direct_write):
    process_handle(handle), baseaddress(base), map_size(size)
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
    return this->baseaddr;
}
    
size_t ProcessMapNative::size() const
{
    return this->map_size;
}

char ProcessMapNative::get_at(size_t offset) {
    if (offset >= map_size)
        throw out_of_range("offset out of range");

    size_t base = reinterpret_cast<size_t>(baseaddress);
    size_t addr = reinterpret_cast<void*>(base + offset);

    if (cache_offset <= offset && offset < cache_offset + cache_size) {
        return cache[offset - cache_offset];
    }

    size_t n;
    if (!ReadProcessMemory(*this->process_handle.get(), addr, cache, CACHE_SIZE, &n)) {
        throw runtime_error("ReadProcessMemory failed");
    }
    this->cache_size = n;

    cache_offset = offset;
    return cache[offset - cache_offset];
}

void ProcessMapNative::set_at(size_t offset, char value) {
    if (offset >= map_size)
        throw out_of_range("offset out of range");

    size_t base = reinterpret_cast<size_t>(baseaddress);
    size_t addr = reinterpret_cast<void*>(base + offset);

    if (this->direct_write) {
        if (!WriteProcessMemory(*this->process_handle.get(), addr, &value, 1, nullptr)) {
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
    size_t addr = reinterpret_cast<void*>(base + this->cache_offset);
    if (!WriteProcessMemory(*this->process_handle.get(), addr, this->cache, cache_size, nullptr))
        throw runtime_error("WriteProcessMemory failed");
}

#endif // _WIN32 || _WIN64