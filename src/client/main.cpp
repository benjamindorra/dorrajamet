#include <iostream>
#include <string>
//#include <SFML/Graphics.hpp>



int main(int argc, char ** argv)
{
    std::cout << "argc = " << argc << std::endl;
    if(argc == 2)
    {
        std::string str(argv[1]);
        if(str == "hello")
            std::cout << "Hello World!" << std::endl;
    }

    /*sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(shape);
        window.display();
    }*/

    return 0;
}