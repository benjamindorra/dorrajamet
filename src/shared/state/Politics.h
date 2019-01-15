// Generated by dia2code
#ifndef STATE__POLITICS__H
#define STATE__POLITICS__H

#include <vector>
#include <string>
#include <map>
#include <json.hpp>

namespace state {
  class Titles;
  class GameState;
  class Relation;
  class Characters;
  class War;
}

#include "Titles.h"
#include "Relation.h"
#include "Characters.h"
#include "War.h"

namespace state {

  /// class Politics - 
  class Politics {
    // Associations
    // Attributes
  private:
    Titles titles;
    GameState * parent;
    std::vector<Relation> relations;
    Characters characters;
    std::map<std::string, War> wars;
    // Operations
  public:
    Politics ();
    Politics (GameState * parent, std::string strJson);
    ~Politics ();
    void refreshChildParentPointers ();
    void setParent (GameState * parent);
    void debug ();
    void updateCharactersData ();
    int computeCharacterGold (std::string characterId);
    int computeCharacterPrestige (std::string characterId);
    bool checkWarStatus (std::string characterA, std::string characterB);
    void transferKingdom (std::string character_from, std::string character_to, std::string kingdomId);
    void handleCharacterDeath (std::string characterId, std::string heirId, int score);
    void updateWars ();
    void endWar (std::string warId, std::string winner);
    nlohmann::json getKingdomColor (std::string kingdomId);
    std::string getKingdomOfCharacter (std::string characterId);
    std::string getCharacterOfKingdom (std::string kingdomId);
    nlohmann::json fetchCharacterData (std::string characterId);
    nlohmann::json fetchKingdomData (std::string kingdomId);
    nlohmann::json fetchAllRelationsData ();
    std::vector<std::string> getClaims (std::string characterId);
    std::vector<std::string> getAllies (std::string characterId);
    std::string createWar (std::vector<std::string> attackerCamp, std::vector<std::string> defenderCamp, std::string claimantId, std::string mainDefender, std::string targetClaim, unsigned int turn);
    void setWar (std::string characterA, std::string characterB, std::string warId);
    bool areAtWar (std::string characterA, std::string characterB);
    void addClaim (std::string claimant, std::string provinceId);
    std::pair<std::vector<std::string>, std::vector<std::string>> getWarCamps (std::string warId);
    std::string getWar (std::string characterA, std::string characterB);
    void setWarScore (std::string warId, int score);
    nlohmann::json fetchAllCharactersData ();
    // Setters and Getters
  };

};

#endif
