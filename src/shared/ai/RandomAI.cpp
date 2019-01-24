#include "RandomAI.h"
#include "json.hpp"
#include <random>
#include <iostream>

namespace ai {
    RandomAI::RandomAI (state::GameState * state, engine::EngineCore * engine){
        this->gameState = state;
        this->engineCore = engine;
    }
    RandomAI::~RandomAI(){}
    void RandomAI::randomActions (){
        using json = nlohmann::json;
        int randInt;
        int posList;
        std::string command;
        std::string argument;
        json j;
        json jarg;
        std::string playerId = gameState->getCurrentPlayer();
        do {
            std::random_device rd;
            std::mt19937 mt(rd());
            std::uniform_int_distribution<int> dist2(0,1);
            randInt = dist2(mt);
            //answer all messages randomly
            json playerMessages = gameState->fetchPlayerMessages(playerId);
            for(json::iterator pm=playerMessages.begin(); pm != playerMessages.end(); pm++) {
                if (pm.value()["requireAnswer"]){
                    json j;
                    j["messageId"] = pm.value()["id"];
                    j["causeId"] = pm.value()["sourceCharacter"];
                    if(randInt==0)
                        engineCore->processCommand(engine::Command(engine::Command::yes, j.dump()));
                    else
                        engineCore->processCommand(engine::Command(engine::Command::no, j.dump()));
                }
                else{
                    json j;
                    j["messageId"] = pm.value()["id"];
                    engineCore->processCommand(engine::Command(engine::Command::ok, j.dump()));
                }
            }

            argument = "{";
            jarg.clear();
            std::uniform_int_distribution<int> dist(0,6);
            randInt = dist(mt);
            switch (randInt) {
                case 0:
                    //end ai turn
                    command = "TurnButton";
                    engineCore->processCommand(engine::Command(engine::Command::turn, ""));
                    break;
                case 1: {
                    command = "moveArmy";
                    j = gameState->fetchAllArmiesData();
                    if (j.size()>0){
                        std::uniform_int_distribution<int> dist2(0,j.size()-1);
                        posList = dist2(mt);
                        jarg["id"] = j[posList]["id"];

                        j = gameState->fetchAllProvincesData();
                        std::uniform_int_distribution<int> dist3(0,j.size()-1);
                        posList = dist3(mt);
                        jarg["dest"] = j[posList]["id"];
                        if(jarg["colorCode"] != 255)
                            engineCore->processCommand(engine::Command(engine::Command::army, jarg.dump()));
                    }
                    break;
                }
                case 2: {
                    //claim
                    j = gameState->fetchAllProvincesData();
                    std::uniform_int_distribution<int> dist2(0,j.size()-1);
                    posList = dist2(mt);
                    jarg["colorCode"] = j[posList]["colorCode"];
                    if(jarg["colorCode"] != 255)
                        engineCore->processCommand(engine::Command(engine::Command::claim, jarg.dump()));
                    break;
                }
                case 3: {
                    //war
                    j = gameState->fetchAllProvincesData();
                    std::uniform_int_distribution<int> dist2(0,j.size()-1);
                    posList = dist2(mt);
                    jarg["colorCode"] = j[posList]["colorCode"];
                    if(jarg["colorCode"] != 255)
                        engineCore->processCommand(engine::Command(engine::Command::war, jarg.dump()));
                    break;
                }
                case 4: {
                    //ask to surrender
                    j = gameState->fetchAllProvincesData();
                    std::uniform_int_distribution<int> dist2(0,j.size()-1);
                    posList = dist2(mt);
                    jarg["colorCode"] = j[posList]["colorCode"];
                    if(jarg["colorCode"] != 255)
                        engineCore->processCommand(engine::Command(engine::Command::peace, jarg.dump()));
                    break;
                }
                case 5: {
                    //surrender
                    j = gameState->fetchAllProvincesData();
                    std::uniform_int_distribution<int> dist2(0,j.size()-1);
                    posList = dist2(mt);
                    jarg["colorCode"] = j[posList]["colorCode"];
                    if(jarg["colorCode"] != 255)
                        engineCore->processCommand(engine::Command(engine::Command::surr, jarg.dump()));
                    break;
                }
                case 6: {
                    //create army
                    j = gameState->fetchAllProvincesData();
                    std::uniform_int_distribution<int> dist2(0,j.size()-1);
                    posList = dist2(mt);
                    jarg["colorCode"] = j[posList]["colorCode"];
                    if(jarg["colorCode"] != 255)
                    engineCore->processCommand(engine::Command(engine::Command::createArmy, jarg.dump()));
                    break;
                }
                default :
                    throw std::string ("Illegal decision from ai");
            }
        }
        while(command != "TurnButton");
    }
}