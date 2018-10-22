#include "GameMap.h"
#include "../json.hpp"
#include <iostream>

namespace state
{
    GameMap::GameMap()
    {
        
    }
    GameMap::GameMap(std::string strJson)
    {
        using json = nlohmann::json;

        try
        {
            json j = json::parse(strJson);
            // Provinces
            json jArray = j["provinces"];
            json jElement;
            json::iterator it;
            for(it = jArray.begin(); it!= jArray.end(); ++it)
            {
                jElement = it.value();
                this->provinces[jElement["id"].get<std::string>()] = Province(jElement.dump());
            }
            // Armies
            jArray = j["armies"];
            for(it = jArray.begin(); it!= jArray.end(); ++it)
            {
                jElement = it.value();
                this->armies[jElement["id"].get<std::string>()] = Army(jElement.dump());
            }
            // Battles
            jArray = j["battles"];
            for(it = jArray.begin(); it!= jArray.end(); ++it)
            {
                jElement = it.value();
                this->battles[jElement["id"].get<std::string>()] = Battle(jElement.dump());
            }
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << std::endl ;
            throw std::runtime_error("Error when loading politics.");
        }
    }
    GameMap::~GameMap ()
    {

    }
    bool GameMap::checkConsistency ()
    {
        return true;
    }
    void GameMap::debug ()
    {

    }
}