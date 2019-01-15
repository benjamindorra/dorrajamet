#include "DeepAI.h"
#include <json.hpp>
#include <iostream>
#include <ctime>
#include <cstdlib>

using json = nlohmann::json;

namespace ai {
    DeepAI::DeepAI (state::GameState* state, engine::EngineCore * engine){
        this->gameState = state;
        this->engine = engine;
        treeRoot = new ScoreNode;
    }
    DeepAI::~DeepAI (){}
    void DeepAI::fillTree (std::string playerId, int turnsToCompute){
        // create a ScoreTree, containing commands and score.
        //TO REMOVE
        std::srand(std::time(nullptr));
        //
        // count the number of players to find how many levels th tree must have
        int playersCount=0;
        json players = gameState->fetchAllPlayersData();
        for(json::iterator it=players.begin(); it != players.end(); it++){
            playersCount++;
        }
        int levels = playersCount * turnsToCompute;
        /*// table used to check if every level of the tree has been correctly filled
        // numbers are paired, first : number of child processed ,second: number of child created
        // all children that have been created must be processed as long as their level
        // is inferior to the size of the tree
        std::pair<int, int> treeCheck[levels];
        for(int i=0;i<levels;i++){treeCheck[i].first=0;treeCheck[i].second=0;}
        treeCheck[0].second=generateChildren(treeRoot);
        if (treeCheck[0].second>0){
            // table used to keep track of how much of the tree has been explored
            int treePosition[levels];
            ScoreNode* currentNode = treeRoot;
            ScoreNode* exploreNode;
            int exploredLevel;*/
            for(int currentLevel=0;currentLevel<levels;currentLevel++){
                /*//init the position table
                for(int i=0;i<levels;i++){treePosition[i]=0;}
                // assign currentNode to the first child
                if  (not currentNode->getChildren().empty()) currentNode=currentNode->getChildren()[0];
                else{}
                exploreNode = currentNode;
                exploredLevel = currentLevel;
                while((treeCheck[currentLevel].first<treeCheck[currentLevel].second) & (exploreNode != nullptr)){
                    if (exploreNode->getChildren().empty()){
                        treeCheck[exploredLevel+1].second+=generateChildren(exploreNode);
                        treeCheck[exploredLevel].first++;
                    }
                    else if (exploredLevel==currentLevel){
                        std::cout<<"go up"<<std::endl;
                        exploreNode=exploreNode->getParent();
                        exploredLevel--;
                        std::cout<<"OK"<<std::endl;
                    }
                    else if (exploredLevel==currentLevel-1){
                        for(auto child : exploreNode->getChildren()){
                            if (child->getChildren().empty()){
                                treeCheck[exploredLevel+2].second+=generateChildren(child);
                                treeCheck[exploredLevel+1].first++;
                            }
                        }
                        if (exploreNode->getParent() != nullptr) {
                            if (treePosition[exploredLevel]<exploreNode->getParent()->getChildren().size()-1){
                                std::cout<<"go right"<<std::endl;
                                exploreNode = exploreNode->getParent()->getChildren()[treePosition[exploredLevel]+1];
                                treePosition[exploredLevel]++;
                                std::cout<<"OK"<<std::endl;
                            }
                            else{
                                exploreNode=exploreNode->getParent();
                                std::cout<<"go up"<<std::endl;
                                treePosition[exploredLevel]=0;
                                exploredLevel--;
                                std::cout<<"OK"<<std::endl;
                            }
                        }
                        else {
                            std::cout<<"go to null"<<std::endl;
                            exploreNode=exploreNode->getParent();
                            std::cout<<"OK"<<std::endl;
                        }
                    }
                    else if (exploredLevel<currentLevel-1){
                        if (exploreNode->getParent() != nullptr) {
                            if (treePosition[exploredLevel]<exploreNode->getParent()->getChildren().size()-1){
                                std::cout<<"go right, then down"<<std::endl;
                                exploreNode = exploreNode->getParent()->getChildren()[treePosition[exploredLevel]+1];
                                treePosition[exploredLevel]++;
                                exploreNode=exploreNode->getChildren()[0];
                                exploredLevel++;
                                std::cout<<"OK"<<std::endl;
                            }
                            else{
                                exploreNode=exploreNode->getParent();
                                std::cout<<"go up"<<std::endl;
                                treePosition[exploredLevel]=0;
                                exploredLevel--;
                                std::cout<<"OK"<<std::endl;
                            }
                        }
                        else {
                            std::cout<<"go to null"<<std::endl;
                            exploreNode=exploreNode->getParent();
                            std::cout<<"OK"<<std::endl;
                        }
                    }
                    //TO REMOVE
                    std::cout<<std::endl;
                    std::cout<<"treeCheck"<<std::endl;
                    for(int i = 0;i<levels;i++){std::cout<<treeCheck[i].first<<" "<<treeCheck[i].second<<";";}
                    std::cout<<std::endl;
                    std::cout<<"treePosition"<<std::endl;
                    for(int i = 0;i<levels;i++){std::cout<<treePosition[i]<<";";}
                    std::cout<<std::endl;
                    std::cout<<"exploredLevel : "<<exploredLevel<<std::endl;
                    std::cout<<"currentLevel : "<<currentLevel<<std::endl;
                    std::cout<<viewTree(treeRoot)<<std::endl;
                    if(exploreNode==nullptr) std::cout<<"exploreNode est remonté jusqu'à la racine..."<<std::endl;
                    //
                }*/
                fillLevel(treeRoot);
                //TO REMOVE
                viewTreeParse(viewTree(treeRoot));
                //
            //}
        }
    }

    int DeepAI::computeMaxChildren(ScoreNode* currentNode){
        if (currentNode->getScore()>-1){
            return currentNode->getScore();
        }
        int max = -1;
        for(ScoreNode* child : currentNode->getChildren()){
            int score = computeMaxChildren(child);
            if (score>max){
                max = score;
            }
        }
        return max;
    }

    std::vector<engine::Command> DeepAI::minimax (){
        std::string playerId = this->gameState->getCurrentPlayer();
        computeLeafScores(treeRoot, playerId);
        // use the minimax algorithm to find a good serie of commands for the AI's turn
        for (ScoreNode* child : this->treeRoot->getChildren()){
            child->setScore(computeMaxChildren(child));
        }
        std::vector<engine::Command> outputCommands;
        int max = -1;
        for (ScoreNode* child : this->treeRoot->getChildren()){
            if (child->getScore()>max){
                max = child->getScore();
                outputCommands = child->getCommands();
            }
        }
        return outputCommands;
    }


    void DeepAI::sendCommands (engine::EngineCore * engineCore, std::vector<engine::Command> commands){
        // send an array of commands to the engine
        for (engine::Command command : commands) {
            engineCore->processCommand(command);
        }
        // end player's turn
        engineCore->processCommand(engine::Command(engine::Command::commandType::turn, ""));
    }
    int DeepAI::computeScore (state::GameState* oneState, std::string playerId){
        // compute score for a player in a game state
        int score = 0;
        json provinces = oneState->fetchAllProvincesData();
        for(json::iterator it=provinces.begin(); it != provinces.end(); it++){
            // count how many provinces a player owns
            if (oneState->getProvinceOwner(it.value()["id"])==oneState->getCharacterOfPlayer(playerId)){
                score++;
            }
        }
        return score;
    }
    
    state::GameState DeepAI::computeGameState (ScoreNode *  node, state::GameState* initialState){
        // return a game state modified by all the commands in node's ancesters
        // go back to root and save every node in a vector
        std::vector<ScoreNode*> successiveNodes;
        while (node->getParent()){
            successiveNodes.push_back(node);
            node = node->getParent();
        }
        // execute every command from first (last in vector) to last.
        state::GameState newState = initialState->copy();
        engine::EngineCore newEngine(&newState);
        for (long unsigned int i=0;i<successiveNodes.size();i++) {
            sendCommands(&newEngine, successiveNodes.back()->getCommands());
        }
        return newState;
    }
    
    void DeepAI::generateLevyChildren (ScoreNode* node, state::GameState* newState, std::string characterId){
        /*json provinces = newState->fetchAllProvincesData();
        for(json::iterator it=provinces.begin(); it != provinces.end(); it++){
            if (newState->getProvinceOwner(it.value()["id"])==characterId){
                for(ScoreNode* child : node->getChildren()){
                    if (it.value()["id"] != json::parse(child->getCommands().back().getArgument())["id"]){
                        ScoreNode* newNode = new ScoreNode;
                        //TO CHANGE
                        json j;
                        j["id"] = it.value()["id"];
                        std::vector<engine::Command> commands = child->getCommands();
                        commands.push_back(engine::Command(engine::Command::raise, j.dump()));
                        //
                        newNode->setCommands(commands);
                        node->addChild(newNode);
                    }
                }
            }
        }*/
    }
    
    void DeepAI::generateArmiesChildren (ScoreNode* node, state::GameState* newState, std::string characterId){
        // temporary node to store children
        ScoreNode tempNode;
        ScoreNode tempNodeChild;
        // temporary state recaculated for each children
        //state::GameState tempState;
        for(ScoreNode* child : node->getChildren()){
            //copy the commands but keep the original in the same place
            tempNodeChild.setCommands(child->getCommands());
            tempNode.addChild(&tempNodeChild);
            // calculate the state for the child node
            state::GameState tempState = newState->copy();
            engine::EngineCore tempEngine(&tempState);
            for (engine::Command command : child->getCommands()) {
                tempEngine.processCommand(command);
            }
            //json armies = tempState.fetchAllArmiesData();
            json armies = tempState.fetchAllArmiesData();
            json provinces = tempState.fetchAllProvincesData();
            for(json::iterator army=armies.begin();army != armies.end();army++){
                if (army.value()["ownerCharacter"]==characterId){
                    for(ScoreNode* tempChild : tempNode.getChildren()){      
                        for(json::iterator province=provinces.begin(); province != provinces.end(); province++){
                            if (province.value()["id"]!="sea"){
                                if (child->getCommands().empty()){
                                    ScoreNode* newNode = new ScoreNode;
                                    json j;
                                    j["id"] = army.value()["id"];
                                    j["dest"] = province.value()["id"];
                                    std::vector<engine::Command> commands = tempChild->getCommands();
                                    //std::vector<engine::Command> commands = tempNodeChild.getCommands();
                                    commands.push_back(engine::Command(engine::Command::army, j.dump()));
                                    newNode->setCommands(commands);
                                    tempNode.addChild(newNode);
                                }
                                else if(army.value()["id"] != json::parse(child->getCommands().back().getArgument())["id"]){
                                    ScoreNode* newNode = new ScoreNode;
                                    json j;
                                    j["id"] = army.value()["id"];
                                    j["dest"] = province.value()["id"];
                                    std::vector<engine::Command> commands = tempChild->getCommands();
                                    //std::vector<engine::Command> commands = tempNodeChild.getCommands();
                                    commands.push_back(engine::Command(engine::Command::army, j.dump()));
                                    newNode->setCommands(commands);
                                    tempNode.addChild(newNode);
                                }
                            }
                        }
                    }
                }
            }
            //do not copy the original node (already in place)
            for (long unsigned int i=1;i<tempNode.getChildren().size();i++){
                node->addChild(tempNode.getChildren()[i]);
            }
            tempNode.clearChildren();
        }
    }
    void DeepAI::generateClaimChildren (ScoreNode* node, state::GameState* newState, std::string characterId){
        json provinces = newState->fetchAllProvincesData();
        for(ScoreNode* child : node->getChildren()){    
            for(json::iterator province=provinces.begin(); province!=provinces.end(); province++){
                if (province.value()["id"]!="sea"){
                    if (newState->getProvinceOwner(province.value()["id"])!=characterId){
                        ScoreNode* newNode = new ScoreNode;
                        //TO CHANGE
                        json j;
                        j["colorCode"] = province.value()["colorCode"];
                        std::vector<engine::Command> commands = child->getCommands();
                        commands.push_back(engine::Command(engine::Command::claim, j.dump()));
                        //
                        newNode->setCommands(commands);
                        node->addChild(newNode);
                    }
                }
            }
        }
    }
    void DeepAI::generateWarChildren (ScoreNode* node, state::GameState* newState, std::string characterId){
        json characters = newState->fetchAllCharactersData();
        for(ScoreNode* child : node->getChildren()){    
            for(json::iterator it=characters.begin(); it != characters.end(); it++){
                if (it.value()["id"]!=characterId){
                    if(not newState->areAtWar(it.value()["id"], characterId)){
                        ScoreNode* newNode = new ScoreNode;
                        //TO CHANGE
                        json j;
                        j["id"] = it.value()["id"];
                        std::vector<engine::Command> commands = child->getCommands();
                        commands.push_back(engine::Command(engine::Command::war, j.dump()));
                        //
                        newNode->setCommands(commands);
                        node->addChild(newNode);
                    }
                }
            }
        }
    }
    void DeepAI::generatePeaceChildren (ScoreNode* node, state::GameState* newState, std::string characterId){
        /*json characters = newState->fetchAllCharactersData();
        for(ScoreNode* child : node->getChildren()){    
            for(json::iterator it=characters.begin(); it != characters.end(); it++){
                if (it.value()["id"]!=characterId){
                    if(newState->areAtWar(it.value()["id"], characterId)){
                        ScoreNode* newNode = new ScoreNode;
                        //TO CHANGE
                        json j;
                        j["id"] = it.value()["id"];
                        std::vector<engine::Command> commands = child->getCommands();
                        commands.push_back(engine::Command(engine::Command::peace, j.dump()));
                        //
                        newNode->setCommands(commands);
                        node->addChild(newNode);
                    }
                }
            }
        }*/
    }
    void DeepAI::generateAllianceChildren (ScoreNode* node, state::GameState* newState, std::string characterId){
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
    }
    void DeepAI::generateAnswerChildren (ScoreNode* node, state::GameState* newState, std::string characterId){}

    int DeepAI::generateChildren(ScoreNode * node){
        // get the current game state for the node
        state::GameState newState = computeGameState(node, this->gameState);
        state::GameState tempState;
        // add a node with no command
        node->addChild(new ScoreNode);
        // get the player id
        std::string playerId = newState.getCurrentPlayer();
        std::string characterId = newState.getCurrentPlayerCharacter();
        //recruit at most 1 levy from each province
        //generateLevyChildren (node, &newState, characterId);
        //move each army at most one
        generateArmiesChildren (node, &newState, characterId);
        //claim at most one province
        //generateClaimChildren (node, &newState, characterId);
        //declare war to at most 1 character
        //generateWarChildren (node, &newState, characterId);
        //make peace with at most 1 character
        //generatePeaceChildren (node, &newState, characterId);
        //make an alliance with at most 1 character
        //generateAllianceChildren (node, &newState, characterId);
        //answer messages
        //generateAnswerChildren (node, &newState, characterId);
        /*int nChildren = std::rand()%3+1;
        for (int i=0;i<nChildren;i++){
            node->addChild(new ScoreNode);
        }*/
        return node->getChildren().size();
    }

    std::string DeepAI::viewTree(ScoreNode * currentNode){
        // explore the tree and return the number of childs of each node in a string
        if (currentNode->getChildren().empty()) {
            return std::to_string(currentNode->getScore());
        }
        else {
            std::string children = std::to_string(currentNode->getScore())+"[";
            for (ScoreNode* child : currentNode->getChildren()) {
                children.append(viewTree(child));
            }
            children.append("]");
            return children;
        }
    }

    void DeepAI::viewTreeParse(std::string tree){
        // parse and show the tree in a readable format
        // find the number of levels
        int levels = 1;
        char character = tree[0];
        int counter = 1;
        while (character != '0'){
            if (character == '[') levels++;
            character=tree[counter];
            counter++;
        }
        // create a table with a string for every level
        std::string treeTable[levels];
        std::cout<<"levels: "<<levels<<std::endl;
        int level = 0;
        for (char character : tree) {
            if (character == '[') {
                level++;
                treeTable[level].append(std::string(1,character)+" ");
            }
            else if (character == ']'){
                treeTable[level].append(std::string(1,character)+" ");
                level--;
            }
            else treeTable[level].append(std::string(1,character)+" ");
        }
        // display the tree
        for(std::string treeLevel : treeTable) {
            std::cout<<treeLevel<<std::endl<<std::endl;
        }
    }

    void DeepAI::fillLevel(ScoreNode * currentNode){
        // fill one level of the tree 
        if (currentNode->getChildren().empty()) {
            generateChildren(currentNode);
        }
        else {
            for (ScoreNode* child : currentNode->getChildren()) {
                fillLevel(child);
            }
        }
    }

    void DeepAI::computeLeafScores(ScoreNode* currentNode, std::string playerId){
        // compute the score of leaf level 
        if (currentNode->getChildren().empty()) {
            state::GameState finalState = computeGameState(currentNode, this->gameState);
            currentNode->setScore(computeScore(&finalState, playerId));
        }
        else {
            for (ScoreNode* child : currentNode->getChildren()) {
                computeLeafScores(child, playerId);
            }
        }
    }
}