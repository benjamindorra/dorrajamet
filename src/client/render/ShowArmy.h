// Generated by dia2code
#ifndef RENDER__SHOWARMY__H
#define RENDER__SHOWARMY__H

#include <string>
#include <SFML/Graphics.hpp>

namespace render {
  class Image;
  class Render;
  class Element;
}

#include "Image.h"
#include "Element.h"

namespace render {

  /// class ShowArmy - 
  class ShowArmy : public render::Element {
    // Attributes
  private:
    Image armyImg;
    std::string id;
    bool selected;
    // Operations
  public:
    ShowArmy ();
    ShowArmy (Render * mainRender, Image& armyImg);
    ShowArmy (Render * mainRender, Image& armyImg, std::string id);
    ShowArmy (Render * mainRender, sf::Texture * texture, std::string id, int x, int y);
    ~ShowArmy ();
    void draw ();
    void setPosition (int x, int y);
    bool contains (int x, int y);
    std::string getId ();
    void select ();
    bool getSelected ();
    // Setters and Getters
  };

};

#endif
