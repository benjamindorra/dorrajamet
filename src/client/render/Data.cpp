/**********************************************************
Data.cpp
Displays game state data and buttons to interact with them.
***********************************************************/
#include "Data.h"
#include "Render.h"
#include <memory>

#include <iostream>

namespace render {
    Data::Data () {}
    Data::Data (Render * mainRender, ToState * state, ToEngine * engine) {
        this->mainRender=mainRender;
        this->state = state;
        this->engine = engine;
        // text
        font.loadFromFile("./res/DejaVuSerifCondensed.ttf");
        text.setFont(font);
        text.setString(data);
        text.setCharacterSize(20);
        text.setStyle(sf::Text::Bold);
        text.setFillColor(sf::Color::Black);
        // button
        // view
        float width = 0.3;
        float height = 0.8;
        float widthText = width*(float)mainRender->getWindow()->getSize().x;
        float heightText = height*(float)mainRender->getWindow()->getSize().y;
        view.setViewport(sf::FloatRect(0, 0.1, width, height));
         // view size
        view.setSize(widthText,heightText-0.1);
        view.setCenter(widthText/2, (heightText-0.1)/2);
        // frame
        frame.setFillColor(sf::Color(128,128,128,255));
        frame.setSize(view.getSize());
        sf::Color color(160,160,160,255);
        buttons.push_back(new Button(mainRender, 0, heightText+0.1, "", color, sf::Vector2i(widthText, mainRender->getWindow()->getSize().y-heightText)));
    }
    Data::~Data () {
        for (Button *button : buttons) {
            delete button;
        }
    }
    void Data::select(Types type,std::string id) {
        this->type = type;
        this->id = id;
        // select data type
        float width = view.getSize().x;
        float top = view.getViewport().top;
        float height = view.getViewport().height;
        float spaceV = (1-top-height)*mainRender->getWindow()->getSize().y;
        float y1 = mainRender->getWindow()->getSize().y*(height+top);
        float left = view.getViewport().left;
        float x1 = left*mainRender->getWindow()->getSize().x;
        sf::Color color(160,160,160,255);
        // reset the view
        view.setCenter(view.getSize().x/2, view.getSize().y/2);
        text.setPosition(0,0);
        // create buttons
        if (type==Types::Character) {
            for (Button *button : buttons) {
                delete button;
            }
            buttons={};
            buttons.push_back(new Button(mainRender, x1, y1, "Relations", color, sf::Vector2i(width, spaceV/3)));
            buttons.push_back(new Button(mainRender, x1, y1+spaceV/3, "Alliance", color, sf::Vector2i(width/2, spaceV/3)));
            buttons.push_back(new Button(mainRender, x1+width/2, y1+spaceV/3, "Murder", color, sf::Vector2i(width/2, spaceV/3)));
            buttons.push_back(new Button(mainRender, x1, y1+2*spaceV/3, "War", color, sf::Vector2i(width/2, spaceV/3)));
            buttons.push_back(new Button(mainRender, x1+width/2, y1+2*spaceV/3, "Peace", color, sf::Vector2i(width/2, spaceV/3)));
            this->data = state->getCharacter(id);
        }
        if (type==Types::Province) {
            for (Button *button : buttons) {
                delete button;
            }
            buttons={};
            buttons.push_back(new Button(mainRender, x1, y1, "Levy", color, sf::Vector2i(width/2, spaceV/2)));
            buttons.push_back(new Button(mainRender, x1+width/2, y1, "Claim", color, sf::Vector2i(width/2, spaceV/2)));
            buttons.push_back(new Button(mainRender, x1, y1+spaceV/2, "", color, sf::Vector2i(width, spaceV/2)));
            this->data = state->getProvince(id);
        }
        
        if (type==Types::Army) {
            for (Button *button : buttons) {
                delete button;
            }
            buttons={};
            buttons.push_back(new Button(mainRender, x1, y1, "", color, sf::Vector2i(width, spaceV)));
            this->data = state->getArmy(id);
        }

        if (type==Types::Relations) {
            for (Button *button : buttons) {
                delete button;
            } 
            buttons={};
            buttons.push_back(new Button(mainRender, x1, y1, "Character", color, sf::Vector2i(width, spaceV/3)));
            buttons.push_back(new Button(mainRender, x1, y1+spaceV/3, "", color, sf::Vector2i(width, 2*spaceV/3)));
            buttons.push_back(new Button(mainRender, x1, y1+spaceV/3, "", color, sf::Vector2i(width, 2*spaceV/3)));
            buttons.push_back(new Button(mainRender, x1, y1+spaceV/3, "", color, sf::Vector2i(width, 2*spaceV/3)));
            buttons.push_back(new Button(mainRender, x1, y1+spaceV/3, "", color, sf::Vector2i(width, 2*spaceV/3)));
            this->data = state->getRelations(id);
        }
        for (int i=0;i<9;i++) {this->data += data;}
        // insert endline characters into the text so that if fits the window
        float charSize = text.getCharacterSize()*1.9*view.getViewport().width;
        size_t length = this->data.length();
        float lineSize = 0;
        size_t lastSpace = 0;
        for (size_t i=0;i<length;i++) {
            lineSize+=charSize;
            if (this->data[i] == ' ') {
                    lastSpace=i; 
            }
            if (this->data[i] == '\n') {
                    lineSize=0;
                    lastSpace=0; 
            }
            else if (lineSize>width) {
                lineSize=0;
                if (lastSpace !=0) {
                    this->data.replace(lastSpace,1,1,'\n');
                    lastSpace=0;
                }
                else if (data[i+1]==' ') {
                    this->data.erase(i+1,1);
                }
                else {
                    this->data.insert(i,1,'\n');
                    length++;
                }
            }
        }
        text.setString(this->data);
    }
    void  Data::draw() {
        for (Button *button : buttons) {
            button->draw();
        }
            // change view
            mainRender->getWindow()->setView(this->view);
            // draw panel
            mainRender->Render::getWindow()->draw(frame);
            mainRender->Render::getWindow()->draw(text);
            // restore view
            mainRender->Render::getWindow()->setView(mainRender->Render::getWindow()->getDefaultView());
    }

    bool Data::contains(int x, int y) {
        for (Button *button : buttons) {
            if (button->contains(x,y)) {
                return true;
            }
        }
        sf::FloatRect hitbox = view.getViewport();
        sf::Vector2u size = mainRender->getWindow()->getSize();
        int left = size.x*hitbox.left;
        int right = left+size.x*hitbox.width;
        int top = size.y*hitbox.top;
        int bottom = top+size.y*hitbox.height;
        if ((x>left) & (x<right)) {
            if ((y>top) & (y<bottom)) {
                return true;
            }
        }
        return false;
    }
    void Data::click(int x,int y) {
        for (Button *button : buttons) {
            if (button->contains(x,y)) {
                handleButtons(button->getDescription());
            }
        }
    } 
    void Data::handleButtons(std::string button) {
        if (button =="") {}
        else if (type==Types::Character) {
            if (button=="Relations") {this->select(Data::Relations, "");}
            else if (button =="Alliance") {}
            else if (button=="Murder") {}
            else if (button=="War") {}
            else if (button=="Peace") {}
            engine->addCommand(id,button);
        }
        else if (type==Types::Province) {
            if (button=="Levy") {}
            else if (button =="Claim") {}
            engine->addCommand(id,button);
        }
        
        else if (type==Types::Army) {
            engine->addCommand(id,button);
        }

        else if (type==Types::Relations) {
            if (button=="Character") {this->select(Types::Character, "");}
        }
    }

    void Data::scroll(int deltaY) {
        float frameH = frame.getSize().y;
        float newTextPos = text.getPosition().y-deltaY;
        float newTextBottom = text.getPosition().y+text.getLocalBounds().height-deltaY;
        if ((newTextPos<=0) & (newTextBottom>=frameH)){
            text.setPosition(0, newTextPos);
        }
        else if ((text.getPosition().y<=0) & (newTextPos>0)) {
            text.setPosition(0, 0);
        }
        else if (((text.getPosition().y+text.getLocalBounds().height)>=frameH) & (newTextBottom<frameH)) {
            text.setPosition(0, frameH-text.getLocalBounds().height);
        }
    }

    
}