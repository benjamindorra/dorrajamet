// Generated by dia2code
#ifndef STATE__GAMESTATE__H
#define STATE__GAMESTATE__H

#include <state.h>
#include <vector>
#include <string>
#include <json.hpp>

namespace state {
  class Politics;
  class GameMap;
  class Ressources;
  class Player;
}

#include "Ressources.h"
#include "Player.h"
#include "Politics.h"
#include "GameMap.h"

namespace state {

  /// class GameState - A class describing the whole state of a game. Can be loaded and saved from a file.
  class GameState {
    // Associations
    // Attributes
  private:
    state::Politics * politics;
    state::GameMap * gameMap;
    state::Ressources ressources;
    std::vector<state::Player> players;
    unsigned int currentPlayer;
    int currentTurn;
    // Operations
  public:
    GameState ();
    GameState (const char* saveFilePath);
    GameState (const std::string saveFilePath);
    ~GameState ();
    void testPointer ();
    void refreshChildParentPointers ();
    void debug ();
    void setArmyOrder (std::string armyId, std::string destId);
    int getCurrentTurn ();
    void updatePlayerCharacter (std::string formerCharacterId, std::string newCharacterId, int score);
    bool checkWarStatus (std::string characterA, std::string characterB);
    bool turnAdvance ();
    void updateArmiesOrders ();
    void checkNewBattles ();
    void updateBattles ();
    void clearFinishedBattles ();
    void clearDeadArmies ();
    void updateLevies ();
    void updateWars ();
    void updateCharactersData ();
    void updateProvinces ();
    std::string getKingdomOfCharacter (std::string characterId);
    std::string getKingdomOwner (std::string kingdomId);
    std::string getArmyOwner (std::string armyId);
    bool canArmyMove (std::string armyId);
    nlohmann::json fetchCharacterData (std::string id);
    nlohmann::json fetchCharacterData (unsigned int colorCode);
    nlohmann::json fetchProvinceData (std::string id);
    nlohmann::json fetchProvinceData (unsigned int colorCode);
    nlohmann::json fetchKingdomData (std::string id);
    nlohmann::json fetchKingdomData (unsigned int colorCode);
    nlohmann::json fetchArmyData (std::string id);
    nlohmann::json fetchAllArmiesData ();
    nlohmann::json fetchAllRelationsData ();
    nlohmann::json fetchAllPlayersData ();
    nlohmann::json fetchAllProvincesData ();
    nlohmann::json fetchAllProvincesKingdomColor ();
    std::string getCurrentPlayer ();
    std::string getCurrentPlayerCharacter ();
    std::string hasClaim (std::string claimantId, std::string targetCharacterId);
    bool areAtWar (std::string characterA, std::string characterB);
    void declareWar (std::string claim, std::string attackerId, std::string defenderId);
    std::string getProvinceOwner (std::string provinceId);
    void addClaim (std::string characterId, std::string provinceId);
    std::string getProvinceFromColor (unsigned int colorCode);
    // Setters and Getters
  };

};

#endif
