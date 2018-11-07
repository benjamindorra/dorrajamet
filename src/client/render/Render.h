// Generated by dia2code
#ifndef RENDER__RENDER__H
#define RENDER__RENDER__H

#include <SFML/Graphics.hpp>
#include <string>
#include <map>

namespace render {
  class Element;
}

#include "Element.h"

namespace render {

  /// class Render - 
  class Render {
    // Associations
    // Attributes
  private:
    sf::RenderWindow window;
    std::map<std::string,Element*> toDraw;
    // Operations
  public:
    Render ();
    Render (int width, int height);
    ~Render ();
    void renderLoop ();
    sf::RenderWindow * getWindow ();
    void addToDraw (std::string id, Element * element);
    void removeToDraw (std::string id);
    // Setters and Getters
  };

};

#endif