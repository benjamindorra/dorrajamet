#include "Relation.h"
#include "../json.hpp"
#include "../tools.hpp"
#include <iostream>

namespace state
{
    Relation::Relation ()
    {
        /*
        enum relType{non_aggression = 0, alliance = 1, friendship = 2, rivalry = 3, war = 4};
        std::string characterA;
        std::string characterB;
        relType type;
        int endTurn;
        */
    }
    Relation::Relation (std::string strJson)
    {
        using json = nlohmann::json;

        try
        {
            json j = json::parse(strJson);
            int t = j["type"].get<int>();
            characterA = j["characterA"].get<std::string>();
            characterB = j["characterB"].get<std::string>();
            endTurn = j["endTurn"].get<int>();
            switch(t)
            {
                case 0:
                    type = non_aggression;
                    break;
                case 1:
                    type = alliance;
                    break;
                case 2:
                    type = friendship;
                    break;
                case 3:
                    type = rivalry;
                    break;
                case 4:
                    type = war;
                    break;
                default:
                    throw std::string("Error: invalid relation type.");
            }
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << std::endl ;
            throw std::runtime_error("Error when loading titles.");
        
        }
    }
    Relation::~Relation ()
    {

    }
    bool Relation::checkConsistency()
    {
        if(characterA.substr(0, 5) != "chara" || characterB.substr(0, 5) != "chara")
            return false;
        if(endTurn < -1)
            return false;
        return true;
    }
    void Relation::debug()
    {
        std::cout << "Debug relation\nType: " << type << "\nCharacter A: " << characterA << "\nCharacterB: " << characterB << "\nEnd Turn: " << endTurn << std::endl;
    }
}