#include "PopUps.h"
#include <json.hpp>

using json = nlohmann::json;

namespace render {
    PopUps::PopUps (Render * mainRender, ToState * state, ToEngine * engine, std::string playerId){
        this->mainRender=mainRender;
        this->state = state;
        this->engine = engine;
        this->playerId = playerId;
    }
    PopUps::~PopUps ( ){}
    void PopUps::newPopUp (bool requireAnswer, PopUp::Action action, std::string causeId, std::string targetId, std::string id){
        if (requireAnswer) {
            popUps[id] = new PopUp(mainRender, PopUp::Type::Question, action, causeId, targetId, state, engine);
        }
        else {
            popUps[id] = new PopUp(mainRender, PopUp::Type::Info, action, causeId, targetId, state, engine);
        }
    }
    void PopUps::deletePopUp (std::string id){
        delete popUps[id];
        popUps.erase(popUps.find(id));
    }
    void PopUps::update (){
        //temporary
        playerId = "player_one";
        //
        for (auto popUp : popUps) {
            deletePopUp(popUp.first);
        }
        json messages = state->fetchPlayerMessagesData(playerId);
        for (json::iterator message = messages.begin(); message != messages.end(); ++message) {
            newPopUp(message.value()["requireAnswer"], message.value()["type"], message.value()["sourceCharacter"], playerId, message.value()["id"]);
        }

    }
    void PopUps::draw (){
        for (auto it = popUps.rbegin(); it != popUps.rend(); ++it){
            it->second->draw();
        }
    }
    
    bool PopUps::contains(int x, int y){
        for (auto p : popUps) {
            if(p.second->contains(x,y)){
                return true;
            }
        }
        return false;  
    }
    
    void PopUps::click(int x, int y){
        for (auto p : popUps) {
            if (p.second->contains(x,y)) {
                if(p.second->click(x,y)){
                    deletePopUp(p.first);
                }
                break;
            }
        }
    }

    void PopUps::move(int x, int y){
        for(auto p: popUps) {
            if (p.second->isSelected()) {
                p.second->move(x,y);
                break;
            }
        }
    }

    void PopUps::release(){
        for(auto p: popUps) {
            p.second->deselect();
        }
    }
}