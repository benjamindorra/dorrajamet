#include "Window.h"
#include <SFML/Graphics.hpp>

namespace render {
    Window::Window () : Window(1920, 1080) {}
    Window::Window (int width, int height) 
    {
        this->width=width;
        this->height=height;
        // draw the window
        this->window.create(sf::VideoMode(this->width, this->height), "Window", sf::Style::Close);
    }
    void Window::renderLoop ()
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

            // draw everything here...
            // window.draw(...);

            // end the current frame
            this->window.display();
        }
    }
    // Setters and Getters
    int Window::getWidth ()
    {
        return this->width;
    }
    int Window::getHeight ()
    {
        return this->height;
    }
}