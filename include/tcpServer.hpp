#ifndef TCPSERVER_H
#define TCPSERVER_H

#include<SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Network.hpp>
#include <iostream>
#include <fstream>
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

class TcpServer 
{
    public:
        TcpServer(unsigned short port);
        ~TcpServer();

        void Start();  //Uruchomienie serwera
        void Stop();   //Zatrzymanie serwera

    private:
        void ReadCongif();                                                          //Odczytuje plik konfiguracyjny
        void SaveCongif();                                                          //Tworzy i zapisuje dane w pliku
        void AcceptClients();                                                       //Akceptowanie nowych klientów
        void HandleClient(int playerId, sf::TcpSocket* client);                     //Obsługa klienta
        void SendingGameInfo(sf::TcpSocket* client,int playerId);                   //Wysyłanie danych o grze
        void AcceptNick(int playerId,sf::TcpSocket* client);                        //Akceptowanie nicku
        void SendMapId(int playerId,sf::TcpSocket* client);                                      //Wysyła informację o mapie serwera
        void InitMap();                                                             //Wczytuje mapę
        int GameLoop();                                                             //Pętla gry
        void ResetPlayers();                                                        //Resetuje pozycje i statystyki graczy
        void DisconnectClient(int playerId, sf::TcpSocket* client,bool nickMsg);    //Rozłącza klienta
        void DisconnectAll();                                                       //Rozłącza wszystkich klientów

        void StartClock();                                      //Włącza monitorowanie zegara TAK NAPRAWDĘ ZEGAR CHODZI CAŁY CZAS
        void StopClock();                                       //Wyłącza monitorowanie zegara TAK NAPRAWDĘ ZEGAR CHODZI CAŁY CZAS
        int GetTime();                                          //Zwraca czas z zegara w sekundach (jeśli zegar jest wyłączony zwraca -1)
        int GetTimeLeft();                                      //Zwraca ile czasu zostało do zakończenia stanu
        void NextState(int nextState);                          //Resetuje zegar i zmienia na podany stan

        void ChangeState(int newState);                         //Zmienia stan gry

        sf::TcpListener listener;                               //Listener do nasłuchiwania połączeń
        std::mutex availableIdsMutex;                           // Mutex do ochrony dostępu do availableIds
        std::vector<int> availableIds;                          // Przechowywanie dostępnych ID
        std::vector<std::unique_ptr<sf::TcpSocket>> clients;    //Lista klientów
        std::thread acceptThread;                               //Wątek akceptujący klientów
        std::atomic<int> nextPlayerId;                          //Id następnego gracza
        std::atomic<bool> running;                              //Flaga działania serwera

        int waitingRoomTime;                                    //Długość czekania w poczekalni
        int gameTime;                                           //Długość trwania gry
        int summaryTime;                                        //Długość trwania podsumowania     
        int minPlayers;                                         //Minimalna liczba graczy potrzebna do włączenia gry 
        int maxPlayers;                                         //Maksymalna liczba graczy
        int respawnsAmount;                                     //Liczba punktów odrodzenia
        int idMap;                                              //Id mapy         

        int stateName;                                          //Stan gry 1 - poczekalnia 2 - gra 3 - zakończenie gry
        bool clockRunning;                                      //informuje o tym czy zegar działa
        sf::Clock serverClock;                                  //Zegar do zmieniania stanów gry
        GameMap serverMap;                                      //Mapa serwera
        std::vector<BoxCollision> obstacles;                    //Przeszkody
        std::mutex playerMutex;                                 //Mutex do ochrony gracza
        std::mutex bulletMutex;                                 // Mutex do ochrony dostępu do pocisków

        std::condition_variable cv;                             //zmienna warunkowa
        std::mutex stateMutex;                                  //Mutex do oczekiwania na wybrany stan

};

#endif
