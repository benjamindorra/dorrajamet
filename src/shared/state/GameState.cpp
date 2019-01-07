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
        politics = new Politics();
        gameMap = new GameMap();
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
            this->politics = new Politics(nullptr, j["politics"].dump());
            this->ressources = Ressources(j["ressources"].dump());
            this->gameMap = new GameMap(nullptr, j["gameMap"].dump());
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
        refreshChildParentPointers();
    }
    GameState::~GameState()
    {
        delete gameMap;
        delete politics;
    }
    void GameState::refreshChildParentPointers()
    {
        this->gameMap->setParent(this);
        this->politics->setParent(this);
    }
    void GameState::debug()
    {
        //this->politics->debug();
        //this->ressources.debug();
        this->gameMap->debug();
    }
    void GameState::updatePlayerCharacter (std::string formerCharacterId, std::string newCharacterId, int score)
    {
        for(auto e: players)
            if(e.getCharacter() == formerCharacterId)
            {
                e.setCharacter(newCharacterId);
                e.addScore(score);
                return;
            }
        throw std::runtime_error("Error: GameState::updatePlayerCharacter - no player with character id: " + formerCharacterId);
    }
    void GameState::setArmyOrder(std::string armyId, std::string destId)
    {
        auto origId = gameMap->getArmyPosition(armyId);
        auto path = ressources.getOrderJson(origId, destId);
        gameMap->setArmyOrder(armyId, path);
    }
    bool GameState::turnAdvance()
    {
        currentPlayer++;
        if(currentPlayer >= players.size())
        {
            currentPlayer = 0;
            return true;
        }
        else
            return false;
    }
    void GameState::updateArmiesOrders ()
    {
        gameMap->updateArmiesOrders();
    }
    void GameState::updateBattles()
    {
        gameMap->updateBattles();
    }
    void GameState::checkNewBattles()
    {
        gameMap->checkNewBattles();
    }
    bool GameState::checkWarStatus (std::string characterA, std::string characterB)
    {
        return politics->checkWarStatus(characterA, characterB);
    }
    int GameState::getCurrentTurn ()
    {
        return currentTurn;
    }
    void GameState::clearFinishedBattles ()
    {
        gameMap->clearFinishedBattles();
    }
    void GameState::clearDeadArmies ()
    {
        gameMap->clearDeadArmies();
    }
    void GameState::updateLevies ()
    {
        gameMap->updateReinforcementRates();
        gameMap->reinforceLevies();
    }
    void GameState::updateWars ()
    {
        politics->updateWars();
    }
    void GameState::updateCharactersData ()
    {
        politics->updateCharactersData();
    }
    void GameState::updateProvinces()
    {
        // Check for new sieges
        gameMap->checkNewSieges();
        // Update ongoing sieges
        gameMap->updateSieges();
        // Update prosperity and tax incomes
        gameMap->updateProvincesData();
    }
    std::string GameState::getKingdomOfCharacter (std::string characterId)
    {
        return politics->getKingdomOfCharacter(characterId);
    }
    std::string GameState::getKingdomOwner (std::string kingdomId)
    {
        return politics->getCharacterOfKingdom(kingdomId);
    }
    nlohmann::json GameState::fetchCharacterData (std::string id)
    {
        return politics->fetchCharacterData(id);
    }
    nlohmann::json GameState::fetchCharacterData (unsigned int colorCode)
    {
        std::string kingdomId = gameMap->getProvinceKingdomId(colorCode);
        std::string characterId =  politics->getCharacterOfKingdom(kingdomId);
        return fetchCharacterData(characterId);
    }

    nlohmann::json GameState::fetchProvinceData (std::string id)
    {
        return gameMap->fetchProvinceData(id);
    }
    nlohmann::json GameState::fetchProvinceData (unsigned int colorCode)
    {
        return gameMap->fetchProvinceData(gameMap->getProvinceId(colorCode));
    }
    nlohmann::json GameState::fetchKingdomData (std::string id)
    {
        return politics->fetchKingdomData(id);
    }
    nlohmann::json GameState::fetchKingdomData (unsigned int colorCode)
    {
        return politics->fetchKingdomData(gameMap->getProvinceId(colorCode));
    }
    nlohmann::json GameState::fetchArmyData (std::string id)
    {
        return gameMap->fetchArmyData(id);
    }
    nlohmann::json GameState::fetchAllArmiesData ()
    {
        return gameMap->fetchAllArmiesData();
    }
    nlohmann::json GameState::fetchAllRelationsData ()
    {
        return politics->fetchAllRelationsData();
    }
    nlohmann::json GameState::fetchAllPlayersData ()
    {
        nlohmann::json res = nlohmann::json::array();
        for(auto player: players)
            res.push_back(player.toJson());
        return res;
    }
    nlohmann::json GameState::fetchAllProvincesData ()
    {
        return gameMap->fetchAllProvincesData();
    }
    nlohmann::json GameState::fetchAllProvincesKingdomColor ()
    {
        auto provinces = gameMap->fetchAllProvincesData();
        nlohmann::json res = nlohmann::json::array(), obj;
        for(auto it = provinces.begin(); it != provinces.end(); ++it)
        {
            obj["provinceId"] = it.value()["id"].get<std::string>();
            if(obj["provinceId"] != "sea")
            {
                obj["color"] = this->politics->getKingdomColor(it.value()["kingdomId"]);
                res.push_back(obj);
            }
        }
        return res;
    }
}