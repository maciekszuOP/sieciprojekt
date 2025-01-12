#ifndef PLAYER_H
#define PLAYER_H

#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Network.hpp>
#include <iostream>
#include <string>
#include "boxCollision.hpp"
#include "direction.hpp"
#include "bullet.hpp"

#pragma once

class Player
{
    public:
        Player();                  //Konstruktor                                                                              
        Player(int newId);         //Konstruktor przypisujący id

        //Przesuwa gracza na podstawie delty time oraz kolizji
        void Move(sf::Vector2f moveVector, sf::Time deltaTime,const std::vector<BoxCollision>& obstacles);

        bool CanShoot();                                                        //Zwraca czy gracz może strzelać
        Bullet Shoot();                                                         //Tworzy pocisk w miejscu zależnym od kierunku

        void SetDirection(sf::Vector2f moveVector);                             //Ustawia kierunek na podstawie vectora kierunku
        void SetDirection(Direction newDirection);                              //Ustawia kierunek na podstawie typu Direction

        void DrawPlayer(sf::RenderWindow* Window);                              //Rysuje gracza na podstawie pozycji obiektu gracza
        void DrawPlayer(sf::RenderWindow* Window,sf::Vector2f drawPosition);    //Rysuje gracza na podstawie pozycji obiektu gracza

        void SetName(std::string newName);                                      //Ustawia nick gracza
        void SetId(int id);                                                     //Ustawia Id gracza
        void SetState(bool active);                                             //Ustawia stan aktywności gracza
        void ResetStats();                                                      //Resetuje statystyki gracza

        void SetPosition(sf::Vector2f newPosition);                             //Ustawia pozycje w podanym miejscu
        void SetPositionOnTiles(sf::Vector2f newPosition);                      //Ustawia pozycje gracza względem siatki kafelek

        void Respawn(sf::Vector2f newPosition);                                 //Przenosi gracza w nowe miejsce i zwiększa statystykę śmierci
        void RespawnOnTiles(sf::Vector2i newPosition);                          //Przenosi gracza w nowe miejsce (na podstawie siatki kafelek) i zwiększa statystykę śmierci

        void IncrementKill();                                                   //Zwiększa liczbę zabójstw
        void SetKills(int value);                                               //Ustawia wartość zabójstw 
        void SetDeaths(int value);                                              //Ustawia wartość śmierci

        bool IsColliding(BoxCollision objectCollider);                          //Sprawdza czy gracz koliduje z innym hitboxem
        std::string GetName() const;                                            //Zwraca nick gracza
        int GetId() const;                                                      //Zwraca Id gracza
        bool isPlayerActive() const;                                            //Zwraca stan aktywności gracza
        BoxCollision GetCollider() const;                                       //Zwraca hitbox gracza
        sf::Vector2f GetPosition() const;                                       //Zwraca pozycję gracza
        Direction GetDirection() const;                                         //Zwraca kierunek gracza
        sf::Vector2f GetCenterPosition() const;                                 //Zwraca pozycję środka gracza
        int GetKills() const;                                                   //Zwraca liczbę zabójstw
        int GetDeaths() const;                                                  //Zwraca liczbę śmierci
        int GetRocksDestroyed() const;                                          //Zwraca liczbę zniszczonych kamieni

        void PlayerInfo() const;                                                //Wypisuje informacje o graczu


    private:
        sf::Texture playerTexture;     // Tekstura gracza
        sf::Sprite playerImage;        // Obraz gracza
        sf::Vector2f position;         // Pozycja gracza
        sf::Vector2f playerSize;       // Rozmiar gracza

        BoxCollision collider;         // Hitbox gracza
        
        float speed;                   // Prędkość gracza
        Direction direction;           // Kierunek ruchu gracza
        sf::Clock shootCooldown;       // Zegar uniemożliwiający ciągłe strzelanie
        sf::Vector2f bulletPosition;   // Pozycja startowa pocisku 

        bool isActive;                 // Czy gracz jest aktywny
        std::string playerName;        // Nazwa gracza
        int playerId;                  // id gracza
        int kills;                     // Ile punktów (zabójstw) ma gracz
        int deaths;                    // How many times player was killed
        int rockDestroyed;             // Ile kamieni gracz zniszczył
    };

sf::Packet& operator<<(sf::Packet& packet, const Player& player);
sf::Packet& operator>>(sf::Packet& packet, Player& player);

#endif
