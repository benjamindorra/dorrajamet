#include "ViewMap.h"
#include "Render.h"
namespace render {
    ViewMap::ViewMap () {
        
    }
    ViewMap::ViewMap (Render * mainRender, Image map) : map(map){
        this->mainRender=mainRender;
    }
    ViewMap::ViewMap (Render * mainRender, Image map, sf::Vector2f size, sf::Vector2f center) : map(map) {
                view.setCenter(center);
                view.setViewport(sf::FloatRect(0.3, 0.1, 0.7, 0.9));
                view.setSize(size);
                this->mainRender=mainRender;
    }
    ViewMap::~ViewMap () {}
    sf::Vector2f ViewMap::getSize () {
        return this->view.getSize();
    }
    void ViewMap::changeZoom (float zoom) {
        // check that zooming doesn't gets the view out of image
        float x = ViewMap::getCenter().x;
        float y = ViewMap::getCenter().y;
        float sizeX = view.getSize().x/2.0*zoom;
        float sizeY = view.getSize().y/2.0*zoom;
        if ((x-sizeX>0) & (x+sizeX<map.getSize().x)){
            if ((y-sizeY>0) & (x+sizeY<map.getSize().y))
                this->view.zoom(zoom);
        }
    }
    sf::Vector2f ViewMap::getCenter () {
        return this->view.getCenter();
    }
    void ViewMap::moveView (float deltaX, float deltaY) {
        // check that moving doesn't gets the view out of image
        float newX = ViewMap::getCenter().x+deltaX;
        float newY = ViewMap::getCenter().y+deltaY;
        float sizeX = view.getSize().x/2.0;
        float sizeY = view.getSize().y/2.0;
        if ((newX-sizeX>0) & (newX+sizeX<map.getSize().x)){
            if ((newY-sizeY>0) & (newY+sizeY<map.getSize().y)) {
                this->view.setCenter(newX,newY);
                std::cout<<view.getSize().x<<" "<<view.getSize().y<<std::endl;
            }
        }
    }
    std::vector<float> ViewMap::getViewPort () {
        std::vector<float> viewPort;
        sf::FloatRect rect = this->view.getViewport();
        viewPort = {rect.left, rect.top, rect.width, rect.height};
        return viewPort;
    }
    void ViewMap::draw() {
        mainRender->getWindow()->setView(view);
        mainRender->Render::getWindow()->draw(map.Image::getSprite());
        mainRender->Render::getWindow()->setView(mainRender->Render::getWindow()->getDefaultView());
    }

}