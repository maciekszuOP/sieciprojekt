#ifndef BOXCOLLISION_H
#define BOXCOLLISION_H

#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <iostream>
#include <string>
#include <vector>

class BoxCollision
{
    public:
        BoxCollision();
        BoxCollision(sf::Vector2f position, sf::Vector2f size); //Względem pozycji (w pikselach)
        BoxCollision(int posX,int posY,int sizeX,int sizeY);    //Względem siatki kafelek
        void UpdateCollision(sf::Vector2f newPosition);
        bool IsColliding(BoxCollision Collider);
        void printBox();
        sf::Vector2f GetSize();

    private:
        sf::Vector2f CurrentPosition;
        sf::Vector2f boxSize;

        float top;
        float bottom;
        float left;
        float right;
};
#endif
