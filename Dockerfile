# 1. Wir nutzen ein Ubuntu Linux
FROM ubuntu:latest

# 2. Installiere CMake, G++ und Make
RUN apt-get update && apt-get install -y \
    cmake \
    g++ \
    make

# 3. Erstelle Arbeitsordner
WORKDIR /app

# 4. Kopiere ALLE Dateien (src, CMakeLists.txt, etc.) in den Container
COPY . .

# 5. Erstelle einen Build-Ordner und baue das Projekt mit CMake
RUN mkdir build
WORKDIR /app/build
RUN cmake ..
RUN make

# 6. Der Startbefehl (Pfad zur fertigen Datei)
# ACHTUNG: Stellen Sie sicher, dass Ihr CMake 'redis-server' erstellt!
CMD ["./redis-server"]
