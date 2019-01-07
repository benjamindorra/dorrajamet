// Generated by dia2code
#ifndef RENDER__POPUP__H
#define RENDER__POPUP__H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

namespace render {
  class Button;
  class ToState;
  class ToEngine;
  class Render;
  class Element;
}

#include "Element.h"

namespace render {

  /// class PopUp - 
  class PopUp : public render::Element {
    // Attributes
  public:
    enum Type{Info=0, Question=1};
    enum Action{War=0, Peace=1, Alliance=2, Claim=3, Plot=4 };
    int clickX;
    int clickY;
    bool selected     = false;
  private:
    sf::RectangleShape frame;
    sf::Text text;
    sf::Font font;
    std::vector<Button*> buttons;
    std::string causeId;
    std::string targetId;
    ToState * state;
    ToEngine * engine;
    // Operations
  public:
    PopUp (Render * mainRender, Type type, Action action, std::string causeId, std::string targetId, ToState * state, ToEngine * engine);
    ~PopUp ();
    void select (Type type, Action action, std::string causeId, std::string targetId);
    void draw ();
    bool contains (int x, int y);
    void click (int x, int y);
    void handleButtons (std::string button);
    void move (int x, int y);
    void deselect ();
    bool isSelected ();
    // Setters and Getters
  };

};

#endif