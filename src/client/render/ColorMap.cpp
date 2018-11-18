/**********************************************************
ColorMap.cpp
Hidden map, show where a province is through unique colors.
***********************************************************/
#include "ColorMap.h"
#include <iostream>
#include <exception>

namespace render {
    ColorMap::ColorMap () {}
    ColorMap::ColorMap (std::string path) {
        try {
            this->image.loadFromFile(path);
        }
        catch (...){
            throw std::runtime_error("Impossible to load the colorMap");
        }
    }
    ColorMap::~ColorMap () {
        //delete this->image;
    }
    unsigned int ColorMap::getPixel (int x, int y) {
        if ((x>=0) & (y>=0)) {
            if (((unsigned int)x<this->image.getSize().x) & ((unsigned int)y<this->image.getSize().y)) {
                return image.getPixel(x,y).toInteger();
            }
        }
        return 0;
    }

}