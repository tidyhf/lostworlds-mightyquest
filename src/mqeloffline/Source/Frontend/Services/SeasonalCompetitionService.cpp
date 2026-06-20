/*
    Initial author: Convery (tcn@hedgehogscience.com)
    Started: 15-01-2018
    License: MIT
    Notes:
        Provides implementations for league stats.
*/

#pragma once
#include "../../Stdinclude.hpp"

// Endpoints.
void CheckSeasonalCompetitionRewards(Gameserver *Server, std::string Request, std::string Body)
{
    // Complete the quest for placing in league.
    Backend::Quest::Complete(21);

    MQEL_json Response = MQEL_json::parse(R"({"Notifications":[{"$type":"HyperQuest.GameServer.Contracts.LeagueUpdatedNotification, HyperQuest.GameServer.Contracts","LeagueId":1,"SubLeagueId":1,"NotificationType":65},{"$type":"HyperQuest.GameServer.Contracts.NewsAddedNotification, HyperQuest.GameServer.Contracts","NewsItem":{"Id":"580d4fa55c98320a9cb0e55d","PublishDate":"2016-10-24T00:00:00Z","Data":{"$type":"HyperQuest.GameServer.Contracts.LeagueUpdatedNewsData, HyperQuest.GameServer.Contracts","CurrentLeagueId":1,"CurrentSubleagueId":1,"PreviousLeagueId":1,"PreviousSubleagueId":1,"Type":18},"IsUnread":true,"Priority":1},"NotificationType":22}]})");

    Sendreply(Server, Response.dump());
}
void GetSeasonalCompetition(Gameserver *Server, std::string Request, std::string Body)
{
    /*
        TODO(Convery):
        This function need to check the request parameters.
        Just returning this data sends it into an infinite-loop.
    */

    auto Response = MQEL_json::parse(R"({"Result":{"CurrentUser":{"AccountSummary":{"Id":3142975,"DisplayName":"Marlise","CountryCode":"US","AvatarId":10,"LeagueId":2,"SubLeagueId":1,"CastleLevel":6,"IsCastleAttackable":true},"Score":103,"IsCastleAttackable":true,"Seconds":251924966608},"WorldLeader":{"AccountSummary":{"Id":2954896,"DisplayName":"YouNeverKillMe","CountryCode":"RO","AvatarId":8,"LeagueId":5,"SubLeagueId":3,"CastleLevel":30,"IsCastleAttackable":true,"GuildHeader":{"Id":152521,"DisplayName":"RIP_MQEL","Emblem":{"Id":20}}},"Score":2407,"IsCastleAttackable":true,"Seconds":251924941161},"Leaders":[{"AccountSummary":{"Id":3143259,"DisplayName":"TacoAdvocate","CountryCode":"US","AvatarId":10,"LeagueId":2,"SubLeagueId":1,"CastleLevel":10,"IsCastleAttackable":true,"GuildHeader":{"Id":152763,"DisplayName":"TacoKnights","Emblem":{"Id":9}}},"Score":226,"IsCastleAttackable":true,"Seconds":251924935822},{"AccountSummary":{"Id":2098747,"DisplayName":"FormerlyNiSaying","CountryCode":"FR","AvatarId":10,"LeagueId":2,"SubLeagueId":1,"CastleLevel":20,"IsCastleAttackable":true},"Score":158,"IsCastleAttackable":true,"Seconds":251924942106},{"AccountSummary":{"Id":3142632,"DisplayName":"Caesium_Citadel","CountryCode":"CA","AvatarId":10,"LeagueId":2,"SubLeagueId":1,"CastleLevel":11,"IsCastleAttackable":true},"Score":150,"IsCastleAttackable":true,"Seconds":251924946098}],"Entries":[{"AccountSummary":{"Id":3142975,"DisplayName":"Marlise","CountryCode":"US","AvatarId":10,"LeagueId":2,"SubLeagueId":1,"CastleLevel":6,"IsCastleAttackable":true},"Score":103,"IsCastleAttackable":true,"Seconds":251924966608}],"TotalCount":515,"FirstEntryRank":201,"RemainingTime":"20h35m10s","ActiveZones":["AF","AS","EU","NA","OC","SA"],"ActiveCountries":["AD","AE","AM","AR","AT","AU","AW","BA","BE","BG","BR","BY","CA","CH","CL","CM","CN","CO","CR","CY","CZ","DE","DK","DZ","EC","EE","EG","ES","FI","FR","GA","GB","GP","GR","GT","GU","HK","HR","HU","ID","IE","IL","IN","IQ","IR","IT","JO","JP","KG","KH","KR","KW","KZ","LA","LT","LU","LV","MA","MK","MN","MT","MX","MY","NC","NG","NL","NO","NZ","PA","PE","PH","PL","PR","PT","RE","RO","RS","RU","SA","SE","SG","SI","SK","TH","TN","TR","TT","TW","UA","US","UY","VE","VN","WS","ZA"],"PreviousLeagueInfo":{"PreviousLeagueRank":30,"PreviousWorldRank":1381,"PreviousLeagueId":1,"PreviousSubLeagueId":1},"NextSeasonStartingDate":"2016-10-26T00:00:00Z"}})");

    Sendreply(Server, Response.dump());
}

// Add the services to the gameserver on startup.
namespace {
    struct Startup {
        Startup()
        {
            Mapservice("/SeasonalCompetitionService.hqs/CheckSeasonalCompetitionRewards", CheckSeasonalCompetitionRewards);
            Mapservice("/SeasonalCompetitionService.hqs/GetSeasonalCompetition", GetSeasonalCompetition);
        };
    };
    static Startup Loader{};
}
