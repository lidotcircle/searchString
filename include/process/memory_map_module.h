#ifndef _MEMORY_MAP_MODULE_H_
#define _MEMORY_MAP_MODULE_H_

#include "memory_map.h"
#include <algorithm>
#include <vector>
#include <memory>
using namespace std;

class MemoryMapModule : public MemoryMap
{
private:
    std::vector<std::shared_ptr<MemoryMap>> pages;
    ptrdiff_t base_addr;
    size_t    mod_size;
    std::string mod_name;

public:
    MemoryMapModule() = delete;
    MemoryMapModule(const std::string& mod_name, std::vector<std::shared_ptr<MemoryMap>> sec_and_pages);

    virtual addr_t baseaddr() const override;
    virtual size_t size() const override;

    virtual char get_at(addr_t index) const override;
    virtual void set_at(addr_t index, char value) override;

    const std::string& module_name() const;
};

#endif // _MEMORY_MAP_MODULE_H_