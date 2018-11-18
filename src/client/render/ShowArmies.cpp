/**********************************************************
ShowArmies.cpp
All graphical armies and methods to manipulate them.
***********************************************************/
#include "ShowArmies.h"
namespace render {
    ShowArmies::ShowArmies () {}
    ShowArmies::~ShowArmies () {}
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
                army->setPosition(x,y);
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
}