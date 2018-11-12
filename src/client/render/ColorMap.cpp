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
        if ((x<this->image.getSize().x) & (y<this->image.getSize().y) & (x>=0) & (y>=0)) {
            return image.getPixel(x,y).toInteger();
        }
        else return 0;
    }

}