#include "BasicAI.h"
#include "json.hpp"

#define ENDTURN 0.00000001

namespace ai {
    BasicAI::BasicAI (state::GameState * state, engine::EngineCore * engine, std::string playerId) {
        toState = render::ToState(state);
        toEngine = render::ToEngine(engine);
        this->playerId = playerId;
        //this->characterId = toState.fetchPlayerData(playerId)["currentCharacter"];
    }
    BasicAI::~BasicAI () {}
    void BasicAI::calculateScores (){
        using json = nlohmann::json;
        int population;
        json myCharacter;
        json myProvinces;
        // end turn score
        scores[std::pair<std::string, std::string>("endTurn", "")]= ENDTURN;

        // defense scores
        json armies = toState.fetchAllArmiesData();
        json provinces = toState.fetchAllProvincesData();
        for(json::iterator ita = armies.begin(); ita != armies.end(); ita++){
            if (ita.value()["ownerCharacter"] != myCharacter["id"]){}
            for(json::iterator itp = provinces.begin(); itp != provinces.end(); itp++) {

            }
        }



    }

    void BasicAI::chooseAction (){
        int max = -1;
        std::pair<std::string, std::string> bestAction;
        for (auto x : scores){
            if (x.second>max) {
                bestAction = x.first;
            }
        }
        toEngine.addCommand(bestAction.first, bestAction.second);
    }
}