#include "ShowArmies.h"
namespace render {
    ShowArmies::ShowArmies () {}
    ShowArmies::~ShowArmies () {}
    void ShowArmies::addArmy(ShowArmy showArmy) {
        armies.push_back(showArmy);
    }
    std::vector<ShowArmy> * ShowArmies::getArmies() {
        return &armies;
    } 
}