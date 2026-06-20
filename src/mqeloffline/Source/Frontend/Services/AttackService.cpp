/*
    Initial author: Convery (tcn@hedgehogscience.com)
    Started: 15-01-2018
    License: MIT
    Notes:
        Provides implementations for attack services.
*/

#pragma once
#include "../../Stdinclude.hpp"

uint64_t CurrentattackID;
constexpr uint64_t TutorialID1 = 12345;
constexpr uint64_t TutorialID2 = 54321;

// Helper.
uint32_t GetCastlelevel()
{
    return 3;
}
MQEL_json GetCastle(uint64_t AccountID, eCastletype Type)
{
    MQEL_json Result;

    // Get the castle-type.
    if (Type == eCastletype::Ubisoft) Result["$type"] = "HyperQuest.GameServer.Contracts.UbisoftCastle, HyperQuest.GameServer.Contracts";
    if (Type == eCastletype::User) Result["$type"] = "HyperQuest.GameServer.Contracts.UserCastle, HyperQuest.GameServer.Contracts";

    // Check tutorial status.
    if (AccountID <= 3) Result["IsTutorialCastle"] = true;

    // The FIFO-maps does not support patching, so we need to create a new object.
    {
        // Raw data.
        std::string Castle = Backend::PvPCastle::Getcastle(AccountID).dump(4);
        std::string Insert = Result.dump(4);

        // Trim "{}".
        Castle = Castle.substr(1, Castle.size() - 2);
        Insert = Insert.substr(1, Insert.size() - 2);

        std::string Hackery = "{" + Insert + ", " + Castle + "}";
        return MQEL_json::parse(Hackery.c_str());
    }
}
MQEL_json GetCreatureloot()
{
    return MQEL_json::parse(R"([{"Id":45,"Gold":1,"Xp":2,"LifeForce":1,"HealthOrbFragments":1},{"Id":46,"Gold":9,"Xp":2,"LifeForce":1,"HealthOrbFragments":1},{"Id":47,"Gold":1,"Xp":2,"LifeForce":2,"HealthOrbFragments":1},{"Id":48,"Gold":1,"Xp":2,"HealthOrbFragments":1},{"Id":49,"Gold":1,"Xp":2,"LifeForce":1,"HealthOrbFragments":1},{"Id":50,"Gold":3,"Xp":2,"LifeForce":5,"HealthOrbFragments":1},{"Id":68,"Gold":2,"Xp":2,"HealthOrbFragments":1},{"Id":58,"Gold":2,"Xp":8,"LifeForce":4,"HealthOrbFragments":16},{"Id":65,"Gold":1,"Xp":2,"LifeForce":2,"HealthOrbFragments":1},{"Id":56,"Gold":1,"Xp":12,"LifeForce":1,"HealthOrbFragments":24,"InventoryItems":[{"$type":"HyperQuest.GameServer.Contracts.HeroEquipmentItem, HyperQuest.GameServer.Contracts","ItemLevel":1,"ArchetypeId":9,"PrimaryStatsModifiers":[0.354,0.998,0.021],"Effects":[{"Id":22,"Level":1}],"IsSellable":true,"TemplateId":37},{"$type":"HyperQuest.GameServer.Contracts.HeroEquipmentItem, HyperQuest.GameServer.Contracts","ItemLevel":1,"ArchetypeId":8,"PrimaryStatsModifiers":[0.375,0.364,0.821],"IsSellable":true,"TemplateId":40}]},{"Id":44,"Gold":1,"Xp":2,"LifeForce":5,"HealthOrbFragments":1},{"Id":69,"Gold":2,"Xp":2,"HealthOrbFragments":1},{"Id":81,"Gold":1,"Xp":2,"LifeForce":1,"HealthOrbFragments":4},{"Id":70,"Gold":2,"Xp":2,"LifeForce":1,"HealthOrbFragments":1},{"Id":59,"Gold":3,"Xp":8,"LifeForce":2,"HealthOrbFragments":16},{"Id":57,"Gold":1,"Xp":8,"LifeForce":5,"HealthOrbFragments":16},{"Id":66,"Gold":2,"Xp":2,"LifeForce":3,"HealthOrbFragments":1}])");
}
MQEL_json GetTraploot()
{
    return MQEL_json::parse(R"([{"Id":1,"InventoryItems":[{"$type":"HyperQuest.GameServer.Contracts.HeroEquipmentItem, HyperQuest.GameServer.Contracts","ItemLevel":1,"ArchetypeId":2,"PrimaryStatsModifiers":[1,1,1],"IsSellable":true,"TemplateId":53}]},{"Id":2}])");
}
MQEL_json GetIGCreward()
{
    return MQEL_json::parse(R"({"CurrencyType": 2,"Amount": 10})");
}
MQEL_json GetLifereward()
{
    return MQEL_json::parse(R"({"CurrencyType": 4})");
}
MQEL_json GetMines()
{
    return MQEL_json::parse(R"([{"CastleBuildingId":9,"StealableAmount":67,"MaxStealableAmount":67}])");
}
MQEL_json GetSpells()
{
    return MQEL_json::parse(R"([{"SpellSpecContainerId":611,"Level":1},{"SpellSpecContainerId":608,"Level":1},{"SpellSpecContainerId":613,"Level":1}])");
}
MQEL_json GetAttackerconsumables()
{
    return MQEL_json::parse(R"([{"TemplateId":1002,"StackCount":2},{"TemplateId":1004,"StackCount":2},{"TemplateId":1011,"StackCount":1},{"TemplateId":1000,"StackCount":1}])");
}
MQEL_json GetDefenderconsumables()
{
    return MQEL_json::parse(R"([{"TemplateId":402,"ConsumableType":10}])");
}

// Endpoints.
void StartAttack(Gameserver *Server, std::string Request, std::string Body)
{
    auto Parsed = MQEL_json::parse(Body);
    eAttacksource Attacksource = (eAttacksource)Parsed["attackSource"].get<uint32_t>();
    eAttacktype Attacktype = (eAttacktype)Parsed["attackType"].get<uint32_t>();
    eCastletype Castletype = (eCastletype)Parsed["castleType"].get<uint32_t>();
    bool Tutorial = Attacktype == eAttacktype::None || Attacktype == eAttacktype::Progression;

    // Log this event.
    Infoprint(va("%s a %s-castle from %s",
        [Attacktype]() -> const char *
        {
            switch (Attacktype)
            {
                case eAttacktype::Competition: return "Challenging";
                case eAttacktype::Progression: return "Questing in";
                case eAttacktype::Revenge: return "Taking revenge on";
                case eAttacktype::Validation: return "Validating";
                case eAttacktype::Visit: return "Visiting";
                case eAttacktype::None: return "Querying";
            }
            return "";
        }(),
        [Castletype]() -> const char *
        {
            switch (Castletype)
            {
                case eCastletype::Ubisoft: return "bot";
                case eCastletype::User: return "player";
            }
            return "";
        }(),
        [Attacksource]() -> const char *
        {
            switch (Attacksource)
            {
                case eAttacksource::Chat: return "chat-link";
                case eAttacksource::Competition: return "challenge";
                case eAttacksource::Friends: return "friendslist";
                case eAttacksource::Guild: return "guildpost";
                case eAttacksource::Machinelearning: return "suggestion";
                case eAttacksource::MOTD: return "welcome page";
                case eAttacksource::News: return "notification";
                case eAttacksource::Quest: return "questlog";
                case eAttacksource::Regular: return "worldmap";
            }
            return "";
        }()));

    // Create the attack ID;
    if (!Tutorial) CurrentattackID = time(NULL);
    if (Tutorial && Attacktype == eAttacktype::None) CurrentattackID = TutorialID1;
    if (Tutorial && Attacktype == eAttacktype::Progression) CurrentattackID = TutorialID1;

    // Create the challenge.
    auto Response = MQEL_json::object();
                    Response["Result"]["AttackId"] = va("%x", CurrentattackID);
                    Response["Result"]["Castle"] = GetCastle(Parsed["castleAccountId"], Castletype);
    if(Tutorial)    Response["Result"]["CastleType"] = Parsed["castleType"].get<uint32_t>();
                    Response["Result"]["Level"] = Tutorial ? 1 : GetCastlelevel();
                    Response["Result"]["Hero"] = Backend::Hero::Serialize();
                    Response["Result"]["AttackerDisplayName"] = "Hedgehog";
                    Response["Result"]["CreatureLoot"] = GetCreatureloot();
                    Response["Result"]["TrapLoot"] = GetTraploot();
    if(!Tutorial)   Response["Result"]["FirstResurrectionCost"] = 500;
                    Response["Result"]["AttackRandomSeed"] = CurrentattackID;
    if(!Tutorial)   Response["Result"]["UnlockedSpells"] = GetSpells();
                    Response["Result"]["UnlockedEmotes"] = MQEL_json::parse(R"([ 1, 2, 3 ])");
                    Response["Result"]["AttackUserSettings"] = MQEL_json::object();
                    Response["Result"]["VictoryConditionRewardRatios"] = MQEL_json::parse(R"([ 1, 0.75, 0.5 ])");
                    Response["Result"]["FreeInventorySlotsCount"] = 42;
    if(!Tutorial)   Response["Result"]["InventoryConsumablesInfo"] = GetAttackerconsumables();
    if(!Tutorial)   Response["Result"]["DefenderActiveConsumables"] = GetDefenderconsumables();
                    Response["Result"]["TreasureRoomStealableIGC"] = GetIGCreward();
                    Response["Result"]["TreasureRoomStealableLifeForce"] = GetLifereward();
    if(!Tutorial)   Response["Result"]["StealableMines"] = GetMines();
    if(Tutorial)    Response["Result"]["IsTutorial"] = true;
                    Response["Result"]["TreasureRoomGoldRatio"] = 0.3;
                    Response["Result"]["TreasureRoomLifeForceRatio"] = 0.3;
                    Response["Result"]["AttackType"] = Tutorial ? 5 : Parsed["attackType"].get<uint32_t>();
    if(!Tutorial)   Response["Result"]["CastleValidationDuration"] = 44.23333;
                    Response["Result"]["IsResurrectionAllowed"] = true;

    // Return the castle info.
    Sendreply(Server, Response.dump());
}
void EndAttack(Gameserver *Server, std::string Request, std::string Body)
{
    MQEL_json Response;

    // Handle tutorial fights.
    if (CurrentattackID == TutorialID1)
    {
        Response = MQEL_json::parse(R"({"Notifications":[{"$type":"HyperQuest.GameServer.Contracts.WalletUpdatedNotification, HyperQuest.GameServer.Contracts","Amounts":[{"CurrencyType":2,"Amount":33},{"CurrencyType":4,"Amount":33}],"NotificationType":24},{"$type":"HyperQuest.GameServer.Contracts.HeroXpChangedNotification, HyperQuest.GameServer.Contracts","HeroSpecContainerId":4,"XpAdded":60,"TotalXp":60,"Level":1,"NotificationType":43},{"$type":"HyperQuest.GameServer.Contracts.InboxItemsAddedNotification, HyperQuest.GameServer.Contracts","InboxItems":[{"$type":"HyperQuest.GameServer.Contracts.InboxHeroEquipmentItem, HyperQuest.GameServer.Contracts","HeroItem":{"ItemLevel":1,"ArchetypeId":9,"PrimaryStatsModifiers":[0.354,0.998,0.021],"Effects":[{"Id":22,"Level":1}],"IsSellable":true,"TemplateId":37},"ItemType":3,"ObjectId":"580d50cd5c98320a9cb0e719"}],"NotificationType":111},{"$type":"HyperQuest.GameServer.Contracts.InboxItemsAddedNotification, HyperQuest.GameServer.Contracts","InboxItems":[{"$type":"HyperQuest.GameServer.Contracts.InboxHeroEquipmentItem, HyperQuest.GameServer.Contracts","HeroItem":{"ItemLevel":1,"ArchetypeId":8,"PrimaryStatsModifiers":[0.375,0.364,0.821],"IsSellable":true,"TemplateId":40},"ItemType":3,"ObjectId":"580d50cd5c98320a9cb0e71a"}],"NotificationType":111},{"$type":"HyperQuest.GameServer.Contracts.InboxItemsAddedNotification, HyperQuest.GameServer.Contracts","InboxItems":[{"$type":"HyperQuest.GameServer.Contracts.InboxHeroEquipmentItem, HyperQuest.GameServer.Contracts","HeroItem":{"ItemLevel":1,"ArchetypeId":2,"PrimaryStatsModifiers":[1,1,1],"IsSellable":true,"TemplateId":53},"ItemType":3,"ObjectId":"580d50cd5c98320a9cb0e71b"}],"NotificationType":111}],"Result":{"AttackId":"580d504f5c98320c1881325a","DefenderCastleId":2,"DefenderCastleType":1,"DefenderOasisNameId":2325,"Duration":119100,"InitialGold":1000,"HeroLevel":1,"TotalXp":60,"TotalGold":33,"TotalLifeForce":33,"KillsXp":60,"KillsGold":33,"KillsLifeForce":33,"CastleRatingFreePrize":{"CurrencyType":2,"Amount":50},"IsTutorial":true,"IsCastleAttackable":true,"UpdatedAccountStats":{"TotalCreaturesKilled":16,"AttackTotalIGCWon":33,"TotalCastlesLooted":1,"CastlesDefeated":{"Medium":1},"KilledCreatures":{"1081":12,"1029":3,"1003":1},"CurrencyAccumulation":{"IGC":33,"LifeForce":33},"DefeatCastleStrike":1,"TotalItemsLooted":3},"TrophyScoreVariation":{},"VictoryConditionRewardRatios":[1,0.75,0.5],"VictoryConditionLevel":1,"MaxVictoryConditionLevel":1,"EnterTreasureRoom":true,"VictoryConditionType":1}})");
    }
    else if (CurrentattackID == TutorialID2)
    {
        Response = MQEL_json::parse(R"({"Notifications":[{"$type":"HyperQuest.GameServer.Contracts.WalletUpdatedNotification, HyperQuest.GameServer.Contracts","Amounts":[{"CurrencyType":2},{"CurrencyType":4,"Amount":29}],"NotificationType":24},{"$type":"HyperQuest.GameServer.Contracts.HeroXpChangedNotification, HyperQuest.GameServer.Contracts","HeroSpecContainerId":4,"XpAdded":66,"TotalXp":1868,"Level":4,"NotificationType":43},{"$type":"HyperQuest.GameServer.Contracts.InboxItemsAddedNotification, HyperQuest.GameServer.Contracts","InboxItems":[{"$type":"HyperQuest.GameServer.Contracts.InboxConsumableItem, HyperQuest.GameServer.Contracts","HeroItem":{"StackCount":1,"TemplateId":1000},"ItemType":4,"ObjectId":"58043d945c98320eb4f50727"}],"NotificationType":111},{"$type":"HyperQuest.GameServer.Contracts.InboxItemsAddedNotification, HyperQuest.GameServer.Contracts","InboxItems":[{"$type":"HyperQuest.GameServer.Contracts.InboxHeroEquipmentItem, HyperQuest.GameServer.Contracts","HeroItem":{"ItemLevel":2,"ArchetypeId":4,"PrimaryStatsModifiers":[0.566,0.342,0.122],"IsSellable":true,"TemplateId":17},"ItemType":3,"ObjectId":"58043d945c98320eb4f50728"}],"NotificationType":111},{"$type":"HyperQuest.GameServer.Contracts.InboxItemsAddedNotification, HyperQuest.GameServer.Contracts","InboxItems":[{"$type":"HyperQuest.GameServer.Contracts.InboxHeroEquipmentItem, HyperQuest.GameServer.Contracts","HeroItem":{"ItemLevel":3,"ArchetypeId":8,"PrimaryStatsModifiers":[0.98,0.24,0.777],"IsSellable":true,"TemplateId":44},"ItemType":3,"ObjectId":"58043d945c98320eb4f50729"}],"NotificationType":111},{"$type":"HyperQuest.GameServer.Contracts.InboxItemsAddedNotification, HyperQuest.GameServer.Contracts","InboxItems":[{"$type":"HyperQuest.GameServer.Contracts.InboxHeroEquipmentItem, HyperQuest.GameServer.Contracts","HeroItem":{"ItemLevel":4,"ArchetypeId":3,"PrimaryStatsModifiers":[0.938,0.568,0.491],"IsSellable":true,"TemplateId":205},"ItemType":3,"ObjectId":"58043d945c98320eb4f5072a"}],"NotificationType":111}],"Result":{"AttackId":"58043d5d5c98320eb4f506fa","DefenderCastleId":3000947,"DefenderAccountDisplayName":"jonsie111","Duration":49933,"InitialXp":1802,"InitialGold":2000,"HeroLevel":4,"TotalXp":66,"TotalGold":103,"TotalLifeForce":29,"KillsXp":66,"KillsGold":29,"KillsLifeForce":29,"TreasureRoomGold":7,"PillagedIGCMinesAmount":67,"PillagedIGCMineCount":1,"CastleRatingFreePrize":{"CurrencyType":2,"Amount":50},"IsCastleAttackable":true,"UpdatedAccountStats":{"TotalCreaturesKilled":202,"AttackTotalIGCWon":833,"TotalCastlesLooted":10,"CastlesDefeated":{"Medium":5,"Easy":4,"Hard":1},"KilledCreatures":{"1081":86,"1029":7,"1003":2,"1000":21,"1006":3,"1023":4,"1079":8,"1155":1,"1001":24,"1026":9,"1024":7,"1016":5,"1009":6,"1015":13,"1002":6},"CurrencyAccumulation":{"IGC":1300,"LifeForce":1035},"DefeatCastleStrike":4,"TotalItemsLooted":31,"TotalPotionsConsumed":5},"TrophyScoreVariation":{},"VictoryConditionRewardRatios":[1,0.75,0.5],"VictoryConditionLevel":3,"DefenderLossIGC":7,"EnterTreasureRoom":true,"CastleShieldedByAttacker":true,"VictoryConditionType":1,"RevengeEnabled":true,"CastleValidationDuration":44233,"LastComment":{"AttackId":"580238555c98321778c363ba","AccountId":3140910,"Comment":{"Raw":"for beginners..."},"AvatarId":10}}})");
    }
    else
    {
        Response = MQEL_json::parse(R"({"Notifications":[{"$type":"HyperQuest.GameServer.Contracts.WalletUpdatedNotification, HyperQuest.GameServer.Contracts","Amounts":[{"CurrencyType":2},{"CurrencyType":4,"Amount":165}],"NotificationType":24},{"$type":"HyperQuest.GameServer.Contracts.HeroXpChangedNotification, HyperQuest.GameServer.Contracts","HeroSpecContainerId":4,"XpAdded":333,"TotalXp":2201,"Level":4,"NotificationType":43},{"$type":"HyperQuest.GameServer.Contracts.InboxItemsAddedNotification, HyperQuest.GameServer.Contracts","InboxItems":[{"$type":"HyperQuest.GameServer.Contracts.InboxHeroEquipmentItem, HyperQuest.GameServer.Contracts","HeroItem":{"ItemLevel":3,"ArchetypeId":10,"PrimaryStatsModifiers":[0.979,0.193,0.183],"IsSellable":true,"TemplateId":318},"ItemType":3,"ObjectId":"58043e625c98320eb4f507ba"}],"NotificationType":111},{"$type":"HyperQuest.GameServer.Contracts.InboxItemsAddedNotification, HyperQuest.GameServer.Contracts","InboxItems":[{"$type":"HyperQuest.GameServer.Contracts.InboxHeroEquipmentItem, HyperQuest.GameServer.Contracts","HeroItem":{"ItemLevel":3,"ArchetypeId":4,"PrimaryStatsModifiers":[0.381,0.771,0.188],"IsSellable":true,"TemplateId":45},"ItemType":3,"ObjectId":"58043e625c98320eb4f507bb"}],"NotificationType":111},{"$type":"HyperQuest.GameServer.Contracts.ObjectiveCompletedNotification, HyperQuest.GameServer.Contracts","ObjectiveId":302,"NotificationType":14},{"$type":"HyperQuest.GameServer.Contracts.InboxItemsAddedNotification, HyperQuest.GameServer.Contracts","InboxItems":[{"$type":"HyperQuest.GameServer.Contracts.InboxConsumableItem, HyperQuest.GameServer.Contracts","HeroItem":{"StackCount":1,"TemplateId":1003},"ItemType":4,"ObjectId":"58043e625c98320eb4f507bc"}],"NotificationType":111},{"$type":"HyperQuest.GameServer.Contracts.InboxItemsAddedNotification, HyperQuest.GameServer.Contracts","InboxItems":[{"$type":"HyperQuest.GameServer.Contracts.InboxConsumableItem, HyperQuest.GameServer.Contracts","HeroItem":{"StackCount":1,"TemplateId":1003},"ItemType":4,"ObjectId":"58043e625c98320eb4f507bd"}],"NotificationType":111},{"$type":"HyperQuest.GameServer.Contracts.InboxItemsAddedNotification, HyperQuest.GameServer.Contracts","InboxItems":[{"$type":"HyperQuest.GameServer.Contracts.InboxConsumableItem, HyperQuest.GameServer.Contracts","HeroItem":{"StackCount":1,"TemplateId":1001},"ItemType":4,"ObjectId":"58043e625c98320eb4f507be"}],"NotificationType":111},{"$type":"HyperQuest.GameServer.Contracts.InboxItemsAddedNotification, HyperQuest.GameServer.Contracts","InboxItems":[{"$type":"HyperQuest.GameServer.Contracts.InboxConsumableItem, HyperQuest.GameServer.Contracts","HeroItem":{"StackCount":1,"TemplateId":1001},"ItemType":4,"ObjectId":"58043e625c98320eb4f507bf"}],"NotificationType":111},{"$type":"HyperQuest.GameServer.Contracts.InboxItemsAddedNotification, HyperQuest.GameServer.Contracts","InboxItems":[{"$type":"HyperQuest.GameServer.Contracts.InboxConsumableItem, HyperQuest.GameServer.Contracts","HeroItem":{"StackCount":1,"TemplateId":1000},"ItemType":4,"ObjectId":"58043e625c98320eb4f507c0"}],"NotificationType":111},{"$type":"HyperQuest.GameServer.Contracts.ObjectiveUnlockedNotification, HyperQuest.GameServer.Contracts","AccountObjective":{"ObjectiveId":303,"LastStatusDate":"2016-10-17T02:58:42Z","Status":1},"NotificationType":17}],"Result":{"AttackId":"58043dab5c98320eb4f50743","DefenderCastleId":101,"DefenderCastleType":1,"DefenderAccountDisplayName":"Fendrick's Farm","DefenderOasisNameId":16659,"Duration":172600,"InitialXp":1868,"InitialGold":2000,"HeroLevel":4,"TotalXp":333,"TotalGold":179,"TotalLifeForce":165,"KillsXp":333,"KillsGold":133,"KillsLifeForce":119,"TreasureRoomGold":46,"TreasureRoomLifeForce":46,"CastleRatingFreePrize":{"CurrencyType":2,"Amount":75},"IsTutorial":true,"IsCastleAttackable":true,"UpdatedAccountStats":{"TotalCreaturesKilled":243,"AttackTotalIGCWon":1012,"TotalCastlesLooted":11,"CastlesDefeated":{"Medium":5,"Easy":5,"Hard":1},"KilledCreatures":{"1081":103,"1029":7,"1003":5,"1000":24,"1006":3,"1023":4,"1079":8,"1155":1,"1001":26,"1026":9,"1024":7,"1016":5,"1009":6,"1015":13,"1002":6,"1028":3,"1007":4,"1090":8,"1087":1},"CurrencyAccumulation":{"IGC":1300,"LifeForce":1200},"DefeatCastleStrike":5,"TotalItemsLooted":33,"TotalPotionsConsumed":5},"TrophyScoreVariation":{},"VictoryConditionRewardRatios":[1,0.75,0.5],"VictoryConditionLevel":1,"MaxVictoryConditionLevel":1,"EnterTreasureRoom":true,"VictoryConditionType":1}})");
    }

    // Modify hardcoded data.
    Response["Result"]["AttackId"] = va("%x", CurrentattackID);


    Sendreply(Server, Response.dump());
}
void RateCastle(Gameserver *Server, std::string Request, std::string Body)
{
    Backend::Wallet::Updateamount(eCurrencytype::IGC, 50);
    Sendreply(Server, Backend::Wallet::Serialize(eCurrencytype::IGC).dump());
}
void Resurrect(Gameserver *Server, std::string Request, std::string Body)
{
    Response_t Response;
    Response.Set("NextResurrectionCost", 750);
    Response.Set("ResurrectionCost", 500);
    Response.Set("ResurrectionCount", 1);

    Backend::Wallet::Updateamount(eCurrencytype::IGC, -500);
    Sendreply(Server, Response.toString());
}

// Add the services to the gameserver on startup.
namespace {
    struct Startup {
        Startup()
        {
            Mapservice("/AttackService.hqs/StartAttack", StartAttack);
            Mapservice("/AttackService.hqs/EndAttack", EndAttack);
            Mapservice("/AttackService.hqs/RateCastle", RateCastle);
            Mapservice("/AttackService.hqs/Resurrect", Resurrect);
        };
    };
    static Startup Loader{};
}
