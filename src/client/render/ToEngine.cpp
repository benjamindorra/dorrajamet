/***************************
ToEngine.cpp
Send command to the engine
****************************/
#include "ToEngine.h"
#include <iostream>

namespace render {
    ToEngine::ToEngine (){}
    ToEngine::ToEngine (std::queue<std::string> * commands){
        this->commands = commands;
    }
    ToEngine::~ToEngine (){}
    void ToEngine::addCommand (std::string idTarget, std::string command){
        std::cout<<"id "<<idTarget<<"command "<<command<<std::endl;
    }
}