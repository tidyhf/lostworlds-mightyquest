/*
    Initial author: Convery (tcn@hedgehogscience.com)
    Started: 14-01-2018
    License: MIT
    Notes:
        Provides implementations for AccouhntInformation services.
*/

#pragma once
#include "../../Stdinclude.hpp"

// Helpers.
MQEL_json GetNews()
{
    auto Object = MQEL_json::object();
    return std::move(Object);
}
MQEL_json GetAchivements()
{
    return MQEL_json::parse(R"([31, 1])");
}
MQEL_json GetShopmodifiers()
{
    return MQEL_json::parse(R"([{"SkuCode":"15224","LimitedQuantityNextUnlockCastleLevelRequirement":2,"LimitedQuantityNextUnlockQuantity":1},{"SkuCode":"15226","LimitedQuantityNextUnlockCastleLevelRequirement":2,"LimitedQuantityNextUnlockQuantity":5},{"SkuCode":"15221","LimitedQuantityNextUnlockCastleLevelRequirement":3,"LimitedQuantityNextUnlockQuantity":10},{"SkuCode":"13911","LimitedQuantityNextUnlockCastleLevelRequirement":3,"LimitedQuantityNextUnlockQuantity":1},{"SkuCode":"15262","LimitedQuantityNextUnlockCastleLevelRequirement":3,"LimitedQuantityNextUnlockQuantity":3},{"SkuCode":"15229","LimitedQuantityNextUnlockCastleLevelRequirement":4,"LimitedQuantityNextUnlockQuantity":1},{"SkuCode":"15230","LimitedQuantityNextUnlockCastleLevelRequirement":6,"LimitedQuantityNextUnlockQuantity":1},{"SkuCode":"15263","LimitedQuantityNextUnlockCastleLevelRequirement":8,"LimitedQuantityNextUnlockQuantity":1},{"SkuCode":"16070","LimitedQuantityNextUnlockQuantity":2},{"SkuCode":"16071","LimitedQuantityNextUnlockQuantity":2},{"SkuCode":"16072","LimitedQuantityNextUnlockQuantity":2},{"SkuCode":"16073","LimitedQuantityNextUnlockQuantity":2},{"SkuCode":"16074","LimitedQuantityNextUnlockQuantity":2},{"SkuCode":"16075","LimitedQuantityNextUnlockQuantity":2},{"SkuCode":"16076","LimitedQuantityNextUnlockQuantity":2},{"SkuCode":"16077","LimitedQuantityNextUnlockQuantity":2},{"SkuCode":"16078","LimitedQuantityNextUnlockQuantity":2},{"SkuCode":"16079","LimitedQuantityNextUnlockQuantity":2},{"SkuCode":"16080","LimitedQuantityNextUnlockQuantity":2},{"SkuCode":"16049","LimitedQuantityNextUnlockQuantity":1},{"SkuCode":"15805","LimitedQuantityNextUnlockQuantity":1},{"SkuCode":"15810","LimitedQuantityNextUnlockQuantity":2},{"SkuCode":"15811","LimitedQuantityNextUnlockQuantity":2},{"SkuCode":"15806","LimitedQuantityNextUnlockQuantity":1},{"SkuCode":"15835","LimitedQuantityNextUnlockQuantity":1},{"SkuCode":"15807","LimitedQuantityNextUnlockQuantity":1},{"SkuCode":"15812","LimitedQuantityNextUnlockQuantity":2},{"SkuCode":"15808","LimitedQuantityNextUnlockQuantity":1},{"SkuCode":"15804","LimitedQuantityNextUnlockQuantity":1},{"SkuCode":"15809","LimitedQuantityNextUnlockQuantity":1},{"SkuCode":"15813","LimitedQuantityNextUnlockQuantity":1},{"SkuCode":"15825","LimitedQuantityNextUnlockQuantity":3},{"SkuCode":"15834","LimitedQuantityNextUnlockQuantity":1},{"SkuCode":"15872","LimitedQuantityNextUnlockQuantity":1},{"SkuCode":"15974","LimitedQuantityNextUnlockQuantity":1},{"SkuCode":"15957","LimitedQuantityNextUnlockQuantity":1},{"SkuCode":"15814","LimitedQuantityNextUnlockQuantity":1},{"SkuCode":"15892","LimitedQuantityNextUnlockQuantity":1},{"SkuCode":"16067","LimitedQuantityNextUnlockQuantity":1},{"SkuCode":"15815","LimitedQuantityNextUnlockQuantity":1},{"SkuCode":"15816","LimitedQuantityNextUnlockQuantity":2},{"SkuCode":"16068","LimitedQuantityNextUnlockQuantity":1},{"SkuCode":"15827","LimitedQuantityNextUnlockQuantity":6},{"SkuCode":"15828","LimitedQuantityNextUnlockQuantity":3},{"SkuCode":"15817","LimitedQuantityNextUnlockQuantity":1},{"SkuCode":"15856","LimitedQuantityNextUnlockQuantity":2},{"SkuCode":"16006","LimitedQuantityNextUnlockQuantity":1},{"SkuCode":"16055","LimitedQuantityNextUnlockQuantity":2},{"SkuCode":"16058","LimitedQuantityNextUnlockQuantity":2},{"SkuCode":"15818","LimitedQuantityNextUnlockQuantity":1},{"SkuCode":"15819","LimitedQuantityNextUnlockQuantity":1},{"SkuCode":"15855","LimitedQuantityNextUnlockQuantity":2},{"SkuCode":"15890","LimitedQuantityNextUnlockQuantity":1},{"SkuCode":"16059","LimitedQuantityNextUnlockQuantity":2},{"SkuCode":"16060","LimitedQuantityNextUnlockQuantity":2},{"SkuCode":"15820","LimitedQuantityNextUnlockQuantity":3},{"SkuCode":"15821","LimitedQuantityNextUnlockQuantity":2},{"SkuCode":"15874","LimitedQuantityNextUnlockQuantity":1},{"SkuCode":"15981","LimitedQuantityNextUnlockQuantity":1},{"SkuCode":"16064","LimitedQuantityNextUnlockQuantity":2},{"SkuCode":"16057","LimitedQuantityNextUnlockQuantity":2},{"SkuCode":"16061","LimitedQuantityNextUnlockQuantity":2},{"SkuCode":"15822","LimitedQuantityNextUnlockQuantity":1},{"SkuCode":"16010","LimitedQuantityNextUnlockQuantity":1},{"SkuCode":"15961","LimitedQuantityNextUnlockQuantity":1},{"SkuCode":"15853","LimitedQuantityNextUnlockQuantity":1},{"SkuCode":"15984","LimitedQuantityNextUnlockQuantity":1},{"SkuCode":"16062","LimitedQuantityNextUnlockQuantity":2},{"SkuCode":"16063","LimitedQuantityNextUnlockQuantity":2},{"SkuCode":"15823","LimitedQuantityNextUnlockQuantity":1},{"SkuCode":"15830","LimitedQuantityNextUnlockQuantity":3},{"SkuCode":"15977","LimitedQuantityNextUnlockQuantity":1},{"SkuCode":"16046","LimitedQuantityNextUnlockQuantity":1},{"SkuCode":"16065","LimitedQuantityNextUnlockQuantity":2},{"SkuCode":"16066","LimitedQuantityNextUnlockQuantity":2},{"SkuCode":"15854","LimitedQuantityNextUnlockQuantity":1},{"SkuCode":"16032","LimitedQuantityNextUnlockQuantity":1}])");
}
MQEL_json GetClientsettings()
{
    return MQEL_json::parse(R"({"ClientTrackingSettings":{"ClientIdleInterval":60,"GameStateTrackingInterval":300,"IdleTimeThresholds":{"None":30,"Build":30,"Attack":30,"AccountCreation":30,"Home":30,"AttackSelection":30,"Replay":600,"StartMenu":30,"Patcher":30,"UplayLinking":30,"StarterCastleSelection":30,"CastleVisit":30},"GlanceViewDurationTriggerInSeconds":5,"EnabledNavigationTrackings":["game:\/\/forge\/open"]},"XmppInfo":{"Username":"3123971","Password":"AkiTXjIk+EqXzFWmFJGbug==","Domain":"mqel-live","Server":"chat.themightyquest.com","Port":80,"Enabled":true,"ConferenceServer":"conference.mqel-live"},"PrimaryShopUrl":"https:\/\/www.themightyquest.com\/%s\/shop?game_token=%s&steamID=%s&steamTicket=%s&embedded=1","PrimaryShopProductPageUrl":"https:\/\/www.themightyquest.com\/%s\/shop\/%s?game_token=%s&steamID=%s&steamTicket=%s&embedded=1","PrimaryShopPremiumPurchaseUrl":"https:\/\/www.themightyquest.com\/%s\/shop\/category\/blings?minimum_blings_quantity=%i&game_token=%s&steam_id=%s&steam_ticket=%s&embedded=1","PrimaryShopBlingsUrl":"https:\/\/www.themightyquest.com\/%s\/shop\/category\/blings?game_token=%s&steamID=%s&steamTicket=%s&embedded=1","PrimaryShopNonBlingsUrl":"https:\/\/www.themightyquest.com\/%s\/shop?game_token=%s&steamID=%s&steamTicket=%s&embedded=1&excluding_category=blings","FriendReferalUrl":"https:\/\/www.themightyquest.com\/%s\/invites\/new?game_token=%s&embedded=1","WelcomePageSmallUrl":"https:\/\/www.themightyquest.com\/%s\/game_welcome_page\/banner?game_token=%s","WelcomePageUrl":"https:\/\/www.themightyquest.com\/%s\/game_welcome_page?game_token=%s","ShowWelcomePage":true,"MaintenanceUrl":"https:\/\/www.themightyquest.com\/%s\/blog"})");
}
MQEL_json GetWallet(bool Herocreated)
{
    auto Object = MQEL_json::object();

    if (Herocreated)
    {
        Object["InGameCoin"] = Backend::Wallet::Getamount(eCurrencytype::IGC);
        Object["LifeForce"] = Backend::Wallet::Getamount(eCurrencytype::Lifeforce);
        Object["InGameCoinStorageCapacity"] = Backend::Wallet::Getcapacity(eCurrencytype::IGC);
        Object["LifeForceStorageCapacity"] = Backend::Wallet::Getcapacity(eCurrencytype::Lifeforce);
    }
    else
    {
        Object = MQEL_json::parse(R"({"InGameCoin":1000})");
    }

    return Object;
}
MQEL_json GetBuild(bool Herocreated)
{
    auto Object = MQEL_json::object();

    if (Herocreated)
    {
        Object = MQEL_json::parse(R"({"Draft":{"AccountId":3123895,"AccountDisplayName":"Ness199X","LayoutId":1,"CreationDate":"2016-08-27T01:10:00Z","ModificationDate":"2016-08-27T01:19:08Z","Rooms":[{"X":4,"Y":3,"Id":1,"SpecContainerId":21},{"Buildings":[{"Rank":1,"RoomZoneId":12,"X":3,"Y":3,"Orientation":2,"Id":1,"SpecContainerId":1},{"Rank":1,"RoomZoneId":11,"Id":2,"SpecContainerId":3},{"Rank":1,"RoomZoneId":13,"Id":3,"SpecContainerId":4},{"RoomZoneId":7,"X":3,"Orientation":3,"Id":4,"SpecContainerId":13},{"RoomZoneId":4,"X":3,"Orientation":3,"Id":6,"SpecContainerId":10},{"RoomZoneId":6,"X":3,"Y":3,"Orientation":2,"Id":7,"SpecContainerId":8},{"RoomZoneId":3,"Y":3,"Orientation":1,"Id":8,"SpecContainerId":9}],"X":3,"Y":3,"Id":3,"SpecContainerId":25}],"ThemeId":22},"Level":1,"InventoryThemes":[1,22],"RoomNextIndex":4,"CreatureNextIndex":33,"TrapNextIndex":4,"DecorationNextIndex":34,"TriggerNextIndex":1,"BuildingNextIndex":9,"CastleStats":{"TotalConstructionPoints":55,"MaxConstructionPoints":55,"WinRatio":0.5,"WinRatioDifficulty":2},"CastleHeartRank":1})");
    }
    else
    {
        Object = MQEL_json::parse(R"({"Draft":{"AccountId":3142975,"LayoutId":1,"CreationDate":"2016-10-23T07:20:03Z","ModificationDate":"2016-10-23T07:20:03Z","ThemeId":2},"InventoryThemes":[2],"RoomNextIndex":1,"CreatureNextIndex":1,"TrapNextIndex":1,"DecorationNextIndex":1,"TriggerNextIndex":1,"BuildingNextIndex":1,"CastleStats":{"MaxConstructionPoints":20,"WinRatio":0.5,"WinRatioDifficulty":2}})");
    }

    return std::move(Object);
}
MQEL_json GetHeroes()
{
    auto Object = MQEL_json::array();

    for (int i = 0; i < (int)eHerotype::Count; ++i)
    {
        auto Serialized = Backend::Hero::Serialize(i);
        if (Serialized.is_null()) continue;
        Object += Serialized;
    }

    return std::move(Object);
}
MQEL_json GetStats(bool Herocreated)
{
    if (Herocreated)
    {
        return MQEL_json::parse(R"({"TotalCreaturesKilled":193,"AttackTotalIGCWon":797,"TotalCastlesLooted":9,"CastlesDefeated":{"Medium":5,"Easy":3,"Hard":1},"KilledCreatures":{"1081":78,"1029":7,"1003":1,"1000":21,"1006":3,"1023":4,"1079":8,"1155":1,"1001":24,"1026":9,"1024":7,"1016":5,"1009":6,"1015":13,"1002":6},"CurrencyAccumulation":{"IGC":1300,"LifeForce":1006},"DefeatCastleStrike":3,"TotalItemsLooted":27,"TotalPotionsConsumed":5})");
    }
    else
    {
        return MQEL_json::object();
    }
}
MQEL_json GetQuests()
{
    /*
        TODO(Convery):
        Implement the tutorial then:
        Backend::Quest::Completed();
    */
    MQEL_json Object = Backend::Quest::Getcompleted();
    return std::move(Object);
    return MQEL_json::parse(R"([ 10, 26, 21, 20, 30, 90, 120, 5003, 5004, 100, 150, 125, 5005, 5010, 5007, 5006 ])");
}
MQEL_json GetObjectives()
{
    return MQEL_json::parse(R"([{"ObjectiveId":300,"LastStatusDate":"2016-08-27T01:50:22Z","Status":2},{"ObjectiveId":301,"LastStatusDate":"2016-09-03T02:00:26Z","Status":2},{"ObjectiveId":302,"LastStatusDate":"2016-09-03T02:00:26Z","Status":1}])");
}

// Endpoints.
void GetAccountInformation(Gameserver *Server, std::string Request, std::string Body)
{
    // Static account information.
    auto Battlelog = MQEL_json::parse(R"({"OfflinePeriod":{"EndDateTime":"2016-10-16T10:40:52Z"}})");
    auto Inventory = MQEL_json::parse(R"({"InventoryTabCount":2})");
    auto Emotes = MQEL_json::parse(R"([1,2,3])");
    bool Herocreated = Backend::Hero::Getheroclass() != 0;

    // Create the response based on if it's the first run.
    auto Response = MQEL_json::object();
                    Response["Result"]["News"] = GetNews();
    if(Herocreated) Response["Result"]["CompletedAchievements"] = GetAchivements();
                    Response["Result"]["DefendLog"] = Battlelog;
                    Response["Result"]["CountryCode"] = "SE";
                    Response["Result"]["ShopSkuModifiers"] = GetShopmodifiers();
                    Response["Result"]["ClientSettings"] = GetClientsettings();
                    Response["Result"]["TargetedAttackAvailableCount"] = 5;
                    Response["Result"]["AccountId"] = 3123971;
    if(Herocreated) Response["Result"]["DisplayName"] = "Hedgehog";
    if(Herocreated) Response["Result"]["DisplayNameValidationDate"] = "2016-08-27T01:22:52Z";
    if(Herocreated) Response["Result"]["GamerScore"] = 15;
    if(Herocreated) Response["Result"]["SelectedHeroId"] = Backend::Hero::Getheroclass();
                    Response["Result"]["Privileges"] = Herocreated ? 401 : 9;
                    Response["Result"]["Wallet"] = GetWallet(Herocreated);
    if(Herocreated) Response["Result"]["CastleRenovationLevel"] = 2;
                    Response["Result"]["BuildInfo"] = GetBuild(Herocreated);
    if(Herocreated) Response["Result"]["Heroes"] = GetHeroes();
                    Response["Result"]["Inventory"] = Inventory;
                    Response["Result"]["BuyBack"] = MQEL_json::object();
                    Response["Result"]["Stats"] = GetStats(Herocreated);
    if(Herocreated) Response["Result"]["CompletedAssignments"] = GetQuests();
                    Response["Result"]["UnlockedEmotes"] = Emotes;
    if(Herocreated) Response["Result"]["Objectives"] = GetObjectives();
                    Response["Result"]["AvatarId"] = 10;
                    Response["Result"]["ProfanityFiltering"] = true;
    if(Herocreated) Response["Result"]["LeagueId"] = 1;
    if(Herocreated) Response["Result"]["SubLeagueId"] = 1;

    // Return the response.
    Sendreply(Server, Response.dump());
}

// Add the services to the gameserver on startup.
namespace {
    struct Startup {
        Startup()
        {
            Mapservice("/AccountInformationService.hqs/GetAccountInformation", GetAccountInformation);
        };
    };
    static Startup Loader{};
}
