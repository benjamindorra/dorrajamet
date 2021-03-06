// Generated by dia2code
#ifndef STATE__TRAIT__H
#define STATE__TRAIT__H

#include <string>

namespace state {

  /// class Trait - 
  class Trait {
    // Attributes
  private:
    std::string id;
    std::string opposedId;
    int diplomacyModifier;
    int stewardshipModifier;
    int martialModifier;
    int intrigueModifier;
    // Operations
  public:
    Trait ();
    Trait (std::string strJson);
    ~Trait ();
    bool checkConsistency ();
    void debug ();
    const std::string getId ();
    const std::string getOpposedId ();
    const int getDiplomacyModifier ();
    const int getStewardshipModifier ();
    const int getMartialModifier ();
    const int getIntrigueModifier ();
    // Setters and Getters
  };

};

#endif
