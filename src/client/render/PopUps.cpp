#include "PopUps.h"
#include <json.hpp>
#include <iostream>

using json = nlohmann::json;

namespace render {
    PopUps::PopUps (Render * mainRender, ToState * state, ToEngine * engine, std::string playerId){
        this->mainRender=mainRender;
        this->state = state;
        this->engine = engine;
        this->playerId = playerId;
    }
    PopUps::~PopUps ( ){}
    void PopUps::newPopUp (bool requireAnswer, PopUp::Action action, std::string causeId, std::string targetId, std::string id, std::string data){
        if (requireAnswer) {
            popUps[id] = new PopUp(mainRender, PopUp::Type::Question, action, causeId, targetId, state, engine, data, id);
        }
        else {
            popUps[id] = new PopUp(mainRender, PopUp::Type::Info, action, causeId, targetId, state, engine, data, id);
        }
    }
    void PopUps::deletePopUp (std::string id){
        delete popUps[id];
        popUps.erase(popUps.find(id));
    }
    void PopUps::update (){
        //std::cout << "Popups update\n";
        playerId = state->getCurrentPlayer();
        
        json playerMessages = state->fetchPlayerMessagesData(playerId);
        if(playerMessages.size())
            std::cout << "found " << playerMessages.size();
        bool alreadyExist;
        /*if(playerMessages.size())
            std::cout << "found " << playerMessages.size();*/
        for (json::iterator playerMessage = playerMessages.begin(); playerMessage != playerMessages.end(); ++playerMessage) {
            alreadyExist=false;
            for (auto popUp : popUps) {
                if(popUp.first==playerMessage.value()["id"]) alreadyExist=true;
            }
            if (not alreadyExist){
                newPopUp(playerMessage.value()["requireAnswer"], playerMessage.value()["type"], playerMessage.value()["sourceCharacter"], playerId, playerMessage.value()["id"], playerMessage.value()["data"]);
            }
        }
        bool alreadyDeleted;
        for (auto popUp : popUps) {
            alreadyDeleted=true;
            for (json::iterator playerMessage = playerMessages.begin(); playerMessage != playerMessages.end(); ++playerMessage) {
                if(popUp.first==playerMessage.value()["id"]) alreadyDeleted=false;
            }
            if (alreadyDeleted){
                deletePopUp(popUp.first);
            }
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