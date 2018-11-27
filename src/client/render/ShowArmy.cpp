/**********************************************************
ShowArmiy.cpp
A graphical army.
***********************************************************/
#include "ShowArmy.h"
#include "../shared/state.h"
#include "Render.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>

namespace render {
    ShowArmy::ShowArmy () {}
    ShowArmy::ShowArmy (Render * mainRender, Image& armyImg) {
        this->mainRender=mainRender;
        this->armyImg=armyImg;
        this->id = "test";
        this->selected=false;
        srand (time(NULL));
        x = (std::rand()%380) + 20;
        y = (std::rand()%380) + 20;
        this->setPosition(x,y);   
    }
    ShowArmy::ShowArmy (Render * mainRender, Image& armyImg, std::string id) {
        this->selected=false;
        this->mainRender=mainRender;
        this->armyImg=armyImg;
        this->id = id;
        srand (time(NULL));
        x = (std::rand()%380) + 20;
        y = (std::rand()%380) + 20;
        this->setPosition(x,y);
    }
    ShowArmy::ShowArmy (Render * mainRender, sf::Texture * texture, std::string id, int x, int y) {
        this->selected=false;
        this->mainRender=mainRender;
        this->armyImg=Image(x, y);
        this->armyImg.addTexture(texture);
        this->id = id;
        this->setPosition(x,y);
    }
    ShowArmy::~ShowArmy () {}
    void ShowArmy::draw() {
        mainRender->getWindow()->draw(armyImg.getSprite());
    }
    void ShowArmy::setPosition(int x, int y) {
        armyImg.Image::setPosition(x,y);
    }

    bool ShowArmy::contains(int x, int y) {
        sf::Vector2f position = armyImg.getPosition();
        sf::Vector2i size = armyImg.getSize();
        int left = position.x;
        int right = left+size.x;
        int top = position.y;
        int bottom = top+size.y;
        if ((x>left) & (x<right)) {
            if ((y>top) & (y<bottom)) {
                return true;
            }
        }
        return false;
    }

    std::string ShowArmy::getId() {
        return id;
    }

    void ShowArmy::select() {
        selected=!selected;
        if (selected) {
            //armyImg.importFile("./res/armySel.bmp");
            armyImg.setColor(sf::Color(0, 0, 255));
        }
        else {
            //armyImg.importFile("./res/army.bmp");
            armyImg.setColor(sf::Color(255, 255, 255));
        }
    }

    bool ShowArmy::getSelected() {
        return selected;
    }
}