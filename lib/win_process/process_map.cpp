#include "win_process/process_map.h"
#include "win_process/process_map_iter.h"
using namespace std;


ProcessMapIter ProcessMap::begin() {
    return ProcessMapIter(this, 0);
}

ProcessMapIter ProcessMap::end() {
    return ProcessMapIter(this, this->size());
}
