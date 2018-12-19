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
                this->provinces[jElement["id"].get<std::string>()] = Province(nullptr, jElement.dump());
            }
            // Armies
            jArray = j["armies"];
            for(it = jArray.begin(); it!= jArray.end(); ++it)
            {
                jElement = it.value();
                this->armies[jElement["id"].get<std::string>()] = Army(nullptr, jElement.dump());
            }
            // Battles
            jArray = j["battles"];
            for(it = jArray.begin(); it!= jArray.end(); ++it)
            {
                jElement = it.value();
                this->battles[jElement["id"].get<std::string>()] = Battle(nullptr, jElement.dump());
            }
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << std::endl ;
            throw std::runtime_error("Error when loading politics.");
        }
        refreshChildParentPointers();
    }
    GameMap::~GameMap ()
    {

    }
    void GameMap::refreshChildParentPointers()
    {
        for(auto const& e: provinces)
            provinces[e.first].setParent(this);
        for(auto const& e: armies)
            armies[e.first].setParent(this);
        for(auto const& e: battles)
            battles[e.first].setParent(this);
    }
    void GameMap::setParent(state::GameState * parent)
    {
        this->parent = parent;
    }
    bool GameMap::checkConsistency ()
    {
        return true;
    }
    void GameMap::debug ()
    {
        std::cout << "TOTAL PROVINCES COUNT: " << provinces.size() << std::endl;
        for(auto const& e: provinces)
            provinces[e.first].debug();
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
        for(auto const& e: armies)
            if(!armies[e.first].isInBattle())
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
        refreshChildParentPointers();
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
        refreshChildParentPointers();
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

        /*for(auto const& e: provinces)
        {
            auto levyProvinceId = e.first;
            if(!provinces[levyProvinceId].isLevyRaised())
                if(provinces[levyProvinceId].isSieged())
                    provinces[levyProvinceId].setLevyReinforcementRate(0.5);
                else if(provinces[levyProvinceId].isCaptured())
                    provinces[levyProvinceId].setLevyReinforcementRate(0.1);
                else
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
        }*/
    }/*
    std::string GameMap::getArmyOfLevy(std::string levyProvinceId)
    {
        for(auto const& e: armies)
            if(armies[e.first].ownsLevy(levyProvinceId))
                return e.first;
        return std::string("");
    }*/
    void GameMap::reinforceLevies ()
    {
        for(auto const& e: provinces)
            provinces[e.first].reinforceLevy();
    }
    void GameMap::checkNewSieges ()
    {
        /*std::map<std::string, std::vector<std::string>> pos; // pos contains all the armies contained in each province
        for(auto const& e: armies)
            pos[armies[e.first].getCurrentProvince()].push_back(e.first);
        for(auto const& e: pos) // for each province
        {
            // Get whoever owns or control the province
            auto provinceId = e.first;
            std::string provinceController;
            if(provinces[provinceId].isCaptured())
                provinceController = provinces[provinceId].getController();
            else
                provinceController = parent->getProvinceOwner(provinceId);
            // No siege starts if a battle or a siege is ongoing
            if(checkForOngoingBattles(provinceId) || checkForOngoingSiege(provinceId))
                continue;
            // For each army
            for(auto const& a: e.second)
            {
                // Get army owner
                auto armyOwner = armies[a].getOwnerCharacter();
                // Check if province controller and army owner are at war
                if(parent->checkWarStatus(provinceController, armyOwner))
                {
                    provinces[provinceId].setSiegingArmy(a);
                    break;// For now we will only consider the first sieging army
                }
            }
        }*/
    }
    bool GameMap::checkForOngoingSiege (std::string provinceId)
    {
        return provinces[provinceId].isSieged();
    }
    void GameMap::updateSieges ()
    {
        for(auto const& e: provinces)
            provinces[e.first].updateSiege();
    }
    void GameMap::updateProvincesData ()
    {
        for(auto const& e: provinces)
        {
            auto provinceId = e.first;
            provinces[provinceId].updateData();
        }
    }
    
    std::string GameMap::getProvinceId (unsigned int colorCode)
    {
        for(auto province: provinces)
            if(province.second.getColorCode() == colorCode)
                return province.first;
        throw std::runtime_error("Error: unknown province color code: " + colorCode);
    }
    nlohmann::json GameMap::fetchProvinceData (std::string provinceId)
    {
        return provinces[provinceId].toJson();
    }
    nlohmann::json GameMap::fetchArmyData (std::string armyId)
    {
        return armies[armyId].toJson();
    }
    nlohmann::json GameMap::fetchAllArmiesData ()
    {
        nlohmann::json res = nlohmann::json::array();
        for(auto army: armies)
            res.push_back(army.second.toJson());
        return res;
    }
    nlohmann::json GameMap::fetchAllProvincesData ()
    {
        nlohmann::json res = nlohmann::json::array();
        for(auto province: provinces)
            res.push_back(province.second.toJson());
        return res;
    }
}