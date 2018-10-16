// Generated by dia2code
#ifndef STATE__PLAYER__H
#define STATE__PLAYER__H

#include <string>

namespace state {
  class Character;
  class AIPlayer;
  class HumanPlayer;
}

#include "Character.h"
#include "AIPlayer.h"
#include "HumanPlayer.h"

namespace state {

  /// class Player - 
  class Player {
    // Associations
    state::Character references;
    // Attributes
  private:
    std::string id;
    IdRef<Character> currentCharacter;
    int score;
    // Setters and Getters
    const Character& getReferences() const;
    void setReferences(const Character& references);
  };

};

#endif