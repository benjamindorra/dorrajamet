#include "Character.h"
#include "../json.hpp"
#include <iostream>

namespace state{
    Character::Character ()
    {
        /*
        std::string name;
        std::string dynastyName;
        int age;
        std::vector<std::string> traits;
        int diplomacy;
        int stewardship;
        int martial;
        int intrigue;
        std::vector<std::string> claims;
        bool alive;
        int prestige;
        int gold;
        bool hasPlot;
        std::string plotTarget;
        plotTypes plotType;
        int plotEnd;
        */
    }
    Character::Character (std::string strJson)
    {
        using json = nlohmann::json;
        json j = json::parse(strJson);

        try
        {
            id = j["id"].get<std::string>();
            name = j["name"].get<std::string>();
            dynastyName = j["dynastyName"].get<std::string>();
            age = j["age"].get<int>();
            if(j["traits"].is_array())
                traits = j["traits"].get<std::vector<std::string>>();
            else
                traits = std::vector<std::string>();
            diplomacy = j["diplomacy"].get<int>();
            stewardship = j["stewardship"].get<int>();
            martial = j["martial"].get<int>();
            intrigue = j["intrigue"].get<int>();
            if(j["claims"].is_array())
                claims = j["claims"].get<std::vector<std::string>>();
            else
                claims = std::vector<std::string>();
            alive = j["alive"].get<bool>();
            prestige = j["prestige"].get<int>();
            gold = j["gold"].get<int>();
            hasPlot = j["hasPlot"].get<bool>();
            if(hasPlot)
            {
                plotTarget = j["plotTarget"].get<std::string>();
                plotEnd = j["plotEnd"].get<int>();
                int t = j["plotType"].get<int>();
                switch(t)
                {
                    case 1:
                        plotType = claim;
                        break;
                    case 2:
                        plotType = murder;
                        break;
                    default:
                        throw std::string("Error: unknown plot type for character: " + id);
                }
            }
            else
            {
                plotTarget = std::string("none");
                plotType = none;
                plotEnd = -1;
            }
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << std::endl;
            throw std::runtime_error("An error occurred when converting Character data from json.");
        }
    }
    Character::~Character ()
    {

    }
    void Character::debug ()
    {
        std::cout <<    "Debug Character" << std::endl <<
                        "Id: " << this->id << std::endl <<
                        "Name: " << this->name << " of " << this->dynastyName << std::endl;
        std::cout << "Traits: ";
        for(auto trait : this->traits) std::cout << trait << " | ";
        std::cout << "Claims: ";
        std::cout << std::endl;
        for(auto claim : this->claims) std::cout << claim << " | ";
        std::cout << std::endl;
    }
    bool Character::checkConsistency ()
    {
        /*
        std::string name;
        std::string dynastyName;
        int age;
        std::vector<std::string> traits;
        int diplomacy;
        int stewardship;
        int martial;
        int intrigue;
        std::vector<std::string> claims;
        bool alive;
        int prestige;
        int gold;
        bool hasPlot;
        std::string plotTarget;
        plotTypes plotType;
        int plotEnd;
        */
        if(!(name.length() && dynastyName.length()))
            return false;
        if(age < 0)
            return false;
        auto validAttr = [](int attr){return attr >= 0 && attr <= 100;};
        if(!(validAttr(diplomacy) && validAttr(stewardship) && validAttr(intrigue) && validAttr(martial)))
            return false;
        return true;
    }
}