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
            this->map.importFile("./res/testMapShown.bmp");
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
        sf::Vector2f mouse = mainRender->getWindow()->mapPixelToCoords (sf::Mouse::getPosition(*mainRender->getWindow()), view);
        x = (x-mouse.x)*zoom+mouse.x;
        y = (y-mouse.y)*zoom+mouse.y;
        float oldSizeY = view.getSize().y/2.0;
        float sizeY = oldSizeY*zoom;
        float mapY = map.getSize().y;
        if ((y-sizeY>=0) & ((y+sizeY)<=mapY)) {
            this->view.setCenter(x,y);
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
        sf::Vector2f mouse = mainRender->getWindow()->mapPixelToCoords (sf::Mouse::getPosition(*mainRender->getWindow()), view);
        x = (x-mouse.x)*zoom+mouse.x;
        y = (y-mouse.y)*zoom+mouse.y;
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

    void ViewMap::update(Types type) {
        // reload the armies if what is shown is different from the data
        try {
            updateArmies();
        }
        catch(const std::exception& e) {
            std::cerr<<e.what()<<std::endl;
            throw std::runtime_error("Error refreshing the armies");
        }
        try {
            static int slowDown = 0;
            slowDown = (slowDown+1)%6;
            if ((slowDown == 1)) {
                if (type == Types::Kingdoms){
                    updateColorsKingdoms();
                }
                else if (type == Types::Relations){
                    updateColorsRelations();
                }
            }
        }
        catch(const std::exception& e) {
            std::cerr<<e.what()<<std::endl;
            throw std::runtime_error("Error refreshing the provinces colors");
        }
    }

    void ViewMap::updateArmies() {
        // update ShowArmies army list to fit the gamestate
        using json = nlohmann::json;
        json j = state->fetchAllArmiesData();
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
                json province = state->fetchProvinceData(it.value()["currentProvince"].get<std::string>());
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
                json province = state->fetchProvinceData(it.value()["currentProvince"].get<std::string>());
                showArmies.newArmy(id, province["dispPosX"].get<int>(), province["dispPosY"].get<int>());
            }
        }
    }
    void ViewMap::updateColorsRelations() {
        // update the colors of the map (green : yours, blue : allies, red : enemies, white : neutral)
        using json = nlohmann::json;
        //player's character
        std::string playerChar = "chara_0001";
        //lists all relations of the player's character
        std::vector<json> relationsList;
        json relations = state->fetchAllRelationsData();
        for(json::iterator it = relations.begin(); it!= relations.end(); ++it)
        {
            if(it.value()["characterA"].get<std::string>()==playerChar) {
                relationsList.push_back(it.value());
            }
        }
        // test if the colors need to be changed
        sf::Image modMap = map.getTexture().copyToImage();
        unsigned int colorCode;
        unsigned int color;
        bool changeColors = false;
        bool relationExists;
        json provinces = state->fetchAllProvincesData();
        for (json::iterator it=provinces.begin(); it !=provinces.end(); it++){
            color = modMap.getPixel(it.value()["dispPosX"],it.value()["dispPosY"]).toInteger();
            colorCode = it.value()["colorCode"];
            json character = state->fetchCharacterDataFromColor(std::to_string(colorCode));
            if (colorCode != 255) {
                if(character["id"]==playerChar){
                    if(sf::Color(0,255,0).toInteger()!=color) changeColors=true;
                }
                else{
                    relationExists=false;
                    for (json r : relationsList) {
                        if(r["characterB"].get<std::string>()==character["id"]){
                            switch(r["type"].get<int>()){
                                case 0:
                                    //type = non_aggression;
                                    if(sf::Color(0,0,127).toInteger()!=color) changeColors=true;
                                    break;
                                case 1:
                                    //type = alliance;
                                    if(sf::Color(0,0,200).toInteger()!=color) changeColors=true;
                                    break;
                                case 2:
                                    //type = friendship;
                                    if(sf::Color(0,0,255).toInteger()!=color) changeColors=true;
                                    break;
                                case 3:
                                    //type = rivalry;
                                    if(sf::Color(127,0,0).toInteger()!=color) changeColors=true;
                                    break;
                                case 4:
                                    //type = war;
                                    if(sf::Color(255,0,0).toInteger()!=color) changeColors=true;
                                    break;
                                default:
                                    throw std::string("Error: invalid relation type.");
                            }
                            //no need to continue if a relation has been found
                            relationExists = true;
                            break;
                        }
                    }
                    //if there is no relation between player's character and the province
                    //owner, color the province in white
                    if (not relationExists) {
                        if(sf::Color(255,255,255).toInteger()!=color) changeColors=true;
                    }
                }
            }
        }
        if (changeColors) {
            //colors the texture accordingly
            int modHeight = (int)modMap.getSize().y;
            int modWidth = (int)modMap.getSize().x;
            for (int y=0;y<modHeight; y++) {
                for (int x=0; x<modWidth; x++) {
                    if (modMap.getPixel(x,y).toInteger()!=255) {
                        colorCode = mainRender->getColorCode(x,y);
                        //std::cout<<state->fetchCharacterDataFromColor(std::to_string(colorCode))["name"]<<std::endl;
                        //if(state->fetchCharacterDataFromColor(std::to_string(colorCode)) == "sea") {}
                        json character = state->fetchCharacterDataFromColor(std::to_string(colorCode));
                        if(character["id"]==playerChar){
                            modMap.setPixel(x,y, sf::Color(0,255,0));
                        }
                        else{
                            relationExists = false;
                            for (json r : relationsList) {
                                if(r["characterB"].get<std::string>()==character["id"]){
                                    switch(r["type"].get<int>()){
                                        case 0:
                                            //type = non_aggression;
                                            modMap.setPixel(x,y, sf::Color(0,0,127));
                                            break;
                                        case 1:
                                            //type = alliance;
                                            modMap.setPixel(x,y, sf::Color(0,0,200));
                                            break;
                                        case 2:
                                            //type = friendship;
                                            modMap.setPixel(x,y, sf::Color(0,0,255));
                                            break;
                                        case 3:
                                            //type = rivalry;
                                            modMap.setPixel(x,y, sf::Color(127,0,0));
                                            break;
                                        case 4:
                                            //type = war;
                                            modMap.setPixel(x,y, sf::Color(255,0,0));
                                            break;
                                        default:
                                            throw std::string("Error: invalid relation type.");
                                    }
                                    //no need to continue if a relation has been found
                                    relationExists = true;
                                    break;
                                }
                            }
                            //if there is no relation between player's character and the province
                            //owner, color the province in white
                            if (not relationExists) {
                                modMap.setPixel(x,y, sf::Color(255,255,255));
                            }
                        }
                    }

                }
            }
            //modMap.saveToFile("testModMap.bmp");
            sf::Texture modTexture;
            modTexture.loadFromImage(modMap);
            map.setTexture(modTexture);
        }
    }

    void ViewMap::updateColorsKingdoms() {
        // update the colors of the map with one color for each kingdom
        using json = nlohmann::json;
        //lists all the provinces and their colors
        json provinces = state->fetchAllProvincesTopLiegeColor();
        // check if the colors need to be changed
        sf::Image modMap = map.getTexture().copyToImage();
        unsigned int color;
        bool changeColors = false;
        json provincesData = state->fetchAllProvincesData();
        for (json::iterator it=provincesData.begin(); it !=provincesData.end(); it++){
            if (it.value()["colorCode"]!=255) {
                        color = modMap.getPixel(it.value()["dispPosX"],it.value()["dispPosY"]).toInteger();
                        for (json::iterator p=provinces.begin(); p!=provinces.end(); ++p) {
                            if (p.value()["provinceId"]==it.value()["id"]){
                                if (p.value()["color"]!=color) {
                                    changeColors=true;
                                    break;
                                } 
                            }
                        }
                    }
        }
        // colors the texture accordingly
        int modHeight = (int)modMap.getSize().y;
        int modWidth = (int)modMap.getSize().x;
        unsigned int colorCode;
        if (changeColors){
            for (int y=0;y<modHeight; y++) {
                for (int x=0; x<modWidth; x++) {
                    if (modMap.getPixel(x,y).toInteger()!=255) {
                        colorCode = mainRender->getColorCode(x,y);
                        json provinceId = state->fetchProvinceDataFromColor(std::to_string(colorCode))["id"];
                        for (json::iterator p=provinces.begin(); p!=provinces.end(); ++p) {
                            if (p.value()["provinceId"]==provinceId){
                                modMap.setPixel(x,y, sf::Color(p.value()["color"]));
                                break;
                            }
                        }
                    }
                }
            }
            //modMap.saveToFile("testModMapKingdoms.bmp");
            sf::Texture modTexture;
            modTexture.loadFromImage(modMap);
            map.setTexture(modTexture);
        }
    }
}