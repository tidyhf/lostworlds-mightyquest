/*
    Initial author: Convery (tcn@hedgehogscience.com)
    Started: 31-01-2018
    License: MIT
    Notes:
        Provides currency tracking.
*/

#include "../Stdinclude.hpp"

namespace Backend
{
    namespace Wallet
    {
        struct Wallet_t
        {
            int32_t Amount;
            uint32_t Capacity;
        };
        std::array<Wallet_t, (int)eCurrencytype::Count> Wallets{};

        // Notify the frontend when updating.
        void NotifyWalletamountupdate(int Type)
        {
            MQEL_json Notification = MQEL_json::object();
            Notification["$type"] = "HyperQuest.GameServer.Contracts.WalletUpdatedNotification, HyperQuest.GameServer.Contracts";

            Notification["Amounts"] += Serialize(Type);

            Notification["NotificationType"] = 24;
            Backend::Notification::Enqueuelocal(Notification);
        }
        void NotifyWalletcapacityupdate(int Type)
        {
            MQEL_json Notification = MQEL_json::object();
            Notification["$type"] = "HyperQuest.GameServer.Contracts.WalletCapacityUpdatedNotification, HyperQuest.GameServer.Contracts";

            Notification["CurrencyType"] = (uint32_t)Type;
            Notification["Amount"] = Wallets[(uint32_t)Type].Capacity;

            Notification["NotificationType"] = 47;
            Backend::Notification::Enqueuelocal(Notification);
        }

        // Modify the wallet state.
        template<> int32_t Getamount(int Type)
        {
            return Wallets[Type].Amount;
        }
        template<> int32_t Getamount(eCurrencytype Type)
        {
            return Getamount((int)Type);
        }
        template<> uint32_t Getcapacity(int Type)
        {
            return Wallets[Type].Capacity;
        }
        template<> uint32_t Getcapacity(eCurrencytype Type)
        {
            return Getcapacity((int)Type);
        }
        template<> void Setcapacity(int Type, uint32_t Max)
        {
            Wallets[Type].Capacity = Max;
            NotifyWalletcapacityupdate(Type);
        }
        template<> void Setcapacity(eCurrencytype Type, uint32_t Max)
        {
            return Setcapacity((int)Type, Max);
        }
        template<> void Updateamount(int Type, int32_t Delta)
        {
            Wallets[Type].Amount += Delta;
            if (Wallets[Type].Amount < 0)
            {
                Wallets[Type].Amount = 0;
            }

            NotifyWalletamountupdate(Type);
        }
        template<> void Updateamount(eCurrencytype Type, int32_t Delta)
        {
            return Updateamount((int)Type, Delta);
        }

        // Serialize to game-readable JSON.
        template<> MQEL_json Serialize(eCurrencytype Type)
        {
            auto Object = MQEL_json::object();

            Object["CurrencyType"] = (int)Type;
            if (Wallets[(int)Type].Amount)
            {
                Object["Amount"] = Wallets[(int)Type].Amount;
            }

            return Object;
        }
        template<typename T> MQEL_json Serialize(T Type)
        {
            return Serialize((eCurrencytype)Type);
        }

        // Load the wallets on startup.
        void Savewallets()
        {
            auto Object = MQEL_json::object();

            // Serialize the array.
            for (size_t i = 0; i < (size_t)eCurrencytype::Count; ++i)
            {
                Object[va("%i", i)]["Amount"] = Wallets[i].Amount;
                Object[va("%i", i)]["Capacity"] = Wallets[i].Capacity;
            }

            // Save to the archive.
            std::string Plaintext = Object.dump(4);
            Package::Write("Wallets.json", Plaintext);
        }
        void Loadwallets()
        {
            // Save the wallets on exit.
            std::atexit(Savewallets);

            // Load the file from the archive.
            auto Filebuffer = Package::Read("Wallets.json");
            if (Filebuffer.size() == 0) return;

            // Deserialize the file.
            MQEL_json Object = MQEL_json::parse(Filebuffer);
            for (size_t i = 0; i < (size_t)eCurrencytype::Count; ++i)
            {
                Wallets[i].Amount = Object[va("%i", i)]["Amount"];
                Wallets[i].Capacity = Object[va("%i", i)]["Capacity"];
            }
        }
        namespace { struct Startup { Startup() { Loadwallets(); }; }; static Startup Loader{}; }
    }
}
