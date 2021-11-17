#ifndef _MEM_INDEXABLE_H_
#define _MEM_INDEXABLE_H_

#include <stdlib.h>


class MemIndexable
{
public:
    virtual ~MemIndexable() = default;
    virtual char get_at(size_t index) const = 0;
    virtual void set_at(size_t index, char value) = 0;
};

#endif // _MEM_INDEXABLE_H_
