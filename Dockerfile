FROM gcc:latest

WORKDIR /app

# Wir kopieren alles
COPY . .

# Wir nutzen den Wildcard *.cpp Trick
# Das -o my_redis_server benennt das fertige Programm
RUN g++ main.cpp redisServer.cpp redisDatabase.cpp redisCommandHandler.cpp -o my_redis_server

# Starten
CMD ["./my_redis_server"]
