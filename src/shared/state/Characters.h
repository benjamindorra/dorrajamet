// Generated by dia2code
#ifndef STATE__CHARACTERS__H
#define STATE__CHARACTERS__H

#include <string>
#include <map>

namespace state {
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
    /// opinions["chara_0001"]["chara_0002"] contains the -100 to +100 opinion score of chara_0001 about chara_0002
    std::map<std::string, std::map<std::string, int>> opinions;
    // Operations
  public:
    Characters ();
    Characters (std::string strJson);
    ~Characters ();
    Character * operator[] (const char * a);
    Character * operator[] (const std::string a);
    int getOpinion (std::string sourceCharacter, std::string targetCharacter);
    bool checkConsistency ();
    void debug ();
    // Setters and Getters
  };

};

#endif