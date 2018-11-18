/**********************************************************
Element.cpp
A virtual graphical element.
***********************************************************/
#include "Element.h"

namespace render {
    // Setters and Getters
    /*const Render *& Element::getMainRender() const {
    }
    void Element::setMainRender(const Render *& mainRender) {}*/
    void Element::draw (){}
    int Element::getWidth() const {
        return 1;
    }
    void Element::setWidth(int width) {}
    int Element::getHeight() const {
        return 1;
    }
    void Element::setHeight(int height) {}
    int Element::getX() const {
        return 1;
    }
    void Element::setX(int x) {}
    int Element::getY() const {
        return 1;
    }
    void Element::setY(int y) {}

}