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
        std::string warId;
        std::string targetCharacter;
        nlohmann::json j;
        switch(command.getType())
        {
            case Command::army:
                moveArmy(command.getArgument());
                break;
            case Command::claim:
                claim(command.getArgument());
                break;
            case Command::turn:
                turnButton();
                break;
            case Command::war:
                war(command.getArgument());
                break;
            case Command::peace:
                peace(command.getArgument());
                break;
            case Command::surr:
                surrender(command.getArgument());
                break;
            case Command::ok:
                okButton(command.getArgument());
                break;
            case Command::yes:
                yesButton(command.getArgument());
                break;
            case Command::no:
                noButton(command.getArgument());
                break;
            case Command::createArmy:
                createArmy(command.getArgument());
                break;
            default:
                throw std::runtime_error("Error: unknown command type.\n");
        }
    }

    void EngineCore::endWar (std::string warId, std::string winner)
    {
        gameState->endWar(warId, winner);
    }
    void EngineCore::turnButton()
    {
        if(gameState->turnAdvance())
            endTurn();
    }
    void EngineCore::endTurn()
    {
        gameState->increaseCurrentTurn();
        gameState->updateArmiesOrders();
        gameState->checkNewBattles();
        gameState->updateBattles();
        gameState->clearFinishedBattles();
        gameState->clearDeadArmies();
        gameState->updateProvinces();
        gameState->updateLevies();
        gameState->updateWars();
        //gameState->updateCharactersData();
    }
    void EngineCore::moveArmy (std::string arguments)
    {
        auto currentCharacter = gameState->getCurrentPlayerCharacter();
        try
        {
            auto j = nlohmann::json::parse(arguments);
            auto id = j["id"].get<std::string>();
            auto dest = j["dest"].get<std::string>();
            auto armyOwner = gameState->getArmyOwner(id);
            if(currentCharacter == armyOwner)
            {
                gameState->setArmyOrder(id, dest);
            }
            else
                std::cout << "Can't give the order\n";
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << std::endl;
            throw std::runtime_error("Error: could not parse move army command arguments\n");
        }
    }
    void EngineCore::claim (std::string arguments)
    {
        auto currentCharacter = gameState->getCurrentPlayerCharacter();
        auto currentTurn = gameState->getCurrentTurn();
        try
        {
            auto j = nlohmann::json::parse(arguments);
            auto targetProvColor = j["colorCode"].get<unsigned int>();
            auto targetProv = gameState->getProvinceFromColor(targetProvColor);
            auto targetOwner = gameState->getProvinceOwner(targetProv);
            auto targetPlayer = gameState->getPlayerOfCharacter(targetOwner);
            if(targetOwner != currentCharacter)
            {
                //1 turn delay before a claim becomes active
                gameState->addClaim(currentCharacter, targetProv, currentTurn+1);
                nlohmann::json mess;
                mess["id"] = currentCharacter + "_claim_" + targetProv + "_from_" + targetOwner + "_turn_" + std::to_string(currentTurn);
                mess["type"] = 3;
                mess["sourceCharacter"] = currentCharacter;
                mess["requiresAnswer"] = false;
                mess["data"] = targetProv;
                gameState->pushMessageToPlayer(targetPlayer, mess);
            }
            else{
                nlohmann::json mess;
                mess["id"] = currentCharacter + "_other_" + "claimOwnProvince" + "_turn_" + std::to_string(currentTurn);
                mess["type"] = 6;
                mess["sourceCharacter"] = currentCharacter;
                mess["requiresAnswer"] = false;
                mess["data"] = "You can't claim your own provinces";
                gameState->pushMessageToPlayer(gameState->getCurrentPlayer(), mess);
            }
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << std::endl;
            throw std::runtime_error("Error: could not parse claim command arguments\n");
        }
    }
    void EngineCore::war (std::string arguments)
    {
        auto currentCharacter = gameState->getCurrentPlayerCharacter();
        auto currentTurn = gameState->getCurrentTurn();
        try
        {
            auto j = nlohmann::json::parse(arguments);
            auto targetOwner = gameState->getProvinceOwner(gameState->getProvinceFromColor(j["colorCode"].get<unsigned int>()));
            if(targetOwner == currentCharacter){
                nlohmann::json mess;
                mess["id"] = currentCharacter + "_other_" + "warOnYourself" + "_turn_" + std::to_string(currentTurn);
                mess["type"] = 6;
                mess["sourceCharacter"] = currentCharacter;
                mess["requiresAnswer"] = false;
                mess["data"] = "Can't declare war on yourself you madman!";
                gameState->pushMessageToPlayer(gameState->getCurrentPlayer(), mess);
            }
            else if(gameState->areAllies(targetOwner, currentCharacter)){
                nlohmann::json mess;
                mess["id"] = currentCharacter + "_other_" + "warOnAlly" + "_turn_" + std::to_string(currentTurn);
                mess["type"] = 6;
                mess["sourceCharacter"] = currentCharacter;
                mess["requiresAnswer"] = false;
                mess["data"] = "Can't declare war on allies you jerk!";
                gameState->pushMessageToPlayer(gameState->getCurrentPlayer(), mess);
            }
            else
            {
                auto claim = gameState->hasActiveClaim(currentCharacter, targetOwner);
                if(claim.size())
                {
                    auto warId = gameState->declareWar(claim, currentCharacter, targetOwner);
                    auto camps = gameState->getWarCamps(warId);
                    for(auto character: camps.first)//attacking camp
                    {
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
                    for(auto character: camps.second)//defending camp
                    {
                        nlohmann::json mess;
                        mess["id"] = currentCharacter + "_def_war:" + warId + "_on_" + targetOwner + "_turn_" + std::to_string(currentTurn);
                        mess["type"] = 0;
                        mess["sourceCharacter"] = currentCharacter;
                        mess["requiresAnswer"] = false;
                        mess["data"] = warId;
                        gameState->pushMessageToPlayer(gameState->getPlayerOfCharacter(character), mess);
                    }
                    
                }
                else{
                    nlohmann::json mess;
                    mess["id"] = currentCharacter + "_other_" + "noClaim" + "_turn_" + std::to_string(currentTurn);
                    mess["type"] = 6;
                    mess["sourceCharacter"] = currentCharacter;
                    mess["requiresAnswer"] = false;
                    mess["data"] = "You have no claim over this character";
                    gameState->pushMessageToPlayer(gameState->getCurrentPlayer(), mess);
                }
            }
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << std::endl;
            throw std::runtime_error("Error: could not parse War declaration command arguments\n");
        }
    }
    void EngineCore::peace (std::string arguments)
    {
        auto currentCharacter = gameState->getCurrentPlayerCharacter();
        auto currentTurn = gameState->getCurrentTurn();
        try
        {
            // Check if the two characters can make peace for a warId that is retrieved
            // Add a peace proposal message to target player with warId as metadata
            auto j = nlohmann::json::parse(arguments);
            auto targetCharacter = gameState->getProvinceOwner(gameState->getProvinceFromColor(j["colorCode"].get<unsigned int>()));
            auto warId = gameState->getWarId(currentCharacter, targetCharacter);
            if(warId.size())
            {
                nlohmann::json mess;
                mess["id"] = currentCharacter + "_peace_offer:" + warId + "_to_" + targetCharacter + "_turn_" + std::to_string(currentTurn);
                mess["type"] = 1;
                mess["sourceCharacter"] = currentCharacter;
                mess["requiresAnswer"] = true;
                mess["data"] = warId;
                gameState->pushMessageToPlayer(gameState->getPlayerOfCharacter(targetCharacter), mess);
            }
            else{
                nlohmann::json mess;
                mess["id"] = currentCharacter + "_other_" + "noWar" + "_turn_" + std::to_string(currentTurn);
                mess["type"] = 6;
                mess["sourceCharacter"] = currentCharacter;
                mess["requiresAnswer"] = false;
                mess["data"] = "Not at war.";
                gameState->pushMessageToPlayer(gameState->getCurrentPlayer(), mess);
            }
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << std::endl;
            throw std::runtime_error("Error: could not parse peace command\n");
        }
    }
    void EngineCore::surrender (std::string arguments)
    {
        auto currentCharacter = gameState->getCurrentPlayerCharacter();
        auto currentTurn = gameState->getCurrentTurn();
        try
        {
            // Check if the two characters can make peace for a warId that is retrieved
            // Add a surrendering message to target player with warId as metadata
            auto j = nlohmann::json::parse(arguments);
            auto targetCharacter = gameState->getProvinceOwner(gameState->getProvinceFromColor(j["colorCode"].get<unsigned int>()));
            auto warId = gameState->getWarId(currentCharacter, targetCharacter);
            if(warId.size())
            {
                nlohmann::json mess;
                mess["id"] = currentCharacter + "_surr_offer:" + warId + "_to_" + targetCharacter + "_turn_" + std::to_string(currentTurn);
                mess["type"] = 5;
                mess["sourceCharacter"] = currentCharacter;
                mess["requiresAnswer"] = true;
                mess["data"] = warId;
                gameState->pushMessageToPlayer(gameState->getPlayerOfCharacter(targetCharacter), mess);
            }
            else {
                nlohmann::json mess;
                mess["id"] = currentCharacter + "_other_" + "noWar" + "_turn_" + std::to_string(currentTurn);
                mess["type"] = 6;
                mess["sourceCharacter"] = currentCharacter;
                mess["requiresAnswer"] = false;
                mess["data"] = "Not at war.";
                gameState->pushMessageToPlayer(gameState->getCurrentPlayer(), mess);
            }
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << std::endl;
            throw std::runtime_error("Error: could not parse peace command\n");
        }
    }
    void EngineCore::okButton (std::string arguments)
    {
        auto currentPlayer = gameState->getCurrentPlayer();
        try
        {
            auto j = nlohmann::json::parse(arguments);
            auto messageId = j["messageId"];
            gameState->removeMessage(currentPlayer, messageId);
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << std::endl;
            throw std::runtime_error("Error: could not parse ok command\n");
        }
    }
    void EngineCore::yesButton (std::string arguments)
    {
        auto currentPlayer = gameState->getCurrentPlayer();
        try
        {
            auto j = nlohmann::json::parse(arguments);
            auto messageId = j["messageId"].get<std::string>();
            auto sourceCharacter = j["causeId"].get<std::string>();

            // Check the nature of the message
            auto messages = gameState->fetchPlayerMessages(currentPlayer);
            nlohmann::json message;

            for(nlohmann::json::iterator it = messages.begin(); it != messages.end(); ++it)
                if((*it)["id"] == messageId)
                    message = (*it);
            
            if(message.is_null())
                throw std::runtime_error("Error: unknown message id: " + messageId + "\n");
            
            switch(message["type"].get<int>())
            {
                case 1: // Peace offer
                    processPeaceMessageReply(true, arguments, message);
                    break;
                case 5: // Surrender
                    processSurrenderMessageReply(true, arguments, message);
                    break;
                default:
                    throw std::runtime_error("Error: unknown message type\n");
            }
            gameState->removeMessage(currentPlayer, messageId);
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << std::endl;
            throw std::runtime_error("Error: could not parse yes command\n");
        }
    }
    void EngineCore::noButton (std::string arguments)
    {
        auto currentPlayer = gameState->getCurrentPlayer();
        try
        {
            auto j = nlohmann::json::parse(arguments);
            auto messageId = j["messageId"].get<std::string>();
            auto sourceCharacter = j["causeId"].get<std::string>();

            // Check the nature of the message
            auto messages = gameState->fetchPlayerMessages(currentPlayer);
            nlohmann::json message;

            for(nlohmann::json::iterator it = messages.begin(); it != messages.end(); ++it)
                if((*it)["id"] == messageId)
                    message = (*it);
            
            if(message.is_null())
                throw std::runtime_error("Error: unknown message id: " + messageId + "\n");
            
            switch(message["type"].get<int>())
            {
                case 1: // Peace offer
                    processPeaceMessageReply(false, arguments, message);
                    break;
                case 5: // Surrender
                    processSurrenderMessageReply(false, arguments, message);
                    break;
                default:
                    throw std::runtime_error("Error: unknown message type\n");
            }
            gameState->removeMessage(currentPlayer, messageId);
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << std::endl;
            throw std::runtime_error("Error: could not parse no command\n");
        }
    }
    void EngineCore::processSurrenderMessageReply (bool repliedYes, std::string arguments, nlohmann::json message)
    {
        // message contains warId
        // determine the winner 
        // end war
        auto currentCharacter = gameState->getCurrentPlayerCharacter();
        auto otherCharacter = message["sourceCharacter"].get<std::string>();
        auto warId = message["data"].get<std::string>();

        if(repliedYes)// source character surrendered to current character, source loses
            gameState->endWar(warId, currentCharacter);
        // otherwise the war goes on
        
    }
    void EngineCore::processPeaceMessageReply (bool repliedYes, std::string arguments, nlohmann::json message)
    {
        // same as above
        auto currentCharacter = gameState->getCurrentPlayerCharacter();
        auto otherCharacter = message["sourceCharacter"].get<std::string>();
        auto warId = message["data"].get<std::string>();

        if(repliedYes)// currentCharacter loses
            gameState->endWar(warId, otherCharacter);
        // otherwise the war goes on
    }
    void EngineCore::createArmy (std::string arguments){
        auto currentCharacter = gameState->getCurrentPlayerCharacter();
        auto currentTurn = gameState->getCurrentTurn();
        try
        {
            // Check if the two characters can make peace for a warId that is retrieved
            // Add a peace proposal message to target player with warId as metadata
            auto j = nlohmann::json::parse(arguments);
            auto targetProvince = gameState->getProvinceFromColor(j["colorCode"].get<unsigned int>());
            auto provinceOwner = gameState->getProvinceOwner(targetProvince);
            nlohmann::json characters = gameState->fetchAllCharactersData();
            if(provinceOwner==currentCharacter){
                bool isAtWar = false;
                for (auto& character : characters){
                    if (gameState->areAtWar(currentCharacter, character["id"])) {
                        isAtWar = true;
                        break;
                    }
                }
                if(isAtWar)
                {
                    nlohmann::json armyJson;
                    armyJson["id"] = "army_" + currentCharacter + "_from_" + targetProvince;
                    armyJson["ownerCharacter"] = currentCharacter;
                    std::vector<std::string> levies;
                    levies.push_back(targetProvince);
                    armyJson["levies"] = levies;
                    armyJson["currentProvince"] = targetProvince;
                    armyJson["currentBattle"] = "";
                    nlohmann::json jOrders = nlohmann::json::array();
                    armyJson["orders"] = jOrders;
                    gameState->createArmy(armyJson);
                    gameState->raiseLevy(targetProvince);
                }
                else{
                    nlohmann::json mess;
                    mess["id"] = currentCharacter + "_other_" + "createArmynoWar" + "_turn_" + std::to_string(currentTurn);
                    mess["type"] = 6;
                    mess["sourceCharacter"] = currentCharacter;
                    mess["requiresAnswer"] = false;
                    mess["data"] = "Can only create armies during a war.";
                    gameState->pushMessageToPlayer(gameState->getCurrentPlayer(), mess);
                }
            }
            else{
                    nlohmann::json mess;
                    mess["id"] = currentCharacter + "_other_" + "provinceNotOwned_" + targetProvince + "_turn_" + std::to_string(currentTurn);
                    mess["type"] = 6;
                    mess["sourceCharacter"] = currentCharacter;
                    mess["requiresAnswer"] = false;
                    mess["data"] = "Cannot create an army on someone else's province.";
                    gameState->pushMessageToPlayer(gameState->getCurrentPlayer(), mess);
                }
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << std::endl;
            throw std::runtime_error("Error: could not parse createArmy command\n");
        }
    }
}