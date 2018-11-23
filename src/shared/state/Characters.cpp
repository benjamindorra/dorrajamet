#include "Characters.h"
#include <json.hpp>
#include <iostream>

namespace state
{
    Characters::Characters ()
    {
        std::map<std::string, Character> characters;
        std::map<std::string, std::map<std::string, int>> opinions;
    }
    Characters::Characters (std::string strJson)
    {
        using json = nlohmann::json;

        try
        {
            json j = json::parse(strJson)["list"];
            std::string id;
            for(json::iterator it = j.begin(); it!= j.end(); ++it)
            {
                id = it.value()["id"].get<std::string>();
                characters.insert(std::pair<std::string, Character>(id, Character(it.value().dump())));
            }

            json o = json::parse(strJson)["opinions"];
            std::string id1, id2;
            for(json::iterator it1 = o.begin(); it1 != o.end(); ++it1)
            {
                id1 = it1.value()["id_source"].get<std::string>();
                id2 = it1.value()["id_target"].get<std::string>();
                opinions[id1][id2] = it1.value()["value"].get<int>();
            }
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << std::endl ;
            throw std::runtime_error("Error when loading characters.");
        }
    }
    Characters::~Characters ()
    {

    }
    Character * Characters::operator[] (const char * a)
    {
        return this->operator[](std::string(a));
    }
    Character * Characters::operator[] (const std::string a)
    {
        return &characters[a];
    }
    int Characters::getOpinion (std::string sourceCharacter, std::string targetCharacter)
    {
        return opinions[sourceCharacter][targetCharacter];
    }
    bool Characters::checkConsistency ()
    {
        for(auto entry : characters)
        {
            if(!(entry.second.checkConsistency()))
                return false;
        }
        for(auto opinionFrom : opinions)
        {
            // Check if opinion emitter exists
            if(!(characters.count(opinionFrom.first)))
                return false;
            for(auto opinionAbout : opinionFrom.second)
            {
                // Check if opinion target exists
                if(!(characters.count(opinionAbout.first)))
                    return false;
                // Check if score is valid
                if(opinionAbout.second < -100 || opinionAbout.second > 100)
                    return false;
            }
        }
        return true;
    }
    void Characters::debug()
    {
        std::cout << "====Characters====\n";
        for(auto chara : characters)
            chara.second.debug();
    }
    std::string Characters::getMainTitle(std::string characterId)
    {
        return characters[characterId].getMainTitle();
    }
}