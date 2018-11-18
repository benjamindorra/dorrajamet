// Generated by dia2code
#ifndef RENDER__IMAGE__H
#define RENDER__IMAGE__H

#include <SFML/Graphics.hpp>
#include <string>

namespace render {
  class Element;
}

#include "Element.h"

namespace render {

  /// class Image - 
  class Image : public render::Element {
    // Attributes
  private:
    sf::Sprite sprite;
    sf::Texture texture;
    // Operations
  public:
    Image ();
    Image (int x, int y);
    Image (int x, int y, int width, int height);
    ~Image ();
    void importFile (std::string path);
    sf::Sprite getSprite ();
    sf::Vector2i getSize ();
    void draw ();
    void setPosition (int x, int y);
    sf::Vector2f getPosition ();
    void setSize (int x, int y, bool keepRatio = false, bool followWidth = true);
    void setColor (sf::Color color);
    // Setters and Getters
  };

};

#endif
