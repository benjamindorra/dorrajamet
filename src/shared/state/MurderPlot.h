// Generated by dia2code
#ifndef STATE__MURDERPLOT__H
#define STATE__MURDERPLOT__H


namespace state {
  class Character;
  class Plot;
};
class IdRef;
#include "Character.h"
#include "Plot.h"
#include "IdRef.h"

namespace state {

  /// class MurderPlot - 
  class MurderPlot : public state::Plot {
    // Associations
    // Attributes
  private:
    IdRef<Character> targetCharacter;
    // Setters and Getters
  };

};

#endif
