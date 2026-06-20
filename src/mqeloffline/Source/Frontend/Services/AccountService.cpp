/*
    Initial author: Convery (tcn@hedgehogscience.com)
    Started: 15-01-2018
    License: MIT
    Notes:
        Provides implementations for Account-modification services.
*/

#pragma once
#include "../../Stdinclude.hpp"

// Endpoints.
void ChooseDisplayName(Gameserver *Server, std::string Request, std::string Body)
{
    Request_t Parsed{ Body };
    auto Username = Parsed.Get("displayName", "Invalid");

    /*
        TODO(Convery):
        Implement an account-info handler.
    */

    Debugprint(va("Accountname changed to: \"%s\"", Username.c_str()));
    Sendreply(Server, "{}");
}

// Add the services to the gameserver on startup.
namespace {
    struct Startup {
        Startup()
        {
            Mapservice("/AccountService.hqs/ChooseDisplayName", ChooseDisplayName);
        };
    };
    static Startup Loader{};
}
