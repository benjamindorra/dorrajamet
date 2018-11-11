#include "Render.h"
#include <SFML/Graphics.hpp>
#include <iostream>

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

    }
    void Render::renderLoop ()
    {
        this->window.setFramerateLimit(60);
        // run the program as long as the window is open
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
                        sf::Vector2f viewSize = viewMap->getSize();
                        sf::Vector2f viewCenter = viewMap->getCenter();
                        std::vector<float> viewPort = viewMap->getViewPort();
                        sf::Vector2u windowSize = window.getSize();
                        // check if click is in the map 
                        // (assumption : the map is always in the bottom right)
                        float x = (float)event.mouseButton.x-float(windowSize.x-1)*viewPort[0];
                        float y = (float)event.mouseButton.y-float(windowSize.y-1)*viewPort[1];
                        if (x>=0. & y>=0.) {
                            // get the position on the colormap
                            unsigned int mapX =  int((viewCenter.x-viewSize.x/2)+x/float(windowSize.x-1)/viewPort[2]*(viewSize.x+7))+1;
                            unsigned int mapY =  int((viewCenter.y-viewSize.y/2)+y/float(windowSize.y-1)/viewPort[3]*(viewSize.y+10))+10;
                            unsigned int pix = colorMap->ColorMap::getPixel(mapX,mapY);
                            std::cout<<"pix: "<<pix<<std::endl;
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
                        if (event.mouseWheelScroll.delta>0) {
                            viewMap->changeZoom(0.9);
                        }
                        else viewMap->changeZoom(1.1);
                        break;
                    }
                }
            }

            // clear the window with black color
            this->window.clear(sf::Color::Black);

            // calls the drawing functions of each object in ToDraw
            for (auto elem : toDraw) {
                elem.second->draw();
            }
            //draw the map
            viewMap->ViewMap::draw();
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
    void Render::loadColormap (ColorMap * colorMap) {
        this->colorMap=colorMap;
    }
    void Render::loadMap (ViewMap * viewMap) {
        this->viewMap=viewMap;
    }
}