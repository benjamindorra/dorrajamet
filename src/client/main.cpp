#include <iostream>
#include <fstream>
#include <string>
#include <json.hpp>
#include <state.h>
#include <engine.h>
#include <render.h>
#include <ai.h>
#include <SFML/Graphics.hpp>

std::string savePath = "./res/savedGameState.json";

void debug(int c)
{
    std::cout << "Jusqu'ici tout va bien : " << c << std::endl;
}

void testRender(state::GameState * testState, engine::EngineCore * testEngine){
    // create the window
    render::Render render1(1280,720);
    //init the render
    render1.init(testState, testEngine);
    std::cout<<"Render init OK"<<std::endl;
    while (render1.getWindow()->isOpen())
    {
        // start the renderloop
        render1.renderLoop();
        testEngine->mainLoop();
    }
    std::cout<<"Render OK"<<std::endl;
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
            std::cout << "State OK" << std::endl;
            // Test of gamestate fetch data
            
        }
        else if(command == "render")
        {
            state::GameState testState("./res/testGameState.json");
            std::queue<engine::Command> commands;
            engine::EngineCore testEngine(&testState, &commands);
            testRender(&testState, &testEngine);
            testState.save(savePath);
         
        }
        else if(command == "engine")
        {
            state::GameState testState("./res/testGameState.json");
            engine::EngineCore testEngine(&testState);
            nlohmann::json arguments;
            arguments["colorCode"] = 16777215;
            testEngine.claim(arguments.dump());
            testEngine.turnButton();
            testEngine.turnButton();
            testEngine.turnButton();
            testEngine.turnButton();
            testEngine.war(arguments.dump());
            testEngine.turnButton();
            testEngine.turnButton();
            testEngine.turnButton();
            testEngine.turnButton();
            arguments["colorCode"] = 65535;
            testEngine.createArmy(arguments.dump());
            testState.save(savePath);
            std::cout << testState.fetchArmyData("army_chara_0001_from_prov_one").dump(2) << std::endl;
            
            //std::cout << testState.fetchAllProvincesTopLiegeColor().dump(2);
            std::cout << "\n\n\n";
            testState.setArmyOrder("army_chara_0001_from_prov_one", "prov_five");
            std::cout << testState.fetchArmyData("army_chara_0001_from_prov_one").dump(2) << std::endl;
            std::cout << "\n\n\n";
            
            testEngine.turnButton();
            testEngine.turnButton();
            testEngine.turnButton();
            testEngine.turnButton();
            std::cout << testState.fetchArmyData("army_chara_0001_from_prov_one").dump(2) << std::endl;
            testEngine.turnButton();
            testEngine.turnButton();
            testEngine.turnButton();
            testEngine.turnButton();
            std::cout << testState.fetchArmyData("army_chara_0001_from_prov_one").dump(2) << std::endl;
            testState.setArmyOrder("army_chara_0001_from_prov_one", "prov_two");
            testEngine.turnButton();
            testEngine.turnButton();
            testEngine.turnButton();
            testEngine.turnButton();
            testEngine.turnButton();
            testEngine.turnButton();
            testEngine.turnButton();
            testEngine.turnButton();
            testEngine.turnButton();
            testEngine.turnButton();
            testEngine.turnButton();
            testEngine.turnButton();
            testEngine.turnButton();
            testEngine.turnButton();
            testEngine.turnButton();
            testEngine.turnButton();

            std::cout << testState.fetchArmyData("army_chara_0001_from_prov_one").dump(2) << std::endl;

            std::cout << "Engine OK" << std::endl;
            //std::cout << testState.fetchAllProvincesTopLiegeColor().dump(2);
        }
        else if(command == "random_ai")
        {
            state::GameState testState("./res/testGameState.json");
            std::queue<engine::Command> commands;
            engine::EngineCore testEngine(&testState, &commands);
            ai::RandomAI randomAI(&testState, &testEngine);
            randomAI.randomActions();
            std::cout<<"AI OK"<<std::endl;
            //std::cout << testState.fetchAllProvincesTopLiegeColor().dump(2);
            testState.save(savePath);
        }
        else if(command == "heuristic_ai") {
            state::GameState testState("./res/testGameState.json");
            engine::EngineCore testEngine(&testState);
            //ia
            ai::BasicAI basicAI(&testState, &testEngine);
            basicAI.main();
            std::cout << "Heuristic AI OK" << std::endl;
        }
        else if(command == "deep_ai"){
            state::GameState testState("./res/testGameState.json");
            std::queue<engine::Command> commands;
            engine::EngineCore testEngine(&testState, &commands);
            ai::DeepAI deepAI(&testState, &testEngine);
            deepAI.main(2);
            std::cout << "Deep AI OK" << std::endl;
            /*render::Render render1(1280,720);
            //init the render
            render1.init(&testState, &testEngine);
            while (render1.getWindow()->isOpen())
            {
                // start the renderloop
                render1.renderLoop();
                testEngine.mainLoop();
            }*/
        }
        else if(command == "load"){
            state::GameState testState(savePath);
            std::queue<engine::Command> commands;
            engine::EngineCore testEngine(&testState, &commands);
            render::Render render1(1280,720);
            //init the render
            render1.init(&testState, &testEngine);
            while (render1.getWindow()->isOpen())
            {
                // start the renderloop
                render1.renderLoop();
                testEngine.mainLoop();
            }
            testState.save(savePath);
            std::cout << "Load OK" << std::endl;
        }
        else
            std::cout << "Unknown parameter: " << command << std::endl;
    }
    else
        std::cout << "Please type: \"client state/render/engine\"\n";

        
    return 0;
}