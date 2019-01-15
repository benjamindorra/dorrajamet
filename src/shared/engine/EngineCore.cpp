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
        auto currentCharacter = gameState->getCurrentPlayerCharacter();
        auto currentPlayer = gameState->getCurrentPlayer();
        auto currentTurn = gameState->getCurrentTurn();
        nlohmann::json j;
        switch(command.getType())
        {
            case Command::army:
                gameState->debug();
                try
                {
                    j = nlohmann::json::parse(command.getArgument());
                    auto id = j["id"].get<std::string>();
                    auto dest = j["dest"].get<std::string>();
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
                    throw std::runtime_error("Error: could not parse move army command arguments\n");
                }
                gameState->debug();
                break;
            case Command::claim:
                try
                {
                    j = nlohmann::json::parse(command.getArgument());
                    auto targetProvColor = j["colorCode"].get<unsigned int>();
                    auto targetProv = gameState->getProvinceFromColor(targetProvColor);
                    auto targetOwner = gameState->getProvinceOwner(targetProv);
                    auto targetPlayer = gameState->getPlayerOfCharacter(targetOwner);
                    if(targetOwner != currentCharacter)
                    {
                        gameState->addClaim(currentCharacter, targetProv);
                        std::cout << "added claim\n";
                        nlohmann::json mess;
                        mess["id"] = currentCharacter + "_claim_" + targetProv + "_from_" + targetOwner + "_turn_" + std::to_string(currentTurn);
                        mess["type"] = 3;
                        mess["sourceCharacter"] = currentCharacter;
                        mess["requiresAnswer"] = false;
                        mess["data"] = targetProv;
                        gameState->pushMessageToPlayer(targetPlayer, mess);
                    }
                    else
                        std::cout << "can't claim your own provinces\n";
                }
                catch(const std::exception& e)
                {
                    std::cerr << e.what() << std::endl;
                    throw std::runtime_error("Error: could not parse claim command arguments\n");
                }
                break;
            case Command::turn:
                turnButton();
                break;
            case Command::war:
                try
                {
                    j = nlohmann::json::parse(command.getArgument());
                    auto targetOwner = gameState->getProvinceOwner(gameState->getProvinceFromColor(j["colorCode"].get<unsigned int>()));
                    if(targetOwner == currentCharacter)
                        std::cout << "Can't declare war on yourself you madman!\n";
                    else if(gameState->areAllies(targetOwner, currentCharacter))
                        std::cout << "Can't declare ass on allies you jerk!\n";
                    else
                    {
                        auto claim = gameState->hasClaim(currentCharacter, targetOwner);
                        if(claim.size())
                        {
                            auto warId = gameState->declareWar(claim, currentCharacter, targetOwner);
                            auto camps = gameState->getWarCamps(warId);
                            std::cout << "attackers: \n";
                            for(auto character: camps.first)//attacking camp
                            {
                                std::cout << character << std::endl;
                                if(character == currentCharacter)
                                    continue;
                                nlohmann::json mess;
                                mess["id"] = currentCharacter + "_atk_war:" + warId + "_on_" + targetOwner + "_turn_" + std::to_string(currentTurn);
                                mess["type"] = 0;
                                mess["sourceCharacter"] = currentCharacter;
                                mess["requiresAnswer"] = false;
                                mess["data"] = warId;
                                gameState->pushMessageToPlayer(gameState->getPlayerOfCharacter(character), mess);
                            }
                            std::cout << "defenders: \n";
                            for(auto character: camps.second)//defending camp
                            {
                                std::cout << character << std::endl;
                                nlohmann::json mess;
                                mess["id"] = currentCharacter + "_def_war:" + warId + "_on_" + targetOwner + "_turn_" + std::to_string(currentTurn);
                                mess["type"] = 0;
                                mess["sourceCharacter"] = currentCharacter;
                                mess["requiresAnswer"] = false;
                                mess["data"] = warId;
                                gameState->pushMessageToPlayer(gameState->getPlayerOfCharacter(character), mess);
                            }
                            
                        }
                        else
                            std::cout << "You have no claim over this character\n";
                    }
                }
                catch(const std::exception& e)
                {
                    std::cerr << e.what() << std::endl;
                    throw std::runtime_error("Error: could not parse War declaration command arguments\n");
                }
                break;
            case Command::ok:
                try
                {
                    j = nlohmann::json::parse(command.getArgument());
                    auto messageId = j["messageId"];
                    gameState->removeMessage(currentPlayer, messageId);
                }
                catch(const std::exception& e)
                {
                    std::cerr << e.what() << std::endl;
                    throw std::runtime_error("Error: could not parse ok command\n");
                }
                break;
            case Command::yes:
                try
                {
                    j = nlohmann::json::parse(command.getArgument());
                    auto messageId = j["messageId"];

                    // TODO: process message

                    gameState->removeMessage(currentPlayer, messageId);
                }
                catch(const std::exception& e)
                {
                    std::cerr << e.what() << std::endl;
                    throw std::runtime_error("Error: could not parse yes command\n");
                }
                break;
            case Command::no:
                try
                {
                    j = nlohmann::json::parse(command.getArgument());
                    auto messageId = j["messageId"];

                    // TODO: process message

                    gameState->removeMessage(currentPlayer, messageId);
                }
                catch(const std::exception& e)
                {
                    std::cerr << e.what() << std::endl;
                    throw std::runtime_error("Error: could not parse no command\n");
                }
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