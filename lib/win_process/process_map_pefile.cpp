#include "win_process/process_map_pefile.h"
#include <pe-parse/parse.h>
#include <stdexcept>
using namespace std;


ProcessMapPEFile::ProcessMapPEFile(const peparse::bounded_buffer* buffr, void* base_addr, size_t size):
    buffer(buffr), base_address(base_addr), map_size(size)
{
}

char ProcessMapPEFile::get_at(size_t offset) {
    if (offset >= map_size) {
        throw runtime_error("offset out of bounds");
    }

    uint8_t value;
    peparse::readByte(const_cast<peparse::bounded_buffer*>(this->buffer), offset, value);

    return value;
}

void ProcessMapPEFile::set_at(size_t offset, char value) {
    throw std::runtime_error("pe file is read only");
}

void* ProcessMapPEFile::baseaddr() {
    return this->base_address;
}

size_t ProcessMapPEFile::size() {
    return this->map_size;
}

ProcessMapPEFile::~ProcessMapPEFile() {
}
