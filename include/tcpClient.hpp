#ifndef TCPSERVER_H
#define TCPSERVER_H

#include<SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Network.hpp>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include "window.hpp"
#include "game.hpp"
#include "player.hpp"
#include "map.hpp"
#include "wall.hpp"
#include "boxCollision.hpp"
#include "direction.hpp"
#include "UI.hpp"

class TcpClient 
{
    public:
        TcpClient(sf::IpAddress serverAddress,unsigned short port);
        ~TcpClient();

        int Game();                                             //Włączenie gry
        bool IsRunning();                                       //Zwraca czy udało się nawiązać połączenie
        void initMap();                                         //Załadowanie danych o mapie
        bool SetNick();                                         //Ustawia nick komunikując się z serwerm zwraca czy udało mu się ustalić nick
        bool GetMapInfo();                                      //Odbiera informacje o mapie gry
        void ReceiveFromServer();                               //Odczytuje dane z serwera

    private:
        sf::TcpSocket socket;
        bool running;

        sf::RenderWindow* Window;                                //Okno gry

        int gameState;                                          //Stan gry
        int timeLeft;                                           //Czas do końca
        int mapId;                                              //Id Mapy
        GameMap map;                                            //Mapa serwera
        std::vector<BoxCollision> obstacles;                    //Przeszkody
        std::mutex bulletMutex;                                 // Mutex do ochrony dostępu do pocisków
        std::mutex playerMutex;                                 //Mutex do ochrony dostępu do graczy
        bool canShutdown;                                       //Do zamykania serwera
        std::mutex shutdownMutex;                               //
        std::condition_variable Shutdown;                       //Czy może się wyłączyć

};

#endif
