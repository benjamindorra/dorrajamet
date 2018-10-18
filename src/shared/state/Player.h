// Generated by dia2code
#ifndef STATE__PLAYER__H
#define STATE__PLAYER__H

#include <string>

namespace state {
  class AIPlayer;
  class HumanPlayer;
}

#include "AIPlayer.h"
#include "HumanPlayer.h"

namespace state {

  /// class Player - 
  class Player {
    // Associations
    // Attributes
  private:
    std::string id;
    std::string currentCharacter;
    int score;
    // Setters and Getters
  };

};

#endif
