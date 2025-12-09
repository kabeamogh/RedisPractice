cmake_minimum_required(VERSION 3.10)

# Name deines Projekts
project(MyRedisServer)

# Wir nutzen modernes C++ (C++17 ist ein guter Standard)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Wo liegen deine Header-Dateien (.h)?
# Da sie bei dir im 'src' Ordner liegen, fügen wir 'src' hinzu.
include_directories(src)

# Erstelle die ausführbare Datei (Executable)
# Name: redis-server
# Quellen: Alle deine .cpp Dateien
add_executable(redis-server
    src/main.cpp
    src/redisServer.cpp
    src/redisDatabase.cpp
    src/redisCommandHandler.cpp
)

# WICHTIG: Da wir std::thread nutzen, müssen wir unter Linux
# die Threading-Bibliothek (pthread) verlinken.
find_package(Threads REQUIRED)
target_link_libraries(redis-server Threads::Threads)