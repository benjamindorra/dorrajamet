#include "ScoreNode.h"

namespace ai {
    ScoreNode::ScoreNode (){
        parent = nullptr;
        score = -1;
    }
    ScoreNode::~ScoreNode (){
        for (ScoreNode * child : this->children){
            delete child;
        }
    }
    void ScoreNode::setScore (int score){
        this->score = score;
    }
    int ScoreNode::getScore (){
        return score;
    }
    void ScoreNode::setCommands (std::vector<engine::Command> commands){
        this->commands = commands;
    }
    std::vector<engine::Command> ScoreNode::getCommands (){
        return commands;
    }
    void ScoreNode::setParent (ScoreNode * parent){
        this->parent = parent;
    }
    ScoreNode * ScoreNode::getParent (){
        return this->parent;
    }
    void ScoreNode::addChild(ScoreNode* child){
        child->setParent(this);
        this->children.push_back(child);
    }
    void ScoreNode::clearChildren(){
        this->children.clear();
    }
    std::vector<ScoreNode*> ScoreNode::getChildren (){
        return this->children;
    }
}