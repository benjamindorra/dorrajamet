/***************************
ToState.cpp
Get data from the game state
****************************/
#include "ToState.h"
#include <json.hpp>

using json = nlohmann::json;

namespace render {
    ToState::ToState () {}
    ToState::ToState (state::GameState * state) {
        this->state=state;
    }
    ToState::~ToState () {}
    std::string ToState::getCharacter (std::string id) {
        json j = json::parse(R"({
                    "id": "chara_0001",
                    "name": "Rayleigh",
                    "dynastyName": "Jones",
                    "age": 22,
                    "traits": ["proud", "patient"],
                    "diplomacy": 12,
                    "stewardship": 10,
                    "martial": 21,
                    "intrigue": 15,
                    "alive": true,
                    "prestige": 200,
                    "gold": 500,
                    "hasPlot": false
                })");
        return j.dump();
    }
    std::string ToState::getProvince (std::string idColor){
        json j = json::parse(R"({"id": "prov_one",
                "name": "Oneland",
                "colorCode": 123456,
                "development": 50,
                "prosperity": 12,
                "baseLevy":
                {
                    "men": 500,
                    "isRaised": false,
                    "reinforcementRate": 1
                },
                "levy":
                {
                    "men": 500,
                    "isRaised": true,
                    "reinforcementRate": 1 
                },
                "baseTaxIncome": 200,
                "taxIncome": 200})");
        return j.dump();
    }
    std::string ToState::getArmy (std::string id){
        json j = json::parse(R"({
                "id": "army_1", 
                "ownerCharacter" : "chara_0001",

                "levies":
                [
                    "prov_one"
                ],
                "currentProvince": "prov_one",
                "currentBattle": "",
                "jOrders":
                []
            })");
        return j.dump();
    }
    std::string ToState::getArmies (){
        json j = json::parse(R"({
                "id": "army_1", 
                "ownerCharacter" : "chara_0001",

                "levies":
                [
                    "prov_one"
                ],
                "currentProvince": "prov_one",
                "currentBattle": "",
                "jOrders":
                []
            })");
        return j.dump();
    }
    std::string ToState::getRelations (std::string id){
        json j = json::parse(R"([
            {
                "type": 2,
                "characterA": "chara_0001",
                "characterB": "chara_0004",
                "endTurn": -1
            },
            {
                "type": 3,
                "characterA": "chara_0001",
                "characterB": "chara_0002",
                "endTurn": -1
            }
        ])");
        return j.dump();
    }
    std::string ToState::getPlayer(std::string id){
        json j = json::parse(R"({
            "id": "player_one",
            "currentCharacter": "chara_0001",
            "score": 0
        })");
        return j.dump();
    }
}