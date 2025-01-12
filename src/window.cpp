#include "window.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>

//Tworzy i zwraca okno gry
sf::RenderWindow* createWindow()
{
    sf::RenderWindow* Window = new sf::RenderWindow(sf::VideoMode(800, 800), "Gra");
    return Window;
}

sf::RenderWindow* createWindow(std::string windowTitle)
{
    sf::RenderWindow* Window = new sf::RenderWindow(sf::VideoMode(800, 800), windowTitle);
    return Window;
}
