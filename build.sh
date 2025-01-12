#!/bin/bash

# Ustawienia
BUILD_DIR="build"           # Katalog budowy
EXECUTABLE_NAME="Serwer"    # Nazwa pliku wykonywalnego
#EXECUTABLE_NAME="Gra"
PORT=8000                   # Port, na którym serwer będzie nasłuchiwał

# Sprawdzenie, czy port jest zajęty
PORT_USED=$(lsof -i :$PORT)

if [ -n "$PORT_USED" ]; then
    echo "Port $PORT jest zajęty. Próba zabicia procesu..."
    
    # Znalezienie PID procesu, który używa portu
    PID=$(lsof -t -i :$PORT)
    
    # Zabicie procesu, który używa portu
    kill -9 $PID
    echo "Proces $PID zabity. Port $PORT został zwolniony."
fi

# Sprawdzenie, czy istnieje katalog build, jeśli nie, utwórz go
if [ ! -d "$BUILD_DIR" ]; then
    echo "Tworzenie katalogu $BUILD_DIR..."
    mkdir "$BUILD_DIR"
fi

# Przejdź do katalogu budowy
cd "$BUILD_DIR" || exit 1

# Uruchom CMake i Make
echo "Konfiguracja projektu za pomocą CMake..."
cmake .. || { echo "Błąd podczas konfiguracji CMake."; exit 1; }

echo "Budowanie projektu..."
make || { echo "Błąd podczas budowania projektu."; exit 1; }

# Sprawdź, czy plik wykonywalny został utworzony
if [ -f "$EXECUTABLE_NAME" ]; then
    echo "Uruchamianie programu $EXECUTABLE_NAME..."
    ./"$EXECUTABLE_NAME"
else
    echo "Nie znaleziono pliku wykonywalnego $EXECUTABLE_NAME."
    exit 1
fi
