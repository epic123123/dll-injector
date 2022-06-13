#include <Windows.h>
#include <TlHelp32.h>
#include <iostream>
#include <comdef.h>
#include <string>

int arg_check(int argc, char** argv);

char* dllpath;
char* proc_name;
DWORD procID;
bool use_id;

DWORD GetProcID(const char* procName) {
    DWORD proc_id = 0;
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hSnap != INVALID_HANDLE_VALUE)
    {
        PROCESSENTRY32 proc_entry;
        proc_entry.dwSize = sizeof(proc_entry);

        if (Process32First(hSnap, &proc_entry)) {
            _bstr_t b(proc_entry.szExeFile);
            do
            {
                if (!_stricmp(b, procName)) {
                    proc_id = proc_entry.th32ProcessID;
                    break;
                }
            } while (Process32Next(hSnap, &proc_entry));
        }
    }
    CloseHandle(hSnap);
    return proc_id;
}


int main(int argc, char **argv)
{
    procID = 0;
    if (arg_check(argc, argv) == 1) { return 1; }
    
    while (!procID && !use_id) {
        procID = GetProcID(proc_name);
        std::cout << "Trying to get process ID from name: " << proc_name << "\n";
        Sleep(100);
    }
    std::cout << "Process ID received > " << procID << std::endl;;

    HANDLE hProc = OpenProcess(PROCESS_ALL_ACCESS, 0, procID);
    if (hProc == NULL)
    {
        std::cout << "OpenProcess failed.";
        return 1;
    }
    std::cout << "Process succesfully opened.\n";

    if (hProc && hProc != INVALID_HANDLE_VALUE)
    {
        void* loc = VirtualAllocEx(hProc, 0, MAX_PATH, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
        if (loc)
        {
            std::cout << "VirtualAllocEx successful.\n";
            if (WriteProcessMemory(hProc, loc, dllpath, strlen(dllpath) + 1, 0) == 0)
            {
                std::cout << "WriteProcessMemory failed # " << GetLastError();
                return 1;
            }
            std::cout << "WriteProcessMemory successful.\n";
        }
        else { std::cout << "VirtualAllocEx wasn't successful."; return 1; }
        HANDLE hThread = CreateRemoteThread(hProc, 0, 0, (LPTHREAD_START_ROUTINE)LoadLibraryA, loc, 0, 0);
        if (hThread == NULL)
        {
            std::cout << "CreateRemoteThread failed! # " << GetLastError();
            return 1;
        }
        std::cout << "Remote Thread created.\n";

        if (hThread)
        {
            CloseHandle(hThread);
        }
        if (hProc)
        {
            CloseHandle(hProc);
        }
        std::cout << "Injection Successful!\n";
    }
}

int arg_check(int argc, char** argv)
{
    if (argc != 5)
    {
        std::cout << "\n-i - specify target process ID\n-dll - specify your injectable dll\n-n - specify target process name(not recommended, also not required)";
        std::cout << "\nUsage : " << argv[0] << " -i 12345 -dll mydll.dll\n";
        std::cout << "\nyou can get process id for example using process hacker\n";
        return 1;
    }

    for (int i = 0; i < argc; i++)
    {
        if (strcmp(argv[i], "-i") == 0)
        {
            use_id = true;
            procID = std::stoi(argv[i + 1]);
        }
        else if (strcmp(argv[i], "-dll") == 0)
        {
            dllpath = argv[i + 1];
        }
        else if (strcmp(argv[i], "-n") == 0)
        {
            use_id = false;
            proc_name = argv[i + 1];
        }
    }

    return 0;
}
