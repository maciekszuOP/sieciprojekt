#include "mapTemplate.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>

MapTemplate::MapTemplate(int mapId)
{
    if(mapId == 1)
    {
        std::string oneLine = "+++++++++++++++++++++++++";
        sizeX = oneLine.size();

        //Po usunięciu pętli For występuje wyciek pamięci :(
        line.push_back(oneLine);                        //23
        for(auto i=0;i<23;i++)
        {
            line.push_back("+|||||||||||||||||||||||+");
        }
        line.push_back(oneLine);


                // "0123456789012345678901234"
        line[1] =  "+|||||||||||||||||||||||+";
        line[2] =  "+|S|++|S|++|S|++|S|++|S|+";
        line[3] =  "+|||++|||+|||||+|||++|||+";
        line[4] =  "+|+++++|||||+|||||+++++|+";
        line[5] =  "+|+++++||+|||||+||+++++|+";
        line[6] =  "+||||||||++|||++||||||||+";
        line[7] =  "+|||||||||||S|||||||||||+";
        line[8] =  "+++|||++|||||||||++|||+++";
        line[9] =  "+++|S|++|S|+++|S|++|S|+++";
        line[10] = "+|||||++|||+++|||++|||||+";
        line[11] = "+||||+|||+++++++|||+||||+";
        line[12] = "+||S|+|S|+++++++|S|+|S||+";
        line[13] = "+||||+|||+++++++|||+||||+";
        line[14] = "+|||+++||||+++||||+++|||+";
        line[15] = "+|||+++|S||+++||S|+++|||+";
        line[16] = "+|||||||||||||||||||||||+";
        line[17] = "+|||||||||||||||||||||||+";
        line[18] = "+++|S|++|S||+||S|++|S|+++";
        line[19] = "+++|||++|||||||||++|||+++";
        line[20] = "+++||+++++++|+++++++||+++";
        line[21] = "+|||||++|||||||||++|||||+";
        line[22] = "+|S||||||S||+||S||||||S|+";
        line[23] = "+|||||||||||+|||||||||||+";
                // "0123456789012345678901234"


        sizeY = line.size();

        RotateLines();  //obrót mapy (tak aby rysowanie było tak jak widoczne jest kodzie)
        
        //Ustawianie kolizji
        colliders.push_back(BoxCollision(0,0,25,1));    //Górna ściana
        colliders.push_back(BoxCollision(0,0,1,25));    //Lewa ściana
        colliders.push_back(BoxCollision(0,24,25,25));  //Dolna ściana
        colliders.push_back(BoxCollision(24,1,25,25));  //Dolna ściana
        //Krzyż na środku
        colliders.push_back(BoxCollision(9,11,7,3));  
        colliders.push_back(BoxCollision(11,9,3,7));    //
        //Dolny korytarz
        colliders.push_back(BoxCollision(12,22,1,2));
        colliders.push_back(BoxCollision(6,18,2,4));
        colliders.push_back(BoxCollision(17,18,2,4));
        colliders.push_back(BoxCollision(5,20,7,1));
        colliders.push_back(BoxCollision(13,20,7,1));
        //Aleja Snajperska
        colliders.push_back(BoxCollision(12,18,1,1));
        colliders.push_back(BoxCollision(4,14,3,2));
        colliders.push_back(BoxCollision(18,14,3,2));
        colliders.push_back(BoxCollision(5,11,1,3));
        colliders.push_back(BoxCollision(19,11,1,3));
        colliders.push_back(BoxCollision(6,8,2,3));
        colliders.push_back(BoxCollision(17,8,2,3));
        colliders.push_back(BoxCollision(1,18,2,3));
        colliders.push_back(BoxCollision(22,18,2,3));
        //Górne pudło
        colliders.push_back(BoxCollision(12,4,1,1));
        colliders.push_back(BoxCollision(9,2,1,2));
        colliders.push_back(BoxCollision(9,2,2,1));
        colliders.push_back(BoxCollision(9,5,1,2));
        colliders.push_back(BoxCollision(9,6,2,1));
        colliders.push_back(BoxCollision(14,2,2,1));
        colliders.push_back(BoxCollision(15,2,1,2));
        colliders.push_back(BoxCollision(15,5,1,2));
        colliders.push_back(BoxCollision(14,6,2,1));
        //Lewy róg
        colliders.push_back(BoxCollision(2,4,5,2));
        colliders.push_back(BoxCollision(4,2,2,4));
        colliders.push_back(BoxCollision(1,8,2,2));
        //Prawy róg
        colliders.push_back(BoxCollision(18,4,5,2));
        colliders.push_back(BoxCollision(19,2,2,4));
        colliders.push_back(BoxCollision(22,8,2,2));
    }
    else if(mapId == 2)
    {
        std::string oneLine = "+++++++++++++++++++++++++";
        sizeX = oneLine.size();

        //Po usunięciu pętli For występuje wyciek pamięci :(
        line.push_back(oneLine);                        //23
        for(auto i=0;i<23;i++)
        {
            line.push_back("+|||||||||||||||||||||||+");
        }
        line.push_back(oneLine);


                // "0123456789012345678901234"
        line[1] =  "+|||||||||||||||||||||||+";
        line[2] =  "+|||+++|||+++++|||+++|||+";
        line[3] =  "+|S|+++|S|+++++|S|+++|S|+";
        line[4] =  "+|||+++|||+++++|||+++|||+";
        line[5] =  "+|||||||||+++++|||||||||+";
        line[6] =  "+||||S||||+++++||||S||||+";
        line[7] =  "+|||||||||||||||||||||||+";
        line[8] =  "+|||+++|||+++++|||+++|||+";
        line[9] =  "+|S|+++|S|+++++|S|+++|S|+";
        line[10] = "+|||+++|||+++++|||+++|||+";
        line[11] = "+|||||||||||||||||||||||+";
        line[12] = "+||||S||+++++++++||S||||+";
        line[13] = "+|||||||||||||||||||||||+";
        line[14] = "+|||+++|||+++++|||+++|||+";
        line[15] = "+|S|+++|S|+++++|S|+++|S|+";
        line[16] = "+|||+++|||+++++|||+++|||+";
        line[17] = "+|||||||||||||||||||||||+";
        line[18] = "+||||S||||+++++||||S||||+";
        line[19] = "+|||||||||+++++|||||||||+";
        line[20] = "+|||+++|||+++++|||+++|||+";
        line[21] = "+|S|+++|S|+++++|S|+++|S|+";
        line[22] = "+|||+++|||+++++|||+++|||+";
        line[23] = "+|||||||||||||||||||||||+";
                // "0123456789012345678901234"


        sizeY = line.size();

        RotateLines();  //obrót mapy (tak aby rysowanie było tak jak widoczne jest kodzie)

        //Ustawianie kolizji
        colliders.push_back(BoxCollision(0,0,25,1));    //Górna ściana
        colliders.push_back(BoxCollision(0,0,1,25));    //Lewa ściana
        colliders.push_back(BoxCollision(0,24,25,25));  //Dolna ściana
        colliders.push_back(BoxCollision(24,1,25,25));  //Dolna ściana
        //Góra
        colliders.push_back(BoxCollision(4,2,3,3));  
        colliders.push_back(BoxCollision(10,2,5,5)); 
        colliders.push_back(BoxCollision(18,2,3,3)); 
        //środek
        colliders.push_back(BoxCollision(4,8,3,3)); 
        colliders.push_back(BoxCollision(10,8,5,3)); 
        colliders.push_back(BoxCollision(18,8,3,3)); 

        colliders.push_back(BoxCollision(8,12,9,1)); 

        colliders.push_back(BoxCollision(4,14,3,3)); 
        colliders.push_back(BoxCollision(10,14,5,3)); 
        colliders.push_back(BoxCollision(18,14,3,3));

        //Dół
        colliders.push_back(BoxCollision(4,20,3,3));  
        colliders.push_back(BoxCollision(10,18,5,5)); 
        colliders.push_back(BoxCollision(18,20,3,3));  


    }
    else
    {
        std::string oneLine = "+++++++++++++++++++++++++";
        sizeX = oneLine.size();

        //Po usunięciu pętli For występuje wyciek pamięci :(
        line.push_back(oneLine);                        //23
        for(auto i=0;i<23;i++)
        {
            line.push_back("+|||||||||||||||||||||||+");
        }
        line.push_back(oneLine);


                // "0123456789012345678901234"
        line[1] =  "+|||||||||||||||||||||||+";
        line[2] =  "+|S||||S||||S||||S||||S|+";
        line[3] =  "+|||||||||||||||||||||||+";
        line[4] =  "+|||||||||||||||||||||||+";
        line[5] =  "+|||||||||||||||||||||||+";
        line[6] =  "+|S||||S||||S||||S||||S|+";
        line[7] =  "+|||||||||||||||||||||||+";
        line[8] =  "+|||||||||||||||||||||||+";
        line[9] =  "+|||||||||||||||||||||||+";
        line[10] = "+|||||||||||||||||||||||+";
        line[11] = "+|||S||||S|||||S||||S|S|+";
        line[12] = "+|S||||S||||S||||S||||S|+";
        line[13] = "+|||||||||||||||||||||||+";
        line[14] = "+|||||||||||||||||||||||+";
        line[15] = "+|||||||||||||||||||||||+";
        line[16] = "+|||||||||||||||||||||||+";
        line[17] = "+|S||||S||||S||||S||||S|+";
        line[18] = "+|||||||||||||||||||||||+";
        line[19] = "+|||||||||||||||||||||||+";
        line[20] = "+|||||||||||||||||||||||+";
        line[21] = "+|||||||||||||||||||||||+";
        line[22] = "+|S||||S||||S||||S||||S|+";
        line[23] = "+|||||||||||||||||||||||+";
                // "0123456789012345678901234"


        sizeY = line.size();

        RotateLines();  //obrót mapy (tak aby rysowanie było tak jak widoczne jest kodzie)
        
        //Ustawianie kolizji
        colliders.push_back(BoxCollision(0,0,25,1));    //Górna ściana
        colliders.push_back(BoxCollision(0,0,1,25));    //Lewa ściana
        colliders.push_back(BoxCollision(0,24,25,25));  //Dolna ściana
        colliders.push_back(BoxCollision(24,1,25,25));  //Dolna ściana
    }
}

void MapTemplate::RotateLines()
{
    std::vector<std::string> copy = line;
    for(int i=0;i<copy.size();i++)
    {
        std::string newLine = "";
        for(int j=0;j<copy.size();j++)
        {
            newLine+=copy[j][i];
        }
        line[i] = newLine;
        //std::cout<<newLine<<std::endl;
    }
}

int MapTemplate::GetSizeX()
{
    return sizeX;
}
int MapTemplate::GetSizeY()
{
    return sizeY;
}
std::vector<std::string> MapTemplate::GetMap()
{
    return line;
}

std::string MapTemplate::GetLine(int index)
{
    return line[index];
}

std::vector<BoxCollision> MapTemplate::GetColliders()
{
    return colliders;
}