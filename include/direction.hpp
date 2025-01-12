#ifndef DIRECTION_H
#define DIRECTION_H
#include <SFML/Network.hpp>
#include <string>


enum class Direction
{
    Up,
    Down,
    Left,
    Right,
};

std::string DirectionToString(Direction direction);

// Operator do serializacji Direction
sf::Packet& operator<<(sf::Packet& packet, const Direction& direction);

// Operator do deserializacji Direction
sf::Packet& operator>>(sf::Packet& packet, Direction& direction);

#endif 