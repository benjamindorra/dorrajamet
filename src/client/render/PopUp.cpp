#include "PopUp.h"
#include "Render.h"
#include <iostream>
#include "json.hpp"

namespace render {
    PopUp::PopUp (Render * mainRender, Type type, Action action, std::string causeId, 
    std::string targetId, ToState * state, ToEngine * engine, std::string data, std::string messageId){
        this->mainRender=mainRender;
        this->state = state;
        this->engine = engine;
        this->causeId = causeId;
        this->targetId = targetId;
        this->data = data;
        this->messageId = messageId;
        // text
        font.loadFromFile("./res/DejaVuSerifCondensed.ttf");
        text.setFont(font);
        text.setCharacterSize(20);
        text.setStyle(sf::Text::Bold);
        text.setFillColor(sf::Color::Black);
        // frame
        frame.setFillColor(sf::Color(128,128,128,255));
        frame.setOutlineColor(sf::Color::Black);
        frame.setOutlineThickness(-1);
        select(type, action, causeId, targetId);
    }
    
    PopUp::~PopUp () {
        for (Button *button : buttons) {
            delete button;
        }        
    }
    
    void PopUp::select (Type type, Action action, std::string causeId, std::string targetId) {
        // select text, buttons and frame size
        std::cout << "new popup cause: " << causeId << " -- target id: " << targetId << std::endl;;
        std::string toDisplay;
        switch (action) {
            case War:
                toDisplay = state->fetchCharacterData(causeId)["name"].get<std::string>()+" declares war to "+state->fetchCharacterData( state->getCharacterOfPlayer(targetId))["name"].get<std::string>()+" !";
                break;
            case Peace:
                toDisplay = state->fetchCharacterData(causeId)["name"].get<std::string>()+" makes peace with "+state->fetchCharacterData(targetId)["name"].get<std::string>()+".";
                break;
            case Alliance:
                toDisplay = state->fetchCharacterData(causeId)["name"].get<std::string>()+" makes an alliance with "+state->fetchCharacterData(targetId)["name"].get<std::string>()+".";
                break;
            case Claim:
                toDisplay = state->fetchCharacterData(causeId)["name"].get<std::string>()+" claims "+state->fetchProvinceData(data)["name"].get<std::string>()+".";
                break;
            case Plot:
                toDisplay = state->fetchCharacterData(causeId)["name"].get<std::string>()+" assassinated "+state->fetchCharacterData(targetId)["name"].get<std::string>()+" !";
                break;
            default:
                throw std::string("unknown action");
        }
        width = text.getCharacterSize()*0.6*toDisplay.size();
        height = 100;
        x = (mainRender->getWindow()->getSize().x-width)/2;
        y = (mainRender->getWindow()->getSize().y-height)/2;
        frame.setPosition(x, y);
        frame.setSize(sf::Vector2f(width, height));
        text.setString(toDisplay);
        text.setPosition(x,y);
        for (Button *button : buttons) {
            delete button;
        }
        sf::Color color(160,160,160,255);
        int widthB = 120;
        switch (type){
            case Info:
                buttons.push_back(new Button(mainRender, x+width/2-20, y+height/2, "OK", color, sf::Vector2i(40,20)));
                break;
            case Question:
                buttons.push_back(new Button(mainRender, x+width/2-widthB, y+height/2, "ACCEPT", color, sf::Vector2i(widthB,20)));
                buttons.push_back(new Button(mainRender, x+width/2, y+height/2, "REFUSE", color, sf::Vector2i(widthB,20)));
                break;
            default:
                throw std::string("unknown type");
        }
    }
    
    void PopUp::draw () {
        // draw panel
        mainRender->Render::getWindow()->draw(frame);
        mainRender->Render::getWindow()->draw(text);
        // draw buttons
        for (Button *button : buttons) {
            button->draw();
        }
    }
    
    bool PopUp::contains (int x, int y) {
        for (Button *button : buttons) {
            if (button->contains(x,y)) {
                return true;
            }
        }
        int left = frame.getPosition().x;
        int right = left+frame.getSize().x;
        int top = frame.getPosition().y;
        int bottom = top+frame.getSize().y;
        if ((x>left) & (x<right)) {
            if ((y>top) & (y<bottom)) {
                return true;
            }
        }
        return false;
    }
    
    bool PopUp::click(int x,int y) {
        this->selected=true;
        this->clickX = x;
        this->clickY = y;
        for (Button *button : buttons) {
            if (button->contains(x,y)) {
                handleButtons(button->getDescription());
                return true;
            }
        }
        return false;
    }

    void PopUp::move(int x, int y) {
        this->x = x-(clickX-this->x);
        this->y = y-(clickY-this->y);
        frame.setPosition(this->x, this->y);
        text.setPosition(this->x,this->y);
        for (Button * button : buttons) {
            button->move(x-clickX, y-clickY);
        }
        this->clickX = x;
        this->clickY = y;
    }

    void PopUp::handleButtons (std::string button){
        nlohmann::json j;
        j["command"] = button;
        j["causeId"] = causeId;
        j["targetId"] = targetId;
        j["messageId"] = messageId;
        engine->addCommand(button, j.dump());
    }
    
    bool PopUp::isSelected() {
        return this->selected;
    }

    void PopUp::deselect() {
        this->selected=false;
    }
}