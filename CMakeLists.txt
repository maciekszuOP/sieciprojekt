cmake_minimum_required(VERSION 3.12)
project(GraSieci)

set(CMAKE_CXX_STANDARD 20)

# Znajdź bibliotekę SFML (wymagane moduły)
find_package(SFML 2.5 COMPONENTS graphics system network REQUIRED)

# Wyszukiwanie plików źródłowych
file(GLOB_RECURSE CLIENT_SRC_FILES src/client/*.cpp)      # Pliki klienta
file(GLOB_RECURSE SERVER_SRC_FILES src/server/*.cpp)      # Pliki serwera
file(GLOB_RECURSE COMMON_SRC_FILES src/*.cpp)             # Pliki wspólne (np. klasy, funkcje)

# Usuń pliki main.cpp z folderów klienta i serwera z plików wspólnych
list(REMOVE_ITEM COMMON_SRC_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/client/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/server/main.cpp)

# Kopiowanie zasobów do folderu budowania
file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})

# Tworzenie pliku wykonywalnego dla klienta
add_executable(Gra ${CLIENT_SRC_FILES} ${COMMON_SRC_FILES})
target_include_directories(Gra PRIVATE include)  # Dodanie katalogów nagłówków
target_link_libraries(Gra sfml-graphics sfml-network)  # Linkowanie SFML

# Tworzenie pliku wykonywalnego dla serwera
add_executable(Serwer ${SERVER_SRC_FILES} ${COMMON_SRC_FILES})
target_include_directories(Serwer PRIVATE include)  # Dodanie katalogów nagłówków
target_link_libraries(Serwer sfml-graphics sfml-network)  # Serwer potrzebuje tylko sieci




#



#cmake_minimum_required(VERSION 3.12)
#project(GraSieci)
#set(CMAKE_CXX_STANDARD 20)
#find_package(SFML 2.5 COMPONENTS graphics system network REQUIRED)
#file(GLOB_RECURSE SRC_FILES src/*.cpp)                                  #Wyszukanie wszystkich plików źródłowych znajdujących w folderze src 
#file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})                       #Kopiowanie plików png do builda
#add_executable(Gra ${SRC_FILES})                                        #Kompilacja plików źródłowych
#target_include_directories(Gra PRIVATE include)                         #Pliki nagłówkowe
#target_link_libraries(Gra sfml-graphics sfml-network)
