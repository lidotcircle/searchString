#ifndef _PROCESS_MAP_PEFILE_H_
#define _PROCESS_MAP_PEFILE_H_

#include "process_map.h"
#include <pe-parse/parse.h>


class ProcessMapPEFile : public ProcessMap
{
private:
    const peparse::bounded_buffer* buffer;
    void* base_address;
    size_t map_size;

public:
    ProcessMapPEFile(const peparse::bounded_buffer* buffer, void* base_address, size_t size);

    virtual char get_at(size_t offset) const override;
    virtual void set_at(size_t offset, char value) override;

    virtual void* baseaddr() const override;
    virtual size_t size() const override;

    ~ProcessMapPEFile();
};

#endif // _PROCESS_MAP_PEFILE_H_
