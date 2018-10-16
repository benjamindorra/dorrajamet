// Generated by dia2code
#ifndef STATE__GAMEMAP__H
#define STATE__GAMEMAP__H

#include <vector>

namespace state {
  class Province;
  class Army;
  class Battle;
}

#include "Province.h"
#include "Army.h"
#include "Battle.h"

namespace state {

  /// class GameMap - A class describing the elements of and on the map
  class GameMap {
    // Associations
    // Attributes
  private:
    std::vector<Province> provinces;
    /// A list of all the armies mobilized on the map
    std::vector<Army> armies;
    /// A list of all ongoing and finished battles between armies
    std::vector<Battle> battles;
    // Setters and Getters
  };

};

#endif
