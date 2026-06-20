// MQEL standalone game server.
// Reuses MQELOffline's frontend (Gameserver + .hqs services + Backend + the OpenSSL
// ISSLServer) over a real loopback/LAN TCP listener on :443. Clients reach us via a
// hosts redirect; the injected cert-patch makes the game accept our self-signed cert.
//
// Part of the Lost Worlds project.  MIT licensed. Game server logic (c) Convery /
// Hedgehogscience (MIT); standalone harness (c) the Lost Worlds project.

#include <winsock2.h>   // MUST precede windows.h (via Stdinclude) to avoid winsock1 clash
#include <ws2tcpip.h>
#include <direct.h>
#include <cstdio>
#include <cstdarg>
#include <ctime>
#include <thread>
#include <mutex>
#include <vector>
#include <exception>

#include "Stdinclude.hpp"
#include "Frontend/Servers/Servers.hpp"

#pragma comment(lib, "ws2_32.lib")

// ---- Portable logging: console + ./mqelserver.log (relative to working dir) ----
static std::mutex g_loglk;
static void LOG(const char *fmt, ...)
{
    char line[1024];
    va_list ap; va_start(ap, fmt); _vsnprintf_s(line, sizeof(line), _TRUNCATE, fmt, ap); va_end(ap);
    char stamp[32]; std::time_t t = std::time(nullptr);
    std::strftime(stamp, sizeof(stamp), "%H:%M:%S", std::localtime(&t));
    std::lock_guard<std::mutex> lk(g_loglk);
    std::printf("[%s] %s\n", stamp, line); std::fflush(stdout);
    FILE *f = std::fopen("mqelserver.log", "a");
    if (f) { std::fprintf(f, "[%s] %s\n", stamp, line); std::fclose(f); }
}

static std::mutex g_lock;  // serialize all access to the (non-thread-safe) frontend

// Handlers reply with Send(0, ...) (a "broadcast" in ISSLServer). Redirect it to the
// socket currently being serviced so concurrent connections don't cross-talk.
struct LocalGameserver : Gameserver
{
    size_t Current = 0;
    virtual void Send(const size_t Socket, const void *Data, const uint32_t Size) override
    {
        ISSLServer::Send(Socket == 0 ? Current : Socket, Data, Size);
    }
};
static LocalGameserver *g_gs = nullptr;

static void DrainTo(SOCKET cs, size_t sid)
{
    std::vector<char> out(65536);
    for (;;)
    {
        uint32_t len = (uint32_t)out.size();
        if (!g_gs->onStreamread(sid, out.data(), &len)) break;
        int sent = 0;
        while (sent < (int)len) { int s = send(cs, out.data() + sent, (int)len - sent, 0); if (s <= 0) return; sent += s; }
    }
}

static void HandleConnection(SOCKET cs)
{
    size_t sid = (size_t)cs;
    // Guard the whole connection: a fault in the TLS/stream/dispatch path (e.g.
    // the client dropping mid-request on logout) must only drop THIS connection,
    // never take down the server for everyone. With /EHa this also traps SEH
    // faults (access violations), so a single bad connection can't crash us.
    try
    {
        { std::lock_guard<std::mutex> lk(g_lock); g_gs->onConnect(sid, 443); }
        char buf[16384];
        for (;;)
        {
            int n = recv(cs, buf, sizeof(buf), 0);
            if (n <= 0) break;
            std::lock_guard<std::mutex> lk(g_lock);
            g_gs->Current = sid;
            g_gs->onStreamwrite(sid, buf, (uint32_t)n);  // TLS + HTTP parse + dispatch
            DrainTo(cs, sid);
        }
        { std::lock_guard<std::mutex> lk(g_lock); g_gs->onDisconnect(sid); }
    }
    catch (const std::exception &e) { LOG("connection %u dropped (error: %s)", (unsigned)sid, e.what()); }
    catch (...)                     { LOG("connection %u dropped (unknown fault)", (unsigned)sid); }
    closesocket(cs);
}

int main(int argc, char **argv)
{
    unsigned short port = 443;
    if (argc > 1) port = (unsigned short)atoi(argv[1]);   // optional: mqelserver.exe <port>

    LOG("MQEL standalone server starting (Lost Worlds project)");
    _mkdir("Plugins"); _mkdir("Plugins\\Logs");   // MQELOffline persists data/logs here

    try
    {
        WSADATA wsa;
        if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) { LOG("WSAStartup failed"); return 1; }

        LOG("Generating TLS certificate (OpenSSL)...");
        g_gs = new LocalGameserver();
        LOG("Backend loaded, certificate ready.");

        SOCKET ls = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (ls == INVALID_SOCKET) { LOG("socket() failed %d", WSAGetLastError()); return 2; }
        int opt = 1; setsockopt(ls, SOL_SOCKET, SO_REUSEADDR, (char *)&opt, sizeof(opt));
        sockaddr_in addr{}; addr.sin_family = AF_INET; addr.sin_addr.s_addr = htonl(INADDR_ANY); addr.sin_port = htons(port);
        if (bind(ls, (sockaddr *)&addr, sizeof(addr)) != 0)
        {
            LOG("bind :%u failed (err %d). Port in use, or run elevated if required.", port, WSAGetLastError());
            return 3;
        }
        if (listen(ls, SOMAXCONN) != 0) { LOG("listen failed %d", WSAGetLastError()); return 4; }
        LOG("Listening on 0.0.0.0:%u  --  point the client's host entry here and play.", port);

        for (;;)
        {
            SOCKET cs = accept(ls, nullptr, nullptr);
            if (cs == INVALID_SOCKET) { LOG("accept failed %d", WSAGetLastError()); continue; }
            std::thread(HandleConnection, cs).detach();
        }
    }
    catch (const std::exception &e) { LOG("FATAL exception: %s", e.what()); return 10; }
    catch (...) { LOG("FATAL unknown exception"); return 11; }
}
