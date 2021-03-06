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
        refreshChildParentPointers();
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
        //std::cout << "ARMIES(" << armies.size() << ")\n\n";
        /*for(auto const& e: armies)
            armies[e.first].debug();*/
    }
    std::string GameMap::getArmyPosition (std::string armyId)
    {
        
        try {
            return armies.at(armyId).getCurrentProvince();
        }
        catch (const std::exception& e) {
            std::cerr << e.what()<<std::endl;
            throw std::runtime_error("Unknown army in GameMap::getArmyPosition");
        }
    }
    void GameMap::setArmyOrder (std::string armyId, nlohmann::json orderJson)
    {
        try {
            armies.at(armyId).setOrders(orderJson);
        }
        catch (const std::exception& e) {
            std::cerr << e.what()<<std::endl;
            throw std::runtime_error("Unknown army in GameMap::setArmyOrder");
        }
    }
    void GameMap::updateArmiesOrders ()
    {
        for(auto const& e: armies)
            if(!armies[e.first].isInBattle())
                armies[e.first].advanceOrders();
    }
    void GameMap::updateBattles()
    {
        //std::vector<std::string> battlesId;
        for(auto const& e : battles)
            battles[e.first].advance();
            //battlesId.push_back(e.first);
        //for(std::string battleId : battlesId){
            //if (battles[battleId].isFinished())
                //battles.erase(battleId);
            //else
        //}
    }
    Army * GameMap::getArmy (std::string idArmy)
    {
        try {
            return &armies.at(idArmy);
        }
        catch (const std::exception& e) {
            std::cerr << e.what()<<std::endl;
            throw std::runtime_error("Unknown army in GameMap::getArmy");
        }
    }
    int GameMap::getLevyMen (std::string idProvince)
    {
        return provinces[idProvince].getLevyMen();
    }
    std::string GameMap::getKingdomOfCharacter (std::string characterId)
    {
        return parent->getKingdomOfCharacter(characterId);
    }
    std::string GameMap::getKingdomOwner (std::string kingdomId)
    {
        return parent->getKingdomOwner(kingdomId);
    }
    void GameMap::killMenFromLevy (std::string idProvince, int victims)
    {
        provinces[idProvince].killMenFromLevy(victims);
    }
    void GameMap::disbandLevy (std::string idProvince)
    {
        provinces.at(idProvince).disbandLevy();
    }
    void GameMap::raiseLevy (std::string idProvince){
        provinces.at(idProvince).raiseLevy();
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
                    for(j = i + 1; j < e.second.size(); j++)
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
        /*for(auto const& e: battles)
            if(battles[e.first].isFinished())
                battles[e.first].close(parent->getCurrentTurn());*/
        std::vector<std::string> battlesId;
        for(auto const& e : battles)
            battlesId.push_back(e.first);
        for(std::string battleId : battlesId){
            if (battles[battleId].isFinished()) {
                battles.erase(battleId);
            }
        }              
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
        std::map<std::string, std::vector<std::string>> pos; // pos contains all the armies contained in each province
        for(auto const& e: armies)
            pos[armies[e.first].getCurrentProvince()].push_back(e.first);
        for(auto const& e: pos) // for each province containing an army
        {
            // Get whoever owns or control the province
            auto provinceId = e.first;
            std::string provinceController;
            if(provinces[provinceId].isCaptured())
                provinceController = provinces[provinceId].getController();
            else
                provinceController = parent->fetchKingdomData(provinces[provinceId].getKingdom())["holder"];
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
        }
        //removes the siege if there is no opponent army anymore
        for (auto province : provinces){
            if (province.second.isSieged()){
                if (pos.find(province.first) == pos.end()){
                    provinces[province.first].setSiegingArmy("none");
                }
                else {
                    bool opponentArmy = false;
                    for(std::string army : pos.at(province.first)){
                        if (parent->areAtWar(parent->getProvinceOwner(province.first), parent->getArmyOwner(army))){
                            opponentArmy = true;
                        }
                    }
                    if (not opponentArmy){
                            provinces[province.first].setSiegingArmy("none");
                    }
                }
            }
        }
    }
    bool GameMap::checkForOngoingSiege (std::string provinceId)
    {
        try {
            return provinces.at(provinceId).isSieged();
        }
        catch (const std::exception& e) {
            std::cerr << e.what()<<std::endl;
            throw std::runtime_error("Unknown province in GameMap::checkForOngoingSiege");
        }
    }
    void GameMap::updateSieges ()
    {
        //only update the siege of a province if it is at war and there is no battle for it
        for(auto const& e: provinces)
            if (parent->isProvinceAtWar(e.first)){
                bool noBattle = true;
                for (auto battle : battles)
                    if (battle.second.getProvince() == e.first)
                        noBattle = false;
                if (noBattle)
                    provinces[e.first].updateSiege();
            }
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
            if(province.second.getColorCode() == colorCode){
                return province.first;
            }
        throw std::runtime_error("Error: unknown province color code: " + colorCode);
    }

    std::string GameMap::getProvinceKingdomId(unsigned int colorCode) {
        return getProvinceKingdomId(getProvinceId(colorCode));
    }
    
    std::string GameMap::getProvinceKingdomId(std::string provinceId) {
        try {
            return provinces.at(provinceId).getKingdom();
        }
        catch (const std::exception& e) {
            std::cerr << e.what()<<std::endl;
            throw std::runtime_error("Error: unknown province in GameMap::getProvinceKingdomId");
        }
    }

    nlohmann::json GameMap::fetchProvinceData (std::string provinceId)
    {
        try {
            return provinces.at(provinceId).toJson();
        }
        catch (const std::exception& e) {
            std::cerr << e.what()<<std::endl;
            throw std::runtime_error("Unknown province in GameMap::fetchProvinceData: " + provinceId + "\n");
        }
    }
    nlohmann::json GameMap::fetchArmyData (std::string armyId)
    {
        try {
            return armies.at(armyId).toJson();
        }
        catch (const std::exception& e) {
            std::cerr << e.what()<<std::endl;
            throw std::runtime_error("Unknown army in GameMap::fetchArmyData");
        }
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
    nlohmann::json GameMap::fetchCharacterData (std::string characterId)
    {
        return parent->fetchCharacterData(characterId);
    }
    std::string GameMap::getArmyOwner (std::string armyId)
    {
        return armies[armyId].getOwnerCharacter();
    }
    bool GameMap::canArmyMove (std::string armyId)
    {
        return armies[armyId].canMove();
    }
    std::vector<std::string> GameMap::getProvincesOfKingdom (std::string kingdomId)
    {
        std::vector<std::string> res;
        for(auto p: provinces)
            if(p.second.getKingdom() == kingdomId)
                res.push_back(p.first);
        return res;
    }
    void GameMap::transferProvince (std::string provinceId, std::string kingdomId)
    {
        try
        {
            provinces.at(provinceId).setKingdom(kingdomId);
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << std::endl;
            throw std::runtime_error("Error: GameMap::transferProvince( " + provinceId + ", " + kingdomId + " )\n");
        }
    }
    void GameMap::createArmy (nlohmann::json armyJson){
        if (this->armies.find(armyJson["id"])==this->armies.end()){
            this->armies[armyJson["id"]]=Army(this, armyJson.dump());
        }
    }
    std::string GameMap::getProvinceController (std::string provinceId){
        return provinces.at(provinceId).getController();
    }
    void GameMap::setProvinceController (std::string provinceId, std::string controller){
        provinces[provinceId].setController(controller);
    }
    nlohmann::json GameMap::toJson (){
        nlohmann::json gameMap;

        nlohmann::json allProvinces = nlohmann::json::array();
        for(auto province : provinces) {
            allProvinces.push_back(province.second.toJson());
        }
        gameMap["provinces"] = allProvinces;
        nlohmann::json allArmies = nlohmann::json::array();
        for(auto army : armies) {
            allArmies.push_back(army.second.toJson());
        }
        gameMap["armies"] = allArmies;
        nlohmann::json allBattles = nlohmann::json::array();
        for(auto battle : battles) {
            allBattles.push_back(battle.second.toJson());
        }
        gameMap["battles"] = allBattles;

        return gameMap;
    }
}