/**********************************************************
ShowArmies.cpp
All graphical armies and methods to manipulate them.
***********************************************************/
#include "ShowArmies.h"
#include "Render.h"
#include <json.hpp>
#include <iostream>

namespace render {
    ShowArmies::ShowArmies () {}
    ShowArmies::ShowArmies (Render * mainRender, ToState * state, ToEngine * engine) {
        this->mainRender=mainRender;
        this->state = state;
        this->engine = engine;
        std::string stateArmies = state->fetchAllArmiesData();
        try {
            this->texture.loadFromFile("./res/army.bmp");
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << std::endl ;
            throw std::runtime_error("Error when loading the army texture.");
        }
        //ShowArmies::newArmy("army0", 300, 300);
    }
    ShowArmies::~ShowArmies () {
        for (ShowArmy * army : armies) {
            delete army;
        }
    }
    void ShowArmies::addArmy(ShowArmy * showArmy) {
        armies.push_back(showArmy);
    }
    std::vector<ShowArmy*> * ShowArmies::getArmies() {
        return &armies;
    }

    std::string ShowArmies::selectArmy(int x, int y) {
        for (ShowArmy * army : armies) {
            if (army->contains(x,y)){
                army->select();
                return army->getId();
            }   
        }
        return "";
    }

    std::string ShowArmies::moveSelected(int x, int y){
        for (ShowArmy * army : armies) {
            if(army->getSelected()) {
                army->select();
                return army->getId();
            }  
        }
        return "";
    }

    void ShowArmies::draw() {
        for (ShowArmy * army : armies) {
             army->draw();
        }
    }

    void ShowArmies::newArmy(std::string id, int x, int y) {
        armies.push_back(new ShowArmy(mainRender, &texture, id, x, y));
    }

    void ShowArmies::deleteArmies(){
        for (ShowArmy * army : armies) {
            delete army;
        }
    }
}