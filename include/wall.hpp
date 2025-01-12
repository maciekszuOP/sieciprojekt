#ifndef WALL_H
#define WALL_H

#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <iostream>
#include <string>
#include <vector>
#include "boxCollision.hpp"

class Wall
{
    public:
        Wall();
        Wall(sf::Vector2i Position);
        Wall(int x, int y);
        bool IsColliding(BoxCollision objectCollider);
        BoxCollision GetCollider() const;
        void Draw(sf::RenderWindow* window);

    private:
        sf::Texture texture;
        sf::Sprite wall;

        BoxCollision collider;         // Hitbox ściany

        sf::Vector2i tilePosition;
        sf::Vector2f drawPosition;
};
#endif
