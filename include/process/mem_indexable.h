#ifndef _MEM_INDEXABLE_H_
#define _MEM_INDEXABLE_H_

#include <stdlib.h>


class MemIndexable
{
public:
    using addr_t = size_t;

    virtual ~MemIndexable() = default;
    virtual char get_at(addr_t index) const = 0;
    virtual void set_at(addr_t index, char value) = 0;

    virtual void flush();
};

#endif // _MEM_INDEXABLE_H_
