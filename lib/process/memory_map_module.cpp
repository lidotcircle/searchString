#include "process/memory_map_module.h"
#include <algorithm>
#include <stdexcept>
using namespace std;


MemoryMapModule::MemoryMapModule(const std::string& mod_name, std::vector<std::shared_ptr<MemoryMap>> pages):
    mod_name(mod_name), pages(std::move(pages))
{
    std::sort(pages.begin(), pages.end(), [](const std::shared_ptr<MemoryMap>& a, const std::shared_ptr<MemoryMap>& b) {
        return a->baseaddr() < b->baseaddr();
    });

    size_t prev_end = 0;
    size_t module_size = 0;
    for (auto& page : pages) {
        if (prev_end > 0 && page->baseaddr() != prev_end) {
            throw std::runtime_error("MemoryMapModule: pages are not continuous");
        }

        module_size += page->size();
        prev_end = page->baseaddr() + page->size();
    }
    this->mod_size = module_size;
    this->base_addr = pages.front()->baseaddr();
}

MemoryMapModule::addr_t MemoryMapModule::baseaddr() const {
    return this->base_addr;
}

size_t MemoryMapModule::size() const {
    return this->mod_size;
}

char MemoryMapModule::get_at(addr_t index) const {
    index += this->base_addr;
    auto page = std::lower_bound(this->pages.begin(), this->pages.end(), index, [](const std::shared_ptr<MemoryMap>& page, addr_t index) {
        return page->baseaddr() + page->size() <= index;
    });

    if (page == this->pages.end()) {
        throw std::runtime_error("MemoryMapModule: index out of range");
    }

    return (*page)->get_at(index - (*page)->baseaddr());
}

void MemoryMapModule::set_at(addr_t index, char value) {
    index += this->base_addr;
    auto page = std::lower_bound(this->pages.begin(), this->pages.end(), index, [](const std::shared_ptr<MemoryMap>& page, addr_t index) {
        return page->baseaddr() + page->size() <= index;
    });

    if (page == this->pages.end()) {
        throw std::runtime_error("MemoryMapModule: index out of range");
    }

    (*page)->set_at(index - (*page)->baseaddr(), value);
}

const std::string& MemoryMapModule::module_name() const {
    return this->mod_name;
}
