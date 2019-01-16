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
    bool Characters::checkConsistency ()
    {
        for(auto entry : characters)
        {
            if(!(entry.second.checkConsistency()))
                return false;
        }
        return true;
    }
    void Characters::debug()
    {
        std::cout << "TOTAL CHARACTERS COUNT: " << characters.size() << std::endl;
        std::cout << "====Characters====\n";
        for(auto chara : characters)
            chara.second.debug();
    }
    std::string Characters::getMainTitle(std::string characterId)
    {
        return characters[characterId].getKingdom();
    }
    void Characters::changeScoreBy (std::string characterId, int amount)
    {
        characters[characterId].addPrestige(amount);
    }
    void Characters::removeClaim (std::string characterId, std::string titleId)
    {
        characters[characterId].removeClaim(titleId);
    }
    std::string Characters::getKingdomOfCharacter (std::string characterId)
    {
        return characters[characterId].getKingdom();
    }
    void Characters::updateCharactersData ()
    {
        // For each independent leader
        /*for(auto const& e: characters)
        {
            // Gold and prestige revenues

            // age progression
            // If the characters dies of old age
                // Generate new random heir
                // Transfer titles
                // Update player data
        }*/
    }
    
    nlohmann::json Characters::fetchCharacterData (std::string characterId)
    {
        try
        {
            return characters.at(characterId).toJson();
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << std::endl;
            throw std::runtime_error("Error: Characters::fetchCharacterData - unknown id: " + characterId);
        }
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
    std::vector<std::string> Characters::getClaims (std::string characterId)
    {
        return characters[characterId].getClaims();
    }
    void Characters::addClaim (std::string claimant, std::string provinceId)
    {
        characters[claimant].addClaim(provinceId);
    }
    nlohmann::json Characters::toJson (){
        nlohmann::json allCharacters = nlohmann::json::array();
        
        for(auto character : characters) {
            allCharacters.push_back(character.second.toJson());
        }
        
        return allCharacters;
    }
}