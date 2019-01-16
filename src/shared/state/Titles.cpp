#include "Titles.h"
#include <json.hpp>
#include <iostream>

namespace state
{
    Titles::Titles()
    {

    }
    Titles::Titles(std::string strJson)
    {
        using json = nlohmann::json;

        try
        {
            json j = json::parse(strJson);
            std::string id;
            for(json::iterator it = j.begin(); it!= j.end(); ++it)
            {
                id = it.value()["id"].get<std::string>();
                kingdoms[id] = Kingdom(it.value().dump());
                
            }
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << std::endl ;
            throw std::runtime_error("Error when loading titles.");
        }
    }
    Titles::~Titles()
    {

    }
    bool Titles::checkConsistency()
    {
        for(auto kingdom : kingdoms)
        {
            if(!kingdom.second.checkConsistency())
            {
                std::cerr << "Error: a title is not consistent with id: " << kingdom.first;
                return false;
            }
        }
        return true;
    }
    void Titles::debug()
    {
        std::cout << "====Titles====\n";
        for(auto kingdom : kingdoms)
            kingdom.second.debug();
    }
    unsigned int Titles::getKingdomColor (std::string kingdomId)
    {
        return kingdoms[kingdomId].getColorCode();
    }
    std::string Titles::getKingdomOwner (std::string kingdomId)
    {
        return kingdoms[kingdomId].getHolder();
    }
    nlohmann::json Titles::fetchKingdomData (std::string kingdomId)
    {
        return kingdoms[kingdomId].toJson();
    }
    nlohmann::json Titles::toJson (){
        nlohmann::json allTitles = nlohmann::json::array();
        for(auto kingdom : kingdoms){
            allTitles.push_back(kingdom.second.toJson());
        }
        return allTitles;
    }
}