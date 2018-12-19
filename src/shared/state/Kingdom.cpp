#include "Kingdom.h"
#include <json.hpp>
#include <iostream>

namespace state
{
    Kingdom::Kingdom()
    {
        id = std::string("");
        name = std::string("");
        holder = std::string("");
        kingdomColorCode = 0;
    }
    Kingdom::Kingdom (std::string strJson)
    {
        using json = nlohmann::json;
        json j = json::parse(strJson);

        try
        {
            id = j["id"].get<std::string>();
            name = j["name"].get<std::string>();
            holder = j["holder"].get<std::string>();
            kingdomColorCode = j["kingdomColorCode"].get<unsigned int>();
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << std::endl;
            throw std::runtime_error("An error occurred when converting Kingdom data from json.");
        }
    }
    Kingdom::~Kingdom ()
    {

    }
    void Kingdom::debug()
    {
        std::cout <<    "Debug Kingdom" << std::endl <<
                        "Id: " << this->id << std::endl <<
                        "Name: " << this->name << std::endl <<
                        "Holder id: " << this->holder << std::endl;
    }
    bool Kingdom::checkConsistency ()
    {
        return !(   id.size() <= 2 ||
                    !(id[0] == 'k' && id[1] == '_') ||
                    name.size() == 0 ||
                    holder.size() == 0);
    }
    unsigned int Kingdom::getColorCode ()
    {
        return kingdomColorCode;
    }
    nlohmann::json Kingdom::toJson ()
    {
        nlohmann::json j;
        j["id"] = id;
        j["name"] = name;
        j["holder"] = holder;
        j["kingdomColorCode"] = kingdomColorCode;

        return j;
    }

    std::string Kingdom::getHolder()
    {
        return holder;
    }
}