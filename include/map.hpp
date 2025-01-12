#ifndef MAP_H
#define MAP_H

#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm> 
#include <wall.hpp>
#include <player.hpp>
#include "boxCollision.hpp"
#include "bullet.hpp"
#include "random.hpp"
#include "mapTemplate.hpp"

class GameMap
{
    public:
        GameMap();                                                      //Konstruktor
        GameMap(int tilesNumber);                                       //Konstruktor z wielkością liczby płytek
        GameMap(MapTemplate mapTemplate);                               //Konstruktor który tworzy mapę na podstawie szablonu
        ~GameMap();                                                     //Dekonstruktor
        void AddWall(Wall newWall);                                     //Dodaje ścianę 1x1 MAŁO WYDAJNE (mapy z szablonu mają lepszą wydajność)
        void AddPlayerSpawn(sf::Vector2i spawnPosition);                //Dodaje miejsce odradzania gracza
        
        bool isPlayerColliding(Player player);                          //Sprawdza czy gracz koliduje z ścianą na mapie
        void SetPlayerList(std::vector<Player*> players);               //Ustawia listę graczy 
        std::vector<Player*> GetPlayerList();                           //Zwraca listę graczy
        Player* GetPlayer(int id);                                      //Zwraca gracza o podanym indexie
        bool IsNickUnused(std::string nick);                            //Sprawdza czy nick nie jest używany przez gracza
        int numberOfActivePlayers();                                    //Zwraca liczbę aktywnych graczy
        void SortPlayersRanking();                                      //Sortuje graczy według liczby zabójstw
        std::vector<Player*> GetRanking();                              //Zwraca ranking graczy
        Player* GetPlayerFromRanking(int id);                           //Zwraca gracza według rankingu 

        
        void addWallCollisions(std::vector<BoxCollision>* colliders);   //Dodaje kolizje do mapy NIE NADPISUJE STARYCH
        void addBoxCollisions(std::vector<BoxCollision> colliders);     //Dodaje kolizje z ścian na mapie NIE NADPISUJE STARYCH
        
        void AddBullet(const Bullet& newBullet);                        //Dodaje pocisk na mapie
        void MoveBullets(sf::Time deltaTime);                           //Porusza pocisk zgodnie z różnicą czasu (delta time)
        void ReplaceBullets(std::vector<Bullet> bullets);               //Zastępuje obecne pociski nowymi (stare zostają w tablicy ale są ustawiane na nieaktywne)
        std::vector<Bullet> GetBulletBox();                             //Zwraca vector pocisków
        void BulletCheck();                                             //Sprawdza i wykonuje interakcje związane z pociskiem
        
        std::vector<BoxCollision> GetAllColliders() const;              //Zwraca wszystkie hitboxy
        void addCollider(BoxCollision collider);                        //Dodaje hitbox
        void addAllWallColliders();                                     //Dodaje kolizje z wszystkich ścian, które zostały już dodane do mapy NIE NADPISUJE STARYCH
        
        void DrawWalls(sf::RenderWindow* window);                       //Rysuje ściany
        void DrawMap(sf::RenderWindow* window);                         //Rysuje podłogę (w przypadku używania szablonów również ściany)
        void DrawBullets(sf::RenderWindow* window);                     //Rysuje pociski
        void DrawPlayer(sf::RenderWindow* window);                      //Rysuje aktywnych graczy

        sf::Vector2i GetSpawnByIndex(int index);                        //Zwraca punkt odrodzenia się o podanym indexie
        sf::Vector2i GetNextSpawn();                                    //Zwraca następny punkt odrodzenia
        sf::Vector2i GetRandomSpawn();                                  //Zwraca losowy punkt odrodzenia
        int GetNumberOfSpawns();                                        //Zwraca liczbę miejsc odrodzeń
        void SetLastSpawn(int value);                                   //Ogranicza liczbę punktów odrodzenia w kolejce
        int GetLastSpawn();                                             //Zwraca ograniczenie punktów odrodzenia

    private:
        sf::Texture tileTexture;                                        //Textura kafelków na mapie
        sf::Sprite tile;                                                //obiekt kafelka
        sf::VertexArray tiles;                                          //Obiekt służcy do rysowania ziemia
        int numberOfTiles;                                              //Liczba kafelków w dowolnej osi (plansza jest kwadratem)
        std::vector<BoxCollision> allColliders;                         //Wszystkie hitboxy na mapie
        std::vector<Player*> playerList;                                //Wszyscy gracze na mapie (również nie aktywni)
        std::vector<Wall> walls;                                        //Wszystkie ściany
        std::vector<Bullet> bulletBox;                                  //Wszystkie pocisi
        int NextSpawn;                                                  //Następny punkt odrodzenia
        int LastSpawn;                                                  //Ograniczenie punktów odrodzenia
        std::vector<sf::Vector2i> playerSpawn;                          //Wszystkie punkty odrodzenia
        std::vector<Player*> playerRanking;                             //Gracza uporządkowani od najlepszego do najgorszego
};
#endif
