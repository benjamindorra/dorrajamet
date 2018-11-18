/***********************************************************
Button.cpp
Simple square button with a description and a black outline.
************************************************************/
#include "Button.h"
#include "Render.h"
#include <iostream>

namespace render {
    Button::Button (Render * mainRender) {}
    Button::Button (Render * mainRender, int x, int y, std::string description) {
        this->mainRender = mainRender;
        this->x=x;
        this->y=y;
        this->description=description;
        // text
        font.loadFromFile("./res/DejaVuSerifCondensed.ttf");
        text.setFont(font);
        text.setString(description);
        text.setCharacterSize(20);
        text.setStyle(sf::Text::Bold);
        text.setFillColor(sf::Color::Black);
        text.setPosition(sf::Vector2f(x,y));
        // frame
        sf::FloatRect rect = text.getLocalBounds();
        frame.setFillColor(sf::Color::Red);
        frame.setOutlineColor(sf::Color::Black);
        frame.setOutlineThickness(-1);
        frame.setSize(sf::Vector2f(rect.width,2*rect.height));
        frame.setPosition(sf::Vector2f(x,y));
    }
    Button::Button (Render * mainRender, int x, int y, std::string description, sf::Color color) {
        this->mainRender = mainRender;
        this->x=x;
        this->y=y;
        this->description=description;
        // text
        font.loadFromFile("./res/DejaVuSerifCondensed.ttf");
        text.setFont(font);
        text.setString(description);
        text.setCharacterSize(20);
        text.setStyle(sf::Text::Bold);
        text.setFillColor(sf::Color::Black);
        text.setPosition(sf::Vector2f(x,y));
        // frame
        sf::FloatRect rect = text.getLocalBounds();
        frame.setFillColor(color);
        frame.setOutlineColor(sf::Color::Black);
        frame.setOutlineThickness(-1);
        frame.setSize(sf::Vector2f(rect.width,2*rect.height));
        frame.setPosition(sf::Vector2f(x,y));
    }
    Button::Button (Render * mainRender, int x, int y, std::string description, sf::Color color, sf::Vector2i size) {
        this->mainRender = mainRender;
        this->x=x;
        this->y=y;
        this->description=description;
        this->width=size.x;
        this->height=size.y;
        // text
        font.loadFromFile("./res/DejaVuSerifCondensed.ttf");
        text.setFont(font);
        text.setString(description);
        text.setCharacterSize(20);
        text.setStyle(sf::Text::Bold);
        text.setFillColor(sf::Color::Black);
        sf::FloatRect rect = text.getLocalBounds();
        float deltaX = (this->width-rect.width)/2;
        float deltaY = (this->height-rect.height-text.getCharacterSize()/2)/2;
        text.setPosition(sf::Vector2f(x+deltaX,y+deltaY));
        // frame
        frame.setFillColor(color);
        frame.setOutlineColor(sf::Color::Black);
        frame.setOutlineThickness(-1);
        frame.setSize(sf::Vector2f(this->width, this->height));
        frame.setPosition(sf::Vector2f(x,y));
    }
    Button::~Button () {}
    bool Button::contains (int x, int y) {
        sf::Vector2f position = frame.getPosition();
        sf::Vector2f size = frame.getSize();
        int left = position.x;
        int right = left+size.x;
        int top = position.y;
        int bottom = top+size.y;
        if ((x>left) & (x<right)) {
            if ((y>top) & (y<bottom)) {
                return true;
            }
        }
        return false;
    }

    void Button::draw() {
        mainRender->Render::getWindow()->draw(frame);
        mainRender->Render::getWindow()->draw(text);
    }

    void Button::setColor (sf::Color color) {
        frame.setFillColor(color);
    }
    std::string Button::getDescription () {
        return this->description;
    }
}