#include "GameMap.h"
#include <json.hpp>
#include <iostream>

namespace state
{
    GameMap::GameMap()
    {
        
    }
    GameMap::GameMap(GameState * parent, std::string strJson)
    {
        this->parent = parent;
        using json = nlohmann::json;

        try
        {
            json j = json::parse(strJson);
            // Provinces
            json jArray = j["provinces"];
            json jElement;
            json::iterator it;
            for(it = jArray.begin(); it!= jArray.end(); ++it)
            {
                jElement = it.value();
                this->provinces[jElement["id"].get<std::string>()] = Province(jElement.dump());
            }
            // Armies
            jArray = j["armies"];
            for(it = jArray.begin(); it!= jArray.end(); ++it)
            {
                jElement = it.value();
                this->armies[jElement["id"].get<std::string>()] = Army(this, jElement.dump());
            }
            // Battles
            jArray = j["battles"];
            for(it = jArray.begin(); it!= jArray.end(); ++it)
            {
                jElement = it.value();
                this->battles[jElement["id"].get<std::string>()] = Battle(this, jElement.dump());
            }
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << std::endl ;
            throw std::runtime_error("Error when loading politics.");
        }
    }
    GameMap::~GameMap ()
    {

    }
    bool GameMap::checkConsistency ()
    {
        return true;
    }
    void GameMap::debug ()
    {

    }
    std::string GameMap::getArmyPosition (std::string armyId)
    {
        return armies[armyId].getCurrentProvince();
    }
    void GameMap::setArmyOrder (std::string armyId, nlohmann::json orderJson)
    {
        armies[armyId].setOrders(orderJson);
    }
    void GameMap::updateArmiesOrders ()
    {
        for(auto const& e : armies)
            if(!(armies[e.first].isInBattle()))
                armies[e.first].advanceOrders();
    }
    void GameMap::updateBattles()
    {
        for(auto const& e : battles)
            battles[e.first].advance();
    }
    Army * GameMap::getArmy (std::string idArmy)
    {
        return &armies[idArmy];
    }
    int GameMap::getLevyMen (std::string idProvince)
    {
        return provinces[idProvince].getLevyMen();
    }
    void GameMap::killMenFromLevy (std::string idProvince, int victims)
    {
        provinces[idProvince].killMenFromLevy(victims);
    }
    void GameMap::checkNewBattles()
    {
        std::map<std::string, std::vector<std::string>> pos;
        for(auto const& e: armies)
            pos[armies[e.first].getCurrentProvince()].push_back(e.first);
        for(auto const& e: pos)
        {
            if(e.second.size() >= 2)
            {
                /*
                std::string province;
                std::vector<std::string> whiteArmies;
                std::vector<std::string> blackArmies;
                int startTurn;
                int endTurn;
                */
                // For each pair of armies get owners
                // Check if owners are at war
                // If it is the case create a battle
                nlohmann::json j;
                j["province"] = e.first;

            }
        }
    }
}