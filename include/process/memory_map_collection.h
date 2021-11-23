#ifndef _MEMORY_MAP_COLLECTION_H_
#define _MEMORY_MAP_COLLECTION_H_

#include <vector>
#include <string>
#include <memory>
#include "memory_map.h"
#include "mem_indexable.h"
#include "memory_value_ref.h"


class MemoryMapCollection: public MemIndexable
{
public:
    virtual size_t map_count() const = 0;
    virtual std::shared_ptr<MemoryMap> get_map(size_t index) = 0;
    virtual const std::shared_ptr<MemoryMap> get_map(size_t index) const;

    virtual char get_at(size_t addr) const override;
    virtual void set_at(size_t addr, char value) override;

    bool is_valid_addr(size_t addr);
    MemoryValueRef operator[](size_t addr);
};

#endif // _MEMORY_MAP_COLLECTION_H_