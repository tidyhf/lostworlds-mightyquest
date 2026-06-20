// certpatch.dll  (x86, injected into MightyQuest.exe at runtime)
// Neutralises the game's libCURL certificate verification so it accepts the local
// server's self-signed cert. Same bypass MQELOffline performs: find two functions in
// the decrypted .text and make them return success. Validated against client build 280139.
//   P1 -> SSL verify routine -> force return 1 (valid)
//   P2 -> result accessor     -> force return 0 (no error)
// Logs to %TEMP%\mqel-certpatch.log.  Part of the Lost Worlds project. MIT.

#include <windows.h>
#include <cstdint>
#include <cstring>
#include <cstdio>

static const unsigned char P1[]  = {0x8B,0x44,0x24,0x04,0x8B,0x4C,0x24,0x08,0x8B,0x54,0x24,0x0C,0x89,0x88,0xC0,0x00,0x00,0x00,0x89,0x90,0xE8,0x00,0x00,0x00,0xC3};
static const unsigned char P2[]  = {0x8B,0x44,0x24,0x04,0x8B,0x80,0xEC,0x00,0x00,0x00,0xC3};
static const unsigned char RET1[]= {0xB8,0x01,0x00,0x00,0x00,0xC3}; // mov eax,1 ; ret
static const unsigned char RET0[]= {0x33,0xC0,0xC3};                // xor eax,eax ; ret

static void logf(const char *fmt, ...)
{
    char msg[256]; va_list ap; va_start(ap, fmt); _vsnprintf_s(msg, sizeof(msg), _TRUNCATE, fmt, ap); va_end(ap);
    char path[MAX_PATH]; DWORD n = GetTempPathA(MAX_PATH, path);
    strncpy_s(path + n, MAX_PATH - n, "mqel-certpatch.log", _TRUNCATE);
    HANDLE f = CreateFileA(path, FILE_APPEND_DATA, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (f != INVALID_HANDLE_VALUE) { DWORD w; WriteFile(f, msg, (DWORD)strlen(msg), &w, nullptr); WriteFile(f, "\r\n", 2, &w, nullptr); CloseHandle(f); }
}

static uint8_t *scan(const unsigned char *pat, size_t plen)
{
    auto base = (uint8_t *)GetModuleHandleA(nullptr);
    auto dos  = (IMAGE_DOS_HEADER *)base;
    auto nt   = (IMAGE_NT_HEADERS *)(base + dos->e_lfanew);
    uint8_t *end = base + nt->OptionalHeader.SizeOfImage;
    MEMORY_BASIC_INFORMATION mbi;
    for (uint8_t *p = base; p < end && VirtualQuery(p, &mbi, sizeof(mbi)); p = (uint8_t *)mbi.BaseAddress + mbi.RegionSize)
    {
        const DWORD execMask = PAGE_EXECUTE | PAGE_EXECUTE_READ | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY;
        bool exec = (mbi.State == MEM_COMMIT) && (mbi.Protect & execMask) && !(mbi.Protect & PAGE_GUARD);
        if (!exec) continue;
        uint8_t *rstart = (uint8_t *)mbi.BaseAddress;
        uint8_t *rend = rstart + mbi.RegionSize; if (rend > end) rend = end;
        for (uint8_t *s = rstart; s + plen <= rend; ++s)
            if (memcmp(s, pat, plen) == 0) return s;
    }
    return nullptr;
}

static void patch(const unsigned char *pat, size_t plen, const unsigned char *stub, size_t slen, const char *name)
{
    uint8_t *a = scan(pat, plen);
    if (!a) { logf("[%s] pattern NOT FOUND (client build mismatch?)", name); return; }
    DWORD old;
    if (VirtualProtect(a, slen, PAGE_EXECUTE_READWRITE, &old))
    {
        memcpy(a, stub, slen);
        VirtualProtect(a, slen, old, &old);
        FlushInstructionCache(GetCurrentProcess(), a, slen);
        logf("[%s] patched at %p", name, a);
    }
    else logf("[%s] VirtualProtect failed at %p (err %lu)", name, a, GetLastError());
}

static DWORD WINAPI Worker(LPVOID)
{
    logf("certpatch injected, base=%p, scanning...", (void *)GetModuleHandleA(nullptr));
    patch(P1, sizeof(P1), RET1, sizeof(RET1), "Verifycert");
    patch(P2, sizeof(P2), RET0, sizeof(RET0), "Getresult");
    logf("certpatch done");
    return 0;
}

BOOL WINAPI DllMain(HINSTANCE h, DWORD reason, LPVOID)
{
    if (reason == DLL_PROCESS_ATTACH) { DisableThreadLibraryCalls(h); CreateThread(nullptr, 0, Worker, nullptr, 0, nullptr); }
    return TRUE;
}
