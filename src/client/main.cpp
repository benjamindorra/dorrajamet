#include <iostream>
#include <fstream>
#include <string>
#include "../shared/json.hpp"
#include "../shared/tools.hpp"
#include "../shared/state.h"

void debug(int c)
{
    std::cout << "Jusqu'ici tout va bien : " << c << std::endl;
}

int main(int argc, char ** argv)
{
    if(argc == 2)
    {
        if(argv[1] == std::string("state"))
        {
            state::GameState testGameState("./res/testGameState.json");
            testGameState.debug();
        }
        else
            std::cout << "Unknown parameter: " << argv[1] << std::endl;
    }
    else
        std::cout << "Please type: \"client state\"\n";

    
    return 0;
}