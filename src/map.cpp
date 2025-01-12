#include "map.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/System/Vector2.hpp>
#include <wall.hpp>
#include <iostream>
#include <string>
#include <fstream>

GameMap::GameMap() : numberOfTiles(16), tiles(sf::Quads, 16 * 16 * 4)
{
    // Wczytanie tekstury
    if (!tileTexture.loadFromFile("assets/ground.png"))
    {
        std::cout << "Nie udało się wczytać tekstury trawy\n";
        return;
    }

    int tileSize = 32;
    int row = 0;

    for (int i = 0; i < numberOfTiles; i++)
    {
        for (int j = 0; j < numberOfTiles; j++)
        {
            // Indeks kafla w tablicy wierzchołków
            int index = (i * numberOfTiles + j) * 4;

            // Lewy górny wierzchołek
            tiles[index].position = sf::Vector2f(i * tileSize, j * tileSize);
            tiles[index].texCoords = sf::Vector2f(0, 0); // Początek tekstury

            // Prawy górny wierzchołek
            tiles[index + 1].position = sf::Vector2f((i + 1) * tileSize, j * tileSize);
            tiles[index + 1].texCoords = sf::Vector2f(tileSize, 0); // Koniec tekstury na osi X

            // Prawy dolny wierzchołek
            tiles[index + 2].position = sf::Vector2f((i + 1) * tileSize, (j + 1) * tileSize);
            tiles[index + 2].texCoords = sf::Vector2f(tileSize, tileSize); // Koniec tekstury na osi Y

            // Lewy dolny wierzchołek
            tiles[index + 3].position = sf::Vector2f(i * tileSize, (j + 1) * tileSize);
            tiles[index + 3].texCoords = sf::Vector2f(0, tileSize); // Początek tekstury na osi Y
        }
    }
    NextSpawn = 0;
    LastSpawn = GetNumberOfSpawns();
}

GameMap::GameMap(int tilesNumber) : numberOfTiles(tilesNumber), tiles(sf::Quads, tilesNumber * tilesNumber * 4)
{
    // Wczytanie tekstury
    if (!tileTexture.loadFromFile("assets/grass.png"))
    {
        std::cout << "Nie udało się wczytać tekstury trawy\n";
        return;
    }

    // Rozmiar kafla
    int tileSize = 32;

    for (int i = 0; i < numberOfTiles; i++)
    {
        for (int j = 0; j < numberOfTiles; j++)
        {
            // Indeks kafla w tablicy wierzchołków
            int index = (i * numberOfTiles + j) * 4;

            // Lewy górny wierzchołek
            tiles[index].position = sf::Vector2f(i * tileSize, j * tileSize);
            tiles[index].texCoords = sf::Vector2f(0, 0); // Początek tekstury

            // Prawy górny wierzchołek
            tiles[index + 1].position = sf::Vector2f((i + 1) * tileSize, j * tileSize);
            tiles[index + 1].texCoords = sf::Vector2f(tileSize, 0); // Koniec tekstury na osi X

            // Prawy dolny wierzchołek
            tiles[index + 2].position = sf::Vector2f((i + 1) * tileSize, (j + 1) * tileSize);
            tiles[index + 2].texCoords = sf::Vector2f(tileSize, tileSize); // Koniec tekstury na osi Y

            // Lewy dolny wierzchołek
            tiles[index + 3].position = sf::Vector2f(i * tileSize, (j + 1) * tileSize);
            tiles[index + 3].texCoords = sf::Vector2f(0, tileSize); // Początek tekstury na osi Y
        }
    }
    NextSpawn = 0;
    LastSpawn = GetNumberOfSpawns();
}

GameMap::GameMap(MapTemplate mapTemplate) : numberOfTiles(mapTemplate.GetSizeX()), tiles(sf::Quads, mapTemplate.GetSizeX() * mapTemplate.GetSizeX() * 4)
{
    // Wczytanie tekstury
    if (!tileTexture.loadFromFile("assets/ground.png"))
    {
        std::cout << "Nie udało się wczytać tekstury trawy\n";
        return;
    }
    // Rozmiar kafla
    int tileSize = 32;
    std::string line;
    for (int i = 0; i < numberOfTiles; i++)
    {
        line = mapTemplate.GetLine(i);
        for (int j = 0; j < numberOfTiles; j++)
        {
            if(line[j] == '+')
            {
                // Indeks kafla w tablicy wierzchołków
                int index = (i * numberOfTiles + j) * 4;

                // Lewy górny wierzchołek
                tiles[index].position = sf::Vector2f(i * tileSize, j * tileSize);
                tiles[index].texCoords = sf::Vector2f(0, tileSize); // Początek tekstury

                // Prawy górny wierzchołek
                tiles[index + 1].position = sf::Vector2f((i + 1) * tileSize, j * tileSize);
                tiles[index + 1].texCoords = sf::Vector2f(tileSize, tileSize); // Koniec tekstury na osi X

                // Prawy dolny wierzchołek
                tiles[index + 2].position = sf::Vector2f((i + 1) * tileSize, (j + 1) * tileSize);
                tiles[index + 2].texCoords = sf::Vector2f(tileSize, tileSize*2); // Koniec tekstury na osi Y

                // Lewy dolny wierzchołek
                tiles[index + 3].position = sf::Vector2f(i * tileSize, (j + 1) * tileSize);
                tiles[index + 3].texCoords = sf::Vector2f(0, tileSize*2); // Początek tekstury na osi Y
            }
            else if(line[j] == 'S')
            {
                AddPlayerSpawn(sf::Vector2i(i,j));
                //std::cout<<i<<" : "<<j<<std::endl;
                // Indeks kafla w tablicy wierzchołków
                int index = (i * numberOfTiles + j) * 4;

                // Lewy górny wierzchołek
                tiles[index].position = sf::Vector2f(i * tileSize, j * tileSize);
                tiles[index].texCoords = sf::Vector2f(0, 0); // Początek tekstury

                // Prawy górny wierzchołek
                tiles[index + 1].position = sf::Vector2f((i + 1) * tileSize, j * tileSize);
                tiles[index + 1].texCoords = sf::Vector2f(tileSize, 0); // Koniec tekstury na osi X

                // Prawy dolny wierzchołek
                tiles[index + 2].position = sf::Vector2f((i + 1) * tileSize, (j + 1) * tileSize);
                tiles[index + 2].texCoords = sf::Vector2f(tileSize, tileSize); // Koniec tekstury na osi Y

                // Lewy dolny wierzchołek
                tiles[index + 3].position = sf::Vector2f(i * tileSize, (j + 1) * tileSize);
                tiles[index + 3].texCoords = sf::Vector2f(0, tileSize); // Początek tekstury na osi Y
            }
            else
            {
                // Indeks kafla w tablicy wierzchołków
                int index = (i * numberOfTiles + j) * 4;

                // Lewy górny wierzchołek
                tiles[index].position = sf::Vector2f(i * tileSize, j * tileSize);
                tiles[index].texCoords = sf::Vector2f(0, 0); // Początek tekstury

                // Prawy górny wierzchołek
                tiles[index + 1].position = sf::Vector2f((i + 1) * tileSize, j * tileSize);
                tiles[index + 1].texCoords = sf::Vector2f(tileSize, 0); // Koniec tekstury na osi X

                // Prawy dolny wierzchołek
                tiles[index + 2].position = sf::Vector2f((i + 1) * tileSize, (j + 1) * tileSize);
                tiles[index + 2].texCoords = sf::Vector2f(tileSize, tileSize); // Koniec tekstury na osi Y

                // Lewy dolny wierzchołek
                tiles[index + 3].position = sf::Vector2f(i * tileSize, (j + 1) * tileSize);
                tiles[index + 3].texCoords = sf::Vector2f(0, tileSize); // Początek tekstury na osi Y
            }
        }
    }
    addBoxCollisions(mapTemplate.GetColliders());
    NextSpawn = 0;
    LastSpawn = GetNumberOfSpawns();
}

GameMap::~GameMap()
{
    for (Player* player : playerList) 
    {
        delete player;
    }
}

void GameMap::AddWall(Wall newWall)
{
    walls.push_back(newWall);
}

void GameMap::AddPlayerSpawn(sf::Vector2i spawnPosition)
{
    playerSpawn.push_back(spawnPosition);
}

bool GameMap::isPlayerColliding(Player player)
{
    for (Wall wall : walls) 
    {
        if(wall.IsColliding(player.GetCollider()))
        {
            return true;
        }
    }
    return false;
}

void GameMap::SetPlayerList(std::vector<Player*> players)
{
    playerList = players;
    playerRanking = players;
    int i = 0;
    for(Player* player : playerList)
    {
        player->SetId(i);
        i++;
    }
}

std::vector<Player*> GameMap::GetPlayerList()
{
    return playerList;
}

Player* GameMap::GetPlayer(int id)
{
    if (id >= 0 && id < playerList.size())
    {
        return playerList[id]; // Zwraca wskaźnik na gracza
    }
    return playerList[0];
}

bool GameMap::IsNickUnused(std::string nick)
{
    bool isUsed = true;
    for(auto* player : playerList)
    {   
        //std::cout<<player->GetName()<<std::endl;
        if (player->GetName() == nick)
        {
            isUsed = false;
            break;
        }
    }
    return isUsed;
}

int GameMap::numberOfActivePlayers()
{
    int amount = 0;
    for(auto* player : playerList)
    {   
        if (player->isPlayerActive())
        {
            amount +=1;
        }
    }
    return amount;
}

void GameMap::SortPlayersRanking()
{
    std::sort(playerRanking.begin(), playerRanking.end(), [](Player* a, Player* b) 
    {
        //Porównanie czy gracz jest aktywny
        if (a->isPlayerActive() != b->isPlayerActive()) {
            return a->isPlayerActive() > b->isPlayerActive();
        }
        //Porównanie liczby zabójstw
        if (a->GetKills() != b->GetKills()) {
            return a->GetKills() > b->GetKills(); 
        }
        //Porównanie liczy śmierci 
        return a->GetDeaths() < b->GetDeaths();
    });
}

std::vector<Player*> GameMap::GetRanking()
{
    return playerRanking;
}

Player* GameMap::GetPlayerFromRanking(int id)  
{
    return playerRanking[id];
}     

void GameMap::addBoxCollisions(std::vector<BoxCollision> boxCollision)
{
    for (BoxCollision box : boxCollision) 
    {
        allColliders.push_back(box);
    }
}

void GameMap::addWallCollisions(std::vector<BoxCollision>* colliders)
{
    for (Wall wall : walls) 
    {
        colliders->push_back(wall.GetCollider());
    }
}

void GameMap::AddBullet(const Bullet& newBullet)
{
    bulletBox.push_back(newBullet);
}

void GameMap::MoveBullets(sf::Time deltaTime)
{
    for (auto& bullet : bulletBox) 
    {  
        bullet.Move(deltaTime);
    }
}

void GameMap::ReplaceBullets(std::vector<Bullet> bullets)
{
    int newBulletSize = bullets.size();
    int bulletBoxSize = bulletBox.size();
    for (auto i=0; i<std::max(newBulletSize,bulletBoxSize);i++)
    {
        if (i < newBulletSize && i < bulletBoxSize)
        {
            // Aktualizuj istniejące pociski
            bulletBox[i].SetPosition(bullets[i].GetPosition());
            bulletBox[i].ChangeActiveState(true);
        }
        else if (i < bulletBoxSize)
        {
            // Dezaktywuj istniejące pociski, które nie mają odpowiedników w `bullets`
            bulletBox[i].ChangeActiveState(false);
        }
        else if (i < newBulletSize)
        {
            // Dodaj nowe pociski z `bullets`, które nie mają odpowiedników w `bulletBox`
            bulletBox.push_back(bullets[i]);
            bulletBox.back().ChangeActiveState(true);
        }
    }
}

std::vector<Bullet> GameMap::GetBulletBox()
{
    return bulletBox;
}

void GameMap::BulletCheck()
{
    std::vector<BoxCollision> walls = GetAllColliders();
    for (auto i = bulletBox.begin(); i != bulletBox.end();)
    {

        //Sprawdzanie kolizji z murem
        for (BoxCollision& wall : walls) 
        {
            if (i->IsColliding(wall))
            {
                i->MarkToDestroy();
                break; 
            }
        }
        //Sprawdzanie kolizji z graczami
        for (Player* player : playerList)
        {
            if(player->GetId() == i->GetOwnerId() || !player->isPlayerActive())
            {
                continue;
            }
            if (i->IsColliding(player->GetCollider()))
            {
                playerList[i->GetOwnerId()]->IncrementKill();
                std::cout <<playerList[i->GetOwnerId()]->GetName()<< " zabija gracza: " << player->GetName() << "\n";
                //player->RespawnOnTiles(GetRandomSpawn());
                player->RespawnOnTiles(GetNextSpawn());
                i->MarkToDestroy();
                break; 
            }
        }
        ++i;
    }
    bulletBox.erase
    (
        std::remove_if(bulletBox.begin(), bulletBox.end(), 
        [](const Bullet& bullet) 
        {
            return bullet.ShouldBeDestroyed(); 
        }),
        bulletBox.end()
    );  //Usuwanie wszystkich pocisków oznaczonych do zniszczenia
}

std::vector<BoxCollision> GameMap::GetAllColliders() const
{
    return allColliders;
}

void GameMap::addCollider(BoxCollision collider)
{
    allColliders.push_back(collider);
}

void GameMap::addAllWallColliders()
{
    for (const Wall& wall : walls)
    {
        addCollider(wall.GetCollider());
    }
}

void GameMap::DrawWalls(sf::RenderWindow* window)
{
    for (Wall wall : walls) 
    {
        wall.Draw(window);
    }
}

void GameMap::DrawMap(sf::RenderWindow* window)
{
    sf::RenderStates states;
    states.texture = &tileTexture;
    window->draw(tiles, states);
}

void GameMap::DrawBullets(sf::RenderWindow* window)
{
    for (auto& bullet : bulletBox) 
    {
        bullet.Draw(window); 
    }
}

void GameMap::DrawPlayer(sf::RenderWindow* window)
{
    for (auto player : playerList) 
    {
        player->DrawPlayer(window); 
    }
    
}


sf::Vector2i GameMap::GetSpawnByIndex(int index)
{
    return playerSpawn[index];
}

sf::Vector2i GameMap::GetNextSpawn()
{
    sf::Vector2i spawn = GetSpawnByIndex(NextSpawn);
    NextSpawn++;
    if(NextSpawn >= LastSpawn)
    {
        NextSpawn = 0;
    }
    //std::cout<<"NextSpawn: "<<spawn.x<<" : "<<spawn.y<<std::endl;
    return spawn;
}

sf::Vector2i GameMap::GetRandomSpawn()
{
    return playerSpawn[Random::GenerateNumber(playerSpawn.size()-1)];
}

int GameMap::GetNumberOfSpawns()
{
    return playerSpawn.size();
}

void GameMap::SetLastSpawn(int value)
{
    if(value < 1 || value > GetNumberOfSpawns())
    {
        return;
    }
    LastSpawn = value;
}
int GameMap::GetLastSpawn()
{
    return LastSpawn;
}