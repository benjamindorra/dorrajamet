#include "ViewMap.h"
#include "Render.h"

namespace render {
    ViewMap::ViewMap () {
        
    }
    ViewMap::ViewMap (Render * render, Image map) : map(map){
        this->render=render;
    }
    ViewMap::ViewMap (Render * render, Image map, sf::Vector2f size, sf::Vector2f center) : map(map),size(size), center(center) {
                view.setCenter(center);
                view.setSize(size);
                this->render=render;
    }
    ViewMap::~ViewMap () {}
    void ViewMap::changeSize (int var) {}
    void ViewMap::moveView (int deltaX, int deltaY) {}
    void ViewMap::draw() {
        render->getWindow()->setView(view);
        render->Render::getWindow()->draw(map.Image::getSprite());
        render->Render::getWindow()->setView(render->Render::getWindow()->getDefaultView());
    }

}