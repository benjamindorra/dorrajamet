#include <iostream>
#include <fstream>
#include <string>
#include <json.hpp>
#include <state.h>
#include <engine.h>
#include <render.h>
#include <ai.h>
#include <SFML/Graphics.hpp>

void debug(int c)
{
    std::cout << "Jusqu'ici tout va bien : " << c << std::endl;
}

void testRender(state::GameState * testState, engine::EngineCore * testEngine){
    // create the window
    render::Render render1(1280,720);
    //init the render
    render1.init(testState, testEngine);
    while (render1.getWindow()->isOpen())
    {
        // start the renderloop
        render1.renderLoop();
        testEngine->mainLoop();
    }
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

            // Test of gamestate fetch data
            
        }
        else if(command == "render")
        {
            state::GameState testState("./res/testGameState.json");
            std::queue<engine::Command> commands;
            engine::EngineCore testEngine(&testState, &commands);
            testRender(&testState, &testEngine);
         
        }
        else if(command == "engine")
        {
            state::GameState testState("./res/testGameState.json");
            engine::EngineCore testEngine(&testState);
            std::cout << testState.fetchArmyData("army_chara0001_xx").dump(2) << std::endl;
            
            //std::cout << testState.fetchAllProvincesTopLiegeColor().dump(2);
            std::cout << "\n\n\n";
            testState.setArmyOrder("army_chara0001_xx", "prov_three");
            std::cout << testState.fetchArmyData("army_chara0001_xx").dump(2) << std::endl;
            std::cout << "\n\n\n";
            
            if(testState.turnAdvance()) std::cout << "true\n";
            if(testState.turnAdvance()) std::cout << "true\n";
            if(testState.turnAdvance()) std::cout << "true\n";
            if(testState.turnAdvance()) std::cout << "true\n";
            std::cout << testState.fetchArmyData("army_chara0001_xx").dump(2) << std::endl;

            //std::cout << testState.fetchAllProvincesTopLiegeColor().dump(2);

        }
        else if(command == "ai")
        {
            state::GameState testState("./res/testGameState.json");
            std::queue<engine::Command> commands;
            engine::EngineCore testEngine(&testState, &commands);
            ai::RandomAI randomAI(&testState, &testEngine);
            randomAI.randomActions();
            std::cout<<"AI OK"<<std::endl;

            //std::cout << testState.fetchAllProvincesTopLiegeColor().dump(2);

        }
        else
            std::cout << "Unknown parameter: " << command << std::endl;
    }
    else
        std::cout << "Please type: \"client state/render/engine\"\n";

    
    return 0;
}