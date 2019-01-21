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
        if (command=="moveArmy") {
            engine->pushCommand(engine::Command(engine::Command::army, argument));
        }
        if (command=="TurnButton") {
            engine->pushCommand(engine::Command(engine::Command::turn, ""));
        }
        if (command=="Claim") {
            engine->pushCommand(engine::Command(engine::Command::claim, argument));
        }
        if (command=="OK")
        {
            engine->pushCommand(engine::Command(engine::Command::ok, argument));
        }
        if (command=="ACCEPT")
        {
            engine->pushCommand(engine::Command(engine::Command::yes, argument));
        }
        if (command=="REFUSE")
        {
            engine->pushCommand(engine::Command(engine::Command::no, argument));
        }
        if (command=="War")
        {
            engine->pushCommand(engine::Command(engine::Command::war, argument));
        }
        if (command=="Peace")
        {
            engine->pushCommand(engine::Command(engine::Command::peace, argument));
        }
        if (command=="Surrender")
        {
            engine->pushCommand(engine::Command(engine::Command::surr, argument));
        }
        if (command=="Levy")
        {
            engine->pushCommand(engine::Command(engine::Command::createArmy, argument));
        }
    }
}