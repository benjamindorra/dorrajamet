#include "GameState.h"
#include <iostream>
#include <fstream>
#include <string>
#include "../json.hpp"
#include "../tools.hpp"

namespace state
{
    GameState::GameState ()
    {
        politics = Politics();
        gameMap = GameMap();
        ressources = Ressources();
        players = std::vector<Player>();
    }
    GameState::GameState (const char* saveFilePath) : GameState(std::string(saveFilePath)){}
    GameState::GameState (std::string saveFilePath)
    {
        using json = nlohmann::json;
        try
        {
            std::string fileContent = loadFile(saveFilePath);
            json j = json::parse(fileContent);
            this->politics = Politics(j["politics"].dump());
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what();
            throw std::runtime_error("Error: an error occurred when loading GameState from saveFile.");
        }
    }
    GameState::~GameState()
    {

    }
    void GameState::debug()
    {
        this->politics.debug();
    }
}