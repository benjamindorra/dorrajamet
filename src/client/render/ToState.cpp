/***************************
ToState.cpp
Get data from the game state
****************************/
#include "ToState.h"
#include <json.hpp>

#include <iostream>

using json = nlohmann::json;

namespace render {
    ToState::ToState () {}
    ToState::ToState (state::GameState * state) {
        this->state=state;
        try {
            json j = json::parse(fetchAllProvincesData());
            for(json::iterator it = j.begin(); it!= j.end(); ++it) {
                this->mapColorId[it.value()["colorCode"].dump()] = it.value()["id"].get<std::string>();
            }
        }
        catch(const std::exception& e) {
            std::cerr << e.what() <<std::endl;
            throw std::runtime_error("Incorrect data for Provinces");
        }
    }
    ToState::~ToState () {}
    std::string ToState::fetchCharacterData (std::string id) {
        /*json j = json::parse(R"({
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
        return j.dump();*/

        return state->fetchCharacterData(id);
        
    }
    std::string ToState::fetchProvinceData (std::string colorCode){
        /*json j = json::parse(R"({"id": "prov_one",
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
        return j.dump();*/
        
        return state->fetchProvinceData(mapColorId[colorCode]);
    }

    std::string ToState::fetchAllProvincesData (){
        /*json j = json::parse(R"({"id": "prov_one",
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
        return j.dump();*/
        
        return state->fetchAllProvincesData();
    }
    std::string ToState::fetchArmyData (std::string id){
        /*json j = json::parse(R"({
                "id": "army_chara0001_xx",
                "ownerCharacter": "chara_0001",
                "levies":
                [
                    "prov_one"
                ],
                "currentProvince": "prov_one",
                "currentBattle": "",
                "orders":
                []
            })");
        return j.dump();*/

        return state->fetchArmyData(id);
    }
    std::string ToState::fetchAllArmiesData (){
        /*json j = json::parse(R"({
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
        return j.dump();*/

        return state->fetchAllArmiesData();
    }
    std::string ToState::fetchAllRelationsData (){
        /*json j = json::parse(R"([
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
        return j.dump();*/
        
        return state->fetchAllRelationsData();
    }
    std::string ToState::fetchAllPlayersData(){
        /*json j = json::parse(R"({
            "id": "player_one",
            "currentCharacter": "chara_0001",
            "score": 0
        })");
        return j.dump();*/

        return state->fetchAllPlayersData();
    }
    std::string ToState::fetchCharacterDataFromColor(std::string colorCode) {
        try {
            json j = json::parse(fetchProvinceData(colorCode));
            return fetchCharacterData(state->getProvinceOwner(j["id"]));
        }
        catch(const std::exception& e) {
            std::cerr << e.what() <<std::endl;
            throw std::runtime_error("No character for color " + colorCode);
        }
    }
}