#include "Canvas.h"

namespace render {
    Canvas::Canvas () {}
    Canvas::Canvas (window) {
        this ->window=window;
        this->x=0;
        this->y=0;
        this->width=window.getWidth();
        this->height=window.getHeight();
    }
    Canvas::Canvas (window, width, height) {
        this->window=window;
        this->width=width;
        this->height=height;
        this->x=(window.getWidth()-width)/2;
        this->y=(window.getHeight()-height)/2;
    }
    Canvas::Canvas (Window window, int width, int height, int x, int y) {
        this->window=window;
        this->width=width;
        this->height=height;
        this->x=x;
        this->y=y;
    }
    Canvas::~Canvas () {}
    // Setters and Getters
    int Canvas::getWidth () {
        return this->width;
    }
    int Canvas::getHeight () {
        return this->height;
    }
    int Canvas::getX () {
        return this->x;
    }
    int Canvas::getY () {
        return this->y;
    }

}

#endif
