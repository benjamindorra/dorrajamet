/************************************************************
PlayerData.cpp
Simple bar to show informations about the player's character.
*************************************************************/
#include "PlayerData.h"
#include "Render.h"

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
    }
    PlayerData::~PlayerData () {}
    void PlayerData::selectCharacter(std::string id) {
        this->data = state->getCharacter(id);
        text.setString(data);
    }
    void  PlayerData::draw() {
        // draw panel
        mainRender->Render::getWindow()->draw(frame);
        mainRender->Render::getWindow()->draw(text);
    }
}