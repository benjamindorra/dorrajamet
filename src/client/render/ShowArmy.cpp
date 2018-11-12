#include "ShowArmy.h"
#include "../shared/state.h"
#include "Render.h"
#include <stdlib.h> 

namespace render {
    ShowArmy::ShowArmy () {}
    ShowArmy::ShowArmy (Render * mainRender, Image armyImg) : armyImg(armyImg) {
        this->mainRender=mainRender;
        x = std::rand() %380 + 20;
        y = std::rand() %380 + 20;
        this->setPosition(x,y);   
    }
    ShowArmy::ShowArmy (Render * mainRender, Image armyImg, state::Army * army) : armyImg(armyImg) {
        this->mainRender=mainRender;
    }
    ShowArmy::~ShowArmy () {}
    void ShowArmy::draw() {
        mainRender->Render::getWindow()->draw(armyImg.Image::getSprite());
    }
    void ShowArmy::setPosition(int x, int y) {
        armyImg.Image::setPosition(x,y);
    }
}