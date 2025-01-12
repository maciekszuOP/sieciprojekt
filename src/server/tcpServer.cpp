#include "tcpServer.hpp"

//Konstruktor serwera
TcpServer::TcpServer(unsigned short port) : running(false), nextPlayerId(0) 
{
    //Otwarcie gniazda nasłuchującego podanym porcie
    if (listener.listen(port) != sf::Socket::Done) 
    {
        throw std::runtime_error("Nie udało się rozpocząć nasłuchiwania na porcie " + std::to_string(port));
    }
    std::cout << "Serwer nasłuchuje na porcie " << port << std::endl;
    
    stateName = 1;
    //Domyślne wartości konfiguracyjne
    waitingRoomTime = 30;
    gameTime = 300;                                 
    summaryTime = 20;
    
    minPlayers = 2;     
    
    maxPlayers = 16;
    idMap = 1;              
    respawnsAmount = 16;
}

// Destruktor serwera
TcpServer::~TcpServer() 
{
    Stop();
}

// Rozpoczęcie działania serwera
void TcpServer::Start() 
{
    ReadCongif();

    InitMap();                                                    //Wczytanie obiektów na mapie
    running = true;                                               //Oznaczenie, że serwer działa
    acceptThread = std::thread(&TcpServer::AcceptClients, this);  //Tworzenie wątku, który będzie akceptować klientów
    std::cout<<"Startowanie Serwera\n";
    GameLoop();
}

// Zatrzymanie serwera
void TcpServer::Stop() 
{
    running = false; 
    listener.close();  // Zamykanie gniazda nasłuchującego
    DisconnectAll();
    //Czekanie na zakończenie wątku
    if (acceptThread.joinable()) 
    {
        acceptThread.join();
    }
    std::cout << "Serwer zatrzymany." << std::endl;
}

void TcpServer::ReadCongif()
{
    const std::string configFilePath = "config.txt";

    std::ifstream configFile(configFilePath);

    if (configFile.is_open()) 
    {
        // Plik istnieje, wczytaj wartości konfiguracyjne
        configFile >> waitingRoomTime;
        configFile >> gameTime;
        configFile >> summaryTime;
        configFile >> minPlayers;
        configFile >> maxPlayers;
        configFile >> respawnsAmount;
        configFile >> idMap;
        configFile.close();
        std::cout << "Konfiguracja wczytana z pliku.\n";
    } 
    else 
    {
        // Plik nie istnieje, zapisz domyślne wartości
        std::ofstream configFile(configFilePath);
        if (configFile.is_open()) 
        {
            configFile << waitingRoomTime << "\n";
            configFile << gameTime << "\n";
            configFile << summaryTime << "\n";
            configFile << minPlayers << "\n";
            configFile << maxPlayers << "\n";
            configFile << respawnsAmount << "\n";
            configFile << idMap << "\n";
            configFile.close();
        } 
        else 
        {
            std::cerr << "Nie można utworzyć pliku konfiguracyjnego!\n";
        }
        std::cout << "Plik konfiguracyjny nie istnieje. Utworzono nowy z domyślnymi wartościami.\n";
    }
    if(minPlayers < 0 )
    {
        minPlayers = 1;
    }
    //Ograniczenie graczy do 16
    if(maxPlayers > 16 || maxPlayers < 1)
    {
        maxPlayers = 16;
    }
}

//Odpowiada za nasłuchiwanie i akceptowanie połączeń od klientów
void TcpServer::AcceptClients() 
{
    //int maxClients = 16;
    sf::SocketSelector selector;  //obiekt monitorujący gniazda

    
    selector.add(listener); //monitorowanie przychodzących połączeń

    //Nadawanie dostępnych id
    //availableIds;
    for (int i = maxPlayers-1; i >= 0; i--) 
    {
        availableIds.push_back(i); 
    }

    while (running)  //Pętla będzie działała tak długo, jak serwer jest uruchomiony
    {
        if (selector.wait())  // Czekanie na aktywność na którymkolwiek z gniazdek
        {
            if (selector.isReady(listener))  // Sprawdzenie czy Listener jest gotowy do przyjęcia połączenia
            {
                if (clients.size() < maxPlayers) // Sprawdzenie, czy liczba połączonych klientów nie przekroczyła maksymalnej liczbt
                {
                    auto client = std::make_unique<sf::TcpSocket>();        //Obiekt TcpSocket, który reprezentuje połączenie z klientem
                    std::unique_lock<std::mutex> lock(availableIdsMutex);   //blokowanie tablicy z indeksami
                    if (!availableIds.empty())                              //Sprawdzenie czy są dostępne id
                    {
                        // Akceptowanie połączenia od klienta
                        if (listener.accept(*client) == sf::Socket::Done) 
                        {
                            int playerId = availableIds.back();
                            availableIds.pop_back();
                            std::cout << "Nowy klient połączony: " << client->getRemoteAddress() << std::endl;
                            clients.push_back(std::move(client));                                                       //Dodanie klienta do listy klientów
                            std::thread(&TcpServer::HandleClient, this, playerId, clients.back().get()).detach();       //Uruchamienie nowego wątku do obsługi tego klienta
                        } 
                    }
                    else
                    {
                        std::cout<<"Brak wolnych id"<<std::endl;
                    }
                    lock.unlock();
                }
            }
        }
    }
}

//Obsługuje komunikację z danym klientem
void TcpServer::HandleClient(int playerId, sf::TcpSocket* client) 
{
    sf::Clock deltaClock;
    
    sf::Packet packet;

    //Ustawianie nicku gracza
    AcceptNick(playerId,client);

    //Przesłanie informacji o mapie
    SendMapId(playerId,client);

    //Przesyłanie informacji o grze
    std::thread(&TcpServer::SendingGameInfo, this, client,playerId).detach();

    while (running)
    {
        sf::Time deltaTime = deltaClock.restart();

        // Próba odbioru danych z klienta
        if (client->receive(packet) == sf::Socket::Done)
        {
            float x, y;
            bool shoot;

            // Deserializacja danych z pakietu
            if (packet >> x >> y >> shoot)
            {
                std::unique_lock<std::mutex> playerLock(playerMutex);
                Player* player = serverMap.GetPlayer(playerId);
                player->Move(sf::Vector2f(x,y),deltaTime,obstacles);
                if(shoot && player->CanShoot())
                {
                    std::unique_lock<std::mutex> lock(bulletMutex);
                    serverMap.AddBullet(player->Shoot());
                    lock.unlock();
                }
                playerLock.unlock();
            }
            else
            {
                std::cerr << "Błąd przy odczycie danych z pakietu!" << std::endl;
            }
        }
        else if (client->getRemoteAddress() == sf::IpAddress::None) // Sprawdzanie, czy klient jest rozłączony
        {
            DisconnectClient(playerId,client,true);                     //Rozłączenie klienta
            break;                                                      // Zakończenie pętli, klient się rozłączył
        }
        else
        {
            std::cerr << "Błąd odbioru danych." << std::endl;
            DisconnectClient(playerId,client,true);                          //Rozłączenie klienta
            break; // Zakończenie pętli w przypadku błędu
        }
    }
}

void TcpServer::AcceptNick(int playerId,sf::TcpSocket* client)                        //Akceptowanie nicku
{
    sf::Packet nickPacket;
    bool nickAccepted = false; 
    std::string nick;

    sf::SocketSelector selector;
    selector.add(*client); // Dodanie gniazda klienta do selektora

    while(!nickAccepted && running)
    {
        sf::Packet requestPacket;
        requestPacket << "Podaj nick:";
        client->send(requestPacket);
        selector.wait(sf::seconds(10));
        if (client->receive(nickPacket) == sf::Socket::Done) 
        {
            nickPacket >> nick;
            //std::cout<<"Otrzymany nick "<<nick<<std::endl;
            nickPacket.clear();

            std::unique_lock<std::mutex> lock(playerMutex);
            if(serverMap.IsNickUnused(nick) && !nick.empty())
            {
                std::cout << "Klient podał nick: " << nick << std::endl;
                serverMap.GetPlayer(playerId)->SetName(nick);
                serverMap.GetPlayer(playerId)->SetState(true);

                // Wysłanie zatwierdzenia do klienta
                sf::Packet confirmationPacket;
                confirmationPacket << true;
                client->send(confirmationPacket);
                nickAccepted = true; // Nick zatwierdzony
            }
            else
            {
                sf::Packet confirmationPacket;
                confirmationPacket << false;
                client->send(confirmationPacket);
            }
            lock.unlock();
        }
        else
        {
            std::cout<<"Blad danyc\n";
        }
    }

    std::cout<<"Koniec zatwierdzania nicku\n";
}

void TcpServer::SendMapId(int playerId ,sf::TcpSocket* client)
{
    sf::SocketSelector selector;
    selector.add(*client); // Dodanie gniazda klienta do selektora

    if(running)
    {
        sf::Packet mapPacket;
        mapPacket << idMap;
        if (client->send(mapPacket) != sf::Socket::Done)
        {
            DisconnectClient(playerId,client,true);
        }
    }
}

void TcpServer::SendingGameInfo(sf::TcpSocket* client, int playerId)                                //Wysyłanie danych o grze
{

    sf::SocketSelector selector;
    selector.add(*client);

    std::cout<<"wysyłanie do klienta\n";
    sf::Packet gameInfoPacket;
    gameInfoPacket << "Odbieranie danych gry rozpoczęte";
    client->send(gameInfoPacket);
    std::cout << "Wysyłanie danych gry do klienta...\n";

    std::vector<Player*> players;
    std::vector<Bullet> bulletBox;

    while (running)
    {
        //Pobranie listy graczy
        std::unique_lock<std::mutex> lock(playerMutex);
        players = serverMap.GetPlayerList();
        lock.unlock();
        
        //Pobranie listy pocisków
        std::unique_lock<std::mutex> lockBullet(bulletMutex);
        bulletBox = serverMap.GetBulletBox();
        int activeBullets = bulletBox.size();  // Liczba aktywnych pocisków
        lockBullet.unlock();

        sf::Packet gameInfoPacket;
        int activePlayers = players.size();  // Liczba aktywnych graczy

        // Serializacja stanu gry i liczby graczy
        gameInfoPacket << stateName;

        gameInfoPacket << GetTimeLeft();

        gameInfoPacket << activePlayers;

        // Serializacja danych graczy
        for (int i = 0; i < activePlayers; ++i)
        {
            Player* player = players[i];
            gameInfoPacket << *player;  // Serializowanie gracza za pomocą operatora <<
        }
        //players[0]->PlayerInfo();
        
        // Serializacja danych o pociskach
        gameInfoPacket << activeBullets;       // Dodawanie liczby pocisków do pakietu
        for (Bullet& bullet : bulletBox)
        {
            gameInfoPacket << bullet;  // Serializowanie każdego pocisku
        }
        // Wysłanie pakietu do klienta
        if(client->send(gameInfoPacket) == sf::Socket::Disconnected)
        {
            DisconnectClient(playerId,client,true);
            break;
        }
        gameInfoPacket.clear();

        sf::sleep(sf::milliseconds(30));

    }
}

void TcpServer::InitMap()
{
    //Załadowanie obiektów na mapie
    MapTemplate templ(idMap);
    serverMap = GameMap(templ);

    //Załadowanie przeszkód
    obstacles = serverMap.GetAllColliders();

    //Załadowanie graczy
    std::vector<Player*> playerList;  // Lista wskaźników na graczy
    for (auto i=0;i<16;i++)
    {
        playerList.push_back(new Player(i));
        //playerList[i]->SetPositionOnTiles(sf::Vector2f(serverMap.GetSpawnByIndex(i)));
        playerList[i]->SetPositionOnTiles(sf::Vector2f(serverMap.GetNextSpawn()));
        
    }

    serverMap.SetPlayerList(playerList);                        //Ustawienie liczby graczy  
    serverMap.SetLastSpawn(respawnsAmount);                     //Ustawienie liczby punktów odrodzenia
}

int TcpServer::GameLoop()
{
    std::srand(std::time(0));                                //Generator losowy
    //sf::RenderWindow* Window = createWindow("Gra: Server");  //Utworzenie okna
    

    const unsigned int targetFPS = 30;                              // Cel FPS
    const sf::Time frameDuration = sf::seconds(1.0f / targetFPS);   // Czas trwania jednej klatki
    sf::Clock frameClock;                                           // Zegar do mierzenia czasu pętli


    sf::Vector2f moveDirection;
    bool shoot; 
    sf::Clock deltaClock;

    //Sprawdzenie czy okno zostało otwarte
    //while(Window->isOpen())
    while(running)
    {
        sf::Time frameStart = frameClock.getElapsedTime();      // Start czasu klatki
        if(stateName == 1)
        {
            //std::cout<<"Stan 1\n";
            if(serverMap.numberOfActivePlayers() >= minPlayers)
            {
                StartClock();
            }
            else
            {
                StopClock();                
            }
            if (GetTimeLeft()<0 && running)
            {
                NextState(2);
            }
            //Window->clear(sf::Color::Black); //Wyczyszczenie ekraniu
            //serverMap.DrawMap(Window);
            //serverMap.DrawPlayer(Window);
            //Window->display();               //Wrzucenie zmian do okna
        }
        else if(stateName == 2)
        {

            sf::Time deltaTime = deltaClock.restart();      //Wyliczenie delty time (czas między klatkami)

            //EventLoop(Window);

            moveDirection = sf::Vector2f(0.0f, 0.0f);       //Wektor przemieszczenia 
            shoot = false;
            std::unique_lock<std::mutex> lock(bulletMutex);
            serverMap.MoveBullets(deltaTime);
            serverMap.BulletCheck();
            lock.unlock();

            //Window->clear(sf::Color::Black); //Wyczyszczenie ekraniu
            //serverMap.DrawPlayer(Window);
            //Window->display();               //Wrzucenie zmian do okna

            if (GetTimeLeft()<0)
            {
                NextState(3);
            }
        }
        else if(stateName == 3)
        {
            //Window->clear(sf::Color::Black); //Wyczyszczenie ekraniu
            //serverMap.DrawPlayer(Window);
            //Window->display();               //Wrzucenie zmian do okna

            if (GetTimeLeft()<0)
            {
                ResetPlayers();
                NextState(1);
            }
        }

        // Oblicz czas trwania iteracji pętli
        sf::Time frameEnd = frameClock.getElapsedTime();
        sf::Time frameElapsed = frameEnd - frameStart;

        // Jeśli iteracja była szybsza niż planowany czas na klatkę, dodaj opóźnienie
        if (frameElapsed < frameDuration)
        {
            sf::sleep(frameDuration - frameElapsed);
        }

    }

    //delete Window; 
    std::cout<<"Koniec\n";
    return 0;
}

void TcpServer::ResetPlayers()
{
    std::unique_lock<std::mutex> lock(playerMutex);
    std::vector<Player*> playerList = serverMap.GetPlayerList();
    for (auto i=0;i<playerList.size();i++)
    {
        //playerList[i]->SetPositionOnTiles(sf::Vector2f(serverMap.GetSpawnByIndex(i)));
        playerList[i]->SetPositionOnTiles(sf::Vector2f(serverMap.GetNextSpawn()));
        playerList[i]->SetDirection(Direction::Up);
        playerList[i]->ResetStats();
    }
    lock.unlock();
}

void TcpServer::DisconnectClient(int playerId, sf::TcpSocket* client,bool nickMsg)
{
    if(nickMsg)
    {
        std::cout << "Klient ("<< serverMap.GetPlayer(playerId)->GetName() <<") rozłączony." << std::endl;
    }
    std::unique_lock<std::mutex> playerLock(playerMutex);   //Blokowanie tablicy z graczami
    serverMap.GetPlayer(playerId)->SetState(false);         //Dezaktywacja gracza
    serverMap.GetPlayer(playerId)->SetName("");             //Dezaktywacja gracza
    playerLock.unlock();
    std::unique_lock<std::mutex> lock(availableIdsMutex);   //blokowanie tablicy z indeksami
    availableIds.push_back(playerId);                       // Zwracanie ID do puli dostępnych ID
    lock.unlock();
    client->disconnect();                                   // Rozłączenie klienta
}

void TcpServer::DisconnectAll()
{
    for (auto& client : clients)
    {
        if (client && client->getRemoteAddress() != sf::IpAddress::None)
        {
            client->disconnect(); // Rozłączenie klienta
            std::cout << "Rozłączono klienta: " << client->getRemoteAddress() << std::endl;
        }
    }

    clients.clear();
}

void TcpServer::StartClock()
{
    if (!clockRunning)
    {
        serverClock.restart();
        clockRunning = true;  
    }
}                                      
void TcpServer::StopClock()
{
    if(clockRunning)
    {
        serverClock.restart();
        clockRunning = false;
    }
}
int TcpServer::GetTime()
{
    if(clockRunning)
    {
        sf::Time elapsed = serverClock.getElapsedTime();
        return elapsed.asSeconds();
    }
    return -1;
}

int TcpServer::GetTimeLeft()
{
    if(!clockRunning)
    {
        return -1;
    }
    if(stateName == 1)
    {
        return waitingRoomTime - GetTime();
    }
    else if(stateName == 2)
    {
        return gameTime - GetTime();
    }
    else if(stateName == 3)
    {
        return summaryTime - GetTime();
    }
    else
    {
        return -1;
    }
}

void TcpServer::NextState(int nextState)
{
    if(clockRunning)
    {
        serverClock.restart();
        ChangeState(nextState);
    }
}


void TcpServer::ChangeState(int newState)
{
    {
        std::lock_guard<std::mutex> lock(stateMutex);
        stateName = newState;
    }
    cv.notify_all(); // Powiadom wszystkie wątki oczekujące na zmianę stanu
}