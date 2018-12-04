#include "War.h"
#include <json.hpp>
#include "../tools.hpp"
#include <iostream>

namespace state
{
    War::War ()
    {
    /*
    std::string id;
    std::string targetTitle;
    std::vector<std::string> attackerCamp;
    std::vector<std::string> defenderCamp;
    /// 100: attacker victory, -100 defender victory
    int warScore;
    */
    }
    War::War (std::string strJson)
    {
        try
        {
            auto j = nlohmann::json::parse(strJson);
            id = j["id"];
            targetTitle = j["targetTitle"];
            attackerCamp = j["attackerCamp"].get<std::vector<std::string>>();
            defenderCamp = j["defenderCamp"].get<std::vector<std::string>>();
            warScore = j["warScore"].get<int>();
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << std::endl;
            throw std::runtime_error("Error: invalid war data\n");
        }
    }
    War::~War ()
    {

    }
    void War::debug ()
    {

    }
    nlohmann::json War::toJson ()
    {
        nlohmann::json res;
        res["id"] = id;
        res["targetTitle"] = targetTitle;
        res["attackerCamp"] = attackerCamp;
        res["defenderCamp"] = defenderCamp;
        res["warScore"] = warScore;
        return res;
    }
    std::string War::getId ()
    {
        return id;
    }
    int War::getScore ()
    {
        return warScore;
    }
}