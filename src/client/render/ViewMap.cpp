#include "ViewMap.h"
#include "Render.h"

namespace render {
    ViewMap::ViewMap () {
        
    }
    ViewMap::ViewMap (Render * mainRender, Image map) : map(map){
        this->mainRender=mainRender;
    }
    ViewMap::ViewMap (Render * mainRender, Image map, sf::Vector2f size, sf::Vector2f center) : map(map),size(size), center(center) {
                view.setCenter(center);
                view.setSize(size);
                this->mainRender=mainRender;
    }
    ViewMap::~ViewMap () {}
    void ViewMap::changeSize (int var) {}
    void ViewMap::moveView (int deltaX, int deltaY) {}
    void ViewMap::draw() {
        mainRender->getWindow()->setView(view);
        mainRender->Render::getWindow()->draw(map.Image::getSprite());
        mainRender->Render::getWindow()->setView(mainRender->Render::getWindow()->getDefaultView());
    }

}