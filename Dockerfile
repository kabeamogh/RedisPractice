# 1. Statt 'ubuntu' nehmen wir 'gcc'.
# Das ist ein Linux, wo g++ und make schon vorinstalliert sind!
FROM gcc:latest

# 2. Arbeitsplatz einrichten
WORKDIR /app

# 3. Dateien kopieren
COPY . .

# 4. Kompilieren (Wir müssen nichts mehr installieren!)
RUN g++ main.cpp redisServer.cpp redisCommandHandler.cpp redisDatabase.cpp -o my_redis_server

# 5. Starten
CMD ["./my_redis_server"]