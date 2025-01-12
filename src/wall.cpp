#include "wall.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <iostream>
#include <string>

Wall::Wall()
{
    //Pobranie tekstury z pliku
    if(!texture.loadFromFile("assets/bricks.png"))
    {
        std::cout<<"Nie udało się wczytać tekstury cegieł\n";
    }
    //Ustawienie tekstury
    wall.setTexture(texture);   //Ustawia teksturę podłogi
    tilePosition = sf::Vector2i(0,0);
    drawPosition = sf::Vector2f(0,0);
    collider = BoxCollision(drawPosition,sf::Vector2f(32.0f,32.0f));
}

Wall::Wall(sf::Vector2i Position)
{
    //Pobranie tekstury z pliku
    if(!texture.loadFromFile("assets/bricks.png"))
    {
        std::cout<<"Nie udało się wczytać tekstury cegieł\n";
    }
    //Ustawienie tekstury
    wall.setTexture(texture);   //Ustawia teksturę podłogi
    tilePosition = Position;
    drawPosition.x = 32 * tilePosition.x;
    drawPosition.y = 32 * tilePosition.y;
    collider = BoxCollision(drawPosition,sf::Vector2f(32.0f,32.0f));
}

Wall::Wall(int x, int y)
{
    //Pobranie tekstury z pliku
    if(!texture.loadFromFile("assets/bricks.png"))
    {
        std::cout<<"Nie udało się wczytać tekstury cegieł\n";
    }
    //Ustawienie tekstury
    wall.setTexture(texture);   //Ustawia teksturę podłogi
    tilePosition = sf::Vector2i(x,y);
    drawPosition.x = 32 * tilePosition.x;
    drawPosition.y = 32 * tilePosition.y;
    collider = BoxCollision(drawPosition,sf::Vector2f(32.0f,32.0f));
}

bool Wall::IsColliding(BoxCollision objectCollider)
{
    if(collider.IsColliding(objectCollider))
    {
        std::cout<<"Kolizja\n";
    }
    return collider.IsColliding(objectCollider);
}

BoxCollision Wall::GetCollider() const
{
    return collider;
}

void Wall::Draw(sf::RenderWindow* window)
{
    wall.setTexture(texture);   //Ustawia teksturę podłogi
    wall.setPosition(drawPosition);
    window->draw(wall);
}