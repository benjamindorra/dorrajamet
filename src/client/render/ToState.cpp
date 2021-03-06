/***************************
ToState.cpp
Get data from the game state
****************************/
#include "ToState.h"
#include <json.hpp>

#include <iostream>

using json = nlohmann::json;

namespace render {
    ToState::ToState () {}
    
    ToState::ToState (state::GameState * state) {
        this->state=state;
    }
    ToState::~ToState () {}
    nlohmann::json ToState::fetchCharacterData (std::string id)
    {
        return state->fetchCharacterData(id);
    }
    nlohmann::json ToState::fetchProvinceData (std::string id)
    {
        return state->fetchProvinceData(id);
    }
    nlohmann::json ToState::fetchArmyData (std::string id)
    {
        return state->fetchArmyData(id);
    }
    nlohmann::json ToState::fetchAllArmiesData ()
    {
        return state->fetchAllArmiesData();
    }
    nlohmann::json ToState::fetchAllRelationsData ()
    {
        return state->fetchAllRelationsData();
    }
    nlohmann::json ToState::fetchAllPlayersData ()
    {
        return state->fetchAllPlayersData();
    }
    nlohmann::json ToState::fetchAllProvincesData ()
    {
        return state->fetchAllProvincesData();
    }
    nlohmann::json ToState::fetchCharacterDataFromColor (std::string colorCode)
    {
        return state->fetchCharacterData(std::stoul(colorCode));
    }
    nlohmann::json ToState::fetchProvinceDataFromColor (std::string colorCode)
    {
        return state->fetchProvinceData(std::stoul(colorCode));
    }
    nlohmann::json ToState::fetchAllProvincesKingdomColor ()
    {
        return state->fetchAllProvincesKingdomColor();
    }
    nlohmann::json ToState::fetchKingdomData (std::string id)
    {
        return state->fetchKingdomData(id);
    }
    
    nlohmann::json ToState::fetchPlayerMessagesData(std::string playerId){
        return state->fetchPlayerMessages(playerId);
    }
    std::string ToState::getCurrentPlayer ()
    {
        return state->getCurrentPlayer();
    }
    std::string ToState::getCurrentPlayerCharacter()
    {
        return state->getCurrentPlayerCharacter();
    }
    std::string ToState::getCharacterOfPlayer (std::string playerId)
    {
        return state->getCharacterOfPlayer(playerId);
    }

}