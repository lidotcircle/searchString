#ifndef _MEMORY_MAP_PEFILE_H_
#define _MEMORY_MAP_PEFILE_H_

#include "memory_map.h"
#include <pe-parse/parse.h>


class MemoryMapPEFile : public MemoryMap
{
private:
    const peparse::bounded_buffer* buffer;
    addr_t base_address;
    size_t map_size;

public:
    MemoryMapPEFile(const peparse::bounded_buffer* buffer, void* base_address, size_t size);

    virtual char get_at(addr_t offset) const override;
    virtual void set_at(addr_t offset, char value) override;

    virtual addr_t baseaddr() const override;
    virtual size_t size() const override;

    ~MemoryMapPEFile();
};

#endif // _MEMORY_MAP_PEFILE_H_