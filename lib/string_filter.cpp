#include "sfilter/string_filter.h"


StringFilter::~StringFilter() {}


MiniumLength::MiniumLength(size_t s): min(s){}

int MiniumLength::filter(const std::string& str)
{
    return str.size() >= this->min ? 0 : -1;
}

MiniumLength::~MiniumLength() {
}

