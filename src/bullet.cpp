#include "bullet.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <iostream>
#include <string>

Bullet::Bullet()
{
    //Pobranie tekstury z pliku
    if(!texture.loadFromFile("assets/bullet.png"))
    {
        std::cout<<"Nie udało się wczytać tekstury pocisku\n";
    }
    //Ustawienie tekstury
    bullet.setTexture(texture);         //Ustawia teksturę podłpociskuogi
    position = sf::Vector2f(0,0);
    speed = 300;                        //Prędkość pocisku
    owner = -1;                         //Przypisanie właściciela -1 niczyj
    SetMoveDirection(Direction::Up);     //Kierunek góra
    bulletSize = sf::Vector2f(8,8);   //Rozmiar pocisku

    collider = BoxCollision(position,bulletSize);
    active = true;
    destroy = false;
}
Bullet::Bullet(sf::Vector2f startPosition,Direction startDirection,int newOwner)
{
    //Pobranie tekstury z pliku
    if(!texture.loadFromFile("assets/bullet.png"))
    {
        std::cout<<"Nie udało się wczytać tekstury pocisku\n";
    }
    //Ustawienie tekstury
    bullet.setTexture(texture);         //Ustawia teksturę pocisku
    SetPosition(startPosition); 
    speed = 300;                        //Prędkość pocisku
    owner = newOwner;                   //Przypisanie właściciela -1 niczyj
    SetMoveDirection(startDirection);
    bulletSize = sf::Vector2f(8,8);   //Rozmiar pocisku
    collider = BoxCollision(position,bulletSize);
    active = true;
    destroy = false;
}

Bullet::Bullet(sf::Vector2f startPosition,int newOwner)
{
    //Pobranie tekstury z pliku
    if(!texture.loadFromFile("assets/bullet.png"))
    {
        std::cout<<"Nie udało się wczytać tekstury pocisku\n";
    }
    //Ustawienie tekstury
    bullet.setTexture(texture);         //Ustawia teksturę pocisku
    SetPosition(startPosition);
    speed = 0;                          //Prędkość pocisku
    owner = newOwner;                   //Przypisanie właściciela -1 niczyj
    SetMoveDirection(Direction::Up);
    bulletSize = sf::Vector2f(8,8);   //Rozmiar pocisku
    collider = BoxCollision(position,bulletSize);
    active = true;
    destroy = false;
}

Bullet::~Bullet()
{
    //std::cout<<"Pocisk został usunięty\n";
}
void Bullet::Move(sf::Time deltaTime)
{
    sf::Vector2f moveVector = directionVector;
    moveVector.x *= deltaTime.asSeconds() * speed;
    moveVector.y *= deltaTime.asSeconds() * speed;

    bullet.move(moveVector);

    position = bullet.getPosition();  // Ustawiamy pozycję na nową
    collider.UpdateCollision(position);
}
void Bullet::SetMoveDirection(Direction newDirection)
{
    bulletDirection = newDirection;
    switch (bulletDirection)
    {
        case Direction::Up:
            directionVector = sf::Vector2f(0,-1);
            break;
        case Direction::Down:
            directionVector = sf::Vector2f(0,1);
            break;
        case Direction::Right:
            directionVector = sf::Vector2f(1,0);
            break;
        case Direction::Left:
            directionVector = sf::Vector2f(-1,0);
            break;
    }
}
bool Bullet::IsColliding(BoxCollision objectCollider)
{
    if(active)
    {
        return collider.IsColliding(objectCollider);
    }
    return false;
}

BoxCollision Bullet::GetCollider() const
{
    return collider;
}

void Bullet::SetOwner(int newOwner)
{
    owner = newOwner;
}

int Bullet::GetOwnerId() const
{
    return owner;
}
//Zmienia pozycje pocisku NIE AKTUALIZUJE POZYCJI COLLIDERA
void Bullet::SetPosition(sf::Vector2f newPosition)
{
    position = newPosition;
    bullet.setPosition(newPosition);
}
sf::Vector2f Bullet::GetPosition() const
{
    return bullet.getPosition();
}

void Bullet::ChangeActiveState(bool newState)
{
    active = newState;
}

bool Bullet::IsActive() const
{
    return active;
}

void Bullet::MarkToDestroy()
{
    destroy = true;
}

bool Bullet::ShouldBeDestroyed() const
{
    return destroy;
}

void Bullet::Draw(sf::RenderWindow* window)
{
    if(active)
    {
        bullet.setTexture(texture);   //Ustawia teksturę podłogi
        window->draw(bullet);
    }
}

sf::Packet& operator<<(sf::Packet& packet, const Bullet& bullet)
{
    packet << bullet.GetOwnerId() << bullet.GetPosition().x << bullet.GetPosition().y;   // Właściciel pocisku << pozycja pocisku X << pozycja pocisku Y
    return packet;
}

// Funkcja do deserializacji gracza (operator >>)
sf::Packet& operator>>(sf::Packet& packet, Bullet& bullet)
{
    int bulletOwner;
    float posX, posY;

    packet >> bulletOwner >> posX >> posY;

    bullet.SetOwner(bulletOwner);
    bullet.SetPosition(sf::Vector2f(posX, posY));

    return packet;
}