#include "Battle.h"

#include "GameMap.h"
#include "Army.h"

#include <json.hpp>
#include <iostream>
#include <cstdlib>

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
    Battle::Battle (GameMap * parent, std::string strJson)
    {
        this->parent = parent;
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
    void Battle::setParent (GameMap * parent)
    {
        this->parent = parent;
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
    void Battle::advance()
    {
        int whiteMen = 0, blackMen = 0;
        for(auto const& e: whiteArmies)
            whiteMen += parent->getArmy(e)->getMen();
        for(auto const& e: blackArmies)
            blackMen += parent->getArmy(e)->getMen();
        int whiteKills = whiteMen / 3;
        whiteKills += (std::rand() % (whiteKills / 10));
        int blackKills = blackMen / 3;
        blackKills += (std::rand() % (blackKills / 10));

        for(auto const& e: whiteArmies)
            parent->getArmy(e)->killMen(blackKills * (parent->getArmy(e)->getMen() / whiteMen));
        for(auto const& e: blackArmies)
            parent->getArmy(e)->killMen(whiteKills * (parent->getArmy(e)->getMen() / blackMen));

    }
    std::string Battle::getProvince()
    {
        return province;
    }
    bool Battle::isFinished ()
    {
        int whiteMen = 0, blackMen = 0;
        for(auto const& e: whiteArmies)
            whiteMen += parent->getArmy(e)->getMen();
        for(auto const& e: blackArmies)
            blackMen += parent->getArmy(e)->getMen();
        
        return !(whiteMen && blackMen);
    }
    void Battle::close(int turn)
    {
        this->endTurn = turn;
    }
    nlohmann::json Battle::toJson (){
        nlohmann::json j;

        j["province"] = province;
        j["whiteArmies"] = whiteArmies;
        j["blackArmies"] = blackArmies;
        j["startTurn"] = startTurn;
        j["endTurn"] = endTurn;

        return j;
    }
}