#ifndef BULLET_H
#define BULLET_H

#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Network.hpp>
#include <iostream>
#include <string>
#include <vector>
#include "boxCollision.hpp"
#include "direction.hpp"

class Bullet
{
    public:
        Bullet();
        Bullet(sf::Vector2f startPosition,Direction startDirection,int newOwner);   //Tworzenie pocisku na wybranych współrzędnych
        Bullet(sf::Vector2f startPosition,int newOwner);                            //Tworzenie pocisku bez kierunku UŻYWANE TYLKO DO RYSOWANIA

        ~Bullet();                                      //Dekonstruktor (powstał do debugowania)
        void Move(sf::Time deltaTime);                  //Porusza pocisk
        void SetMoveDirection(Direction newDirection);  //Ustawia kierunek poruszania się
        bool IsColliding(BoxCollision objectCollider);  //Sprawdza czy inny hitbox koliduje z hitboxem pocisku
        BoxCollision GetCollider() const;               //Zwraca hitbox
        void MarkToDestroy();                           //Oznacza pocisk do usunięcia

        void SetOwner(int newOwner);                    //Ustawia id właściciela
        int GetOwnerId() const;                         //Zwraca Id właściciela
        
        void SetPosition(sf::Vector2f newPosition);     //Zmienia pozycje pocisku NIE AKTUALIZUJE POZYCJE COLLIDERA (hitboxa)
        sf::Vector2f GetPosition() const;               //Zwraca obecna pozycję
        
        void ChangeActiveState(bool newState);          //Zmienia stan aktywności pocisku
        bool IsActive() const;                          //Sprawdza czy pocisk jest aktywny
        bool ShouldBeDestroyed() const;                 //Sprawdza czy pocisk został oznaczony do usunięcia
        
        void Draw(sf::RenderWindow* window);            //Rysuje pocisk

    private:
        sf::Texture texture;            //Textura pocisku
        sf::Sprite bullet;              //Sprite pocisku

        BoxCollision collider;          //Hitbox pocisku
        Direction bulletDirection;      //Kierunek pocisku

        float speed;                    //Prędkość pocisku
        
        int owner;
        sf::Vector2f bulletSize;        //Rozmiar pocisku

        sf::Vector2f position;          
        sf::Vector2f directionVector;   //Wektor kierunku

        bool active;                    //Czy pocisk jest aktywny
        bool destroy;                   //Czy pocisk powinien być zniszczony
};

//Do wysyłyania danych o pocisku przez sieć
sf::Packet& operator<<(sf::Packet& packet, const Bullet& bullet);       //Serializacja pocisku (operator <<)
sf::Packet& operator>>(sf::Packet& packet, Bullet& bullet);             //Deserializacja pocisku (operator >>)

#endif
