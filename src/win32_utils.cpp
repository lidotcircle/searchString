#if defined(_WIN32) || defined(_WIN64)
#include <string>
#include <vector>
#include <Windows.h>
#include <tlhelp32.h>
using namespace std;


int GetPIDByProcessName(const std::string& processName)
{
    HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (hProcessSnap == INVALID_HANDLE_VALUE)
        return 0;

    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(PROCESSENTRY32);

    if (!Process32First(hProcessSnap, &pe32))
    {
        wprintf(L"Error getting first process\n");
        CloseHandle(hProcessSnap);
        return 0;
    }

    DWORD pid = 0;

    do
    {
        if (_stricmp(pe32.szExeFile, processName.c_str()) == 0)
        {
            pid = pe32.th32ProcessID;
            break;
        }
    } while (Process32Next(hProcessSnap, &pe32));

    CloseHandle(hProcessSnap);
    return pid;
}
#endif // defined(_WIN32) || defined(_WIN64)