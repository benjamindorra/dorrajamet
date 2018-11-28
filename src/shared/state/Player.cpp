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
    nlohmann::json Player::fetchPlayerData() {
        using json = nlohmann::json;
        json j;
        j["id"] = id;
        j["currentCharacter"] = currentCharacter;
        j["score"] = score;
        return j;
    }
}