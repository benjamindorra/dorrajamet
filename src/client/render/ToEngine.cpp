/***************************
ToEngine.cpp
Send command to the engine
****************************/
#include "ToEngine.h"
#include <iostream>

namespace render {
    ToEngine::ToEngine (){}
    ToEngine::ToEngine (engine::EngineCore * engine){
        this->engine = engine;
    }
    ToEngine::~ToEngine (){}
    void ToEngine::addCommand (std::string command, std::string argument){
        std::cout<<"command "<<command<<" argument "<<argument<<std::endl;
        if (command=="moveArmy") {
            engine->pushCommand(engine::Command(engine::Command::army, argument));
        }
        if (command=="TurnButton") {
            engine->pushCommand(engine::Command(engine::Command::turn, ""));
        }
        if (command=="Claim") {
            engine->pushCommand(engine::Command(engine::Command::claim, argument));
        }
       //TODO for PopUps
        /*else {
            engine->pushCommand(engine::Command(engine::Command::ActionButton, argument));
        }*/

    }
}