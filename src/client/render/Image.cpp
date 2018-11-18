/************************
Image.cpp
Load an image to display.
*************************/
#include "Image.h"
#include <iostream>
#include <vector>

namespace render {
    Image::Image (){
    }

    Image::Image (int x, int y){
        this->x=x;
        this->y=y;
        sprite.setPosition(sf::Vector2f(x, y));
    }
    
    Image::Image (int x, int y, int width, int height){
        this->x=x;
        this->y=y;
        this->width=width;
        this->height=height;
        sprite.setPosition(sf::Vector2f(this->x, this->y));
    }

    Image::~Image() {}
    
    void Image::importFile (std::string path){
        if (!this->texture.loadFromFile(path))
        {
            throw std::runtime_error("Error when loading texture");
        }
        sprite.setTexture(this->texture);
        sf::Vector2u size = this->texture.getSize();
        width=size.x;
        height=size.y;
    }

    sf::Sprite Image::getSprite () {
        return this->sprite;
    }

    sf::Vector2i Image::getSize() {
        return sf::Vector2i(width,height);
    }

    void Image::setSize(int height, int width, bool keepRatio, bool followWidth) {
        sf::Vector2u size = this->texture.getSize();
        if (!keepRatio) {
            width/=size.x;
            height/=size.y; 
        }
        else if (followWidth){
            width/=size.x;
            height/=size.x; 
        }
        else if (!followWidth){
            width/=size.y;
            height/=size.y; 
        }
            sprite.setScale(sf::Vector2f(width, height));
    }

    sf::Vector2f Image::getPosition(){
        return sprite.getPosition();
    }
    
    void Image::setPosition(int x, int y) {
        sprite.setPosition(sf::Vector2f(x, y));
        this->x=x;
        this->y=y;
    }

    void Image::setColor(sf::Color color) {
        this->sprite.setColor(color);
    }
    
    void Image::draw() {}
}