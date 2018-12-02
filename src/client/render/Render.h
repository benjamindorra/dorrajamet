// Generated by dia2code
#ifndef RENDER__RENDER__H
#define RENDER__RENDER__H

#include <SFML/Graphics.hpp>
#include <string>
#include <map>
#include <state.h>
#include <engine.h>

namespace render {
  class ColorMap;
  class ViewMap;
  class Data;
  class PlayerData;
  class ToState;
  class ToEngine;
  class Element;
};
namespace state {
  class GameState;
};
namespace render {
  class Button;
}

#include "ColorMap.h"
#include "ToState.h"
#include "ToEngine.h"
#include "Button.h"
#include "PlayerData.h"
#include "Data.h"
#include "ViewMap.h"
#include "Element.h"

namespace render {

  /// class Render - 
  class Render {
    // Associations
    // Attributes
  private:
    sf::RenderWindow window;
    std::map<std::string,Element*> toDraw;
    ColorMap colorMap;
    ViewMap * viewMap     = nullptr;
    Data * data     = nullptr;
    PlayerData * playerData     = nullptr;
    ToState state;
    ToEngine engine;
    // Operations
  public:
    Render ();
    Render (int width, int height);
    ~Render ();
    void renderLoop ();
    sf::RenderWindow * getWindow ();
    void addToDraw (std::string id, Element * element);
    void removeToDraw (std::string id);
    void init (state::GameState * state, engine::EngineCore * engine);
    void update ();
    unsigned int getColorCode (int x, int y);
    // Setters and Getters
  };

};

#endif
