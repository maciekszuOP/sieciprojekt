#ifndef UI_H
#define UI_H

#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Network.hpp>
#include <iostream>
#include <string>
#include <vector>
#include "player.hpp"

class UI
{
    public:
        UI();
        void AddToList(int amount);                                                 //Dodaje do listę obiekty tekstów o podanej ilości                 
        void SetTimer(int timeLeft);                                                //Ustawia czas na czasomierzu
        void SetGameTimer(int timeLeft);                                            //Ustawia czas na czasomierzu gry             
        void SetNumberOfPlayers(int number);                                        //Ustawia liczbę graczy w UI
        void DrawUI(sf::RenderWindow* window, std::vector<Player*> Players);        //Rysuje UI
        void DrawGameUI(sf::RenderWindow* window, std::vector<Player*> Players);    //Rysuje UI do gry
        void DrawResultUI(sf::RenderWindow* window, std::vector<Player*> Players);  //Rysuje UI podsumowania gry

    private:
    sf::Font font;

    sf::Text header;
    sf::Text timer;
    sf::Text gameTimer;
    sf::Text playerNumber;
    sf::RectangleShape rectangle;
    std::vector<sf::Text> scoreboard;
    std::vector<sf::Text> textObjects;
    std::vector<sf::Text> statsObjects;
};


#endif
