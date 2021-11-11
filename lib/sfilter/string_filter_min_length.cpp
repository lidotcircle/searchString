#include "sfilter/string_filter_min_length.h"


MiniumLength::MiniumLength(size_t len): min(len) {}

int MiniumLength::filter(const std::string& str) const {
    if (str.size() < this->min) return -1;
    return 0;
}
