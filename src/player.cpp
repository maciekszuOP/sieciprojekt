#include "player.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <iostream>
#include <string>

Player::Player()
{
    //Pobranie tekstury z pliku
    if(!playerTexture.loadFromFile("assets/player24.png"))
    {
        std::cout<<"Nie udało się wczytać tekstury gracza\n";
    }
    //Ustawienie tekstury
    playerImage.setTexture(playerTexture);   //Ustawia teksturę gracza

    //Ustawienie widocznej części sprite'a
    playerImage.setTextureRect(sf::IntRect(0, 0, 24, 24));

    //Ustawienie pozycji startowej
    isActive = false;
    position = sf::Vector2f(64,64);
    direction = Direction::Up;
    speed = 200;
    playerId = -1;
    playerName = "Player";
    playerSize = sf::Vector2f(24.0f,24.0f);
    collider = BoxCollision(position,playerSize);

    ResetStats();
}

Player::Player(int newId)
{
    std::string path = "assets/Player" + std::to_string(newId) + ".png";
    //Pobranie tekstury z pliku
    if(!playerTexture.loadFromFile(path))
    {
        std::cout<<"Nie udało się wczytać tekstury gracza\n";
    }
    //Ustawienie tekstury
    playerImage.setTexture(playerTexture);   //Ustawia teksturę gracza

    //Ustawienie widocznej części sprite'a
    playerImage.setTextureRect(sf::IntRect(0, 0, 24, 24));

    //Ustawienie pozycji startowej
    isActive = false;
    position = sf::Vector2f(64,64);
    direction = Direction::Up;
    speed = 200;
    playerId = newId;
    playerName = "Player";
    playerSize = sf::Vector2f(24.0f,24.0f);
    collider = BoxCollision(position,playerSize);
    ResetStats();
}

void Player::Move(sf::Vector2f moveVector, sf::Time deltaTime,const std::vector<BoxCollision>& obstacles)
{
    //std::cout<<deltaTime.asSeconds()<<std::endl;
    
    //Potencjalny nowy wektor przeniesienia
    moveVector.x *= deltaTime.asSeconds() * speed;
    moveVector.y *= deltaTime.asSeconds() * speed;

    SetDirection(moveVector);

    //Obliczanie potencjalnych nowych pozycji
    sf::Vector2f potentialPositionX = position + sf::Vector2f(moveVector.x, 0);
    sf::Vector2f potentialPositionY = position + sf::Vector2f(0, moveVector.y);

    //Tworzenie tymczasowych kolizji do testowania
    BoxCollision testColliderX(potentialPositionX, collider.GetSize());
    BoxCollision testColliderY(potentialPositionY, collider.GetSize());

    //Sprawdzanie kolizji na osi X
    for (const BoxCollision& obstacle : obstacles)
    {
        if (testColliderX.IsColliding(obstacle))
        {
            moveVector.x = 0; //Zablokowanie ruchu w osi X
            break;
        }
    }

    //Sprawdzanie kolizji na osi Y
    for (const BoxCollision& obstacle : obstacles)
    {
        if (testColliderY.IsColliding(obstacle))
        {
            moveVector.y = 0; ///Zablokowanie ruchu w osi Y
            break;
        }
    }

    //Zaktualizowanie pozycji gracza
    sf::Vector2f finalMove = sf::Vector2f(moveVector.x, moveVector.y);
    position += finalMove;
    playerImage.move(finalMove);
    
    //Zaktualizuowanie collidera gracza
    collider.UpdateCollision(position);
}

bool Player::CanShoot()
{
    sf::Time timePassed = shootCooldown.getElapsedTime();                            //czas, jaki minął od ostaniego strzału
    return timePassed.asSeconds() >= 0.5;
}

Bullet Player::Shoot()
{
    shootCooldown.restart();
    return Bullet(bulletPosition, direction,playerId);
}

void Player::SetDirection(sf::Vector2f moveVector)
{
    if(moveVector == sf::Vector2f(0,0))
    {
        return;
    }

    int currentFrame = 0;  // Zmienna do kontrolowania aktualnej klatki
    sf::Vector2f shootPos = GetCenterPosition();
    //Określenie kierunku na podstawie moveVector
    if (moveVector.y > 0)
    {
        direction = Direction::Down;
        currentFrame = 2;
        shootPos.y += playerSize.y;
    }
    else if (moveVector.y < 0)
    {
        direction = Direction::Up;
        currentFrame = 0;
        shootPos.y -= playerSize.y;
    }
    if (moveVector.x > 0)
    {
        direction = Direction::Right;
        currentFrame = 3;
        shootPos.x += playerSize.x;
    }
    else if (moveVector.x < 0)
    {
        direction = Direction::Left;
        currentFrame = 1;
        shootPos.x -= playerSize.x;
    }

    bulletPosition = shootPos;

    playerImage.setTextureRect(sf::IntRect(0, currentFrame * 24, 24, 24));
}

void Player::SetDirection(Direction newDirection)
{
    //std::cout<<"SetDirection: "<<DirectionToString(newDirection)<<std::endl;
    sf::Vector2f moveVector = sf::Vector2f(0,1);
    if(newDirection == Direction::Down)
    {
        moveVector = sf::Vector2f(0,1);
    }
    else if(newDirection == Direction::Up)
    {
        moveVector = sf::Vector2f(0,-1);
    }
    else if(newDirection == Direction::Left)
    {
        moveVector = sf::Vector2f(-1,0);
    }
    else if(newDirection == Direction::Right)
    {
        moveVector = sf::Vector2f(1,0);
    }
    SetDirection(moveVector);
}


void Player::DrawPlayer(sf::RenderWindow* Window)
{
    if(isActive)
    {
        Window->draw(playerImage);
    }
}

void Player::DrawPlayer(sf::RenderWindow* Window,sf::Vector2f drawPosition)
{
    playerImage.setPosition(drawPosition);
    Window->draw(playerImage);
    playerImage.setPosition(position);
}

void Player::SetName(std::string newName)
{
    playerName = newName;
}

void Player::SetId(int id)
{
    playerId = id;
}

void Player::SetState(bool active)
{
    isActive = active;
}

void Player::ResetStats()
{
    kills = 0;                    
    deaths = 0;                    
    rockDestroyed = 0;             
}
//Ustawia pozycję gracza
void Player::SetPosition(sf::Vector2f newPosition)
{
    playerImage.setPosition(newPosition);  
    position = newPosition; 
    collider.UpdateCollision(position);
}
//Ustawia pozycję gracza względem kafelek
void Player::SetPositionOnTiles(sf::Vector2f newPosition)
{
    newPosition.x*=32;
    newPosition.y*=32;
    playerImage.setPosition(newPosition);  
    position = newPosition; 
    collider.UpdateCollision(position);
}

//Odradza gracza na podanych współrzędnych
void Player::Respawn(sf::Vector2f newPosition)
{
    SetPosition(newPosition);
    deaths+=1;
}
//Odradza gracza względem podanych kafelek
void Player::RespawnOnTiles(sf::Vector2i newPosition)
{
    sf::Vector2f newPositionFloat;
    newPositionFloat.x = newPosition.x;
    newPositionFloat.y = newPosition.y;
    SetPositionOnTiles(newPositionFloat);
    deaths+=1;
}

void Player::IncrementKill()
{
    kills +=1;
}

void Player::SetKills(int value)
{
    kills = value;
}                                
void Player::SetDeaths(int value)
{
    deaths = value;
}                                           

bool Player::IsColliding(BoxCollision objectCollider)
{
    if(!isActive)
    {
        return false;
    }
    return collider.IsColliding(objectCollider);
}

std::string Player::GetName() const
{
    return playerName;
}

int Player::GetId() const
{
    return playerId;
}

bool Player::isPlayerActive() const
{
    return isActive;
}

BoxCollision Player::GetCollider() const
{
    //collider.printBox();
    return collider;
}

sf::Vector2f Player::GetPosition() const
{
    return playerImage.getPosition();
}

Direction Player::GetDirection() const
{
    return direction;
}

sf::Vector2f Player::GetCenterPosition() const
{
    sf::Vector2f Center = GetPosition();
    Center.x += playerSize.x/2;
    Center.y += playerSize.y/2;
    return Center;
    
}

int Player::GetKills() const
{
    return kills;
}

int Player::GetDeaths() const
{
    return deaths;
}

int Player::GetRocksDestroyed() const
{
    return rockDestroyed;
}

void Player::PlayerInfo() const
{
    std::cout << "Gracz: " << GetId() << " " << GetName() 
        << " Pozycja: (" << position.x << ", " << position.y << ") "
        << " Kierunek: "<<DirectionToString(GetDirection()) << ", " << isPlayerActive() 
        << " Statystyki: K: "<<GetKills() <<" D: "<< GetDeaths() << std::endl;
}

// Funkcja do serializacji gracza (operator <<)
sf::Packet& operator<<(sf::Packet& packet, const Player& player)
{
    packet << player.GetId()           // ID gracza
           << player.GetName()         // Nazwa gracza
           << player.GetPosition().x   // Pozycja X gracza
           << player.GetPosition().y   // Pozycja Y gracza
           << player.isPlayerActive()  // Czy gracz jest aktywny
           << player.GetDirection()    // Kierunek gracza
           << player.GetKills()        // Zabójstwa gracza
           << player.GetDeaths();      // Śmierci gracza
    return packet;
}

// Funkcja do deserializacji gracza (operator >>)
sf::Packet& operator>>(sf::Packet& packet, Player& player)
{
    int id, kills, deaths;
    std::string name;
    float posX, posY;
    bool active;
    Direction playerDirection;

    packet >> id >> name >> posX >> posY >> active >> playerDirection >> kills >> deaths;

    player.SetId(id);
    player.SetName(name);
    player.SetPosition(sf::Vector2f(posX, posY));
    player.SetState(active);
    player.SetDirection(playerDirection);
    player.SetKills(kills);
    player.SetDeaths(deaths);

    return packet;
}