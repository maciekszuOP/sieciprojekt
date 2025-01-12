# SieciProjekt

## Parametry konfiguracyjne

Program pobiera dane konfiguracyjne z pliku `config.txt` (jeśli go nie ma, utworzy go i wprowadzi wartości domyślne).

### Opis parametrów (wszystkie są liczbami):
1. **Czas trwania poczekalni** po połączeniu wymaganej liczby graczy.
2. **Czas trwania rozgrywki**.
3. **Czas trwania ekranu podsumowania**.
4. **Minimalna liczba graczy** do uruchomienia gry.
5. **Maksymalna liczba graczy** (maksymalnie 16).
6. **ID mapy**.
7. **Liczba punktów odrodzeń**.

## Komendy

- `./Serwer <port>`: Jeśli nie podano portu, zostanie ustawiony na wartość domyślną `8000`.
- `./Gra <adres> <port>`: Wymaga podania obu wartości (`adres` i `port`).

## Kompilowanie i uruchomienie

Skrypt `build.sh` kompiluje wszystkie pliki i włącza serwer na porcie domyślnym (8000).

### Komenda:
```bash
./build.sh
```

Pliki wykonywalne tworzą się w folderze `build` i otrzymują nazwy:

- `Serwer` - Serwer gry.
- `Gra` - Klient gry.

Klienta należy włączyć osobno:

```bash
cd build
./Gra <adres> <port>
```
