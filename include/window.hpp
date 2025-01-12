#ifndef WINDOW_H
#define WINDOW_H
#include<SFML/Graphics.hpp>

// Deklaracja funkcji tworzącej okno
sf::RenderWindow*  createWindow();
sf::RenderWindow*  createWindow(std::string windowTitle);

#endif // WINDOW_H