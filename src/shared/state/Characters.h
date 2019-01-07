// Generated by dia2code
#ifndef STATE__CHARACTERS__H
#define STATE__CHARACTERS__H

#include <string>
#include <map>
#include <json.hpp>

namespace state {
  class Politics;
  class Character;
}

#include "Character.h"

namespace state {

  /// class Characters - 
  class Characters {
    // Associations
    // Attributes
  private:
    std::map<std::string, Character> characters;
    Politics * parent;
    // Operations
  public:
    Characters ();
    Characters (Politics * parent, std::string strJson);
    ~Characters ();
    void setParent (Politics * parent);
    Character * operator[] (const char * a);
    Character * operator[] (const std::string a);
    bool checkConsistency ();
    void debug ();
    std::string getMainTitle (std::string characterId);
    void changeScoreBy (std::string characterId, int amount);
    void removeClaim (std::string characterId, std::string provinceId);
    std::string getKingdomOfCharacter (std::string characterId);
    void updateCharactersData ();
    nlohmann::json fetchCharacterData (std::string characterId);
    nlohmann::json fetchAllCharactersData ();
    // Setters and Getters
  };

};

#endif
