#ifndef _WIN_PROCESS_H_
#define _WIN_PROCESS_H_

#include <vector>
#include <string>
#include <memory>
#include "process_map.h"
#include "mem_indexable.h"
#include "memory_value_ref.h"


class WinProcess: public MemIndexable
{
public:
    virtual size_t map_count() const = 0;
    virtual std::shared_ptr<ProcessMap> get_map(size_t index) = 0;

    virtual char get_at(size_t addr) override;
    virtual void set_at(size_t addr, char value) override;

    bool is_valid_addr(size_t addr);
    MemoryValueRef operator[](size_t addr);
};

#endif // _WIN_PROCESS_H_
