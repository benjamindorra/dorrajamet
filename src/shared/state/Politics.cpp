#include "Politics.h"

#include "GameState.h"

#include "../tools.hpp"
#include <json.hpp>
#include <iostream>
#include <fstream>
#include <algorithm>

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
            titles = Titles(j["kingdoms"].dump());
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
        //titles.debug();
        std::cout << "\n\n\n";
        //characters.debug();
        for(auto war: wars)
        {
            std::cout << war.first << std::endl;
            war.second.debug();
        }
        std::cout << "endPolitics\n";
    }
    void Politics::updateCharactersData()
    {
        characters.updateCharactersData();
    }
    int Politics::computeCharacterGold (std::string characterId)
    {
        /*auto ownedProvinces = titles.getProvincesOf(characterId);
        int gold = 0;
        for(auto const& provinceId: ownedProvinces)
        {
            auto provData = parent->fetchProvinceData(provinceId);
            gold += provData["taxIncome"].get<int>();
        }
        return gold;*/
        return 0;
    }
    int Politics::computeCharacterPrestige (std::string characterId)
    {
        return 0;
    }
    bool Politics::checkWarStatus (std::string characterA, std::string characterB)
    {
        for(unsigned int i = 0; i < relations.size(); i++)
            if(relations[i].isBetween(characterA, characterB))
                return relations[i].getType() == Relation::war;
        return false;
    }
    void Politics::handleCharacterDeath (std::string characterId, std::string heirId, int score)
    {
        parent->updatePlayerCharacter(characterId, heirId, score);
    }
    void Politics::updateWars ()
    {
        /*// For war in wars
            // Get all provinces for each camp
            // Get percentage of occupied provinces for each camp
            // Compute and set warScore
            // End finished wars
        std::vector<std::string> finishedWars;
        for(auto const& e: wars)
        {
            std::vector<std::string> attackersProvinces, defendersProvinces;
            auto attackers = wars[e.first].getAttackerCamp();
            auto defenders = wars[e.first].getDefenderCamp();
            for(auto const& attacker: attackers) // For each attacking province, if the province isn't already recorded
                if(!isIn(attackersProvinces, attacker))
                    attackersProvinces.push_back(attacker);
            for(auto const& defender: defenders) // For each defending province, if the province isn't already recorded
                if(!isIn(defendersProvinces, defender))
                    defendersProvinces.push_back(defender);
            int occupiedAttackersProvinces = 0, occupiedDefendersProvinces = 0;
            for(auto const& province: attackersProvinces)
                if(isIn(defenders, parent->getProvinceOccupant(province)))
                    occupiedAttackersProvinces++;
            for(auto const& province: defendersProvinces)
                if(isIn(attackers, parent->getProvinceOccupant(province)))
                    occupiedDefendersProvinces++;
            float defendersOccupation = (float)occupiedDefendersProvinces / (float)defendersProvinces.size();
            float attackersOccupation = (float)occupiedAttackersProvinces / (float)attackersProvinces.size();
            wars[e.first].setScore((int)(200 * (defendersOccupation - attackersOccupation)));
            if(wars[e.first].attackerWon() || wars[e.first].defenderWon())
                finishedWars.push_back(e.first);
        }
        for(auto const& e: finishedWars)
            endWar(e);
        */
        //simplified version.
        //the siegeStatus of a province is between 0 and 100, starting at 50.
        //if it reaches 0, the defender gains control of the province, he keeps the province and the war ends.
        //if it reaches 100, the attacker wins, he takes control of the province and the war ends.
        //it is NOT a good practice to iterate over a list here you are adding/removing elements.
        //this is why we iterate over relations, and not wars. else it creates a bug.
        for(Relation relation : relations){
            if (relation.getType()==Relation::relType::war){
                std::string warId = relation.getWarId();
                std::cout<<std::endl<<warId<<std::endl;
                std::string provinceId = wars.at(warId).getTargetTitle();
                std::string provinceController = parent->getProvinceController(provinceId);
                if (provinceController != "none") {
                    std::string attackerId = wars.at(warId).getClaimantCharacter();
                    std::string defenderId = wars.at(warId).getDefendingCharacter();
                    parent->setProvinceController(provinceId, "none");
                    if (titles.getKingdomOwner(provinceController) == defenderId){
                        endWar(warId, defenderId);
                    }
                    else {
                        endWar(warId, attackerId);
                    }
                }
            }
        }
    }
    void Politics::endWar (std::string warId, std::string winner)
    {
        try
        {
            War war = wars.at(warId);
            auto attackers = war.getAttackerCamp();
            auto defenders = war.getDefenderCamp();
            auto title = war.getTargetTitle();
            auto claimant = war.getClaimantCharacter();
            auto mainDefender = war.getDefendingCharacter();
            if(winner == mainDefender)
                war.setScore(-100);
            else if(winner == claimant)
                war.setScore(100);
            else
                throw std::runtime_error("Error: attempted to end a war of id: " + warId + " with invalid winner id: " + winner + "\n");
            if(war.attackerWon())
            {
                for(auto const& a: attackers)
                    characters.changeScoreBy(a, -40);
                for(auto const& a: defenders)
                    characters.changeScoreBy(a, 40);
                parent->transferProvince(title, characters.getKingdomOfCharacter(claimant));
                characters.removeClaim(claimant, title);
            }
            else if(war.defenderWon())
            {
                for(auto const& a: attackers)
                    characters.changeScoreBy(a, -50);
                for(auto const& a: defenders)
                    characters.changeScoreBy(a, 50);
                characters.removeClaim(claimant, title);
            }
            else // White peace
            {
                
            }
            for(auto const& attacker: attackers)
                for(auto const& defender: defenders)
                {
                    for(unsigned int i = 0; i < relations.size(); i++){
                        if(relations[i].isBetween(attacker, defender) && relations[i].getType() == Relation::war && relations[i].getWarId() == warId){
                            nlohmann::json j;
                            j["type"] = 0;
                            j["characterA"] = attacker;
                            j["characterB"] = defender;
                            j["endTurn"] = (parent->getCurrentTurn()) + 10;
                            j["characterA"] = defender;
                            j["characterB"] = attacker;
                            relations[i] = Relation(j.dump());
                        }
                    }
                }
            wars.erase(warId);
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << std::endl;
            throw std::runtime_error("Error: Politics::endWar( " + warId + " )\n");
        }

        
    }
    nlohmann::json Politics::getKingdomColor (std::string kingdomId)
    {
        return titles.getKingdomColor(kingdomId);
    }
    std::string Politics::getKingdomOfCharacter (std::string characterId)
    {
        return characters.getKingdomOfCharacter(characterId);
    }
    std::string Politics::getCharacterOfKingdom (std::string kingdomId)
    {
        return titles.getKingdomOwner(kingdomId);
    }
    nlohmann::json Politics::fetchCharacterData (std::string characterId)
    {
        return characters.fetchCharacterData(characterId);
    }
    nlohmann::json Politics::fetchKingdomData (std::string kingdomId)
    {
        return titles.fetchKingdomData(kingdomId);
    }
    nlohmann::json Politics::fetchAllRelationsData ()
    {
        nlohmann::json res = nlohmann::json::array();
        for(auto relation: relations)
            res.push_back(relation.toJson());
        return res;
    }
    std::map<std::string, int> Politics::getClaims (std::string characterId)
    {
        return characters.getClaims(characterId);
    }
    std::vector<std::string> Politics::getAllies (std::string characterId)
    {
        std::vector<std::string> res;
        for(auto r: relations)
        {
            if(r.concerns(characterId) && r.getType() == Relation::alliance)
            {
                auto other = r.getOther(characterId);
                if(!isIn(res, other))
                    res.push_back(other);
            }
        }
        return res;
    }
    std::string Politics::createWar (std::vector<std::string> attackerCamp, std::vector<std::string> defenderCamp, std::string claimantId, std::string mainDefender, std::string targetClaim, unsigned int turn)
    {
        nlohmann::json initJson;
        initJson["id"] = "war_" + claimantId + "-" + mainDefender + "_turn" + std::to_string(turn) + "_for:" + targetClaim;
        initJson["targetProvince"] = targetClaim;
        initJson["claimantCharacter"] = claimantId;
        initJson["mainDefender"] = mainDefender;
        initJson["attackerCamp"] = attackerCamp;
        initJson["defenderCamp"] = defenderCamp;
        initJson["warScore"] = 0;
        wars[initJson["id"]] = War(initJson.dump());
        return initJson["id"];
    }
    void Politics::setWar (std::string characterA, std::string characterB, std::string warId)
    {
        // change relations to WAR. Create them if they don't exist.
        bool relationExist = false;
        for(long unsigned int i=0;i<relations.size();i++)
        {
            if(relations[i].isBetween(characterA, characterB))
            {
                relationExist = true;
                nlohmann::json initJson;
                initJson["characterA"] = characterA;
                initJson["characterB"] = characterB;
                initJson["endTurn"] = -1;
                initJson["type"] = 4;
                initJson["warId"] = warId;
                relations[i] = Relation(initJson.dump());
            }
        }
        if(not relationExist){
            nlohmann::json initJson;
            initJson["characterA"] = characterA;
            initJson["characterB"] = characterB;
            initJson["endTurn"] = -1;
            initJson["type"] = 4;
            initJson["warId"] = warId;
            relations.push_back(Relation(initJson.dump()));
        }
    }
    bool Politics::areAtWar (std::string characterA, std::string characterB)
    {
        for(auto r: relations)
            if((r.isBetween(characterA, characterB) && r.getType() == Relation::war))
                return true;
        return false;
    }
    bool Politics::isProvinceAtWar(std::string provinceId){
        for(auto war: wars)
            if(war.second.getTargetTitle()==provinceId)
                return true;
        return false;
    }
    void Politics::addClaim (std::string claimant, std::string provinceId, int turn)
    {
        characters.addClaim(claimant, provinceId, turn);
    }
    nlohmann::json Politics::fetchAllCharactersData(){
        return characters.fetchAllCharactersData();
    }
    std::pair<std::vector<std::string>, std::vector<std::string>> Politics::getWarCamps (std::string warId)
    {
        try
        {
            std::pair<std::vector<std::string>, std::vector<std::string>> res;
            res.first = wars.at(warId).getAttackerCamp();
            res.second = wars.at(warId).getDefenderCamp();
            return res;
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << std::endl;
            throw std::runtime_error("Politics::getWarCamps with id: " + warId + "\n");
        }
    }
    nlohmann::json Politics::getWar (std::string characterA, std::string characterB)
    {
        for(auto war: wars)
            if((war.second.getClaimantCharacter() == characterA && war.second.getDefendingCharacter() == characterB) || (war.second.getClaimantCharacter() == characterB && war.second.getDefendingCharacter() == characterA))
                return war.second.toJson();
        return "";
    }
    std::string Politics::getWarId (std::string characterA, std::string characterB)
    {
        for(auto war: wars)
            if((war.second.getClaimantCharacter() == characterA && war.second.getDefendingCharacter() == characterB) || (war.second.getClaimantCharacter() == characterB && war.second.getDefendingCharacter() == characterA))
                return war.first;
        return "";
    }
    nlohmann::json Politics::toJson (){
        nlohmann::json politics;
        politics["kingdoms"] = titles.toJson();
        politics["characters"]["list"] = characters.toJson();
        nlohmann::json allRelations = nlohmann::json::array();
        for(Relation relation : relations){
            allRelations.push_back(relation.toJson());
        }
        politics["characters"]["relations"] = allRelations;
        nlohmann::json allWars = nlohmann::json::array();
        for(auto war: wars){
            allWars.push_back(war.second.toJson());
        }
        politics["wars"] = allWars;
        return politics;
    }
}
