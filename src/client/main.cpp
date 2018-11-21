#include <iostream>
#include <fstream>
#include <string>
#include "../shared/json.hpp"
#include "../shared/tools.hpp"
#include "../shared/state.h"
#include "render.h"
#include <SFML/Graphics.hpp>

void debug(int c)
{
    std::cout << "Jusqu'ici tout va bien : " << c << std::endl;
}

void testRender(){
    // create the window
    render::Render render1(1280,720);
    // start the renderloop
    render1.render::Render::renderLoop();
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
        else if(argv[1] == std::string("render"))
        {
            testRender();
        }
        else
            std::cout << "Unknown parameter: " << argv[1] << std::endl;
    }
    else
        std::cout << "Please type: \"client state\"\n";

    
    return 0;
}