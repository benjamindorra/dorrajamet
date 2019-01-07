#include "RandomAI.h"
#include "json.hpp"
#include <random>
#include <iostream>

namespace ai {
    RandomAI::RandomAI (state::GameState * state, engine::EngineCore * engine){
        this->toState = render::ToState(state);
        this->toEngine = render::ToEngine(engine);
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
        do {
            std::random_device rd;
            std::mt19937 mt(rd());
            argument = "{";
            jarg.clear();
            std::uniform_int_distribution<int> dist(0,2);
            randInt = dist(mt);
            switch (randInt) {
                case 0:
                    command = "TurnButton";
                    toEngine.addCommand(command, "");
                    break;
                case 1: {
                    command = "moveArmy";
                    j = toState.fetchAllArmiesData();
                    std::uniform_int_distribution<int> dist2(0,j.size()-1);
                    posList = dist2(mt);
                    jarg["id"] = j[posList]["id"];

                    j = toState.fetchAllProvincesData();
                    std::uniform_int_distribution<int> dist3(0,j.size()-1);
                    posList = dist3(mt);
                    jarg["dest"] = j[posList]["id"];

                    toEngine.addCommand("moveArmy", jarg.dump());
                    break;
                }
                case 2: {
                    command = "actionButton";
                    std::string commands[6] = {"Alliance", "Murder", "War", "Peace", "Levy", "Claim"};
                    std::uniform_int_distribution<int> dist2(0,5);
                    posList = dist2(mt);
                    jarg["command"] = commands[posList];

                    j = toState.fetchAllProvincesData();
                    std::uniform_int_distribution<int> dist3(0,j.size()-1);
                    posList = dist3(mt);
                    jarg["colorCode"] = j[posList]["colorCode"];
                    toEngine.addCommand("actionButton", jarg.dump());
                    break;
                }
                default :
                    throw std::string ("Illegal decision from ai");
            }
        }
        while(command != "TurnButton");
    }
}