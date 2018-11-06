#include "Render.h"
#include <SFML/Graphics.hpp>

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
        // run the program as long as the window is open
        while (this->window.isOpen())
        {
            // check all the window's events that were triggered since the last iteration of the loop
            sf::Event event;
            while (this->window.pollEvent(event))
            {
                // "close requested" event: we close the window
                if (event.type == sf::Event::Closed)
                    this->window.close();
            }

            // clear the window with black color
            this->window.clear(sf::Color::Black);

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
}