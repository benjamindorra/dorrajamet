#include "EngineCore.h"
#include <iostream>
#include <json.hpp>

namespace engine
{
    EngineCore::EngineCore ()
    {

    }
    EngineCore::EngineCore (state::GameState gameState)
    {

    }
    EngineCore::EngineCore (state::GameState gameState, std::queue<Command> commands)
    {
        this->gameState = gameState;
        this->commands = commands;
    }
    EngineCore::~EngineCore ()
    {

    }
    int EngineCore::mainLoop ()
    {
        while(true)
        {
            processCommand(commands.front());
            commands.pop();
            
        }
        return 0;
    }
    void EngineCore::pushCommand (Command command)
    {
        commands.push(command);
    }
    void EngineCore::processCommand(Command command)
    {
        switch(command.getType())
        {
            case Command::moveArmy:
                using json = nlohmann::json;
                try
                {
                    json j = json::parse(command.getArgument());
                    auto id = j["id"].get<std::string>();
                    auto dest = j["dest"].get<std::string>();
                    gameState.setArmyOrder(id, dest);
                }
                catch(const std::exception& e)
                {
                    std::cerr << e.what() << std::endl;
                    throw std::runtime_error("Error: could not parse command arguments\n");
                }
                break;
            case Command::ActionButton:

                break;
            case Command::TurnButton:
                if(gameState.turnAdvance())
                    endTurn();
                break;
            default:
                throw std::runtime_error("Error: unknown command type.\n");
        }
    }
    void EngineCore::turnButton()
    {

    }
    void EngineCore::endTurn()
    {
        gameState.updateArmiesOrders();
        gameState.checkNewBattles();
        gameState.updateBattles();
        gameState.clearFinishedBattles();
        gameState.clearDeadArmies();
        gameState.updateLevies();
        //gameState.updateValues();
    }
}