#include "EngineCore.h"
#include <iostream>
#include <json.hpp>

namespace engine
{
    EngineCore::EngineCore ()
    {

    }
    EngineCore::EngineCore (state::GameState * gameState)
    {
        this->gameState = gameState;
    }
    EngineCore::EngineCore (state::GameState * gameState, std::queue<Command> * commands)
    {
        this->gameState = gameState;
        this->commands = commands;
    }
    EngineCore::~EngineCore ()
    {

    }
    int EngineCore::mainLoop ()
    {
        while(!commands->empty())
        {
            processCommand(commands->front());
            commands->pop();
        }
        return 0;
    }
    void EngineCore::pushCommand (Command command)
    {
        commands->push(command);
    }
    void EngineCore::processCommand(Command command)
    {
        switch(command.getType())
        {
            case Command::moveArmy:
                gameState->debug();
                using json = nlohmann::json;
                try
                {
                    json j = json::parse(command.getArgument());
                    auto id = j["id"].get<std::string>();
                    auto dest = j["dest"].get<std::string>();
                    auto currentCharacter = gameState->getCurrentPlayerCharacter();
                    auto armyOwner = gameState->getArmyOwner(id);
                    if(currentCharacter == armyOwner)
                    {
                        gameState->setArmyOrder(id, dest);
                        std::cout<<"Orders given to army"<<std::endl;
                    }
                    else
                        std::cout << "Can't give the order\n";
                }
                catch(const std::exception& e)
                {
                    std::cerr << e.what() << std::endl;
                    throw std::runtime_error("Error: could not parse command arguments\n");
                }
                gameState->debug();
                break;
            case Command::ActionButton:

                break;
            case Command::TurnButton:
                turnButton();
                break;
            default:
                throw std::runtime_error("Error: unknown command type.\n");
        }
    }
    void EngineCore::turnButton()
    {
        if(gameState->turnAdvance())
            endTurn();
    }
    void EngineCore::endTurn()
    {
        gameState->updateArmiesOrders();
        gameState->checkNewBattles();
        gameState->updateBattles();
        gameState->clearFinishedBattles();
        gameState->clearDeadArmies();
        gameState->updateProvinces();
        gameState->updateLevies();
        /*gameState->updateWars();
        gameState->updateCharactersData();*/
        std::cout << "End turn procedure finished\n";
        
    }
}