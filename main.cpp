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



int main()
{
    std::srand(std::time(0)); // Inicjalizacja generatora losowego
    sf::RenderWindow* Window = createWindow();  //Utworzenie okna

    int width = 25;

    GameMap GameMap(width);

    for(auto i=0;i<width;i++)
    {
        //GameMap.AddWall(Wall(sf::Vector2i(0,i)));
        //GameMap.AddWall(Wall(sf::Vector2i(width-1,i)));
        //GameMap.AddWall(Wall(sf::Vector2i(i,0)));
        //GameMap.AddWall(Wall(sf::Vector2i(i,width-1)));
    }
    GameMap.AddWall(Wall(sf::Vector2i(10,10)));
    GameMap.AddWall(Wall(sf::Vector2i(20,5)));

    GameMap.addAllWallColliders();

    std::vector<BoxCollision> obstacles = GameMap.GetAllColliders();
    GameMap.addBoxCollisions(&obstacles);

    //Pozycja pojawiania się gracza po śmierci
    GameMap.AddPlayerSpawn(sf::Vector2i(1,1));
    GameMap.AddPlayerSpawn(sf::Vector2i(23,1));
    GameMap.AddPlayerSpawn(sf::Vector2i(23,23));
    GameMap.AddPlayerSpawn(sf::Vector2i(1,23));

    std::vector<Player*> playerList;  // Lista wskaźników na graczy
    Player player1;
    Player player2;

    // Dodanie graczy do wektora
    playerList.push_back(&player1);
    playerList.push_back(&player2);

    player1.SetPosition(sf::Vector2f(64,64));
    player2.SetPosition(sf::Vector2f(100,100));

    GameMap.SetPlayerList(playerList);

    sf::Vector2f moveDirection;
    bool shoot; 
    sf::Clock deltaClock;

    //Sprawdzenie czy okno zostało otwarte
    while(Window->isOpen())
    {
        sf::Time deltaTime = deltaClock.restart();      //Wyliczenie delty time (czas między klatkami)

        //Pętla wydarzeń
        EventLoop(Window);

        moveDirection = sf::Vector2f(0.0f, 0.0f);       //Wektor przemieszczenia 
        shoot = false;

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Return) || sf::Keyboard::isKeyPressed(sf::Keyboard::E) || sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        {
            shoot = true;
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            //playerImage.move(0,-1);
            moveDirection.y -= 1;
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            //playerImage.move(0,1);
            moveDirection.y += 1;
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            //playerImage.move(-1,0);
            moveDirection.x -= 1;
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            moveDirection.x += 1;
            //playerImage.move(1,0);
        }
        if(shoot && player1.CanShoot())
        {
            GameMap.AddBullet(player1.Shoot());
        }
        player1.Move(moveDirection,deltaTime,obstacles);
        GameMap.MoveBullets(deltaTime);
        GameMap.BulletCheck();


        //Window->clear(sf::Color::Black); //Wyczyszczenie ekraniu
        
        GameMap.DrawMap(Window);
        GameMap.DrawWalls(Window);
        GameMap.DrawBullets(Window);


        //player1.DrawPlayer(Window);
        GameMap.GetPlayer(0)->DrawPlayer(Window);
        player2.DrawPlayer(Window);
        Window->display();               //Wrzucenie zmian do okna

    }

    delete Window; 
    std::cout<<"Koniec\n";
    return 0;
}