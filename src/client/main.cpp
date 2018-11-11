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
    //create the map image
    render::Image map(400, 400);
    //import the image
    map.render::Image::importFile("./res/provinces.bmp");
    //create the viewmap
    render::ViewMap viewmap(&render1, map, sf::Vector2f(224.f, 162.f), sf::Vector2f(200.f,200.f)); 
    //tell render to draw the viewmap
    render1.render::Render::loadMap(&viewmap);
    //create the colormap
    render::ColorMap colormap("./res/provinces.bmp");
    //give render access to the colormap
    render1.loadColormap(&colormap);
    //start the renderloop
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