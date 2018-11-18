// Generated by dia2code
#ifndef RENDER__BUTTON__H
#define RENDER__BUTTON__H

#include <SFML/Graphics.hpp>
#include <string>

namespace render {
  class Render;
  class Element;
}

#include "Element.h"

namespace render {

  /// class Button - 
  class Button : public render::Element {
    // Attributes
  private:
    sf::Text text;
    std::string description;
    sf::Font font;
    sf::RectangleShape frame;
    // Operations
  public:
    Button (Render * mainRender);
    Button (Render * mainRender, int x, int y, std::string description);
    Button (Render * mainRender, int x, int y, std::string description, sf::Color color);
    Button (Render * mainRender, int x, int y, std::string description, sf::Color color, sf::Vector2i size);
    ~Button ();
    bool contains (int x, int y);
    void draw ();
    void setColor (sf::Color color);
    std::string getDescription ();
    // Setters and Getters
  };

};

#endif
