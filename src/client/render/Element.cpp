#include "Element.h"

namespace render {
    /*const Frame *& Element::getFrame() const
    {
        //return this->frame; Useless getter with dumb signature proudly presented by useless dia2code
    }
    void Element::setFrame(const Frame *& frame)
    {
        //this->frame=frame; Useless setter with dumb signature proudly presented by useless dia2code
    }*/
    int Element::getWidth() const
    {
        return this->width;
    }
    void Element::setWidth(int width)
    {
        this->width=width;
    }
    int Element::getHeight() const
    {
        return this->height;
    }
    void Element::setHeight(int height)
    {
        this->height=height;
    }
    int Element::getX() const
    {
        return this->x;
    }
    void Element::setX(int x)
    {
        this->x=x;
    }

}