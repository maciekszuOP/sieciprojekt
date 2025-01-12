#include "direction.hpp"

std::string DirectionToString(Direction direction)
{
    switch (direction)
    {
        case Direction::Up:    return "Up";
        case Direction::Down:  return "Down";
        case Direction::Left:  return "Left";
        case Direction::Right: return "Right";
        default:               return "Unknown";
    }
}

// Operator do serializacji Direction
sf::Packet& operator<<(sf::Packet& packet, const Direction& direction)
{
    // Serializujemy numer enum (int), bo enum jest bazowo liczbą całkowitą
    return packet << static_cast<int>(direction);
}

// Operator do deserializacji Direction
sf::Packet& operator>>(sf::Packet& packet, Direction& direction)
{
    int directionInt;
    packet >> directionInt;
    direction = static_cast<Direction>(directionInt);
    return packet;
}