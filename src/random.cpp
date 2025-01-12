#include "random.hpp"
#include <random>

int Random::GenerateNumber(int maxValue) {

    //Tworzenie generatora
    static std::random_device rd;      
    static std::mt19937 gen(rd());
    
    // Określenie zakresu losowania
    std::uniform_int_distribution<> dist(0, maxValue);

    // Zwrócenie wylosowanej liczby
    return dist(gen);
}