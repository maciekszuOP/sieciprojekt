#ifndef MAPTEMPLATE_H
#define MAPTEMPLATE_H

#include <iostream>
#include <string>
#include <vector>
#include "boxCollision.hpp"

class MapTemplate
{
    public:
    MapTemplate(int mapId);
    void RotateLines(); 
    int GetSizeX();
    int GetSizeY();
    std::vector<std::string> GetMap();
    std::string GetLine(int index);
    std::vector<BoxCollision> GetColliders();

    private:
    int mapNumber;
    int sizeX;
    int sizeY;
    std::vector<std::string> line;
    std::vector<BoxCollision> colliders;
};
#endif

//f r a b c d   