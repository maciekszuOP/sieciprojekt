#include "UI.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <iostream>
#include <string>

UI::UI()
{

    if (!font.loadFromFile("assets/Baloo-Regular.ttf")) // Zastąp "arial.ttf" własną ścieżką do czcionki
    {
        std::cout<<"Nie udało się odczytać czcionki"<<std::endl;
        return; // Wyjście, jeśli czcionka nie zostanie załadowana
    }

    // UI w trakcie czekania
    header.setFont(font);
    header.setCharacterSize(24); 
    header.setFillColor(sf::Color::Black);
    header.setString("Lista Graczy");
    header.setPosition(240, 155); 

    timer.setFont(font);
    timer.setCharacterSize(24); 
    timer.setFillColor(sf::Color::Black);
    timer.setString("Czekanie");
    timer.setPosition(450, 155);

    //UI w trakcie gry
    gameTimer.setFont(font);
    gameTimer.setCharacterSize(24); 
    gameTimer.setFillColor(sf::Color::White);
    gameTimer.setString("Czekanie");
    gameTimer.setPosition(380, 1);

    playerNumber.setFont(font);
    playerNumber.setCharacterSize(24);
    playerNumber.setFillColor(sf::Color::White);
    playerNumber.setString("Liczba graczy: ");
    playerNumber.setPosition(10, 1); 

    for(auto i=0;i<5;i++)
    {
        //Tekst nazwy gracza i jego wynik
        sf::Text text;
        text.setFont(font);
        text.setCharacterSize(18); // Rozmiar czcionki
        text.setFillColor(sf::Color::White);
        text.setString("Player :1 ");
        text.setPosition(10, 25 + i * 18); // Ustawienie pozycji tekstu wewnątrz prostokąta
        scoreboard.push_back(text);
    }

    // Tworzenie prostokąta
    rectangle.setSize(sf::Vector2f(400, 450));          
    rectangle.setFillColor(sf::Color(200, 200, 200)); 
    rectangle.setOutlineColor(sf::Color::Black);
    rectangle.setOutlineThickness(2);
    rectangle.setPosition(200, 150); 
}
    

void UI::AddToList(int amount)
{
    for (size_t i = 0; i < amount; i++) 
    {
        //Tekst nazwy gracza
        sf::Text text;
        text.setFont(font);
        text.setCharacterSize(18); // Rozmiar czcionki
        text.setFillColor(sf::Color::Black);
        text.setPosition(240, 185 + i * 24); // Ustawienie pozycji tekstu wewnątrz prostokąta
        textObjects.push_back(text);
        
        //Tekst statystyk gracza
        sf::Text stats;
        stats.setFont(font);
        stats.setCharacterSize(18); // Rozmiar czcionki
        stats.setFillColor(sf::Color::Black);
        stats.setPosition(400, 185 + i * 24); // Ustawienie pozycji tekstu wewnątrz prostokąta
        statsObjects.push_back(stats);
    }
}

void UI::SetTimer(int timeLeft)
{
    if(timeLeft != -1)
    {
        timer.setString(std::to_string(timeLeft));
    }
    else
    {
        timer.setString("Czekanie");
    }
}

void UI::SetGameTimer(int timeLeft)
{
    if(timeLeft != -1)
    {
        gameTimer.setString(std::to_string(timeLeft));
    }
    else
    {
        gameTimer.setString("Czekanie");
    }
}

void UI::SetNumberOfPlayers(int number)
{
    playerNumber.setString("Liczba graczy: "+std::to_string(number));
}

void UI::DrawUI(sf::RenderWindow* window, std::vector<Player*> Players)
{
    window->draw(rectangle);
    window->draw(timer);
    header.setString("Lista Graczy");
    window->draw(header);
    for (size_t i = 0; i < textObjects.size(); i++)
    {
        if(!Players[i]->isPlayerActive())
        {
            continue;
        }
        Players[i]->DrawPlayer(window,sf::Vector2f(210, 185 + (i) * 24));
        textObjects[i].setString(Players[i]->GetName());
        window->draw(textObjects[i]);
    }
}

void UI::DrawGameUI(sf::RenderWindow* window, std::vector<Player*> Players)
{
    window->draw(gameTimer);
    window->draw(playerNumber);
    for (size_t i = 0; i < scoreboard.size(); i++)
    {
        if(!Players[i]->isPlayerActive())
        {
            break;
        }
        scoreboard[i].setString(Players[i]->GetName()+": "+std::to_string(Players[i]->GetKills()));
        window->draw(scoreboard[i]);
    }
}

void UI::DrawResultUI(sf::RenderWindow* window, std::vector<Player*> Players)
{
    window->draw(rectangle);
    window->draw(timer);
    header.setString("Wygrywa: "+Players[0]->GetName()+" ("+std::to_string(Players[0]->GetKills())+")");
    window->draw(header);
    for (size_t i = 0; i < textObjects.size(); i++)
    {
        if(!Players[i]->isPlayerActive())
        {
            continue;
        }
        Players[i]->DrawPlayer(window,sf::Vector2f(210, 185 + (i) * 24));
        textObjects[i].setString(Players[i]->GetName());
        statsObjects[i].setString("Zabojstwa/Smierci: " + std::to_string(Players[i]->GetKills()) + "/" + std::to_string(Players[i]->GetDeaths()));

        window->draw(textObjects[i]);
        window->draw(statsObjects[i]);
    }
}