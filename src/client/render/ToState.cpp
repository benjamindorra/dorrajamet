#include "ToState.h"

namespace render {
    ToState::ToState () {}
    ToState::ToState (state::GameState * state) {
        this->state=state;
    }
    ToState::~ToState () {}
    std::string ToState::getCharacter (std::string id) {
        return "plop";
    }
    std::string ToState::getProvince (std::string idColor){
        return "plop";
    }
    std::string ToState::getArmy (std::string id){
        return "plop";
    }
    std::string ToState::getArmies (){
        return "plop";
    }
    std::string ToState::getRelations (std::string id){
        return "plop";
    }
}