#include "sfilter/string_filter.h"


StringFilter::~StringFilter() {}

bool apply_filters(const std::vector<std::shared_ptr<StringFilter>>& filters, const std::string& str) {
    for (auto filter: filters) {
        auto result = filter->filter(str);
        if (result < 0)
            return false;
        if (result > 0)
            return true;
    }
    return true;
}

