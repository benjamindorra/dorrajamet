// Generated by dia2code
#ifndef AI__RANDOMAI__H
#define AI__RANDOMAI__H

#include <state.h>
#include <engine.h>

namespace engine {
  class Command;
}

#include "engine/Command.h"

namespace ai {

  /// class RandomAI - 
  class RandomAI {
    // Associations
    // Attributes
  private:
    state::GameState* gameState;
    engine::EngineCore* engineCore;
    // Operations
  public:
    RandomAI (state::GameState * state, engine::EngineCore * engine);
    ~RandomAI ();
    void randomActions ();
    // Setters and Getters
  };

};

#endif
