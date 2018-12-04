#include "Politics.h"

#include "GameState.h"

#include "../tools.hpp"
#include <json.hpp>
#include <iostream>
#include <fstream>

namespace state
{
    Politics::Politics()
    {

    }
    Politics::Politics(GameState * parent, std::string strJson)
    {
        using json = nlohmann::json;

        try
        {
            json j = json::parse(strJson);
            titles = Titles(j["titles"].dump());
            if(titles.checkConsistency())
                std::cout << "Titles consistency check success\n\n\n";
            else
                throw std::string("Titles consistency check failed\n");
            characters = Characters(nullptr, j["characters"].dump());
            if(characters.checkConsistency())
                std::cout << "Characters consistency check success\n\n\n";
            else
                throw std::string("Characters consistency check failed\n");
            for(auto relStr:j["relations"])
            {
                relations.push_back(Relation(relStr.dump()));
            }
            for(auto warStr:j["wars"])
            {
                auto war = War(warStr.dump());
                wars[war.getId()] = war;
            }
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << std::endl ;
            throw std::runtime_error("Error when loading politics.");
        }
        this->parent = parent;
        refreshChildParentPointers();
    }
    Politics::~Politics()
    {
        
    }
    void Politics::refreshChildParentPointers()
    {
        characters.setParent(this);
    }
    void Politics::setParent (GameState * parent)
    {
        this->parent = parent;
    }
    void Politics::debug()
    {
        titles.debug();
        std::cout << "\n\n\n";
        characters.debug();
    }
    void Politics::updateCharactersData()
    {
        characters.updateCharactersData();
    }
    int Politics::computeCharacterGold (std::string characterId)
    {
        auto ownedProvinces = titles.getProvincesOf(characterId);
        int gold = 0;
        for(auto const& provinceId: ownedProvinces)
        {
            auto provData = parent->fetchProvinceData(provinceId);
            gold += provData["taxIncome"].get<int>();
        }
        return gold;
    }
    int Politics::computeCharacterPrestige (std::string characterId)
    {
        return titles.computeCharacterPrestige(characterId);
    }
    bool Politics::checkWarStatus (std::string characterA, std::string characterB)
    {
        for(unsigned int i = 0; i < relations.size(); i++)
            if(relations[i].isBetween(characterA, characterB))
                return relations[i].getType() == Relation::war;
        return false;
    }
    std::string Politics::getProvinceOwner (std::string provinceId)
    {
        return titles.getProvinceOwner(provinceId);
    }
    std::string Politics::getCharacterTopLiege (std::string characterId)
    {
        auto mainTitle = characters.getMainTitle(characterId);
        return titles.getTopLiege(mainTitle);
    }
    std::vector<std::string> Politics::getCharacterDirectVassals (std::string characterId)
    {
        std::vector<std::string> res;
        auto ownedTitles = getCharacterAllTitles(characterId);
        for(auto const& title: ownedTitles)
        {
            auto temp = titles.getTitleDirectVassals(title);
            res.insert(res.end(), temp.begin(), temp.end());
        }
        return res;
    }
    std::vector<std::string> Politics::getCharacterAllTitles (std::string characterId)
    {
        return titles.getTitlesOf(characterId);
    }
    unsigned int Politics::getTitleColor (std::string titleId)
    {
        return titles.getTitleColor(titleId);
    }
    void Politics::transferTitle(std::string character_from, std::string character_to, std::string titleId)
    {
        titles.setTitleHolder(character_to, titleId);
    }
    void Politics::transferAllTitles (std::string character_from, std::string character_to)
    {
        for(auto const& e: getCharacterAllTitles(character_from))
            transferTitle(character_from, character_to, e);
    }
    void Politics::handleCharacterDeath (std::string characterId, std::string heirId, int score)
    {
        parent->updatePlayerCharacter(characterId, heirId, score);
    }
    void Politics::updateWars ()
    {
        // For war in wars
            // Get all provinces for each camp
            // Get percentage of occupied provinces for each camp
            // Compute and set warScore
            // End finished wars
            // 
    }
    nlohmann::json Politics::fetchCharacterData (std::string characterId)
    {
        return characters.fetchCharacterData(characterId);
    }
    nlohmann::json Politics::fetchAllCharactersData ()
    {
        return characters.fetchAllCharactersData();
    }
    nlohmann::json Politics::fetchAllRelationsData ()
    {
        nlohmann::json res = nlohmann::json::array();
        for(auto e: relations)
            res.push_back(e.toJson());
        return res;
    }
}
