// Generated by dia2code
#ifndef STATE__CHARACTER__H
#define STATE__CHARACTER__H

#include <string>
#include <vector>
#include <map>
#include <json.hpp>

namespace state {
  class Character;
}


namespace state {

  /// class Character - 
  class Character {
    // Attributes
  private:
    std::string id;
    std::string name;
    std::string dynastyName;
    std::string kingdomId;
    /// In turns number
    int age;
    std::vector<std::string> traits;
    int diplomacy;
    int stewardship;
    int martial;
    int intrigue;
    /// map of provinces claimed and the turns the claims become active. An active claim enables you to start a war.
    std::map<std::string, int> claims;
    bool alive;
    int prestige;
    int gold;
    enum plotTypes{none = 0, claim = 1, murder = 2};
    bool hasPlot;
    std::string plotTarget;
    plotTypes plotType;
    int plotEnd;
    // Operations
  public:
    Character ();
    Character (std::string strJson);
    ~Character ();
    void debug ();
    bool checkConsistency ();
    std::string getId ();
    int getPrestige ();
    void addGold (int amount);
    void addPrestige (int amount);
    /// returns true if the character dies
    bool ageUp ();
    Character generateHeir ();
    void removeClaim (std::string provinceId);
    std::string getKingdom ();
    std::map<std::string, int> getClaims ();
    void addClaim (std::string provinceId, int turn);
    nlohmann::json toJson ();
    // Setters and Getters
  };

};

#endif
