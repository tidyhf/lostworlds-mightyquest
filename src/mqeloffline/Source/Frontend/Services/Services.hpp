/*
    Initial author: Convery (tcn@hedgehogscience.com)
    Started: 14-01-2018
    License: MIT
    Notes:
        Includes and declares all services.
*/

#pragma once
#include "../../Stdinclude.hpp"

using Servicecallback = std::function<void(struct Gameserver *Server, std::string Request, std::string Body)>;
extern void Mapservice(std::string Request, Servicecallback Callback);
extern void Sendreply(struct Gameserver *Server, std::string Result);
extern std::unordered_map<std::string, Servicecallback> *Servicemap;
