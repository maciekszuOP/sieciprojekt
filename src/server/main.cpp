#include<iostream>
#include <cstdlib>
#include <ctime>
#include<SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Network.hpp>
#include "window.hpp"
#include "game.hpp"
#include "player.hpp"
#include "map.hpp"
#include "wall.hpp"
#include "boxCollision.hpp"
#include "tcpServer.hpp"



int main(int argc, char* argv[])
{
    std::srand(std::time(0)); // Inicjalizacja generatora losowego
    
    try 
    {
        unsigned short port = 8000;

        // Jeśli podano argumenty w linii poleceń, spróbuj ustawić port
        if (argc > 1) 
        {
            try 
            {
                port = static_cast<unsigned short>(std::stoi(argv[1]));
                if (port == 0 || port > 65535) 
                {
                    throw std::out_of_range("Port poza zakresem (1-65535)");
                }
            } 
            catch (const std::exception& e) 
            {
                std::cerr << "Niepoprawny port: " << argv[1] << ". Port musi być liczbą w zakresie 1-65535." << std::endl;
                return 1;
            }
        }
        TcpServer server(port);

        server.Start();

        std::cin.get();

        server.Stop();
    } 
    catch (const std::exception& e) 
    {
        std::cerr << "Błąd: " << e.what() << std::endl;
    }
    
    std::cout<<"Serwer wyłączony\n";
    return 0;
}