// Generated by dia2code
#ifndef RENDER__SHOWARMIES__H
#define RENDER__SHOWARMIES__H

#include <vector>
#include <SFML/Graphics.hpp>
#include <string>

namespace render {
  class ShowArmy;
  class Render;
  class ToState;
  class ToEngine;
}

#include "ToEngine.h"
#include "ToState.h"
#include "ShowArmy.h"

namespace render {

  /// class ShowArmies - 
  class ShowArmies {
    // Associations
    // Attributes
  private:
    std::vector<ShowArmy*> armies;
    Render * mainRender;
    ToState * state;
    ToEngine * engine;
    sf::Texture texture;
    // Operations
  public:
    ShowArmies ();
    ShowArmies (ToState * state, ToEngine * engine);
    ShowArmies (Render * mainRender, ToState * state, ToEngine * engine);
    ~ShowArmies ();
    void addArmy (ShowArmy * showArmy);
    void newArmy (std::string id, int x, int y);
    std::vector<ShowArmy*> * getArmies ();
    std::string selectArmy (int x, int y);
    std::string moveSelected (int x, int y);
    void draw ();
    // Setters and Getters
  };

};

#endif
