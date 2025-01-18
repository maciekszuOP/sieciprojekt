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

TcpClient::TcpClient(sf::IpAddress serverAddress,unsigned short port)
{
    // Próba połączenia z serwerem
    if (socket.connect(serverAddress, port) != sf::Socket::Done) 
    {
        std::cerr << "Nie udało się połączyć z serwerem!" << std::endl;
        running = false;
        return;
    }
    running = true;
    canShutdown = false;
    gameState = 1;
    timeLeft = 0;
    mapId = 0;
    socket.setBlocking(false);
}

TcpClient::~TcpClient()
{
    socket.disconnect();
    delete Window; 
}

bool TcpClient::IsRunning()
{
    return running;
}

void TcpClient::initMap()
{
    //Załadowanie obiektów na mapie
    MapTemplate templ(mapId);
    map = GameMap(templ);

    //Załadowanie przeszkód
    obstacles = map.GetAllColliders();

    //Załadowanie graczy
    std::vector<Player*> playerList;  // Lista wskaźników na graczy
    for (auto i=0;i<16;i++)
    {
        playerList.push_back(new Player(i));
        playerList[i]->SetPositionOnTiles(sf::Vector2f(map.GetSpawnByIndex(i)));
    }

    map.SetPlayerList(playerList);
}

int TcpClient::Game()
{
    if(!running)
    {
        return -1;
    }
    std::cout<<"Włączenie gry\n";
    std::srand(std::time(0)); // Inicjalizacja generatora losowego

    //Ustwianie nicku gracza
    if(!SetNick())
    {
        running = false;
        std::cout<<"Nie udało się ustalić nicku (brak połączenia)"<<std::endl;
        return -1;
    }

    if(!GetMapInfo())
    {
        running = false;
        std::cout<<"Nie udało się pobrać informacji o mapie"<<std::endl;
        return -1; 
    }

    
    Window = createWindow("Gra");  //Utworzenie okna

    initMap();

    UI interface;

    interface.AddToList(map.GetPlayerList().size());

    //Utworzenie wątku odbierającego
    std::thread(&TcpClient::ReceiveFromServer, this).detach();

    sf::Vector2f moveDirection;
    bool shoot; 
    sf::Clock deltaClock;

    socket.setBlocking(false);

    //Sprawdzenie czy okno zostało otwarte
    while(Window->isOpen())
    {
        sf::Time deltaTime = deltaClock.restart();      //Wyliczenie delty time (czas między klatkami)

        //Sprawdzenie czy okno zostało zamknięte
        if(EventLoop(Window)  == -1)  
        {
            running = false;
            continue;
        }

        if(gameState == 1)
        {
            Window->clear(sf::Color::Black); //Wyczyszczenie ekraniu
            map.DrawMap(Window);
            
            interface.SetTimer(timeLeft);
            
            // Rysowanie tekstów
            std::unique_lock<std::mutex> lockPlayer(playerMutex);
            interface.DrawUI(Window,map.GetPlayerList());
            lockPlayer.unlock();
        }

        else if(gameState == 2)
        {

            interface.SetGameTimer(timeLeft);

            moveDirection = sf::Vector2f(0.0f, 0.0f);       //Wektor przemieszczenia 
            shoot = false;

            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Return) || sf::Keyboard::isKeyPressed(sf::Keyboard::E) || sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
            {
                shoot = true;
            }
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            {
                moveDirection.y -= 1;
            }
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            {
                moveDirection.y += 1;
            }
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            {
                moveDirection.x -= 1;
            }
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            {
                moveDirection.x += 1;
            }

            // Wysyłanie danych o inputach
            sf::Packet sendPacket;
            sendPacket << moveDirection.x << moveDirection.y << shoot;

            // Wysłanie danych do serwera
            if (socket.send(sendPacket) != sf::Socket::Done)
            {
                std::cerr << "Błąd podczas wysyłania danych: KIERUNEK PORUSZANIA SIĘ!" << std::endl;
                return -1;
            }
            
            Window->clear(sf::Color::Black); //Wyczyszczenie ekraniu
            map.DrawWalls(Window);
            map.DrawMap(Window);
            std::unique_lock<std::mutex> lock(bulletMutex);
            map.DrawBullets(Window);
            lock.unlock();
            std::unique_lock<std::mutex> lockPlayer(playerMutex);
            map.DrawPlayer(Window);
            map.SortPlayersRanking();           //Sortowanie rankingu
            interface.SetNumberOfPlayers(map.numberOfActivePlayers());
            interface.DrawGameUI(Window,map.GetRanking());
            lockPlayer.unlock();
        }
        if (gameState == 3)
        {
            Window->clear(sf::Color::Black); //Wyczyszczenie ekraniu
            map.DrawMap(Window);
            
            interface.SetTimer(timeLeft);
            
            // Rysowanie tekstów
            std::unique_lock<std::mutex> lockPlayer(playerMutex);
            interface.DrawResultUI(Window,map.GetRanking());
            lockPlayer.unlock();
        }
        Window->display();               //Wrzucenie zmian do okna
    }

    std::unique_lock<std::mutex> lock(shutdownMutex);
    Shutdown.wait(lock, [this]{ return canShutdown; });  // Czekanie na zakończenie wątku pobierania danych

    std::cout<<"Koniec\n";
    return 0;
}

bool TcpClient::SetNick()
{
    sf::Packet nickPacket, responsePacket;
    std::string nick;

    sf::SocketSelector selector;
    selector.add(socket);

    bool nickAccepted;

    while(true)
    {
        if (selector.wait(sf::seconds(10))) 
        {
            if (socket.receive(responsePacket) == sf::Socket::Done)
            {
                std::string responseMessage;
                if (responsePacket >> responseMessage)
                {
                    std::cout<<responseMessage<<std::endl;
                    std::cin >> nick;
                    std::cout<<"wysłany nick "<<nick<<std::endl;
                    nickPacket << nick;
                    if (socket.send(nickPacket) != sf::Socket::Done) 
                    {
                        std::cerr << "Błąd podczas wysyłania nicku do serwera." << std::endl;
                        return false; // Zakończenie gry w przypadku błędu
                    }
                    nickPacket.clear();
                    responsePacket.clear();
                    selector.wait(sf::seconds(10));
                    if (socket.receive(responsePacket) == sf::Socket::Done)
                    {
                        if (responsePacket >> nickAccepted)
                        {
                            if (nickAccepted)
                            {
                                std::cout<<"Nick został zaakceptowany\n";
                                break;
                            }
                            else
                            {
                                std::cout<<"Nick został odrzucony\n";
                                continue;
                            }
                        }
                        else
                        {
                            std::cout<<"Błąd formatu danych\n";
                            return false;
                        }
                    }
                    else
                    {
                        std::cout<<"Serwer nie odpowiada\n";
                        return false;
                    }
                }
            }
        }
    }

    std::cout<<"Koniec podawania nicku\n";

    return true;
}

bool TcpClient::GetMapInfo()
{
    sf::Packet mapPacket;

    sf::SocketSelector selector;
    selector.add(socket);

    sf::Packet responsePacket;

    // Próba odbioru pakietu
    selector.wait(sf::seconds(10));
    if (socket.receive(mapPacket) == sf::Socket::Done)
    {
        if (mapPacket >> mapId) // Próba deserializacji ID mapy z pakietu
        {
            this->mapId = mapId; // Zapisz ID mapy w obiekcie klienta
            return true;
        }
        else
        {
            std::cerr << "Błąd deserializacji ID mapy.\n";
        }
    }
    else
    {
        std::cerr << "Błąd odbioru pakietu z ID mapy.\n";
    }

    return false; // Jeśli odbiór się nie powiódł
}

void TcpClient::ReceiveFromServer()
{
    sf::SocketSelector selector;
    selector.add(socket);

    sf::Packet responsePacket;

    selector.wait(sf::seconds(10));
    if (socket.receive(responsePacket) == sf::Socket::Done)
    {
        std::string responseMessage;
        if (responsePacket >> responseMessage)
        {
            std::cout<<responseMessage<<std::endl;
        }
        else
        {
            std::cout<<"błąd w odczycie\n";
        }
    }
    else
    {
        std::cout<<"Błąd przy odbiorze\n"<<std::endl;
    }

    selector.add(socket);

    while(running)
    {
        sf::Packet gameInfoPacket;          // Pakiet do odbierania
        int state;                          // Stan gry
        int activePlayers;                  // Liczba aktywnych graczy
        int activeBullets;                  // Liczba aktywnych pocisków

        selector.wait(sf::seconds(10));
        if (socket.receive(gameInfoPacket) == sf::Socket::Done)
        {
            // Odczytanie stanu gry
            gameInfoPacket >> gameState;

            gameInfoPacket >> timeLeft;

            // Odczytanie liczby aktywnych graczy
            gameInfoPacket >> activePlayers;

            // Odczytanie informacji o aktywnych graczach
            std::unique_lock<std::mutex> lock(playerMutex);
            for (int i = 0; i < activePlayers; i++)
            {
                Player player;
                gameInfoPacket >> player;  // Deserializacja gracza z pakietu
                
                map.GetPlayer(i)->SetPosition(sf::Vector2f(player.GetPosition().x,player.GetPosition().y));
                map.GetPlayer(i)->SetDirection(player.GetDirection());
                map.GetPlayer(i)->SetName(player.GetName());
                map.GetPlayer(i)->SetState(player.isPlayerActive());
                map.GetPlayer(i)->SetKills(player.GetKills());
                map.GetPlayer(i)->SetDeaths(player.GetDeaths());

                //player.PlayerInfo();
            }
            lock.unlock();

            // Odczytanie liczby aktywnych pocisków
            gameInfoPacket >> activeBullets;

            //std::cout<<"Pobieranie pocisków\n";
            // Odczytanie informacji o pociskach
            std::vector<Bullet> bullets;  // Lista pocisków odczytanych z pakietu
            for (int i = 0; i < activeBullets; i++)
            {
                Bullet bullet;
                gameInfoPacket >> bullet;  // Deserializacja pocisku z pakietu
                bullets.push_back(bullet);

                    //std::cout << "Pocisk: Właściciel: " << bullet.GetOwnerId()
                    //        << " Pozycja: (" << bullet.GetPosition().x << ", "
                    //        << bullet.GetPosition().y << ")" << std::endl;
                }
            //std::cout<<"Ładowanie pocisków\n";
            // Aktualizacja stanu pocisków na mapie
            std::unique_lock<std::mutex> bulletLock(bulletMutex);
            map.ReplaceBullets(bullets);
            bulletLock.unlock();
        }
    }
    std::unique_lock<std::mutex> lock(shutdownMutex);
    canShutdown = true;  // Ustawienie flagi zakończenia
    lock.unlock();
    Shutdown.notify_all();  // Powiadomienie oczekujących wątków
}

