// Generated by dia2code
#ifndef RENDER__TOSTATE__H
#define RENDER__TOSTATE__H

#include <state.h>
#include <string>
#include <map>
#include <json.hpp>

namespace state {
  class GameState;
}

#include "state/GameState.h"

namespace render {

  /// class ToState - 
  class ToState {
    // Associations
    // Attributes
  public:
    state::GameState * state;
    std::map<std::string, std::string> mapColorId;
    // Operations
  public:
    ToState ();
    ToState (state::GameState * state);
    ~ToState ();
    nlohmann::json fetchCharacterData (std::string id);
    nlohmann::json fetchProvinceData (std::string id);
    nlohmann::json fetchArmyData (std::string id);
    nlohmann::json fetchAllArmiesData ();
    nlohmann::json fetchAllRelationsData ();
    nlohmann::json fetchAllPlayersData ();
    nlohmann::json fetchAllProvincesData ();
    nlohmann::json fetchCharacterDataFromColor (std::string colorCode);
    nlohmann::json fetchProvinceDataFromColor (std::string colorCode);
    // Setters and Getters
  };

};

#endif
