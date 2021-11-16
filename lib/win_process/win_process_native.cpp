#if defined(_WIN32) || defined(_WIN64)
#include "win_process/win_process_native.h"
#include "win_process/process_map_native.h"
#include <stdexcept>
#include <Windows.h>
#include <psapi.h>
#include <tlhelp32.h>
using namespace std;


WinProcessNative::WinProcessNative(int pid)
{
    HANDLE ph = OpenProcess( PROCESS_QUERY_INFORMATION | PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_VM_OPERATION,
                             FALSE, pid );

    if (ph == NULL)
        throw runtime_error("OpenProcess failed");

    this->process_handle = std::shared_ptr<HANDLE>(new HANDLE(ph), [](HANDLE* ph) { CloseHandle(*ph); delete ph;});


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
            auto mmap = std::make_shared<ProcessMapNative>(this->process_handle, info.lpBaseOfDll, info.SizeOfImage, false);
            this->process_maps.push_back(mmap);
        }
        else
        {
            throw runtime_error("GetModuleInformation failed");
        }
    }

#elif defined(_WIN32)

    HANDLE hSnapshot=CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pid);
    if ( hSnapshot == INVALID_HANDLE_VALUE )
    {
        throw runtime_error("CreateToolhelp32Snapshot failed");
    }

    MODULEENTRY32 tmpModule;
    tmpModule.dwSize = sizeof(MODULEENTRY32);
    if( Module32First(hSnapshot, &tmpModule) )
    {
        auto mmap = std::make_shared<ProcessMapNative>(this->process_handle, tmpModule.modBaseAddr,
                                                       tmpModule.modBaseSize, false);
        this->process_maps.push_back(mmap);

        tmpModule.dwSize = sizeof(MODULEENTRY32);
        while(Module32Next(hSnapshot,&tmpModule))
        {
            auto mmap = std::make_shared<ProcessMapNative>(this->process_handle, 
                                                           tmpModule.modBaseAddr,
                                                           tmpModule.modBaseSize, 
                                                           false);
            this->process_maps.push_back(mmap);
            tmpModule.dwSize = sizeof(MODULEENTRY32);
        }
    }
#endif // _WIN64
}

size_t WinProcessNative::map_count() const {
    return this->process_maps.size();
}

std::shared_ptr<ProcessMap> WinProcessNative::get_map(size_t index) {
    if (index >= this->process_maps.size())
        throw runtime_error("index out of range");

    return this->process_maps[index];
}

#endif // _WIN32 || _WIN64