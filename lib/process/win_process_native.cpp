#if defined(_WIN32) || defined(_WIN64)
#include "process/win_process_native.h"
#include "process/memory_map_win_page.h"
#include <stdexcept>
#include <Windows.h>
#include <psapi.h>
#include <tlhelp32.h>
#include <algorithm>
#include <iostream>
using namespace std;


WinProcessNative::WinProcessNative(int pid): process_id(pid) {
    this->refresh_process();
}

void WinProcessNative::refresh_process()
{
    HANDLE ph = OpenProcess( PROCESS_QUERY_INFORMATION | PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION,
                             FALSE, this->process_id );

    if (ph == NULL)
        throw runtime_error("OpenProcess failed");

    this->process_handle = std::shared_ptr<HANDLE>(new HANDLE(ph), [](HANDLE* ph) { CloseHandle(*ph); delete ph;});
    this->process_maps.clear();


#if defined(_WIN64)
    // List modules on a 64 bit machine. A 64 bit machine is assumed to be Windows Vista+
    HMODULE hMods[2048];
    DWORD cbNeeded;
    unsigned int i;

    if (!EnumProcessModulesEx(ph, hMods, sizeof(hMods), &cbNeeded, LIST_MODULES_ALL))
        throw runtime_error("EnumProcessModulesEx failed");

    for ( i = 0; i < (cbNeeded / sizeof(HMODULE)); i++ )
    {
        MODULEINFO info;
        if( GetModuleInformation( ph, hMods[i], &info, sizeof(MODULEINFO) ) )
        {
            auto mmap = std::make_shared<MemoryMapWinPage>(this->process_handle, info.lpBaseOfDll, info.SizeOfImage, false);
            this->process_maps.push_back(mmap);
        }
        else
        {
            throw runtime_error("GetModuleInformation failed");
        }
    }

#elif defined(_WIN32)

    HANDLE hSnapshot=CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, this->process_id);
    if ( hSnapshot == INVALID_HANDLE_VALUE )
    {
        throw runtime_error("CreateToolhelp32Snapshot failed");
    }

    MODULEENTRY32 tmpModule;
    tmpModule.dwSize = sizeof(MODULEENTRY32);
    if( Module32First(hSnapshot, &tmpModule) )
    {
        auto mmap = std::make_shared<MemoryMapWinPage>(this->process_handle, tmpModule.modBaseAddr,
                                                       tmpModule.modBaseSize, false);
        this->process_maps.push_back(mmap);

        tmpModule.dwSize = sizeof(MODULEENTRY32);
        while(Module32Next(hSnapshot,&tmpModule))
        {
            auto mmap = std::make_shared<MemoryMapWinPage>(this->process_handle, 
                                                           tmpModule.modBaseAddr,
                                                           tmpModule.modBaseSize, 
                                                           false);
            this->process_maps.push_back(mmap);
            tmpModule.dwSize = sizeof(MODULEENTRY32);
        }
    }
#endif // _WIN64

    this->add_nomodule_pages();
}

void WinProcessNative::add_nomodule_pages()
{
    auto handle = *this->process_handle.get();
    MEMORY_BASIC_INFORMATION mbi;
    LPCVOID addr = nullptr;

    this->process_maps.clear();
    while(VirtualQueryEx(handle, addr, &mbi, sizeof(mbi))) {
        if (!this->is_valid_addr(reinterpret_cast<size_t>(mbi.BaseAddress))) {
            auto mmap = std::make_shared<MemoryMapWinPage>(this->process_handle, mbi.BaseAddress, mbi.RegionSize, false);
            if (mbi.State == MEM_COMMIT && mbi.Type != MEM_MAPPED) {
                this->process_maps.push_back(mmap);
            }
        }

        addr = reinterpret_cast<LPCVOID>(reinterpret_cast<size_t>(mbi.BaseAddress) + mbi.RegionSize);
    }

    std::sort(this->process_maps.begin(),
              this->process_maps.end(),
              [](const std::shared_ptr<MemoryMap>& a, const std::shared_ptr<MemoryMap>& b) {
        return a->baseaddr() < b->baseaddr();
    });
}

size_t WinProcessNative::map_count() const {
    return this->process_maps.size();
}

std::shared_ptr<MemoryMap> WinProcessNative::get_map(size_t index) {
    if (index >= this->process_maps.size())
        throw runtime_error("index out of range");

    return this->process_maps[index];
}

#endif // _WIN32 || _WIN64