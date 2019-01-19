#include "BasicAI.h"
#include <iostream>
#include "json.hpp"

using json = nlohmann::json;

namespace ai {
    BasicAI::BasicAI (state::GameState * state) {
        this->gameState = state;
        this->playerId = state->getCurrentPlayer();
        this->characterId = state->getCurrentPlayerCharacter();
    }
    BasicAI::BasicAI (state::GameState * state, engine::EngineCore * engine) {
        this->gameState = state;
        this->engineCore = engine;
        this->playerId = state->getCurrentPlayer();
        this->characterId = state->getCurrentPlayerCharacter();
    }
    BasicAI::~BasicAI () {}
    
    std::vector<std::vector<engine::Command>> BasicAI::createArmies (std::vector<std::vector<engine::Command>> possibleActions){
        // create armies
        /*json provinces = gameState->fetchAllProvincesData();
        for(json::iterator it=provinces.begin(); it != provinces.end(); it++){
            if (gameState->getProvinceOwner(it.value()["id"])==characterId){
                for(std::vector<engine::Command> action : possibleActions){
                    //never twice the same command in an action
                    bool alreadyDone = false;
                    for (engine::Command command : action){
                        if (it.value()["id"] == json::parse(command.getArgument())["id"]){
                            alreadyDone = true;
                        }
                        if (not alreadyDone){
                            //TO CHANGE
                            json j;
                            j["id"] = it.value()["id"];
                            std::vector<engine::Command> newAction = action;
                            newAction.push_back(engine::Command(engine::Command::raise, j.dump()));
                            //
                            possibleActions.push_back(newAction);
                        }
                    }
                }
            }
        }*/
        return possibleActions;
    }

    std::vector<std::vector<engine::Command>> BasicAI::moveArmies (std::vector<std::vector<engine::Command>> possibleActions){
        // Moving armies is a bit tricky because armies may be moved as soon as they are created; this must be taken into account.
        // temporary vector to store actions
        std::vector<std::vector<engine::Command>> tempActions;
        std::vector<engine::Command> tempAction;
        // temporary state recaculated for each action
        for(std::vector<engine::Command> action : possibleActions){
            //copy the commands but keep the original in the same place
            tempAction = action;
            tempActions.push_back(tempAction);
            // calculate the state for the child node
            state::GameState tempState = gameState->copy();
            engine::EngineCore tempEngine(&tempState);
            for (engine::Command command : tempAction) {
                tempEngine.processCommand(command);
            }
            //json armies = tempState.fetchAllArmiesData();
            json armies = tempState.fetchAllArmiesData();
            json provinces = tempState.fetchAllProvincesData();
            for(json::iterator army=armies.begin();army != armies.end();army++){
                if (army.value()["ownerCharacter"]==characterId){
                    for(std::vector<engine::Command> tempAction : tempActions){      
                        for(json::iterator province=provinces.begin(); province != provinces.end(); province++){
                            if (province.value()["id"]!="sea"){
                                bool alreadyDone = false;
                                for (engine::Command command : tempAction){
                                    if (army.value()["id"] == json::parse(command.getArgument())["id"]){
                                        alreadyDone = true;
                                    }
                                    if (not alreadyDone){
                                        json j;
                                        j["id"] = army.value()["id"];
                                        j["dest"] = province.value()["id"];
                                        std::vector<engine::Command> newAction = tempAction;
                                        newAction.push_back(engine::Command(engine::Command::army, j.dump()));
                                        tempActions.push_back(newAction);
                                    }
                                }
                            }
                        }
                    }
                }
            }
            //do not copy the original node (already in place)
            for (long unsigned int i=1;i<tempActions.size();i++){
                possibleActions.push_back(tempActions[i]);
            }
            tempActions.clear();
        }
        return possibleActions;
    }
    std::vector<std::vector<engine::Command>> BasicAI::makeClaim (std::vector<std::vector<engine::Command>> possibleActions){
        // ONE claim MAX (to simplify)
        json provinces =    gameState->fetchAllProvincesData();
        for(std::vector<engine::Command> action : possibleActions){    
            for(json::iterator province=provinces.begin(); province!=provinces.end(); province++){
                if (province.value()["id"]!="sea"){
                    if (gameState->getProvinceOwner(province.value()["id"])!=characterId){
                        //TO CHANGE
                        json j;
                        j["colorCode"] = province.value()["colorCode"];
                        std::vector<engine::Command> newAction = action;
                        newAction.push_back(engine::Command(engine::Command::claim, j.dump()));
                        //
                        possibleActions.push_back(newAction);
                    }
                }
            }
        }
        return possibleActions;
    }
    std::vector<std::vector<engine::Command>> BasicAI::startWars (std::vector<std::vector<engine::Command>> possibleActions){
        // ONE war MAX (to simplify)
        json characters = gameState->fetchAllCharactersData();
        for(std::vector<engine::Command> action : possibleActions){    
            for(json::iterator character=characters.begin();character!=characters.end();character++){
                if (gameState->hasClaim(characterId,character.value()["id"])!=""){
                    if(not gameState->areAtWar(character.value()["id"], characterId)){
                        json provinces = gameState->fetchAllProvincesData();
                        for (json::iterator province=provinces.begin();province!=provinces.end();province++){
                            if(gameState->getProvinceOwner(province.value()["id"]) == character.value()["id"]){
                                json j;
                                j["colorCode"] = province.value()["colorCode"];
                                std::vector<engine::Command> newAction = action;
                                newAction.push_back(engine::Command(engine::Command::war, j.dump()));
                                possibleActions.push_back(newAction);
                            }
                        }
                    }
                }
            }
        }
        return possibleActions;
    }
    std::vector<std::vector<engine::Command>> BasicAI::makePeace (std::vector<std::vector<engine::Command>> possibleActions){
        // ONE demand that the enemy surrenders MAX (to simplify)
        json characters = gameState->fetchAllCharactersData();
        for(std::vector<engine::Command> action : possibleActions){    
            for(json::iterator it=characters.begin(); it != characters.end(); it++){
                if(gameState->areAtWar(it.value()["id"], characterId)){
                    json war = gameState->getWar(it.value()["id"], characterId);
                    //if you are attacking, you may ask the enemy to surrender
                    if (characterId != gameState->getProvinceOwner(war["targetProvince"])){
                        //TO CHANGE
                        json j;
                        j["colorCode"] = gameState->fetchProvinceData(war["targetProvince"].get<std::string>())["colorCode"];
                        std::vector<engine::Command> newAction = action;
                        newAction.push_back(engine::Command(engine::Command::peace, j.dump()));
                        //
                        possibleActions.push_back(newAction);
                    }
                }
            }
        }
        return possibleActions;
    }
    std::vector<std::vector<engine::Command>> BasicAI::makeSurr (std::vector<std::vector<engine::Command>> possibleActions){
        // ONE surrender MAX (to simplify)
        json characters = gameState->fetchAllCharactersData();
        for(std::vector<engine::Command> action : possibleActions){    
            for(json::iterator it=characters.begin(); it != characters.end(); it++){
                if(gameState->areAtWar(it.value()["id"], characterId)){
                    json war = gameState->getWar(it.value()["id"], characterId);
                    //if you are defending, you may surrender
                    if (characterId == gameState->getProvinceOwner(war["targetProvince"])){
                        //TO CHANGE
                        json j;
                        j["colorCode"] = gameState->fetchProvinceData(war["targetProvince"].get<std::string>())["colorCode"];
                        std::vector<engine::Command> newAction = action;
                        newAction.push_back(engine::Command(engine::Command::surr, j.dump()));
                        //
                        possibleActions.push_back(newAction);
                    }
                }
            }
        }
        return possibleActions;
    }
    std::vector<std::vector<engine::Command>> BasicAI::makeAlliance (std::vector<std::vector<engine::Command>> possibleActions){
        // ONE alliance MAX (to simplify)
        //TODO
        //here is a general basis (just swap nodes with vectors and hopefully it should work)
        /*json provinces = newState->fetchAllProvincesData();
        for(ScoreNode* child : node->getChildren()){    
            for(json::iterator province=provinces.begin(); province!=provinces.end(); province++){
                if (province.value()["id"]!="sea"){
                    if (newState->getProvinceOwner(province.value()["id"])!=characterId){
                        ScoreNode* newNode = new ScoreNode;
                        //TO CHANGE
                        json j;
                        j["id"] = province.value()["id"];
                        std::vector<engine::Command> commands = child->getCommands();
                        commands.push_back(engine::Command(engine::Command::alliance, j.dump()));
                        //
                        newNode->setCommands(commands);
                        node->addChild(newNode);
                    }
                }
            }
        }*/
        return possibleActions;
    }
    std::vector<std::vector<engine::Command>> BasicAI::answerMessages (std::vector<std::vector<engine::Command>> possibleActions){
        //in every possibility, all messages must have an answer
        json playerMessages = gameState->fetchPlayerMessages(playerId);
        for(json::iterator pm=playerMessages.begin(); pm != playerMessages.end(); pm++) {
            long unsigned int sizeActions = possibleActions.size();
            for(long unsigned int i =0;i<sizeActions;i++){
                if (pm.value()["requireAnswer"]){
                    json j;
                    j["messageId"] = pm.value()["id"];
                    j["causeId"] = pm.value()["sourceCharacter"];
                    std::vector<engine::Command> newActionYes = possibleActions[i];
                    std::vector<engine::Command> newActionNo = possibleActions[i];
                    newActionYes.push_back(engine::Command(engine::Command::yes, j.dump()));
                    possibleActions[i] = newActionYes;
                    newActionNo.push_back(engine::Command(engine::Command::no, j.dump()));
                    possibleActions.push_back(newActionNo);
                }
                else{
                    json j;
                    j["messageId"] = pm.value()["id"];
                    std::vector<engine::Command> newAction = possibleActions[i];
                    newAction.push_back(engine::Command(engine::Command::ok, j.dump()));
                    possibleActions[i] = newAction;
                }
            }
        }
        // answer all messages
        return possibleActions;
    }

    std::vector<std::vector<engine::Command>> BasicAI::generatePossibleActions (){
        // create all possible series of commands
        std::vector<std::vector<engine::Command>> possibleActions;
        // add empty action
        std::vector<engine::Command> emptyAction;
        possibleActions.push_back(emptyAction);
        //create armies
        possibleActions = createArmies(possibleActions);
        //move armies
        possibleActions = moveArmies(possibleActions);
        //make claim
        possibleActions = makeClaim(possibleActions);
        //start war
        possibleActions = startWars(possibleActions);
        //alliance(TODO)
        //surrender
        possibleActions = makeSurr(possibleActions);
        //make peace
        possibleActions = makePeace(possibleActions);
        //answer messages
        possibleActions = answerMessages(possibleActions);


        return possibleActions;
    }

    int BasicAI::calculateScore (std::vector<engine::Command> action) {
        using json = nlohmann::json;
        int score = 0;
        try{
            //a new state is necessary to compute the fact that armies may be created, 
            //without preemptively modifying the game state.
            state::GameState newState = gameState->copy();
            engine::EngineCore newEngine(&newState);
            for (engine::Command command : action){
                switch(command.getType()){
                    /*//TO CHANGE
                    case engine::Command::levy:{
                    //
                        // creating an army score
                        //push the command to register the newly created army
                        newEngine.pushCommand(command);
                        json characters = gameState->fetchAllCharactersData();
                        for(json::iterator character = characters.begin(); character != characters.end(); character++){
                            //spawning armies is only useful in case of a war
                            if(gameState->areAtWar(character.value()["id"], characterId)){
                                json war = gameState->getWar(character.value()["id"], characterId);
                                // bonus for defense. When attacked, making armies is always good.
                                if (characterId == gameState->getProvinceOwner(war["targetProvince"])){
                                    score+=10;
                                }
                                //bonus for defense+creating an army on the spot
                                if (war["targetProvince"] == json::parse(command.getArgument())["id"]){
                                    score+=10;
                                }
                                //bonus for proximity
                                else if(int duration = gameState->getOrderJson(json::parse(command.getArgument())["id"],war["targetProvince"])["duration"]<3){
                                    score+=6/duration;
                                }
                            }
                        }
                    }*/
                    case engine::Command::army:{
                        // moving an armie score. prioritize defense.
                        json characters = newState.fetchAllCharactersData();
                        for(json::iterator character = characters.begin(); character != characters.end(); character++){
                            //if there is no war moving an army is generally not that useful
                            if(newState.areAtWar(character.value()["id"], characterId)){
                                json armyMoved = newState.fetchArmyData(json::parse(command.getArgument())["id"]);
                                std::string destination = json::parse(command.getArgument())["dest"];
                                //if the army is in battle, no point in trying to move it
                                if (armyMoved["currentBattle"].size() == 0) {
                                    //telling an army to go to its current position can only be useful to cancel an order
                                    if ((destination != armyMoved["currentProvince"]) | (armyMoved["destinationId"] != destination)) {
                                        json war = newState.getWar(character.value()["id"], characterId);
                                        //if one of your provinces is attacked
                                        if (characterId == newState.getProvinceOwner(war["targetProvince"])){
                                            //but to get the bonus you have to send your army to the right province !!
                                            if(destination==war["targetProvince"]){
                                                score+=10;
                                            }
                                        }
                                        //in any case, sending your army to somewhere you are fighting for is positive
                                        if(destination==war["targetProvince"]){
                                            score+=10;
                                            //bonus for short movement (sending the closest army)
                                            if(int duration = gameState->getOrderJson(armyMoved["currentPosition"], war["targetProvince"])["duration"]<5){
                                                score+=12/(duration+1);
                                            }
                                        }
                                    }
                                }  
                            }
                        }
                    }
                    break;
                    case engine::Command::claim:{
                        // claim a province score
                        std::vector<std::string> claims = newState.fetchCharacterData(characterId)["claims"].get<std::vector<std::string>>();
                        //no claims ?! to succeed one must be ambitious !
                        if (claims.size()==0) score+=15;
                        //greed is good but too much is dangerous
                        if(claims.size()>1) score-=10*claims.size();
                        std::string claimedProvince = newState.getProvinceFromColor(json::parse(command.getArgument())["colorCode"]);
                        std::string opponentId = newState.getProvinceOwner(claimedProvince);
                        //calculate populations and find the minimum distance 
                        //between your provinces and the claimed province
                        int yourPopulation = 0;
                        int opponentPopulation = 0;
                        int distance;
                        //huge value supposed to be bigger than any realistic distance
                        int distanceMin = 1000000000;
                        json provinces = newState.fetchAllProvincesData();
                        for(json::iterator province = provinces.begin(); province != provinces.end(); province++){
                            if (newState.getProvinceOwner(province.value()["id"])==opponentId) opponentPopulation+=province.value()["levy"]["men"].get<int>();
                            if (newState.getProvinceOwner(province.value()["id"])==characterId){
                                yourPopulation+=province.value()["levy"]["men"].get<int>();
                                distance = newState.getOrderJson(province.value()["id"], claimedProvince)["duration"];
                                if(distance<distanceMin) distanceMin = distance;
                            } 
                        }
                        //comparing populations in provinces to roughly estimate strength
                        score += (yourPopulation-opponentPopulation)/100;
                        //bonus for proximity !
                        if(distanceMin<5){
                            score+=12/distanceMin;
                        }
                    }
                    break;
                    case engine::Command::war:{
                        // start a war score
                        int wars = 0;
                        json characters = newState.fetchAllCharactersData();
                        std::string targetProvinceId = newState.getProvinceFromColor(json::parse(command.getArgument())["colorCode"]);
                        std::string opponentId = newState.getProvinceOwner(targetProvinceId);
                        std::vector<std::string> claims;
                        for(json::iterator character = characters.begin(); character != characters.end(); character++){
                            //if there is no war moving an army is generally not that useful
                            if(newState.areAtWar(character.value()["id"], characterId)) wars++;
                            //getting your character claims
                            if (character.value()["id"]==characterId) claims = character.value()["claims"].get<std::vector<std::string>>();
                        }
                        for (std::string claim : claims){
                            //only consider starting a war if there is a casus belli
                            if(newState.getProvinceOwner(claim) == opponentId){
                                //if you are at war with nobody, now might be a good time to start !
                                if (wars==0) score+=15;
                                //fighting the whole world at the same time is not recommended
                                if(wars>1) score-=10*wars;
                                //comparing populations in provinces to roughly estimate strength
                                int yourPopulation = 0;
                                int opponentPopulation = 0;
                                json provinces = newState.fetchAllProvincesData();
                                for(json::iterator province = provinces.begin(); province != provinces.end(); province++){
                                    if (newState.getProvinceOwner(province.value()["id"])==opponentId) opponentPopulation+=province.value()["levy"]["men"].get<int>();
                                    if (newState.getProvinceOwner(province.value()["id"])==characterId) yourPopulation+=province.value()["levy"]["men"].get<int>();
                                }
                                score += (yourPopulation-opponentPopulation)/100;
                            }
                        }                           
                    }
                    break;
                    case engine::Command::surr:{
                        //surrender score
                        //basically the opposite of the war score, except the ai is NOT pacifist
                        int wars = 0;
                        json characters = newState.fetchAllCharactersData();
                        std::string targetProvinceId = newState.getProvinceFromColor(json::parse(command.getArgument())["colorCode"]);
                        std::string opponentId = newState.getProvinceOwner(targetProvinceId);
                        std::vector<std::string> claims;
                        for(json::iterator character = characters.begin(); character != characters.end(); character++){
                            //if there is no war moving an army is generally not that useful
                            if(newState.areAtWar(character.value()["id"], characterId)) wars++;
                            //getting your character claims
                            if (character.value()["id"]==characterId) claims = character.value()["claims"].get<std::vector<std::string>>();
                        }
                        for (std::string claim : claims){
                            //only consider starting a war if there is a casus belli
                            if(newState.getProvinceOwner(claim) == opponentId){
                                //if you are at war with nobody, now might be a good time to start !
                                if (wars==0) score-=30;
                                //fighting the whole world at the same time is not recommended
                                if(wars>2) score+=5*wars;
                                //comparing populations in provinces to roughly estimate strength
                                int yourPopulation = 0;
                                int opponentPopulation = 0;
                                json provinces = newState.fetchAllProvincesData();
                                for(json::iterator province = provinces.begin(); province != provinces.end(); province++){
                                    if (newState.getProvinceOwner(province.value()["id"])==opponentId) opponentPopulation+=province.value()["levy"]["men"].get<int>();
                                    if (newState.getProvinceOwner(province.value()["id"])==characterId) yourPopulation+=province.value()["levy"]["men"].get<int>();
                                }
                                score += (opponentPopulation-yourPopulation)/100;
                            }
                        }       
                    }
                    break;
                    case engine::Command::peace:{
                        //ask for peace score
                        //basically ask the ennemy to instantly surrender and give you what you want. 
                        //what is there not to like here ?
                        score += 10;
                    }
                    break;
                    /*case engine::Command::alliance:{
                        // ask for an alliance score
                        //TODO
                    }*/
                    case engine::Command::yes:{
                        // accept message score
                        std::string messageId = json::parse(command.getArgument())["messageId"];
                        json messages = newState.fetchPlayerMessages(playerId);
                        for(json::iterator pm=messages.begin(); pm!=messages.end(); pm++){
                            if (pm.value()["id"]==messageId) {
                                //do you surrender
                                if (pm.value()["type"] == 1){
                                    score-=10;
                                }
                                //alliance
                                if (pm.value()["type"] == 2){
                                    score+=10;
                                }
                                //claim
                                if (pm.value()["type"] == 3){
                                    score-=10;
                                } 
                                //ennemy surrenders
                                if (pm.value()["type"] == 3){
                                    score+=10;
                                } 
                                break;
                            }
                        }
                        
                    }
                    break;
                    case engine::Command::no:{
                        // refuse message score
                        std::string messageId = json::parse(command.getArgument())["messageId"];
                        json messages = newState.fetchPlayerMessages(playerId);
                        for(json::iterator pm=messages.begin(); pm!=messages.end(); pm++){
                            if (pm.value()["id"]==messageId) {
                                //do you surrender
                                if (pm.value()["type"] == 1){
                                    score+=10;
                                }
                                //alliance
                                if (pm.value()["type"] == 2){
                                    score-=10;
                                }
                                //claim
                                if (pm.value()["type"] == 3){
                                    score+=10;
                                } 
                                //ennemy surrenders
                                if (pm.value()["type"] == 3){
                                    score-=10;
                                } 
                                break;
                            }
                        }
                    }
                    break;
                    default: {}
                }
            }
        }
        catch(const std::exception& e){
            std::cerr << e.what() << std::endl;
            throw std::runtime_error("Error in BasicAI::calculateScore\n");
        }
        return score;
    }

    std::pair<std::vector<std::vector<engine::Command>>, std::vector<int>> BasicAI::chooseBestActions (std::pair<std::vector<std::vector<engine::Command>>, std::vector<int>> actions, int n=1){
        std::pair<std::vector<std::vector<engine::Command>>, std::vector<int>> bestActions;
        std::vector<engine::Command> bestAction;
        int max;
        int position;
        for (int i=0;i<n;i++){
            max = -1;
            position = 0;
            bestAction.clear();
            for (long unsigned int i=0;i<actions.first.size();i++){
                if (actions.second[i]>max) {
                    max=actions.second[i];
                    bestAction=actions.first[i];
                    position = i;
                }
            }
            //only add something if there is an interesting action
            if (not bestAction.empty()){
                std::pair<std::vector<engine::Command>, int> maxPair;
                bestActions.first.push_back(bestAction);
                bestActions.second.push_back(max);
                //diminish the best action score so that the second best can be chosen next
                actions.second[position]=-2;
            }
        }
        //if there is no interesting action, do nothing
        std::vector<engine::Command> emptyAction;
        if (bestActions.first.empty()){
            bestActions.first.push_back(emptyAction);
            bestActions.second.push_back(0);
        }
        return bestActions;
    }

    void BasicAI::sendAction (std::vector<engine::Command> action){
        for (engine::Command command : action) {
            engineCore->processCommand(command);
        }
        // end player's turn
        engineCore->processCommand(engine::Command(engine::Command::commandType::turn, ""));
    }

    std::pair<std::vector<std::vector<engine::Command>>, std::vector<int>> BasicAI::computeScores (std::vector<std::vector<engine::Command>> actions){
        std::pair<std::vector<std::vector<engine::Command>>, std::vector<int>> scoresActions;
        for(auto action : actions){
            scoresActions.first.push_back(action);
            scoresActions.second.push_back(calculateScore(action));
        }
        return scoresActions;
    }

    void BasicAI::main (){
        std::vector<std::vector<engine::Command>> actions;
        std::pair<std::vector<std::vector<engine::Command>>, std::vector<int>> scoresActions;
        std::vector<engine::Command> bestAction;
        actions = generatePossibleActions();
        scoresActions = computeScores(actions);
        scoresActions = chooseBestActions(scoresActions, 1);
        bestAction = scoresActions.first[0];
        sendAction(bestAction);
    }
}