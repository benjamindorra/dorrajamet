// Generated by dia2code
#ifndef ENGINE__ENGINECORE__H
#define ENGINE__ENGINECORE__H

#include <queue>

namespace engine {
  class EngineEvent;
}

#include "EngineEvent.h"

namespace engine {

  /// class EngineCore - 
  class EngineCore {
    // Associations
    // Attributes
  public:
    std::queue<EngineEvent> eventQueue;
    // Operations
  public:
    int mainLoop ();
    // Setters and Getters
  };

};

#endif
