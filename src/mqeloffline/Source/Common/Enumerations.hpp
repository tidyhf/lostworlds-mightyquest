/*
    Initial author: Convery (tcn@hedgehogscience.com)
    Started: 16-01-2018
    License: MIT
    Notes:
        Some game-defined enumerations.
*/

#pragma once
#include "../Stdinclude.hpp"

enum class eCurrencytype
{
    None = 0,
    PremiumCash = 1,
    IGC = 2,
    Lifeforce = 4,
    Count
};

enum class eRegionstatus
{
    None = 0,
    Locked = 1,
    Unlocked = 2,
    Completed = 3
};

enum class eHerotype
{
    Knight = 2,
    Archer = 3,
    Mage = 4,
    Runaway = 5,
    Count
};

enum class eItemslot
{
    None = 0,
    Head = 1,
    Shoulders = 2,
    Body = 3,
    Back = 4,
    Neck = 5,
    Finger = 6,
    Hands = 7,
    Mainhand = 8,
    Offhand = 9,
    Costume = 10,
    Pet = 11,
    Count
};

enum class eCastletype
{
    User = 0,
    Ubisoft = 1,
    Synergy = 2,
};

enum class eAttacksource
{
    Regular = 0,
    Machinelearning = 1,
    Friends = 2,
    News = 3,
    Quest = 4,
    Revenge = 5,
    Chat = 6,
    Competition = 7,
    MOTD = 8,
    Guild = 9
};

enum class eAttacktype
{
    None = 0,
    Validation = 1,
    Visit = 2,
    Revenge = 3,
    Progression = 5,
    Competition = 6
};

enum class eInventoryItemType
{
	HeroEquipmentItem = 1,
	Consumable = 2,
	Count
};
