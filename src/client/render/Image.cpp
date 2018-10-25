#include "Image.h"
#include <iostream>

namespace render {
    Image::Image (){}
    Image::Image (Frame* frame){
        this->frame=frame;
        this->x=0;
        this->y=0;
        this->width=frame->getWidth();
        this->height=frame->getHeight();
        this->keepRatio=true;
        this->followWidth=true;
    }
    Image::Image (Frame* frame, int width, int height){
        this->frame=frame;
        this->width=width;
        this->height=height;
        this->x=(frame->getWidth()-width)/2;
        this->y=(frame->getHeight()-height)/2;
        this->keepRatio=false;
        this->followWidth=true;
        sprite.setScale(sf::Vector2f(width, height));
    }
    Image::Image (Frame* frame, int width, int height, int x, int y){
        this->frame=frame;
        this->width=width;
        this->height=height;
        this->x=x;
        this->y=y;
        this->keepRatio=false;
        this->followWidth=true;
        sprite.setScale(sf::Vector2f(width, height));
    }
    Image::Image (Frame* frame, int width, int height, int x, int y, bool keepRatio, bool followWidth){
        this->frame=frame;
        this->width=width;
        this->height=height;
        this->x=x;
        this->y=y;
        this->keepRatio=keepRatio;
        this->followWidth=followWidth;
    }
    void Image::importFile (std::string path){
        if (!this->texture.loadFromFile(path))
        {
            throw std::runtime_error("Error when loading texture");
        }
        sprite.setTexture(this->texture);
    }

    sf::Sprite Image::getSprite () {
        return this->sprite;
    }
}