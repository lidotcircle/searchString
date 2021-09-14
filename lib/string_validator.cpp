#include <string_validator.h>


StringValidator::~StringValidator() {}


MiniumLength::MiniumLength(size_t s): min(s){}

bool MiniumLength::validate(const std::string& str)
{
    return str.size() >= this->min;
}

MiniumLength::~MiniumLength() {
}

