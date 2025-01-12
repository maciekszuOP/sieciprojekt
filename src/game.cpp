#include "game.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>

//
//Tworzy i zwraca okno gry
int EventLoop(sf::RenderWindow* Window)
{
    sf::Event Event;

    while(Window->pollEvent(Event))
    {
        //Zamknięcie okna
        switch(Event.type)
        {
            case sf::Event::Closed:
                Window->close();
                return -1;
            case sf::Event::MouseButtonPressed:
                if(Event.mouseButton.button == sf::Mouse::Left)
                std::cout<<"x="<<Event.mouseButton.x<<": y="<<Event.mouseButton.y<<std::endl;
        }
    }
    return 0;
}