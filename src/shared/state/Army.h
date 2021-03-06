// Generated by dia2code
#ifndef STATE__ARMY__H
#define STATE__ARMY__H

#include <string>
#include <vector>
#include <json.hpp>

namespace state {
  class GameMap;
  class TravelOrder;
  class Army;
}

#include "TravelOrder.h"

namespace state {

  /// class Army - Class to describe a raised army on the map
  class Army {
    // Associations
    // Attributes
  private:
    GameMap * parent;
    std::string id;
    std::string ownerCharacter;
    /// Id(s) of the province(s) of which levies are in this army
    std::vector<std::string> levies;
    /// Pointer of the province where the army is currently located
    std::string currentProvince;
    std::string currentBattle;
    /// Queue of ongoing moving orders for the army
    std::vector<TravelOrder> orders;
    // Operations
  public:
    Army ();
    Army (GameMap * parent, std::string strJson);
    ~Army ();
    Army& operator= (const Army& army);
    void setParent (GameMap * parent);
    bool checkConsistency ();
    void debug ();
    std::string getId ();
    std::string getOwnerCharacter ();
    std::string getCurrentProvince ();
    void setOrders (nlohmann::json orderJson);
    std::string getCurrentBattle ();
    bool isInBattle ();
    bool hasOrder ();
    void advanceOrders ();
    int getMen ();
    void killMen (int victims);
    void setPosition (std::string provinceId);
    bool isDead ();
    void disband ();
    bool ownsLevy (std::string levyProvinceId);
    bool canMove ();
    nlohmann::json toJson ();
    // Setters and Getters
  };

};

#endif
