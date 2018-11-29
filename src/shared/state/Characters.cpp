#include "Characters.h"

#include "Politics.h"

#include <json.hpp>
#include <iostream>

namespace state
{
    Characters::Characters ()
    {
        std::map<std::string, Character> characters;
        std::map<std::string, std::map<std::string, int>> opinions;
    }
    Characters::Characters (Politics * parent, std::string strJson)
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
        this->parent = parent;
    }
    Characters::~Characters ()
    {

    }
    void Characters::setParent(Politics * parent)
    {
        this->parent = parent;
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
    void Characters::updateCharactersData ()
    {
        // For each independent leader
        for(auto const& e: characters)
        {
            // Gold and prestige revenues
            if(parent->getCharacterTopLiege(e.first) == "none")
                updateCharacterRecursively(e.first);
            // age progression
            if(characters[e.first].ageUp())// If the characters dies of old age
            {
                // Generate new random heir
                auto heir = characters[e.first].generateHeir();
                auto heirId = heir.getId();
                characters[heirId] = heir;
                // Transfer titles
                parent->transferAllTitles(e.first, heirId);
                // Update player data
                parent->handleCharacterDeath(e.first, heirId, heir.getPrestige());
            }
        }
    }
    std::pair<int, int> Characters::updateCharacterRecursively (std::string characterId)
    {
        // Compute base income for this character's properties
        int turnGoldGains = parent->computeCharacterGold(characterId);
        int turnPrestigeGains = parent->computeCharacterPrestige(characterId);
        // Compute the added bonus from this character's vassals
        auto vassals = parent->getCharacterDirectVassals(characterId);
        for(auto const& vassalId: vassals)
        {
            auto temp = updateCharacterRecursively(vassalId);
            turnGoldGains += temp.first;
            turnPrestigeGains += temp.second;
        }
        // If character is independant
        if(parent->getCharacterTopLiege(characterId) == "none")
        {
            // All his gold and prestige go to him
            characters[characterId].addGold(turnGoldGains);
            characters[characterId].addPrestige(turnPrestigeGains);
            return(std::pair<int, int>(turnGoldGains, turnPrestigeGains));// Return is useless anyway
        }
        else
        {
            // 75% of his gold and prestige go to him, the rest to his liege
            int liegeGold = turnGoldGains * 0.25;
            int liegePrestige = turnPrestigeGains * 0.25;
            turnGoldGains -= liegeGold;
            turnPrestigeGains -= liegePrestige;
            characters[characterId].addGold(turnGoldGains);
            characters[characterId].addPrestige(turnPrestigeGains);
            return(std::pair<int, int>(liegeGold, liegePrestige));
        }
        
    }
    nlohmann::json Characters::fetchCharacterData (std::string characterId)
    {
        return characters[characterId].toJson();
    }
    nlohmann::json Characters::fetchAllCharactersData ()
    {
        nlohmann::json res = nlohmann::json::array();
        for(auto const& e: characters)
        {
            res.push_back(characters[e.first].toJson());
        }
        return res;
    }
}