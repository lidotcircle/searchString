#ifndef _PROCESS_MAP_NATIVE_H_
#define _PROCESS_MAP_NATIVE_H_

#inlcude "process_map.h"
#include <memory>


class ProcessMapNative : public ProcessMap
{
public:
    using ProcessHandle = std::shared_ptr<HANDLE>;

private:
    ProcessHandle process_handle;
    void* baseaddress;
    size_t map_size;
    char* cache;
    size_t cache_offset;
    size_t cache_size;
    bool direct_write;
    bool write_dirty;

public:
    ProcessMapNative(ProcessHandle process_handle, void* baseaddress, size_t map_size, bool direct_write);

    virtual void* baseaddr() const override;
    virtual size_t size() const override;

    virtual char get_at(size_t index) const override;
    virtual void set_at(size_t index, char value) override;

    void flush();
    ~ProcessMapNative() override;
};

#endif // _PROCESS_MAP_NATIVE_H_