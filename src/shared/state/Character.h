// Generated by dia2code
#ifndef STATE__CHARACTER__H
#define STATE__CHARACTER__H

#include <string>
#include <vector>

namespace state {

  /// class Character - 
  class Character {
    // Attributes
  private:
    std::string id;
    std::string name;
    std::string dynastyName;
    std::string mainTitle;
    /// In turns number
    int age;
    std::vector<std::string> traits;
    int diplomacy;
    int stewardship;
    int martial;
    int intrigue;
    std::vector<std::string> claims;
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
    std::string getMainTitle ();
    // Setters and Getters
  };

};

#endif
