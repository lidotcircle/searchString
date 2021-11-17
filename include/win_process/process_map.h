#ifndef _PROCESS_MAP_H_
#define _PROCESS_MAP_H_

#include <vector>
#include <string>
#include "process_map_iter.h"
#include "mem_indexable.h"


class ProcessMap: public MemIndexable {
public:
    virtual void* baseaddr() const = 0;
    virtual size_t size() const = 0;

    virtual ~ProcessMap() = default;

    ProcessMapIter begin();
    ProcessMapIter end();
};

#endif // _PROCESS_MAP_H_
