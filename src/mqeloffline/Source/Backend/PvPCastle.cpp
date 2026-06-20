/*
    Initial author: Convery (tcn@hedgehogscience.com)
    Started: 08-02-2018
    License: MIT
    Notes:
        Provides matchmaking castles.
*/

#include "../Stdinclude.hpp"

namespace Backend
{
    namespace PvPCastle
    {
        std::unordered_map<uint64_t /* PlayerID */, MQEL_json::object_t> Castles;

        // Populate the castle-map.
        void Loadcastles()
        {
            auto Filelist = Package::Findfiles("Castle_");

            // Iterate over the castles.
            for (auto &Item : Filelist)
            {
                // Get the castleID.
                uint64_t CastleID{ 0 };
                std::sscanf(Item.c_str(), "Castle_%lld", &CastleID);

                // Invalid filename or already loaded.
                if (CastleID == 0) continue;
                auto &Entry = Castles[CastleID];
                if (Entry.size() > 0) continue;

                // Parse the on-disk file.
                try
                {
                    Entry = MQEL_json::parse(Package::Read(Item).c_str());
                }
                catch (std::exception &e)
                {
                    Debugprint(va("%s (\"%s\"): %s", __FUNCTION__, Item.c_str(), e.what()));
                }
            }
        }

        // Get a players castle.
        MQEL_json Getcastle(uint64_t AccountID)
        {
            // Start checking for updated castles.
            static bool Initialized = false;
            if(!Initialized) Createrecurringtask(Loadcastles, 5000);
            Initialized = true;

            return Castles[AccountID];
        }

        // Initialize the castles on startup.
        namespace { struct Startup { Startup() { Loadcastles(); }; }; static Startup Loader{}; }
    }
}
