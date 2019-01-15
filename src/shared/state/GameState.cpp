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
            for(auto p: players)
            {
                std::cout << "player: " << p.getId() << std::endl;
            }
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
            std::cout << "player " << players[currentPlayer].getId() << " has " << players[currentPlayer].getMessages().size() << " messages\n";
            return true;
        }
        else
        {
            std::cout << "player " << players[currentPlayer].getId() << " has " << players[currentPlayer].getMessages().size() << " messages\n";
            return false;
        }
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
    std::string GameState::getArmyOwner (std::string armyId)
    {
        return gameMap->getArmyOwner(armyId);
    }
    bool GameState::canArmyMove (std::string armyId)
    {
        return gameMap->canArmyMove(armyId);
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
    std::string GameState::getCurrentPlayer ()
    {
        return players[currentPlayer].getId();
    }
    std::string GameState::getCurrentPlayerCharacter ()
    {
        return players[currentPlayer].getCharacter();
    }
    std::string GameState::hasClaim (std::string claimantId, std::string targetCharacterId)
    {
        auto targetKingdom = politics->getKingdomOfCharacter(targetCharacterId);
        auto targetProvinces = gameMap->getProvincesOfKingdom(targetKingdom);
        auto claims = politics->getClaims(claimantId);
        for(auto p: targetProvinces)
            for(auto c: claims)
                if(p == c)
                    return p;
        return "";
    }
    bool GameState::areAtWar (std::string characterA, std::string characterB)
    {
        return politics->areAtWar(characterA, characterB);
    }
    std::string GameState::declareWar (std::string claim, std::string attackerId, std::string defenderId)
    {
        auto attackerCamp = politics->getAllies(attackerId);
        auto defenderCamp = politics->getAllies(defenderId);
        attackerCamp.push_back(attackerId);
        defenderCamp.push_back(defenderId);
        auto warId = politics->createWar(attackerCamp, defenderCamp, attackerId, defenderId, claim, currentTurn);
        for(auto a: attackerCamp)
            for(auto d: defenderCamp)
                politics->setWar(a, d, warId);
        return warId;
    }
    std::string GameState::getProvinceOwner (std::string provinceId)
    {
        auto kingdomId = gameMap->getProvinceKingdomId(provinceId);
        return politics->getCharacterOfKingdom(kingdomId);
    }
    void GameState::addClaim (std::string characterId, std::string provinceId)
    {
        politics->addClaim(characterId, provinceId);
    }
    std::string GameState::getProvinceFromColor (unsigned int colorCode)
    {
        return gameMap->getProvinceId(colorCode);
    }
    nlohmann::json GameState::fetchPlayerMessages (std::string playerId) {
        //std::cout << "Fetching player " << playerId << "'s message\n";
        for (Player player : players) {
            if (player.getId()==playerId) {
                auto messages = player.getMessages();
                if(messages.size())
                    std::cout << "found " << messages.size() << " messages for player: " << playerId << std::endl;
                return messages;
            }
        }
        std::cerr << "FetchMessages\n";
        throw std::runtime_error("No player of id: " + playerId + "\n");
    }
    std::string GameState::getPlayerOfCharacter (std::string characterId)
    {
        for(auto p: players)
            if(p.getCharacter() == characterId)
                return p.getId();
        std::cerr << "getPlayerOfChar\n";
        throw std::runtime_error("No player for character id: " + characterId + "\n");
    }
    void GameState::pushMessageToPlayer (std::string playerId, nlohmann::json initJson)
    {
        for(unsigned int i = 0; i < players.size(); i++)
            if(players[i].getId() == playerId)
            {
                players[i].addMessage(initJson);
                return;
            }
        std::cerr << "pushMessage\n";
        throw std::runtime_error("No player of id: " + playerId + "\n");
    }
    void GameState::removeMessage (std::string playerId, std::string messageId)
    {
        for(unsigned i = 0; i < players.size(); i++)
            if(players[i].getId() == playerId)
                players[i].removeMessage(messageId);
    }
    bool GameState::areAllies (std::string characterA, std::string characterB)
    {
        auto temp = politics->getAllies(characterA);
        return std::find(temp.begin(), temp.end(), characterB) != temp.end();
    }
    std::pair<std::vector<std::string>, std::vector<std::string>> GameState::getWarCamps (std::string warId)
    {
        return politics->getWarCamps(warId);
    }
    std::string GameState::getCharacterOfPlayer (std::string playerId)
    {
        auto res = std::find_if(players.begin(), players.end(), [playerId](Player p) -> bool{ return p.getId() == playerId; });
        if(res != players.end())
            return res->getCharacter();
        else
            throw std::runtime_error("No player of id: " + playerId + "\n");
    }
    std::string GameState::getWar (std::string characterA, std::string characterB)
    {
        return politics->getWar(characterA, characterB);
    }
    void GameState::peaceOffer (std::string warId, std::string offeringCharacter)
    {
        
    }
    void GameState::surrender (std::string warId, std::string surrenderingCharacter)
    {

    }
    void GameState::endWar (std::string warId, std::string winner)
    {
        politics->endWar(warId, winner);
    }
    void GameState::transferProvince (std::string provinceId, std::string kingdomId)
    {
        gameMap->transferProvince(provinceId, kingdomId);
    }
    GameState GameState::copy(){
        GameState copy;
        *copy.politics = *this->politics;
        copy.politics->setParent(&copy);
        *copy.gameMap = *this->gameMap;
        copy.gameMap->setParent(&copy);
        copy.ressources = this->ressources;
        copy.players = this->players;
        copy.currentPlayer = this->currentPlayer;
        copy.currentTurn = this->currentTurn;
        return copy;
    }
    nlohmann::json GameState::fetchAllCharactersData(){
        return politics->fetchAllCharactersData();
    }

}
