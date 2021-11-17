#ifndef _WIN_PROCESS_NATIVE_H_
#define _WIN_PROCESS_NATIVE_H_

#include "win_process.h"
#include <vector>
#include <memory>
#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#else
typedef int HANDLE;
#endif // defined(_WIN32) || defined(_WIN64)


class WinProcessNative : public WinProcess
{
public:
    using ProcessHandle = std::shared_ptr<HANDLE>;

private:
    ProcessHandle process_handle;
    std::vector<std::shared_ptr<ProcessMap>> process_maps;

public:
    WinProcessNative(int pid);

    virtual size_t map_count() const override;
    virtual std::shared_ptr<ProcessMap> get_map(size_t index) override;
};

#endif // _WIN_PROCESS_NATIVE_H_