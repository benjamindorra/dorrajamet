// Generated by dia2code
#ifndef RENDER__CADRE__H
#define RENDER__CADRE__H


namespace render {
  class Image;
  class Text;
  class Button;
}

#include "Image.h"
#include "Text.h"
#include "Button.h"

namespace render {

  /// class Cadre - 
  class Cadre {
    // Associations
    // Attributes
  public:
    pair<int x, int y> position;
    pair<int height, int width> size;
    // Operations
  public:
    Cadre ();
    // Setters and Getters
  };

};

#endif
