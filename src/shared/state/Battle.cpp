#include "Battle.h"
#include <json.hpp>
#include <iostream>

namespace state
{
    Battle::Battle()
    {
        /*
        std::string province;
        std::vector<std::string> whiteArmies;
        std::vector<std::string> blackArmies;
        int startTurn;
        int endTurn;
        */
    }
    Battle::Battle (std::string strJson)
    {
        using json = nlohmann::json;
        try
        {
            json j = json::parse(strJson);

            province = j["province"].get<std::string>();
            whiteArmies = j["whiteArmies"].get<std::vector<std::string>>();
            blackArmies = j["blackArmies"].get<std::vector<std::string>>();
            startTurn = j["startTurn"].get<int>();
            endTurn = j["endTurn"].get<int>();
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << std::endl;
            throw std::runtime_error("Error when loading battle.");
        }
    }
    Battle::~Battle ()
    {

    }
    bool Battle::checkConsistency ()
    {
        if(endTurn != -1)
            return (startTurn < endTurn);
        return true;
    }
    void Battle::debug ()
    {
        std::cout << "Battle debug\nIn province: " << province << "\nBegan at turn: " << startTurn << "\nEnded at turn: " << endTurn << std::endl;
    }
}