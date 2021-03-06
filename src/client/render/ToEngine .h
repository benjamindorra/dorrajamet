// Generated by dia2code
#ifndef RENDER__TOENGINE __H
#define RENDER__TOENGINE __H

#include <string>

namespace render {

  /// class ToEngine  - 
  class ToEngine  {
    // Attributes
  public:
    engine::Commands * commands;
    // Operations
  public:
    ToEngine ();
    ToEngine (engine::Commands * commands);
    ~ToEngine ();
    void addCommand (std::string idTarget, std::string command);
    // Setters and Getters
  };

};

#endif
