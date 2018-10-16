// Generated by dia2code
#ifndef STATE__GAMESTATE__H
#define STATE__GAMESTATE__H

#include <vector>

namespace state {
  class Politics;
  class GameMap;
  class Character;
  class Ressources;
  class Player;
}

#include "Politics.h"
#include "GameMap.h"
#include "Character.h"
#include "Ressources.h"
#include "Player.h"

namespace state {

  /// class GameState - A class describing the whole state of a game. Can be loaded and saved from a file.
  class GameState {
    // Associations
    // Attributes
  private:
    Politics politics;
    GameMap gameMap;
    std::vector<Character> characters;
    Ressources ressources;
    // Setters and Getters
  };

};

#endif
