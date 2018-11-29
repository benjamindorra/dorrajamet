/************************************************************
PlayerData.cpp
Simple bar to show informations about the player's character.
*************************************************************/
#include "PlayerData.h"
#include "Render.h"
#include <json.hpp>
#include <iostream>

using json = nlohmann::json;

namespace render {
    PlayerData::PlayerData () {}
    PlayerData::PlayerData (Render * mainRender, ToState * state) {
        this->mainRender = mainRender;
        this->state = state;
        // text
        font.loadFromFile("./res/DejaVuSerifCondensed.ttf");
        text.setFont(font);
        text.setString(data);
        text.setCharacterSize(20);
        text.setStyle(sf::Text::Bold);
        text.setFillColor(sf::Color::Black);
        // frame
        float width = 0.8;
        float height = 0.1;
        float widthText = width*(float)mainRender->getWindow()->getSize().x;
        float heightText = height*(float)mainRender->getWindow()->getSize().y;
        frame.setSize(sf::Vector2f(widthText, heightText));
        frame.setPosition(sf::Vector2f(0,0));
        frame.setFillColor(sf::Color(200,200,0,255));
        frame.setOutlineColor(sf::Color::Black);
        frame.setOutlineThickness(-1);
        //temporary. change to fit the actual player.
        selectCharacter("player_one");
    }
    PlayerData::~PlayerData () {}
    void PlayerData::selectCharacter(std::string id) {
        this->id = id;
        try {
            json j = state->fetchAllPlayersData();
            for(json::iterator it = j.begin(); it!= j.end(); ++it){
                if(it.value()["id"].get<std::string>()==this->id) {
                    this->data = "Score :"+ it.value()["score"].dump()+" ";
                    this->id = it.value()["currentCharacter"].get<std::string>();
                    j = state->fetchCharacterData(this->id);
                    this->data = "Character: "+j["name"].dump()+" "+j["dynastyName"].dump()+" "
                    +"Prestige: "+j["prestige"].dump()+" "+"Gold: "+j["gold"].dump()+" "
                    +"Plot active: "+j["hasPlot"].dump()+"\n"
                    +"Diplomacy: "+j["diplomacy"].dump()+" "+"Stewardship: "+j["stewardship"].dump()+" "
                    +"Martial: "+j["martial"].dump()+" "+"Intrigue: "+j["intrigue"].dump();
                    text.setString(data);
                    break;
                }  
            }
        }
        catch(const std::exception& e) {
            std::cerr << e.what() <<std::endl;
            throw std::runtime_error("Incorrect data for playerData");
        }
    }
    void  PlayerData::draw() {
        // draw panel
        mainRender->Render::getWindow()->draw(frame);
        mainRender->Render::getWindow()->draw(text);
    }
    void PlayerData::update() {
        selectCharacter(this->id);
    }
}