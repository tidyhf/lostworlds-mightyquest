/*
    Initial author: Convery (tcn@hedgehogscience.com)
    Started: 15-01-2018
    License: MIT
    Notes:
        Provides implementations for hero services.
*/

#pragma once
#include "../../Stdinclude.hpp"

// Endpoints.
void ChooseFirstHero(Gameserver *Server, std::string Request, std::string Body)
{
    // Get which hero-class we use.
    int Heroclass = MQEL_json::parse(Body)["heroSpecContainerId"];
    Infoprint(va("Creating hero of type: %s", [Heroclass]() -> const char *
    {
        switch ((eHerotype)Heroclass)
        {
            case eHerotype::Archer:     return "Archer";
            case eHerotype::Knight:     return "Knight";
            case eHerotype::Mage:       return "Mage";
            case eHerotype::Runaway:    return "Runaway";
            default:                    return "Unknown";
        }
    }()));

    // Create the core data for the character.
    Backend::Hero::Createhero(Heroclass);

    // Add gear to the character.
    switch ((eHerotype)Heroclass)
    {
        case eHerotype::Knight:
        {
            std::vector<double> Modifiers = { 0.4, 0.4, 0.4 };
            Equipment_t Basegear = { 0, 0, false, false, 1, 8, "HeroEquipmentItem", Modifiers, {} };

            Basegear.ID = 108;  // "Board with a nail in it"
            Basegear.Archetype = 2;
            Backend::Hero::Equipgear(eItemslot::Mainhand, Basegear);

            Basegear.ID = 109;  // "Mom's Favorite Haircut"
            Basegear.Archetype = 8;
            Backend::Hero::Equipgear(eItemslot::Head, Basegear);

            Basegear.ID = 110;  // "Chiseled Pecs"
            Basegear.Archetype = 8;
            Backend::Hero::Equipgear(eItemslot::Body, Basegear);

            Basegear.ID = 111;  // "Perfect Manicure"
            Basegear.Archetype = 8;
            Backend::Hero::Equipgear(eItemslot::Hands, Basegear);

            Basegear.ID = 132;  // "Bane of Grindstone"
            Basegear.Archetype = 8;
            Backend::Hero::Equipgear(eItemslot::Shoulders, Basegear);
            break;
        }
        case eHerotype::Archer:
        {
            std::vector<double> Modifiers = { 0.4, 0.4, 0.4 };
            Equipment_t Basegear = { 0, 0, false, false, 1, 8, "HeroEquipmentItem", Modifiers, {} };

            Basegear.ID = 124;  // "Pea shooter"
            Basegear.Archetype = 2;
            Backend::Hero::Equipgear(eItemslot::Mainhand, Basegear);

            Basegear.ID = 125;  // "Pure Ginger"
            Basegear.Archetype = 8;
            Backend::Hero::Equipgear(eItemslot::Head, Basegear);

            Basegear.ID = 126;  // "Trusty Underpants"
            Basegear.Archetype = 8;
            Backend::Hero::Equipgear(eItemslot::Body, Basegear);

            Basegear.ID = 127;  // "Trigger Finger"
            Basegear.Archetype = 8;
            Backend::Hero::Equipgear(eItemslot::Hands, Basegear);

            Basegear.ID = 133;  // "Smirking Shrug"
            Basegear.Archetype = 8;
            Backend::Hero::Equipgear(eItemslot::Shoulders, Basegear);
            break;
        }
        case eHerotype::Mage:
        {
            std::vector<double> Modifiers = { 0.4, 0.4, 0.4 };
            Equipment_t Basegear = { 0, 0, false, false, 1, 8, "HeroEquipmentItem", Modifiers, {} };

            Basegear.ID = 128;  // "Twig"
            Basegear.Archetype = 9;
            Backend::Hero::Equipgear(eItemslot::Mainhand, Basegear);

            Basegear.ID = 129;  // "Sleepytime 2000"
            Basegear.Archetype = 8;
            Backend::Hero::Equipgear(eItemslot::Head, Basegear);

            Basegear.ID = 130;  // "Silken Robe"
            Basegear.Archetype = 8;
            Backend::Hero::Equipgear(eItemslot::Body, Basegear);

            Basegear.ID = 131;  // "Creepy Claw"
            Basegear.Archetype = 8;
            Backend::Hero::Equipgear(eItemslot::Hands, Basegear);

            Basegear.ID = 131;  // "Drape of Wrath"
            Basegear.Archetype = 8;
            Backend::Hero::Equipgear(eItemslot::Shoulders, Basegear);
            break;
        }
        case eHerotype::Runaway:
        {
            std::vector<double> Modifiers = { 0.4, 0.4, 0.4 };
            Equipment_t Basegear = { 0, 0, false, false, 1, 8, "HeroEquipmentItem", Modifiers, {} };

            /*
                TODO(Convery):
                Further research is needed.

                The items seems to be in the 14415-14419 range,
                but sending an ID in the 9000+ range crashes the
                game. Which means that this class uses a different
                base from the others (4968).

                Sending the Knights equipment allows creation of the
                hero, but does of course crash when starting a map.

                We might need to bruteforce this.
            */

            #if 0
            Basegear.ID = 108;  // "Board with a nail in it"
            Basegear.Archetype = 2;
            Backend::Hero::Equipgear(eItemslot::Mainhand, Basegear);

            Basegear.ID = 109;  // "Mom's Favorite Haircut"
            Basegear.Archetype = 8;
            Backend::Hero::Equipgear(eItemslot::Head, Basegear);

            Basegear.ID = 110;  // "Chiseled Pecs"
            Basegear.Archetype = 8;
            Backend::Hero::Equipgear(eItemslot::Body, Basegear);

            Basegear.ID = 111;  // "Perfect Manicure"
            Basegear.Archetype = 8;
            Backend::Hero::Equipgear(eItemslot::Hands, Basegear);

            Basegear.ID = 132;  // "Bane of Grindstone"
            Basegear.Archetype = 8;
            Backend::Hero::Equipgear(eItemslot::Shoulders, Basegear);

            //Basegear.ID = 9447;     // "Dilapidated Lute"
            //Basegear.Archetype = 2;
            //Backend::Hero::Equipgear(eItemslot::Mainhand, Basegear);

            //Basegear.ID = 9448;  // "Perfect Haircut"
            //Basegear.Archetype = 8;
            //Backend::Hero::Equipgear(eItemslot::Head, Basegear);

            //Basegear.ID = 9449;     // "Ironic T-Shirt"
            //Basegear.Archetype = 8;
            //Backend::Hero::Equipgear(eItemslot::Body, Basegear);

            //Basegear.ID = 9450;     // "Vintage Bracelets"
            //Basegear.Archetype = 8;
            //Backend::Hero::Equipgear(eItemslot::Hands, Basegear);

            //Basegear.ID = 9451;     // "Musician's Elbow"
            //Basegear.Archetype = 8;
            //Backend::Hero::Equipgear(eItemslot::Shoulders, Basegear);
            #else
            assert(false);
            #endif
            break;
        }
    }

    // Return this basic character to the game.
    MQEL_json Response; Response["Result"] = Backend::Hero::Serialize();
    Sendreply(Server, Response.dump());
}

// Add the services to the gameserver on startup.
namespace {
    struct Startup {
        Startup()
        {
            Mapservice("/HeroService.hqs/ChooseFirstHero", ChooseFirstHero);
        };
    };
    static Startup Loader{};
}
