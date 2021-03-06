#include "Player.h"
#include <iostream>

namespace state
{
    Player::Player ()
    {
        /*std::string id;
        std::string currentCharacter;
        int score;*/
    }
    Player::Player (nlohmann::json initJson)
    {
        try
        {
            id = initJson["id"].get<std::string>();
            currentCharacter = initJson["currentCharacter"].get<std::string>();
            score = initJson["score"].get<int>();
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << std::endl;
            throw std::runtime_error("Error: could not parse player object.\n");
        }
    }
    Player::~Player ()
    {

    }
    std::string Player::getId()
    {
        return id;
    }
    std::string Player::getCharacter ()
    {
        return currentCharacter;
    }
    void Player::setCharacter (std::string newCharacterId)
    {
        currentCharacter = newCharacterId;
    }
    void Player::addScore (int amount)
    {
        score+=amount;
    }
    nlohmann::json Player::toJson ()
    {
        nlohmann::json j;
        j["id"] = id;
        j["currentCharacter"] = currentCharacter;
        j["score"] = score;
        return j;
    }
    nlohmann::json Player::getMessages () {
        nlohmann::json messages = nlohmann::json::array();
        for(auto message : pendingMessages) {
            messages.push_back(message.toJson());
        }
        return messages;
    }
    void Player::addMessage (nlohmann::json initJson)
    {
        //std::cout << "player " << id << " had " << pendingMessages.size() << " pending messages\n";
        pendingMessages.push_back(Message(initJson));
        //std::cout << "player " << id << " has " << pendingMessages.size() << " pending messages\n";
    }

    void Player::removeMessage (std::string messageId)
    {
        pendingMessages.erase(std::find_if(pendingMessages.begin(), pendingMessages.end(), [messageId](Message m) -> bool{ return m.getId() == messageId; }));
    }
}