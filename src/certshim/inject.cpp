// inject.exe (x86) - load a DLL into a running same-user process via LoadLibraryA.
// usage: inject <pid|exeName> <dllPath>
#include <windows.h>
#include <tlhelp32.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>

static DWORD findPid(const char* name) {
    HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    PROCESSENTRY32 pe; pe.dwSize = sizeof(pe); DWORD pid = 0;
    if (Process32First(snap, &pe)) do {
        if (_stricmp(pe.szExeFile, name) == 0) { pid = pe.th32ProcessID; break; }
    } while (Process32Next(snap, &pe));
    CloseHandle(snap);
    return pid;
}

int main(int argc, char** argv) {
    if (argc < 3) { printf("usage: inject <pid|exeName> <dllPath>\n"); return 1; }
    DWORD pid = (DWORD)atoi(argv[1]);
    if (pid == 0) pid = findPid(argv[1]);
    if (!pid) { printf("process '%s' not found\n", argv[1]); return 2; }

    char full[MAX_PATH]; GetFullPathNameA(argv[2], MAX_PATH, full, nullptr);
    if (GetFileAttributesA(full) == INVALID_FILE_ATTRIBUTES) { printf("dll not found: %s\n", full); return 3; }
    printf("target pid=%lu  dll=%s\n", pid, full);

    HANDLE hp = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    if (!hp) { printf("OpenProcess failed (err %lu)\n", GetLastError()); return 4; }

    size_t len = strlen(full) + 1;
    void* rmem = VirtualAllocEx(hp, nullptr, len, MEM_COMMIT|MEM_RESERVE, PAGE_READWRITE);
    if (!rmem) { printf("VirtualAllocEx failed (err %lu)\n", GetLastError()); return 5; }
    WriteProcessMemory(hp, rmem, full, len, nullptr);

    auto pLoad = (LPTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandleA("kernel32.dll"), "LoadLibraryA");
    HANDLE ht = CreateRemoteThread(hp, nullptr, 0, pLoad, rmem, 0, nullptr);
    if (!ht) { printf("CreateRemoteThread failed (err %lu)\n", GetLastError()); return 6; }
    WaitForSingleObject(ht, 5000);
    DWORD ec = 0; GetExitCodeThread(ht, &ec);
    VirtualFreeEx(hp, rmem, 0, MEM_RELEASE);
    CloseHandle(ht); CloseHandle(hp);
    printf("LoadLibraryA remote module handle (low32) = 0x%lx\n", ec);
    printf(ec ? "injected OK\n" : "inject likely FAILED (handle 0)\n");
    return ec ? 0 : 7;
}
