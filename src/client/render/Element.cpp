#include "Element.h"

namespace render {

    const Frame& DynamicContent::getFrame() const {
        return this->frame;
    }
    void DynamicContent::setFrame(const Frame& frame) {
        this->frame=frame;
    }
    int DynamicContent::getWidth() const {
        return this->width;
    }
    void DynamicContent::setWidth(int width) {
        this->width=width;
    }
    int DynamicContent::getHeight() const {
        return this->height;
    }
    void DynamicContent::setHeight(int height) {
        this->height=height;
    }
    int DynamicContent::getX() const {
        return this->x;
    }
    void DynamicContent::setX(int x) {
        this->x=x;
    }

}