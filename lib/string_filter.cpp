#include <string_filter.h>


StringFilter::~StringFilter() {}


MiniumLength::MiniumLength(size_t s): min(s){}

bool MiniumLength::filter(const std::string& str)
{
    return str.size() >= this->min;
}

MiniumLength::~MiniumLength() {
}

