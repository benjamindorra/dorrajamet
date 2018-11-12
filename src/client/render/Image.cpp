#include "Image.h"
#include <iostream>
#include <vector>

namespace render {
    Image::Image (){
    }
    Image::Image (int width, int height){
        this->width=width;
        this->height=height;
        this->x=0;
        this->y=0;
        this->keepRatio=false;
        this->followWidth=true;
        sprite.setPosition(sf::Vector2f(this->x, this->y));
    }
    Image::Image (int width, int height, int x, int y){
        this->width=width;
        this->height=height;
        this->x=x;
        this->y=y;
        this->keepRatio=false;
        this->followWidth=true;
        sprite.setPosition(sf::Vector2f(x, y));
    }
    Image::Image (int width, int height, int x, int y, bool keepRatio, bool followWidth){
        this->width=width;
        this->height=height;
        this->x=x;
        this->y=y;
        this->keepRatio=keepRatio;
        this->followWidth=followWidth;
        sprite.setPosition(sf::Vector2f(x, y));
    }

    Image::~Image() {}
    
    void Image::importFile (std::string path){
        if (!this->texture.loadFromFile(path))
        {
            throw std::runtime_error("Error when loading texture");
        }
        sprite.setTexture(this->texture);
        sf::Vector2u size = this->texture.getSize(); 
        if (!this->keepRatio) {
            sprite.setScale(sf::Vector2f((float)this->width/size.x, (float)this->height/size.y));
        }
        else if (this->followWidth){
            float ratio = (float)this->width/size.x;
            sprite.setScale(sf::Vector2f(ratio, ratio));
        }
        else if (!this->followWidth){
            float ratio = (float)this->height/size.y;
            sprite.setScale(sf::Vector2f(ratio, ratio));
        }
    }

    sf::Sprite Image::getSprite () {
        return this->sprite;
    }

    sf::Vector2u Image::getSize() {
        return texture.getSize();
    }
    
    void Image::setPosition(int x, int y) {
        sprite.setPosition(sf::Vector2f(x, y));
    }

    void Image::draw() {}
}