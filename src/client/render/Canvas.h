// Generated by dia2code
#ifndef RENDER__CANVAS__H
#define RENDER__CANVAS__H


namespace render {
  class Window;
  class Frame;
}

#include "Window.h"
#include "Frame.h"

namespace render {

  /// class Canvas - 
  class Canvas {
    // Associations
    // Attributes
  private:
    Window window;
    int x;
    int y;
    int width;
    int height;
    // Operations
  public:
    Canvas ();
    Canvas (Window window);
    Canvas ( window:Window, int width, int height);
    Canvas (Window window, int width, int height, int x, int y);
    ~Canvas ();
    int getWidth ();
    int getHeight ();
    int getX ();
    int getY ();
    // Setters and Getters
  };

};

#endif
