#include "Duchy.h"
#include "../json.hpp"
#include <iostream>

namespace state
{
    Duchy::Duchy()
    {
        id = std::string("");
        name = std::string("");
        holder = std::string("");
        liege = std::string("");
        deJureLiege = std::string("");
    }
    Duchy::Duchy (std::string strJson)
    {
        using json = nlohmann::json;
        json j = json::parse(strJson);

        try
        {
            id = j["id"].get<std::string>();
            name = j["name"].get<std::string>();
            holder = j["holder"].get<std::string>();
            liege = j["liege"].get<std::string>();
            deJureLiege = j["deJureLiege"].get<std::string>();
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << std::endl;
            throw std::runtime_error("An error occurred when converting Duchy data from json.");
        }
    }
    Duchy::~Duchy ()
    {

    }
    void Duchy::debug()
    {
        std::cout <<    "Debug Duchy" << std::endl <<
                        "Id: " << this->id << std::endl <<
                        "Name: " << this->name << std::endl <<
                        "Holder id: " << this->holder << std::endl <<
                        "Liege title id: " << this->liege << std::endl <<
                        "de Jure liege title id: " << this->deJureLiege << std::endl;
    }
    bool Duchy::checkConsistency ()
    {
        return !(   id.size() <= 2 ||
                    !(id[0] == 'd' && id[1] == '_') ||
                    name.size() == 0 ||
                    holder.size() == 0 ||
                    liege.size() == 0 ||
                    deJureLiege.size() <= 2);
    }
    std::string Duchy::getLiege ()
    {
        return this->liege;
    }
    std::string Duchy::getDeJureLiege ()
    {
        return this->deJureLiege;
    }
}