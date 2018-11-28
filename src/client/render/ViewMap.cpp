/**********************************************************
ViewMap.cpp
Visible map displayed on screen. Include methods to zoom 
and move the view.
***********************************************************/
#include "ViewMap.h"
#include "Render.h"
#include <iostream>
#include <json.hpp>

namespace render {
    ViewMap::ViewMap () {
        
    }
    ViewMap::ViewMap (Render * mainRender,ToState * state, ToEngine * engine) {
        this->mainRender=mainRender;
        this->state = state;
        this->engine = engine;
        try {
            // import the image
            this->map.importFile("./res/provinces.bmp");
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << std::endl ;
            throw std::runtime_error("Error when importing the texture.");
        }
        view.setViewport(sf::FloatRect(0.3, 0.1, 0.7, 0.9));
        view.setSize(map.getSize().x, map.getSize().y*view.getViewport().width/view.getViewport().height);
        sf::Vector2f viewCenter = sf::Vector2f(map.getSize().x/2, map.getSize().y/2);
        view.setCenter(viewCenter);
        // create the ShowArmies
        this->showArmies = ShowArmies(mainRender,state,engine);
    }
    ViewMap::ViewMap (Render * mainRender, Image map, ShowArmies& showArmies) : map(map), showArmies(showArmies){
        this->mainRender=mainRender;
    }
    ViewMap::ViewMap (Render * mainRender, Image map, sf::Vector2f size, sf::Vector2f center, ShowArmies& showArmies) : map(map), showArmies(showArmies) {
                view.setCenter(center);
                view.setViewport(sf::FloatRect(0.3, 0.1, 0.7, 0.9));
                view.setSize(size);
                this->mainRender=mainRender;
    }
    ViewMap::~ViewMap () {}
    sf::Vector2f ViewMap::getSize () {
        return this->view.getSize();
    }

    void ViewMap::checkZoomY(float zoom) {
        // check if zooming over the y axis is possible
        float x = ViewMap::getCenter().x;
        float y = ViewMap::getCenter().y;
        /*sf::Vector2f mouse = mainRender->getWindow()->mapPixelToCoords (sf::Mouse::getPosition(*mainRender->getWindow()), view);
        float x = mouse.x;
        float y = mouse.y;*/
        float oldSizeY = view.getSize().y/2.0;
        float sizeY = oldSizeY*zoom;
        float mapY = map.getSize().y;
        if ((y-sizeY>=0) & ((y+sizeY)<=mapY)) {
            this->view.zoom(zoom);
        }
        else if (2*sizeY<=mapY) {
            if ((y-oldSizeY>=0) & (y-sizeY<0)) {
                this->view.setCenter(x, sizeY);
                this->view.zoom(zoom);
            }
            else if ((y+oldSizeY<=mapY) & (y+sizeY>mapY)) {
                this->view.setCenter(x, mapY-sizeY);
                this->view.zoom(zoom);
            }
        }
    }

    void ViewMap::changeZoom (float zoom) {
        // check that zooming doesn't get the view out of image
        float x = ViewMap::getCenter().x;
        float y = ViewMap::getCenter().y;
        /*sf::Vector2f mouse = mainRender->getWindow()->mapPixelToCoords (sf::Mouse::getPosition(*mainRender->getWindow()), view);
        float x = mouse.x;
        float y = mouse.y;*/
        float oldSizeX = view.getSize().x/2.0;
        float sizeX = oldSizeX*zoom;
        float mapX = map.getSize().x;
        if ((x-sizeX>=0) & ((x+sizeX)<=mapX)){
            checkZoomY(zoom);
        }
        else if (2*sizeX<=mapX) {
            if ((x-oldSizeX>=0) & (x-sizeX<0)) {
                this->view.setCenter(sizeX, y);
                checkZoomY(zoom);
            }
            else if ((x+oldSizeX<=mapX) & (x+sizeX>mapX)) {
                this->view.setCenter(mapX-sizeX, y);
                checkZoomY(zoom);
            }
        }
    }
    sf::Vector2f ViewMap::getCenter () {
        return this->view.getCenter();
    }
    void ViewMap::moveView (float deltaX, float deltaY) {
        // check that moving doesn't get the view out of image
        float x = ViewMap::getCenter().x;
        float y = ViewMap::getCenter().y;
        float newX = x+deltaX;
        float newY = y+deltaY;
        float sizeX = view.getSize().x/2.0;
        float sizeY = view.getSize().y/2.0;
        float mapX = map.getSize().x;
        float mapY = map.getSize().y;
        if ((newX-sizeX>=0) & (newX+sizeX<=mapX)){
            if ((newY-sizeY>=0) & (newY+sizeY<=mapY)) {
                this->view.setCenter(newX,newY);
            }
            else if ((y-sizeY>0) & (newY-sizeY<0)) {
                this->view.setCenter(x,sizeY);
            }
            else if ((y+sizeY<mapY) & (newY+sizeY>mapY)) {
                this->view.setCenter(x,mapY-sizeY);
            }
        }
        else if ((x-sizeX>0) & (newX-sizeX<0)) {
            this->view.setCenter(sizeX, y);
        }
        else if ((x+sizeX<mapX) & (newX+sizeX>mapX)) {
            this->view.setCenter(mapX-sizeX, y);
        }
    }

    sf::View * ViewMap::getView() {
        return &view;
    }

    // draw map and armies
    void ViewMap::draw() {
        // change view
        mainRender->getWindow()->setView(view);
        // render map
        mainRender->getWindow()->draw(map.getSprite());
        // render armies
        showArmies.draw();
        // restore view
        mainRender->getWindow()->setView(mainRender->getWindow()->getDefaultView());
    }

    bool ViewMap::contains(int x, int y) {
        sf::FloatRect hitbox = view.getViewport();
        sf::Vector2u size = mainRender->getWindow()->getSize();
        int left = size.x*hitbox.left;
        int right = left+size.x*hitbox.width;
        int top = size.y*hitbox.top;
        int bottom = top+size.y*hitbox.height;
        if ((x>left) & (x<right)) {
            if ((y>top) & (y<bottom)) {
                return true;
            }
        }
        return false;
    }

    std::string ViewMap::leftClick(sf::Vector2f pixel) {
        int pixX=(int)pixel.x;
        int pixY=(int)pixel.y;
        std::string armyId = showArmies.selectArmy(pixX,pixY);
        if (armyId != "") {
            return armyId;
        }
        return "province";
    }

    std::string ViewMap::rightClick(sf::Vector2f pixel) {
        int pixX=(int)pixel.x;
        int pixY=(int)pixel.y;
        std::string armyId = showArmies.moveSelected(pixX,pixY);
        if (armyId != "") {
            return armyId;
        }
        return "province";
    }

    void ViewMap::update() {
        // reload the armies if what is shown is different from the data
        using json = nlohmann::json;
        try {
            json j = json::parse(state->fetchAllArmiesData());
            long unsigned int count = 0;
            bool isEqual = true;
            std::vector<render::ShowArmy*> armies = *showArmies.getArmies();
            for (json::iterator it = j.begin(); it != j.end(); ++it) {
                if (count>=armies.size()) {
                    isEqual = false;
                     break;
                }
                else if (armies[count]->getId() != it.value()["id"].get<std::string>()) {
                    isEqual = false;
                    break;
                }
                else {
                    json province = json::parse(state->fetchProvinceData(it.value()["currentProvince"].get<std::string>()));
                    if (armies[count]->getX() != province["dispPosX"].get<int>()) {
                        isEqual = false;
                        break;
                    }
                    else if (armies[count]->getY() != province["dispPosY"].get<int>()) {
                        isEqual = false;
                        break;
                    }
                }
                count++;
            }
            if (not isEqual) {
                showArmies.deleteArmies();
                for (json::iterator it = j.begin(); it != j.end(); ++it) {
                    std::string id = it.value()["id"].get<std::string>();
                    json province = json::parse(state->fetchProvinceData(it.value()["currentProvince"].get<std::string>()));
                    showArmies.newArmy(id, province["dispPosX"].get<int>(), province["dispPosY"].get<int>());
                }
            }
        }
        catch(const std::exception& e) {
            std::cerr<<e.what()<<std::endl;
            throw std::runtime_error("Error refreshing the armies");
        }
    }
}