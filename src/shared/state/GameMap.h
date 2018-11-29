// Generated by dia2code
#ifndef STATE__GAMEMAP__H
#define STATE__GAMEMAP__H

#include <string>
#include <map>
#include <json.hpp>

namespace state {
  class GameState;
  class Army;
  class Battle;
  class Province;
}

#include "Battle.h"
#include "Army.h"
#include "Province.h"

namespace state {

  /// class GameMap - A class describing the elements of and on the map
  class GameMap {
    // Associations
    // Attributes
  private:
    GameState * parent;
    std::map<std::string, Province> provinces;
    /// A list of all the armies mobilized on the map
    std::map<std::string, Army> armies;
    /// A list of all ongoing and finished battles between armies
    std::map<std::string, Battle> battles;
    // Operations
  public:
    GameMap ();
    GameMap (GameState * parent, std::string strJson);
    ~GameMap ();
    void refreshChildParentPointers ();
    void setParent (GameState * parent);
    bool checkConsistency ();
    void debug ();
    std::string getArmyPosition (std::string armyId);
    void setArmyOrder (std::string armyId, nlohmann::json orderJson);
    void updateArmiesOrders ();
    void checkNewBattles ();
    void updateBattles ();
    Army * getArmy (std::string idArmy);
    int getLevyMen (std::string idProvince);
    void killMenFromLevy (std::string idProvince, int victims);
    void disbandLevy (std::string idProvince);
    bool checkForOngoingBattles (std::string province);
    void clearFinishedBattles ();
    void clearDeadArmies ();
    void updateReinforcementRates ();
    void reinforceLevies ();
    void checkNewSieges ();
    bool checkForOngoingSiege (std::string provinceId);
    void updateSieges ();
    void updateProvincesData ();
    std::string getArmyOfLevy (std::string levyProvinceId);
    nlohmann::json fetchProvinceData (int provinceColorCode);
    std::string getProvinceId (int provinceColorCode);
    std::string getProvinceOwner (std::string provinceId);
    nlohmann::json fetchCharacterData (std::string characterId);
    // Setters and Getters
  };

};

#endif
