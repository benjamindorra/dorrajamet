#include "GameMap.h"

#include "GameState.h"

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
                this->provinces[jElement["id"].get<std::string>()] = Province(this, jElement.dump());
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
    void GameMap::disbandLevy (std::string idProvince)
    {
        provinces[idProvince].disbandLevy();
    }
    bool GameMap::checkForOngoingBattles (std::string province)
    {
        for(auto const& e: battles)
            if(battles[e.first].getProvince() == province)
                return true;
        return false;
    }
    void GameMap::checkNewBattles()
    {
        std::map<std::string, std::vector<std::string>> pos; // pos contains all the armies contained in each province
        for(auto const& e: armies)
            pos[armies[e.first].getCurrentProvince()].push_back(e.first);
        for(auto const& e: pos) // for each province
        {
            if(checkForOngoingBattles(e.first))
                continue;
            if(e.second.size() >= 2) // If there are two armies or more in a province
            {
                // For each pair of armies get owners
                // Check if owners are at war
                // If it is the case create a battle
                unsigned int i, j;
                std::string character1, character2, army1, army2;
                for(i = 0; i < e.second.size() - 1; i++)
                {
                    if(checkForOngoingBattles(e.first))
                        continue;
                    army1 = e.second[i];
                    for(j = i + 1; i < e.second.size(); j++)
                    {
                        if(checkForOngoingBattles(e.first))
                            continue;
                        army2 = e.second[j];
                        character1 = armies[army1].getOwnerCharacter();
                        character2 = armies[army2].getOwnerCharacter();
                        if(parent->checkWarStatus(character1, character2)) // If characters are at war, start a battle!
                        {
                            nlohmann::json battle, wArmies, bArmies;
                            battle["province"] = e.first;
                            auto battleId = armies[army1].getId() + "_vs_" + armies[army2].getId() + "_t" + std::to_string(parent->getCurrentTurn());
                            battle["id"] = battleId;
                            wArmies.push_back(army1);
                            bArmies.push_back(army2);
                            battle["whiteArmies"] = wArmies;
                            battle["blackArmies"] = bArmies;
                            battle["startTurn"] = parent->getCurrentTurn();
                            battle["endTurn"] = -1;
                            battles[battleId] = Battle(this, battle.dump());
                            // For now we will only consider 1 battle of 1 army vs 1 army per province
                        }
                    }
                }
                

            }
        }
    }
    void GameMap::clearFinishedBattles ()
    {
        for(auto const& e: battles)
            if(battles[e.first].isFinished())
                battles[e.first].close(parent->getCurrentTurn());                
    }
    void GameMap::clearDeadArmies ()
    {
        std::vector<std::string> deadArmies;
        for(auto const& e:armies)
            if(armies[e.first].isDead())
                deadArmies.push_back(e.first);
        for(auto const& army:deadArmies)
        {
            armies[army].disband();
            armies.erase(army);
        }
    }
    void GameMap::updateReinforcementRates ()
    {
        // For each province
            // If Levy is not raised
                // Set its reinforcement rate to 1.0
            // Else
                // Get levy's army
                // If army is in battle
                    // Set reinforcement rate 0.0
                // Else
                    // Get army's position
                    // Get province owner
                    // Get army's position's owner
                    // If the two are the same
                        // Set reinforcement rate 1.0
                    // Else If the two are at war
                        // Set reinforcement rate 0.0
                    // Else
                        // Get both owner's top lieges
                        // If lieges are the same (ie both owners belong to the same realm)
                            // Set reinforcement rate 0.5
                        // Else
                            // Set reinforcement rate 0.0

        for(auto const& e: provinces)
        {
            auto levyProvinceId = e.first;
            if(!provinces[levyProvinceId].isLevyRaised())
                provinces[levyProvinceId].setLevyReinforcementRate(1);
            else
            {
                auto armyId = getArmyOfLevy(levyProvinceId);
                if(armies[armyId].isInBattle())
                    provinces[levyProvinceId].setLevyReinforcementRate(0);
                else
                {
                    auto levyPosition = armies[armyId].getCurrentProvince();
                    auto levyOwner = parent->getProvinceOwner(levyProvinceId);
                    auto positionOwner = parent->getProvinceOwner(levyPosition);
                    if(levyOwner == positionOwner)
                        provinces[levyProvinceId].setLevyReinforcementRate(1);
                    else if(parent->checkWarStatus(levyOwner, positionOwner))
                        provinces[levyProvinceId].setLevyReinforcementRate(0);
                    else
                    {
                        auto levyOwnerTopLiege = parent->getCharacterTopLiege(levyOwner);
                        auto positionOwnerTopLiege = parent->getCharacterTopLiege(positionOwner);
                        if(levyOwnerTopLiege == positionOwnerTopLiege)
                            provinces[levyProvinceId].setLevyReinforcementRate(0.5);
                        else
                            provinces[levyProvinceId].setLevyReinforcementRate(0);
                    }
                }
            }
        }
    }
    std::string GameMap::getArmyOfLevy(std::string levyProvinceId)
    {
        for(auto const& e: armies)
            if(armies[e.first].ownsLevy(levyProvinceId))
                return e.first;
        return std::string("");
    }
    void GameMap::reinforceLevies ()
    {
        for(auto const& e: provinces)
            provinces[e.first].reinforceLevy();
    }
    std::string GameMap::fetchProvinceData(std::string id) {
        return provinces[id].fetchProvinceData().dump();
    }
    std::string GameMap::fetchAllProvincesData() {
        using json = nlohmann::json;
        json allProvincesData;
        for (auto p : provinces) {
            allProvincesData.push_back(p.second.fetchProvinceData());
        }
        return allProvincesData.dump();
    }
    std::string GameMap::fetchArmyData(std::string id) {
        return armies[id].fetchArmyData().dump();
    }
    std::string GameMap::fetchAllArmiesData() {
        using json = nlohmann::json;
        json allArmiesData;
        for (auto a : armies) {
            allArmiesData.push_back(a.second.fetchArmyData());
        }
        return allArmiesData.dump();
    }
}