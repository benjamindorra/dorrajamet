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
    std::string targetProvince;
    std::string claimantCharacter;
    std::string mainDefender;
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
            targetProvince = j["targetProvince"];
            claimantCharacter = j["claimantCharacter"];
            mainDefender = j["mainDefender"];
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
        std::cout << toJson().dump(2);
    }
    nlohmann::json War::toJson ()
    {
        nlohmann::json res;
        res["id"] = id;
        res["targetProvince"] = targetProvince;
        res["claimantCharacter"] = claimantCharacter;
        res["mainDefender"] = mainDefender;
        res["attackerCamp"] = attackerCamp;
        res["defenderCamp"] = defenderCamp;
        res["warScore"] = warScore;
        return res;
    }
    std::string War::getId ()
    {
        return id;
    }
    std::string War::getTargetTitle ()
    {
        return targetProvince;
    }
    int War::getScore ()
    {
        return warScore;
    }
    std::string War::getClaimantCharacter ()
    {
        return claimantCharacter;
    }
    std::string War::getDefendingCharacter ()
    {
        return mainDefender;
    }
    std::vector<std::string> War::getAttackerCamp ()
    {
        return attackerCamp;
    }
    std::vector<std::string> War::getDefenderCamp ()
    {
        return defenderCamp;
    }
    void War::setScore (int score)
    {
        warScore = score;
        if(warScore < -100)
            warScore = -100;
        else if(warScore > 100)
            warScore = 100;
    }
    bool War::attackerWon ()
    {
        return warScore == 100;
    }
    bool War::defenderWon ()
    {
        return warScore == -100;
    }
}