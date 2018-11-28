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

void testRender(state::GameState testState, engine::EngineCore testEngine){
    // create the window
    render::Render render1(1280,720);
    //init the render
    render1.init(&testState, &testEngine);
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
            //testGameState.debug();

            // Test of gamestate fetch data
            //std::cout << testGameState.fetchAllCharactersData().dump(4);
            std::cout << testGameState.fetchProvinceData(123456).dump(4);
            std::cout << testGameState.fetchProvinceOwnerData(456123).dump(4);
        }
        else if(command == "render")
        {
            state::GameState testState("./res/testGameState.json");
            engine::EngineCore testEngine(&testState);
            testRender(testState, testEngine);
        }
        else if(command == "engine")
        {
            state::GameState testState("./res/testGameState.json");
            engine::EngineCore testEngine(&testState);
        }
        else
            std::cout << "Unknown parameter: " << command << std::endl;
    }
    else
        std::cout << "Please type: \"client state/render/engine\"\n";

    
    return 0;
}