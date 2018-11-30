/*********************************************************
Render.cpp
Main class of render. Control the rendering of the other
objects and handle the events.
**********************************************************/
#include "Render.h"
#include <SFML/Graphics.hpp>
#include <json.hpp>
#include <iostream>

using json = nlohmann::json;

namespace render {
    Render::Render () {
        // draw the window
        this->window.create(sf::VideoMode(1920, 1080), "Window", sf::Style::Close);
    }
    Render::Render (int width, int height) {
        // draw the window
        this->window.create(sf::VideoMode(width, height), "Window", sf::Style::Close);
    }
    Render::~Render () {
        if((this->data != nullptr) & (this->playerData != nullptr) & (this->viewMap != nullptr)) {
            delete data;
            delete playerData;
            delete viewMap;
        }
    }

    void Render::init(state::GameState * gameState, engine::EngineCore * gameEngine) {
        // create objects
        // create objects to interact with state and engine
        this->state = ToState(gameState);
        this->engine = ToEngine(gameEngine);
        // create the viewmap
        this->viewMap = new ViewMap(this, &state, &engine);
        // sf::Vector2f(224.f, 162.f), sf::Vector2f(200.f,200.f),
        // create the colormap
        this->colorMap = ColorMap("./res/testMap.bmp");
        // show data
        this->data = new Data(this, &state, &engine);
        // show player data
        this->playerData = new PlayerData(this, &state);
        std::cout<<"Render init OK"<<std::endl;
    }

    // main loop, handles render and events
    void Render::renderLoop ()
    {   
        // end turn button
        Button endTurn(this, window.getSize().x*0.8, 0, "end turn", sf::Color::Red, sf::Vector2i(window.getSize().x*0.2, window.getSize().y*0.1));
        // run the program as long as the window is open
        this->window.setFramerateLimit(60);
        while (this->window.isOpen())
        {
            // check all the window's events that were triggered since the last iteration of the loop
            sf::Event event;
            while (this->window.pollEvent(event))
            {
                // check the type of the event...
                switch (event.type){
                // "close requested" event: we close the window
                    case sf::Event::Closed:
                        this->window.close();
                        break;

                    // mouse click
                    case sf::Event::MouseButtonPressed: {
                        // get color of the pixel clicked in the colorMap
                        int x = event.mouseButton.x;
                        int y = event.mouseButton.y;
                        // check if click is in the data
                        if (data->contains(x, y)) {
                            if (event.mouseButton.button==sf::Mouse::Button::Left) {
                                data->click(x,y);
                            }
                        }
                        // check if click is in the end turn button
                        else if (endTurn.contains(x,y)) {
                            if (event.mouseButton.button==sf::Mouse::Button::Left) {
                                engine.addCommand("endTurn", "");
                            }
                        }
                        // check if click is in the map
                        else if (viewMap->contains(x, y)) {
                            sf::Vector2f pixel = window.mapPixelToCoords (sf::Vector2i(x,y), *viewMap->getView());
                            if (event.mouseButton.button==sf::Mouse::Button::Left) {
                                std::string leftclick = viewMap->leftClick(pixel);
                                if (leftclick=="province") {
                                    unsigned int mapX = (unsigned int)pixel.x;
                                    unsigned int mapY = (unsigned int)pixel.y;
                                    unsigned int pix = colorMap.ColorMap::getPixel(mapX,mapY);
                                    std::cout<<"pix: "<<pix<<std::endl;
                                    std::cout<<"x: "<<mapX<<"y: "<<mapY<<std::endl;
                                    data->select(Data::Province, std::to_string(pix));
                                }
                                else {
                                    std::cout<<"army !"<<std::endl;
                                    data->select(Data::Army, leftclick);
                                }

                            }
                            if (event.mouseButton.button==sf::Mouse::Button::Right) {
                                std::string rightclick = viewMap->rightClick(pixel);
                                if (rightclick=="province") {
                                    unsigned int mapX = (unsigned int)pixel.x;
                                    unsigned int mapY = (unsigned int)pixel.y;
                                    unsigned int pix = colorMap.ColorMap::getPixel(mapX,mapY);
                                    std::cout<<"pix: "<<pix<<std::endl;
                                    std::cout<<"x: "<<mapX<<"y: "<<mapY<<std::endl;
                                    std::string text = "William Thomson\n"
                                    "Titles:\nDuchy of Wales\n\nStatistics:\nIntelligence 10\nFighting 2\n\nTraits:\nGluttony\n";
                                    data->Data::select(Data::Character, std::to_string(pix));
                                }
                                else {
                                    unsigned int mapX = (unsigned int)pixel.x;
                                    unsigned int mapY = (unsigned int)pixel.y;
                                    std::string pix = std::to_string(colorMap.getPixel(mapX,mapY));
                                    json j;
                                    j["dest"] = pix;
                                    j["id"] = rightclick;
                                    engine.addCommand("moveArmy", j.dump());
                                }
                            }
                        }
                        else std::cout<<"x: "<<x<<"y: "<<y<<std::endl;
                        break;
                    }
                    case sf::Event::KeyPressed: {
                        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
                            viewMap->moveView(10,0);
                        }
                         if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
                            viewMap->moveView(-10,0);
                        }
                        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {
                            viewMap->moveView(0,-10);
                        }
                        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
                            viewMap->moveView(0,10);
                        }
                        break;
                    }
                    case sf::Event::MouseWheelScrolled: {
                        int x = event.mouseWheelScroll.x;
                        int y = event.mouseWheelScroll.y;
                        // scroll the data
                        if (data->contains(x, y)) {
                            if (event.mouseWheelScroll.delta>0) {
                                data->scroll(-10);
                            }
                            else data->scroll(10);
                        }
                        // zoom the map
                        else if (viewMap->contains(x,y)) {
                            if (event.mouseWheelScroll.delta>0) {
                                viewMap->changeZoom(0.9);
                            }
                            else viewMap->changeZoom(1.1);
                        }
                        break;
                    }
                    default: {}
                    }
                }

            // clear the window with black color
            this->window.clear(sf::Color::Black);
            // update
            update();
            viewMap->update();
            data->update();
            playerData->update();
            // draw
            //draw the map
            viewMap->ViewMap::draw();
            // draw the data
            data->draw();
            // draw the playerdata
            playerData->draw();
            // draw end turn button
            endTurn.draw();
            // calls the drawing functions of each object in ToDraw
            for (auto elem : toDraw) {
                elem.second->draw();
            }
            // end the current frame
            this->window.display();
        }
    }
    // Setters and Getters
    sf::RenderWindow* Render::getWindow ()
    {
        return &this->window;
    }
    
    void Render::addToDraw(std::string id, Element * element) {
        this->toDraw[id]=element;
    }
    void Render::removeToDraw(std::string id) {
        this->toDraw.erase(id);
    }
    void Render::update(){}
}