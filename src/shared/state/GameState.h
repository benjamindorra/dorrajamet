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
    void updateCharactersData ();
    std::string getProvinceOwner (std::string provinceId);
    std::string getCharacterTopLiege (std::string characterId);
    void updateProvinces ();
    nlohmann::json fetchCharacterData (std::string characterId);
    nlohmann::json fetchAllCharactersData ();
    nlohmann::json fetchProvinceData (int provinceColorCode);
    nlohmann::json fetchProvinceData (std::string provinceId);
    nlohmann::json fetchProvinceOwnerData (int provinceColorCode);
    // Setters and Getters
  };

};

#endif
