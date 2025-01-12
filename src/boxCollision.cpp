#include "boxCollision.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <iostream>
#include <string>

BoxCollision::BoxCollision()
{
    CurrentPosition = sf::Vector2f(0,0);
    boxSize = sf::Vector2f(0,0);

    top = 0;
    bottom = 0;
    left = 0;
    right = 0;
}
BoxCollision::BoxCollision(sf::Vector2f position, sf::Vector2f size)
{
    boxSize = size;
    UpdateCollision(position);
    //std::cout<<CurrentPosition.x<<" : "<<CurrentPosition.y<<std::endl;  
}

BoxCollision::BoxCollision(int posX,int posY,int sizeX,int sizeY)
{
    boxSize = sf::Vector2f(sizeX * 32, sizeY * 32);
    sf::Vector2f position = sf::Vector2f(posX * 32, posY * 32);
    UpdateCollision(position);
    //std::cout<<CurrentPosition.x<<" : "<<CurrentPosition.y<<std::endl;  
}

void BoxCollision::UpdateCollision(sf::Vector2f newPosition)
{
    CurrentPosition = newPosition;
    top = CurrentPosition.y;
    bottom = CurrentPosition.y + boxSize.y;
    left = CurrentPosition.x;
    right = CurrentPosition.x + boxSize.x;
    //std::cout<<CurrentPosition.x<<" : "<<CurrentPosition.y<<std::endl;  
}

bool BoxCollision::IsColliding(BoxCollision Collider)
{
    if (right < Collider.left || left > Collider.right || bottom < Collider.top || top > Collider.bottom)
    {
        return false;  // Brak kolizji
    }
    return true;  // Kolizja
}

sf::Vector2f BoxCollision::GetSize()
{
    return boxSize;
}

void BoxCollision::printBox()
{
    std::cout<<"collision "<<left<<" : "<<right<<" : "<<top<<" : "<<bottom<<std::endl;
}