#include <iostream>
#include <fstream>
#include <string>
#include <json.hpp>
#include <state.h>
#include <engine.h>
#include <render.h>
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
        std::string command = argv[1];
        if(command == "state")
        {
            state::GameState testGameState("./res/testGameState.json");
            testGameState.debug();
        }
        else if(command == "render")
        {
            testRender();
        }
        else if(command == "engine")
        {
            state::GameState testState("./res/testGameState.json");

        }
        else
            std::cout << "Unknown parameter: " << command << std::endl;
    }
    else
        std::cout << "Please type: \"client state/render/engine\"\n";

    
    return 0;
}