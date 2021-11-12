#include "sgetter/string_getter_base.h"
#include "sgetter/string_getter_iter.h"


StringGetterIter StringGetterBase::begin() {
    return StringGetterIter(this, false);
}

StringGetterIter StringGetterBase::end() {
    return StringGetterIter(this, true);
}

StringGetterBase::~StringGetterBase() { }

