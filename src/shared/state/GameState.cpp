#include "GameState.h"
#include <iostream>
#include <fstream>
#include <string>
#include <json.hpp>
#include "../tools.hpp"

namespace state
{
    GameState::GameState ()
    {
        politics = Politics();
        gameMap = GameMap();
        ressources = Ressources();
        players = std::vector<Player>();
    }
    GameState::GameState (const char* saveFilePath) : GameState(std::string(saveFilePath)){}
    GameState::GameState (const std::string saveFilePath)
    {
        using json = nlohmann::json;
        try
        {
            std::string fileContent = loadFile(saveFilePath);
            json j = json::parse(fileContent);
            this->politics = Politics(j["politics"].dump());
            this->ressources = Ressources(j["ressources"].dump());
            this->gameMap = GameMap(this, j["gameMap"].dump());
            Player p;
            for(json::iterator it = j["players"].begin(); it != j["players"].end(); ++it)
                players.push_back(Player(it.value()));
            this->currentPlayer = j["currentPlayer"].get<int>();
            this->currentTurn = j["currentTurn"].get<int>();
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what();
            throw std::runtime_error("Error: an error occurred when loading GameState from saveFile.");
        }
    }
    GameState::~GameState()
    {

    }
    void GameState::debug()
    {
        this->politics.debug();
        this->ressources.debug();
        this->gameMap.debug();
    }
    void GameState::setArmyOrder(std::string armyId, std::string destId)
    {
        auto origId = gameMap.getArmyPosition(armyId);
        auto path = ressources.getOrderJson(origId, destId);
        gameMap.setArmyOrder(armyId, path);
    }
    bool GameState::turnAdvance()
    {
        currentPlayer++;
        if(currentPlayer <= players.size())
        {
            currentPlayer = 0;
            return true;
        }
        else
            return false;
    }
    void GameState::updateArmiesOrders ()
    {
        gameMap.updateArmiesOrders();
    }
    void GameState::updateBattles()
    {
        gameMap.updateBattles();
    }
    void GameState::checkNewBattles()
    {
        gameMap.checkNewBattles();
    }
    bool GameState::checkWarStatus (std::string characterA, std::string characterB)
    {
        return politics.checkWarStatus(characterA, characterB);
    }
    int GameState::getCurrentTurn ()
    {
        return currentTurn;
    }
    void GameState::clearFinishedBattles ()
    {
        gameMap.clearFinishedBattles();
    }
    void GameState::clearDeadArmies ()
    {
        gameMap.clearDeadArmies();
    }
    void GameState::updateLevies ()
    {
        gameMap.updateReinforcementRates();
        gameMap.reinforceLevies();
    }
    std::string GameState::getProvinceOwner (std::string provinceId)
    {
        return politics.getProvinceOwner(provinceId);
    }
    std::string GameState::getCharacterTopLiege (std::string characterId)
    {
        return politics.getCharacterTopLiege(characterId);
    }
    std::string GameState::fetchCharacterData (std::string id) {
        return politics.fetchCharacterData(id);
    }
    std::string GameState::fetchProvinceData(std::string id) {
        return gameMap.fetchProvinceData(id);
    }
    std::string GameState::fetchAllProvincesData() {
        return gameMap.fetchAllProvincesData();
    }
    std::string GameState::fetchArmyData(std::string id) {
        return gameMap.fetchArmyData(id);
    }
    std::string GameState::fetchAllArmiesData() {
        return gameMap.fetchAllArmiesData();
    }
    std::string GameState::fetchAllRelationsData() {
        return politics.fetchAllRelationsData();
    }
    std::string GameState::fetchAllPlayersData() {
        using json = nlohmann::json;
        json allPlayersData;
        for (Player p : players) {
            allPlayersData.push_back(p.fetchPlayerData());
        }
        return allPlayersData.dump();
    }
}