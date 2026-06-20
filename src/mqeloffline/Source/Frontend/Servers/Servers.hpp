/*
    Initial author: Convery (tcn@hedgehogscience.com)
    Started: 09-01-2018
    License: MIT
    Notes:
        Provides implementations for the servertypes.
*/

#pragma once
#include "../../Stdinclude.hpp"
#include "Interfaces/IServer.hpp"
#include "Interfaces/IStreamserver.hpp"
#include "Interfaces/IDatagramserver.hpp"

#include "Interfaces/ISSLServer.hpp"
#include "Interfaces/IHTTPServer.hpp"

// This game uses a single server for everything.
struct Gameserver : IHTTPSServer
{
    // Callbacks on parsed data.
    virtual void onGET(const size_t Socket, HTTPRequest &Request) override;
    virtual void onPUT(const size_t Socket, HTTPRequest &Request) override;
    virtual void onPOST(const size_t Socket, HTTPRequest &Request) override;
    virtual void onCOPY(const size_t Socket, HTTPRequest &Request) override;
    virtual void onDELETE(const size_t Socket, HTTPRequest &Request) override;

    // Initialize the SSL certificate.
    Gameserver()
    {
        if (!CreateSSLCert("Gameserver"))
        {
            // There's no point in even trying anymore.
            std::exit(3);
        }
    }
};
