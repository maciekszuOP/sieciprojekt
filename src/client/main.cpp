#include<SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Network.hpp>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include "window.hpp"
#include "game.hpp"
#include "player.hpp"
#include "map.hpp"
#include "wall.hpp"
#include "boxCollision.hpp"
#include "tcpClient.hpp"

int main(int argc, char* argv[]) 
{
    //Sprawdzenie, czy podano odpowiednią liczbę argumentów
    if (argc != 3) 
    {
        std::cerr << "Użycie: " << argv[0] << " <adres IP> <port>" << std::endl;
        return 1;
    }

    //Ustawienie adresu
    std::string serverAddressInput = argv[1];
    sf::IpAddress serverAddress(serverAddressInput);
    if (serverAddress == sf::IpAddress::None) 
    {
        std::cerr << "Niepoprawny adres IP: " << serverAddressInput << std::endl;
        return 1;
    }

    //Ustawienie portu
    unsigned short port;
    try 
    {
        port = static_cast<unsigned short>(std::stoi(argv[2]));
    } 
    catch (const std::exception& e) 
    {
        std::cerr << "Niepoprawny port: " << argv[2] << ". Port musi być liczbą." << std::endl;
        return 1;
    }

    //sf::IpAddress serverAddress = "127.0.0.1";  //Adres IP serwera (tutaj localhost)
    //unsigned short port = 8000;                 //Port, na którym nasłuchuje serwer
    
    TcpClient client = TcpClient(serverAddress,port);       //Tworzeni klienta TCP

    client.Game();
    std::cout<<"Zakończono działanie gry"<<std::endl;

    return 0;
}
