#include "Trait.h"
#include <json.hpp>
#include <iostream>

namespace state
{
    Trait::Trait()
    {
        /*
        std::string id;
        std::string opposedId;
        int diplomacyModifier;
        int stewardshipModifier;
        int martialModifier;
        int intrigueModifier;
        */
    }
    Trait::Trait(std::string strJson)
    {
        using json = nlohmann::json;

        try
        {
            json j = json::parse(strJson);
            id = j["id"].get<std::string>();
            opposedId = j["opposedId"].get<std::string>();
            diplomacyModifier = j["diplomacyModifier"].get<int>();
            stewardshipModifier = j["stewardshipModifier"].get<int>();
            martialModifier = j["martialModifier"].get<int>();
            intrigueModifier = j["intrigueModifier"].get<int>();
            
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what();
            throw std::runtime_error("Error: Could not load trait.");
        }
    }
    Trait::~Trait ()
    {

    }
    bool Trait::checkConsistency ()
    {
        return id.size() && opposedId.size();
    }
    void Trait::debug ()
    {
        std::cout << "Debug Trait\n";
        std::cout << "Id: " << id << "\nOpposed trait Id: " << opposedId << "\nDiplomacy: " << diplomacyModifier << "\nStewardship: " << stewardshipModifier << "\nMartial: " << martialModifier << "\nIntrigue: " << intrigueModifier << std::endl; 
    }
    const std::string Trait::getId ()
    {
        return id;
    }
    const std::string Trait::getOpposedId ()
    {
        return opposedId;
    }
    const int Trait::getDiplomacyModifier ()
    {
        return diplomacyModifier;
    }
    const int Trait::getStewardshipModifier ()
    {
        return stewardshipModifier;
    }
    const int Trait::getMartialModifier ()
    {
        return martialModifier;
    }
    const int Trait::getIntrigueModifier ()
    {
        return intrigueModifier;
    }
}