/*
    Initial author: Convery (tcn@hedgehogscience.com)
    Started: 14-01-2018
    License: MIT
    Notes:
        Provides implementations for ServerCommand services.
*/

#include "../../Stdinclude.hpp"

// Statistics / telemetry.
void Handletracking(Request_t Request)
{
    // Get the tracking-type.
    auto Trackinginfo = Request.Get("TrackingTag", Message_t());
    auto Type = Trackinginfo.Get("$type", "Invalid");
    Type = Type.substr(0, Type.find_first_of(','));
    Type = Type.substr(Type.find_last_of('.') + 1);

    // Handle the telemetry.
    switch (Hash::FNV1a_32(Type.c_str()))
    {
        // Client has started.
        case Hash::FNV1a_32("GameStartTracking"):
        {
            auto Version = Trackinginfo.Get("GameClientVersion", "");
            auto Timestamp = Trackinginfo.Get("CreationDate", "");
            auto MachineID = Trackinginfo.Get("MachineId", "");
            auto TagID = Trackinginfo.Get("TrackingTagId", 0);

            Infoprint(va("Telemetry: Client started with game-version %s and HWID %s", Version.c_str(), MachineID.c_str()));
            break;
        }

        // Clients state has changed.
        case Hash::FNV1a_32("GameStateTracking"):
        {
            auto Totaltime = Trackinginfo.Get("GameStateTotalTime", 0);
            auto Nextstate = Trackinginfo.Get("NextGameStateType", 0);
            auto Idletime = Trackinginfo.Get("GameStateIdleTime", 0);
            auto Statetype = Trackinginfo.Get("GameStateType", 0);
            auto Timestamp = Trackinginfo.Get("CreationDate", "");
            auto StateID = Trackinginfo.Get("GameStateId", 0);
            auto TagID = Trackinginfo.Get("TrackingTagId", 0);

            Infoprint(va("Telemetry: Gamestate %i updated", StateID));
            break;
        }

        // Client wants a new logfile / epoch.
        case Hash::FNV1a_32("GameInitializeTracking"):
        {
            auto Timestamp = Trackinginfo.Get("CreationDate", "");
            auto TagID = Trackinginfo.Get("TrackingTagId", 0);

            Infoprint("Telemetry: Epoch changed");
            break;
        }

        // Errors.
        case Hash::FNV1a_32("Invalid"): Infoprint("Could not parse telemetry."); break;
        default: Infoprint(va("No handler for tracking-tag \"%s\".", Type.c_str())); break;
    }
}
void Handleidle(Request_t Request)
{
    auto Idletime = Request.Get("IdleTime", 0);

    Infoprint(va("Telemetry: Client was idle for %i seconds.", Idletime));
}

// Questing.
void Handleassignmentupdate(Request_t Request)
{
    auto AssignmentID = Request.Get("AssignmentId", 0);
    auto ActionID = Request.Get("ActionIndex", 0);

    Backend::Quest::Update(AssignmentID, ActionID);
}
void Handleassignmentaccepted(Request_t Request)
{
    auto AssignmentID = Request.Get("AssignmentId", 0);

    Backend::Quest::Start(AssignmentID);
}
void Handleassignmentcomplete(Request_t Request)
{
    auto AssignmentID = Request.Get("AssignmentId", 0);

    Backend::Quest::Complete(AssignmentID);
}

// Marketplace.
void Handlecastlebuy(Request_t Request)
{
    Printfunction();

    // Set the max capacity for gold and life.
    Backend::Wallet::Updateamount(eCurrencytype::IGC, 2000);
    Backend::Wallet::Updateamount(eCurrencytype::Lifeforce, 2000);

    /* TODO(Convery): Implement these notifications rather than hardcoding. */
    Backend::Notification::Enqueuelocal(MQEL_json::parse(R"({"$type":"HyperQuest.GameServer.Contracts.CastleBoughtNotification, HyperQuest.GameServer.Contracts","IsStartupCastle":true,"BuildInfo":{"Draft":{"AccountId":3142975,"LayoutId":1,"CreationDate":"2016-10-23T07:20:03Z","ModificationDate":"2016-10-24T00:00:50Z","Rooms":[{"X":4,"Y":3,"Id":1,"SpecContainerId":21},{"Buildings":[{"Rank":1,"RoomZoneId":12,"X":3,"Y":3,"Orientation":2,"Id":1,"SpecContainerId":1},{"Rank":1,"RoomZoneId":11,"Id":2,"SpecContainerId":3},{"Rank":1,"RoomZoneId":13,"Id":3,"SpecContainerId":4},{"RoomZoneId":7,"X":3,"Orientation":3,"Id":4,"SpecContainerId":13},{"RoomZoneId":4,"X":3,"Orientation":3,"Id":6,"SpecContainerId":10},{"RoomZoneId":6,"X":3,"Y":3,"Orientation":2,"Id":7,"SpecContainerId":8},{"RoomZoneId":3,"Y":3,"Orientation":1,"Id":8,"SpecContainerId":9}],"X":3,"Y":3,"Id":3,"SpecContainerId":25}],"ThemeId":21},"Level":1,"InventoryThemes":[ 2,21],"RoomNextIndex":4,"CreatureNextIndex":40,"TrapNextIndex":5,"DecorationNextIndex":32,"TriggerNextIndex":1,"BuildingNextIndex":9,"CastleStats":{"TotalConstructionPoints":56,"MaxConstructionPoints":56,"WinRatio":0.5,"WinRatioDifficulty":2},"CastleHeartRank":1},"NotificationType":86})"));
    Backend::Notification::Enqueuelocal(MQEL_json::parse(R"({"$type":"HyperQuest.GameServer.Contracts.SkusModifiersUpdatedNotification, HyperQuest.GameServer.Contracts","ShopSkuModifiers":[{"SkuCode":"15221","LimitedQuantityMax":15,"LimitedQuantityNextUnlockCastleLevelRequirement":3,"LimitedQuantityNextUnlockQuantity":10},{"SkuCode":"13911","LimitedQuantityMax":1,"LimitedQuantityNextUnlockCastleLevelRequirement":2,"LimitedQuantityNextUnlockQuantity":1},{"SkuCode":"15266","LimitedQuantityMax":2},{"SkuCode":"15226","LimitedQuantityMax":20,"LimitedQuantityNextUnlockCastleLevelRequirement":2,"LimitedQuantityNextUnlockQuantity":5},{"SkuCode":"15224","LimitedQuantityMax":2,"LimitedQuantityNextUnlockCastleLevelRequirement":2,"LimitedQuantityNextUnlockQuantity":1},{"SkuCode":"15502","LimitedQuantityMax":4},{"SkuCode":"15503","LimitedQuantityMax":4},{"SkuCode":"15504","LimitedQuantityMax":3},{"SkuCode":"15513","LimitedQuantityMax":3},{"SkuCode":"15262","LimitedQuantityNextUnlockCastleLevelRequirement":2,"LimitedQuantityNextUnlockQuantity":4},{"SkuCode":"15229","LimitedQuantityNextUnlockCastleLevelRequirement":3,"LimitedQuantityNextUnlockQuantity":2},{"SkuCode":"15220","LimitedQuantityNextUnlockCastleLevelRequirement":4,"LimitedQuantityNextUnlockQuantity":20},{"SkuCode":"15264","LimitedQuantityNextUnlockCastleLevelRequirement":4,"LimitedQuantityNextUnlockQuantity":2},{"SkuCode":"15230","LimitedQuantityNextUnlockCastleLevelRequirement":5,"LimitedQuantityNextUnlockQuantity":5},{"SkuCode":"15263","LimitedQuantityNextUnlockCastleLevelRequirement":6,"LimitedQuantityNextUnlockQuantity":2},{"SkuCode":"15268","LimitedQuantityNextUnlockCastleLevelRequirement":6,"LimitedQuantityNextUnlockQuantity":5},{"SkuCode":"15225","LimitedQuantityNextUnlockCastleLevelRequirement":7,"LimitedQuantityNextUnlockQuantity":2},{"SkuCode":"15467","LimitedQuantityNextUnlockCastleLevelRequirement":8,"LimitedQuantityNextUnlockQuantity":3},{"SkuCode":"15234","LimitedQuantityNextUnlockCastleLevelRequirement":9,"LimitedQuantityNextUnlockQuantity":1},{"SkuCode":"16070","LimitedQuantityNextUnlockQuantity":2},{"SkuCode":"16071","LimitedQuantityNextUnlockQuantity":2},{"SkuCode":"16072","LimitedQuantityNextUnlockQuantity":2},{"SkuCode":"16073","LimitedQuantityNextUnlockQuantity":2},{"SkuCode":"16074","LimitedQuantityNextUnlockQuantity":2},{"SkuCode":"16075","LimitedQuantityNextUnlockQuantity":2},{"SkuCode":"16076","LimitedQuantityNextUnlockQuantity":2},{"SkuCode":"16077","LimitedQuantityNextUnlockQuantity":2},{"SkuCode":"16078","LimitedQuantityNextUnlockQuantity":2},{"SkuCode":"16079","LimitedQuantityNextUnlockQuantity":2},{"SkuCode":"16080","LimitedQuantityNextUnlockQuantity":2},{"SkuCode":"16049","LimitedQuantityNextUnlockQuantity":1},{"SkuCode":"15805","LimitedQuantityNextUnlockQuantity":1},{"SkuCode":"15810","LimitedQuantityNextUnlockQuantity":2},{"SkuCode":"15811","LimitedQuantityNextUnlockQuantity":2},{"SkuCode":"15806","LimitedQuantityNextUnlockQuantity":1},{"SkuCode":"15835","LimitedQuantityNextUnlockQuantity":1},{"SkuCode":"15807","LimitedQuantityNextUnlockQuantity":1},{"SkuCode":"15812","LimitedQuantityNextUnlockQuantity":2},{"SkuCode":"15808","LimitedQuantityNextUnlockQuantity":1},{"SkuCode":"15804","LimitedQuantityNextUnlockQuantity":1},{"SkuCode":"15809","LimitedQuantityNextUnlockQuantity":1},{"SkuCode":"15813","LimitedQuantityNextUnlockQuantity":1},{"SkuCode":"15825","LimitedQuantityNextUnlockQuantity":3},{"SkuCode":"15834","LimitedQuantityNextUnlockQuantity":1},{"SkuCode":"15872","LimitedQuantityNextUnlockQuantity":1},{"SkuCode":"15974","LimitedQuantityNextUnlockQuantity":1},{"SkuCode":"15957","LimitedQuantityNextUnlockQuantity":1},{"SkuCode":"15814","LimitedQuantityNextUnlockQuantity":1},{"SkuCode":"15892","LimitedQuantityNextUnlockQuantity":1},{"SkuCode":"16067","LimitedQuantityNextUnlockQuantity":1},{"SkuCode":"15815","LimitedQuantityNextUnlockQuantity":1},{"SkuCode":"15816","LimitedQuantityNextUnlockQuantity":2},{"SkuCode":"16068","LimitedQuantityNextUnlockQuantity":1},{"SkuCode":"15827","LimitedQuantityNextUnlockQuantity":6},{"SkuCode":"15828","LimitedQuantityNextUnlockQuantity":3},{"SkuCode":"15817","LimitedQuantityNextUnlockQuantity":1},{"SkuCode":"15856","LimitedQuantityNextUnlockQuantity":2},{"SkuCode":"16006","LimitedQuantityNextUnlockQuantity":1},{"SkuCode":"16055","LimitedQuantityNextUnlockQuantity":2},{"SkuCode":"16058","LimitedQuantityNextUnlockQuantity":2},{"SkuCode":"15818","LimitedQuantityNextUnlockQuantity":1},{"SkuCode":"15819","LimitedQuantityNextUnlockQuantity":1},{"SkuCode":"15855","LimitedQuantityNextUnlockQuantity":2},{"SkuCode":"15890","LimitedQuantityNextUnlockQuantity":1},{"SkuCode":"16059","LimitedQuantityNextUnlockQuantity":2},{"SkuCode":"16060","LimitedQuantityNextUnlockQuantity":2},{"SkuCode":"15820","LimitedQuantityNextUnlockQuantity":3},{"SkuCode":"15821","LimitedQuantityNextUnlockQuantity":2},{"SkuCode":"15874","LimitedQuantityNextUnlockQuantity":1},{"SkuCode":"15981","LimitedQuantityNextUnlockQuantity":1},{"SkuCode":"16064","LimitedQuantityNextUnlockQuantity":2},{"SkuCode":"16057","LimitedQuantityNextUnlockQuantity":2},{"SkuCode":"16061","LimitedQuantityNextUnlockQuantity":2},{"SkuCode":"15822","LimitedQuantityNextUnlockQuantity":1},{"SkuCode":"16010","LimitedQuantityNextUnlockQuantity":1},{"SkuCode":"15961","LimitedQuantityNextUnlockQuantity":1},{"SkuCode":"15853","LimitedQuantityNextUnlockQuantity":1},{"SkuCode":"15984","LimitedQuantityNextUnlockQuantity":1},{"SkuCode":"16062","LimitedQuantityNextUnlockQuantity":2},{"SkuCode":"16063","LimitedQuantityNextUnlockQuantity":2},{"SkuCode":"15823","LimitedQuantityNextUnlockQuantity":1},{"SkuCode":"15830","LimitedQuantityNextUnlockQuantity":3},{"SkuCode":"15977","LimitedQuantityNextUnlockQuantity":1},{"SkuCode":"16046","LimitedQuantityNextUnlockQuantity":1},{"SkuCode":"16065","LimitedQuantityNextUnlockQuantity":2},{"SkuCode":"16066","LimitedQuantityNextUnlockQuantity":2},{"SkuCode":"15854","LimitedQuantityNextUnlockQuantity":1},{"SkuCode":"16032","LimitedQuantityNextUnlockQuantity":1}],"NotificationType":90})"));
}
void Handleitembuy(Request_t Request)
{
    auto Materials = Request.Get("ClientCraftingMaterials", Message_t());
    auto Consumed = Request.Get("ConsumedHeroInventory", Message_t());
    auto Discounted = Request.Get("DiscountApplied", false);
    auto Price = Request.Get("ClientPrice", Message_t());
    auto Currencytype = Price.Get("CurrencyType", 0);
    auto Currencyamount = Price.Get("Amount", 0);
    auto SKUCode = Request.Get("SkuCode", "");
    auto Slot = Request.Get("SlotIndex", 0);

    // Subtract the price from our wallet..
    Backend::Wallet::Updateamount((eCurrencytype)Currencytype, Currencyamount * -1);
    Infoprint(va("Bought item %s for %i %s.", SKUCode.c_str(), Currencyamount, [&Currencytype]()
    {
        switch ((eCurrencytype)Currencytype)
        {
            case eCurrencytype::PremiumCash:    return "Premium-cash";
            case eCurrencytype::Lifeforce:      return "Lifeforce";
            case eCurrencytype::IGC:            return "Gold";
            default:                            return "???";
        }
    }()));

    /*
        TODO(Convery):
        Implement some type of inventory system.
    */
}
void Handleconsumable(Request_t Request)
{
    auto Materials = Request.Get("ClientCraftingMaterials", Message_t());
    auto Consumed = Request.Get("ConsumedHeroInventory", Message_t());
    auto Discounted = Request.Get("DiscountApplied", false);
    auto Slots = Request.Get("SlotIndexes", Message_t());
    auto Price = Request.Get("ClientPrice", Message_t());
    auto Currencytype = Price.Get("CurrencyType", 0);
    auto Currencyamount = Price.Get("Amount", 0);
    auto SKUCode = Request.Get("SkuCode", "");

    // Subtract the price from our wallet..
    Backend::Wallet::Updateamount((eCurrencytype)Currencytype, Currencyamount * -1);
    Infoprint(va("Bought consumable %s for %i %s.", SKUCode.c_str(), Currencyamount, [&Currencytype]()
    {
        switch ((eCurrencytype)Currencytype)
        {
            case eCurrencytype::PremiumCash:    return "Premium-cash";
            case eCurrencytype::Lifeforce:      return "Lifeforce";
            case eCurrencytype::IGC:            return "Gold";
            default:                            return "???";
        }
    }()));

    /*
        TODO(Convery):
        Implement some type of inventory system.
    */
}

// Hero modification.
void Handleequipment(Request_t Request)
{
    auto DSTSlot = Request.Get("DestinationSlot", 0);
    auto SRCSlot = Request.Get("SourceSlotId", 0);
    auto HeroID = Request.Get("HeroId", 0);

    Infoprint(va("Shuffled the %s's inventory from %i to %i.", [&HeroID]() -> const char *
    {
        switch ((eHerotype)HeroID)
        {
            case eHerotype::Archer:     return "Archer";
            case eHerotype::Knight:     return "Knight";
            case eHerotype::Mage:       return "Mage";
            case eHerotype::Runaway:    return "Runaway";
            default:                    return "???";
        }
    }(), SRCSlot, DSTSlot));

    /*
        TODO(Convery):
        Implement some type of inventory system.
    */
}
void Handleinbox(Request_t Request)
{
    auto Slots = Request.Get("SlotIndexes", Message_t());
    auto Item = Request.Get("ObjectId", "");

    Infoprint(va("Acquired item %s", Item.c_str()));

    /*
        TODO(Convery):
        Implement some type of inventory system.
    */
}

// Endpoints.
void SendCommand(Gameserver *Server, std::string Request, std::string Body)
{
    Request_t Parsed{ Body };

    // Deserialize the commands.
    for (auto &Item : Parsed.Get("commands", std::vector<Message_t>{}))
    {
        // Get the command-type.
        auto Type = Item.Get("$type", "Invalid");
        Type = Type.substr(0, Type.find_first_of(','));
        Type = Type.substr(Type.find_last_of('.') + 1);

        // Trigger the callback.
        switch (Hash::FNV1a_32(Type.c_str()))
        {
            // Statistics / telemetry.
            case Hash::FNV1a_32("TrackingCommand"): Handletracking(Item); break;
            case Hash::FNV1a_32("ClientIdleCommand"): Handleidle(Item); break;

            // Questing.
            case Hash::FNV1a_32("StartAssignmentCommand"): Handleassignmentaccepted(Item); break;
            case Hash::FNV1a_32("CompleteAssignmentCommand"): Handleassignmentcomplete(Item); break;
            case Hash::FNV1a_32("ExecuteAssignmentActionCommand"): Handleassignmentupdate(Item); break;

            // Marketplace.
            case Hash::FNV1a_32("BuyCommand"): Handlecastlebuy(Item); break;
            case Hash::FNV1a_32("BuyHeroItemCommand"): Handleitembuy(Item); break;
            case Hash::FNV1a_32("BuyConsumableCommand"): Handleconsumable(Item); break;

            // Hero modification.
            case Hash::FNV1a_32("HeroEquipmentEquipCommand"): Handleequipment(Item); break;
            case Hash::FNV1a_32("InboxCollectToHeroInventoryCommand"): Handleinbox(Item); break;

            // Errors.
            case Hash::FNV1a_32("Invalid"): Infoprint("Could not parse command."); break;
            default: Infoprint(va("No handler for command \"%s\".", Type.c_str())); break;
        }
    }

    Sendreply(Server, "{}");
}

// Add the services to the gameserver on startup.
namespace {
    struct Startup {
        Startup()
        {
            Mapservice("/ServerCommandService.hqs/SendCommands", SendCommand);
        };
    };
    static Startup Loader{};
}
